#include "db.h"
using namespace SIMPLE_DB;

database::database():cur_memtables_id(0),level_num(LEVEL_NUM),L0_file_num(0),L1_file_num(0),L2_file_num(0),is_create(true),locker(ATOMIC_FLAG_INIT){

        //先检测是打开还是创建数据库
    std::vector<std::string> files;
        
    std::string db_path= DB_PATH;
	
	//获取全部路径下的文件名
    get_all_files(db_path,files);

        //for (auto x:files) std::cout<<x<<std::endl;
    {
        //正则表达式，找日志文件，看是不是有问题
		
		std::regex log_e("(.*)(.log)");
        
		for (auto x:files){
        
			if (regex_match(x,log_e)){
                    
                    is_create = false;
                    
            } 
        }
    }

	//统计l0文件的数目
    if (!is_create){

        std::regex e("(.*)(.l0)");

        for(auto x:files){

            if (regex_match(x,e)) {

                    L0_file_num++;
                    //std::cout<<"NIce"<<std::endl;
            } 
        }
    }

	//统计L1的文件数目
    if (!is_create){

        std::regex e("(.*)(.l1)");

        for(auto x:files){

            if (regex_match(x,e)) {

                L1_file_num++;
                    //std::cout<<"NIce"<<std::endl;
            } 
        }
    }


    if (!is_create){

            std::regex e("(.*)(.l2)");

            for(auto x:files){

                if (regex_match(x,e)) {

                    L2_file_num++;

                    //std::cout<<"NIce"<<std::endl;
                } 
            }
    }
        
	//如果是没创建，就创建bloom，如果存在就读取
    if (is_create){

        for  (int i= 0;i<=LEVEL_NUM;i++){


                //创建bloom过滤器

            bloomfilters.push_back(new bloomfilter(BLOOMFILTER_LENGTH,100000000));

            std::string t = std::to_string(i) + ".bf";

            bloomfilters[i]->to_file(t);

        }
    }

    else{
    
		for (int i = 0;i<=LEVEL_NUM;i++){

                bloomfilters.push_back(new bloomfilter(BLOOMFILTER_LENGTH,100000000));

                std::string t = std::to_string(i) + ".bf";

                bloomfilters[i]->load(t);
        }
    }
        //这里是布隆过滤器的使用


        //这里处理run_map，如果创建就读文件初始化
    if (is_create){

        for(int i = 0;i<(LEVEL_NUM);i++){
                
                run_maps.push_back(new run_map(i));
            
        }
    }
        
    else{
        run_maps.push_back(new run_map(0));
        
        run_maps.push_back(new run_map(1));

        run_maps[0]->load(L0_file_num);
        
        run_maps[1]->load(L1_file_num);

    };


    flush_threadspool = new Threadpool(FLUSH_THREAD_NUM);
        //flush_threadspool.reset(new Threadpool(FLUSH_THREAD_NUM));

        //其实完全可以把日志后台线程写到日志类实例里边

        //log_instance.reset(new LOG_SYS);
    log_instance = new LOG_SYS;

        //启动日志后台线程
    log_thread = std::thread(LOG_SYS::cycle,&(*log_instance));
        

        //compaction_threadpool.resert();

        //一定要设置分离或者join,不然会报 terminate called without an activate exception
    log_thread.detach();

	//创建memtables
    for(int i = 0;i<(MEMTABLE_NUM);i++){

            //std::shared_ptr<memtable> pp(new memtable(MEMTABLE_SIZE,cur_memtables_id));
        memtable* pp = new memtable(MEMTABLE_SIZE,cur_memtables_id);

        cur_memtables_id++;

        memtables_list.insert({cur_memtables_id-1,pp});

    }

        //缓存的初始化

        reading_cache = new LRU_cache(MAX_KV_NUM_IN_CACHE);
        
};
    

//这里是用于显示调用析构的的函
database::~database(){
        

    //把内存中的memtable都强制写入文件，保证不丢失，
    while(!memtables_list.empty()){
            
        //自旋锁获取
        while(locker.test_and_set(std::memory_order_acquire));        

        L0_file_num++;
        
        if (memtables_list.begin()->second->data.empty()) {
                
				memtables_list.erase(memtables_list.begin());
            
                locker.clear(std::memory_order_release);
                
				continue;
        }
		
		//打包，function
        auto flush_work = std::bind(flush_4,memtables_list.begin()->second,bloomfilters[0],L0_file_num-1,run_maps[0]);
        
		flush_threadspool->push_task(flush_work);

        memtables_list.erase(memtables_list.begin());
            
        locker.clear(std::memory_order_release);
    }
        //for (int i = 0;i<LEVEL_NUM;i++){   }
    sleep(2);
    //让他们做完
    delete flush_threadspool;

        //std::cout<<"OKKKK"<<std::endl;
    delete log_instance;
}

    
bool database::Put(std::string K,std::string V){
 
    //先转化k,v 转化为bitset

    log_instance->append("\nPut " + K + "  " + V + "  " + std::to_string(time(NULL)) );
        
    if (L0_file_num >= (L0_MAX_FILE_NUM)) {
                //std::cout<<"Must to do compatcion"<<std::endl;
            compaction_call_back(this);
    }

    auto k = trans_string_to_key(K);
 
    auto v = trans_string_to_value(V);
 
        //写入bloomfilter
        //bloomfilters[0]->SetKey(k);
        //bloomfilters.to_file("0.bf");
    for (auto i = memtables_list.begin();i!=memtables_list.end();i++){

            //std::cout<<L0_file_num<<std::endl;
            
            //先找到没有被冻结的哪一个memtable
            if (i->second->is_freeze){
 
                continue;
            }

            if (i->second->insert(Node(k,v))) return true;

            while(locker.test_and_set(std::memory_order_acquire));        

            L0_file_num++;

            auto flush_work = std::bind(flush_3,i->second,bloomfilters[0],L0_file_num-1,run_maps[0]);

            locker.clear(std::memory_order_release);
           
            //解决bug，这里一顶要少点没有的，不然flush delete memtable之后，就会出错
            memtables_list.erase(i);            
            
            //仍到线程池里边
            flush_threadspool->push_task(flush_work);

            

            memtable* pp = new memtable(MEMTABLE_SIZE,cur_memtables_id);
            
            cur_memtables_id++;
            
            memtables_list.insert({cur_memtables_id-1,pp});
            
            return true;
    }
}

    
std::string database::Get(std::string K){
    
	//缓存里边找
    
	auto p = this->reading_cache->get(K);
    
    if (p != nullptr) return *(p);
    
    std::bitset<key_bitset_long> key = trans_string_to_key(K);
        
        //memtable里边找

    for (auto x:memtables_list ){

        auto p = x.second->find(key);

        if(p!= nullptr) {

                std::string i = trans_value_to_string((*p).value); 

                reading_cache->set(K,i);

                return i;
        }
    }

        //再L0的布隆过滤器中找找
        //如果有就去run_map中找
    if (bloomfilters[0]->VaryExit(key)){
            
        std::vector<int> r = run_maps[0]->return_id(key);
        
        std::cout<<"I am OK"<<std::endl;
            
        for (auto x:r){
                //std::cout<<x<<std::endl;

            std::string s_p = std::to_string(x) + ".l0";
                //std::vector<Node> dd =  load_memtable_from_sst_2(s_p);
                
            std::vector<Node> dd =  load_sst(s_p);
                
            for (auto z:dd){
                
                if(z.key == key){
                        
                        std::string i = trans_value_to_string(z.value); 
                        
                        reading_cache->set(K,i);
                        
                        return i;
                }
            }
        }
    }

    if (bloomfilters[1]->VaryExit(key)){

            std::cout<<"I am Wrong"<<std::endl;

            std::vector<int> r = run_maps[1]->return_id(key);

            for (auto x:r){
                //std::cout<<x<<std::endl;

                std::string s_p = std::to_string(x) + ".l1";
                std::vector<Node> dd =  load_sst(s_p);

                for (auto z:dd){
                    if(z.key == key){
                        
                        std::string i = trans_value_to_string(z.value); 
                        
                        reading_cache->set(K,i);
                        
                        return i;
                    }
                }
            }
    }
    return "None";
}

bool database::Delete(std::string K){

        //先删缓存
        
    reading_cache->remove(K);

    std::bitset<key_bitset_long> key = trans_string_to_key(K);

        
        //删memtable中的
    for (auto x:memtables_list ){

        if(x.second->remove(key)) return true;

    }

        //删除L0中的,如果没有就下一个
        if (bloomfilters[0]->VaryExit(key)){
            
            std::vector<int> r = run_maps[0]->return_id(key);

            for (auto x:r){
                //std::cout<<x<<std::endl;

                std::string s_p = std::to_string(x) + ".l0";
                std::vector<Node> dd =  load_sst(s_p);
            
                for(int i = 0;i<dd.size();i++){
                    if (dd[i].key == key){
                        for (int j = 0;j<key_bitset_long;j++) dd[i].key[j] = 0;
                        
                        remove(s_p.c_str());
                        auto task = std::bind(dump_file,s_p,dd);

                        flush_threadspool->push_task(task);
                        return true;
                    }
                }
                dd.clear();
            }
        }
        
    if (bloomfilters[1]->VaryExit(key)){
            
        std::vector<int> r = run_maps[1]->return_id(key);

        for (auto x:r){
                //std::cout<<x<<std::endl;

            std::string s_p = std::to_string(x) + ".l1";
            std::vector<Node> dd =  load_sst(s_p);
            
            for(int i = 0;i<dd.size();i++){
                if (dd[i].key == key){
                    for (int j = 0;j<key_bitset_long;j++) dd[i].key[j] = 0;
                        
                    remove(s_p.c_str());
                    auto task = std::bind(dump_file,s_p,dd);

                    flush_threadspool->push_task(task);
                    return true;
                    
                }
            }
            dd.clear();  
      }
        
    }

    return false;
}
