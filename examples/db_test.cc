#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cassert>
#include <time.h>
#include <mutex>
#include <atomic>
#include <memory>
#include <thread>
#include <fstream>

#include "../db.h"
#include "../compaction.h"
using namespace std;

int main(){
    
    cout<<"Create the Database" <<endl;

    auto db = (new SIMPLE_DB::database);
    
    db->compaction_call_back = SIMPLE_DB::compaction;

    cout<<"Insert Key  :  Yanjianyu, Value : idoit" <<endl;   
    
    db->Put("Yanjianyu","idoit");
    
    delete db;

    cout<<"Close one times"<<endl;

    sleep(10);
    
    cout<<"Opene the database"<<endl;
    db = (new SIMPLE_DB::database);
    
    db->compaction_call_back = SIMPLE_DB::compaction;

    cout<<"Show the result of get Yanjianyu" <<endl;
    
    cout<<db->Get("Yanjianyu")<<endl;;
    
    cout<<"Delete the Yanjianyu"<<endl;

    db->Delete("Yanjianyu");
    
    cout<<"Show the result of get Yanjianyu" <<endl;
    cout<<db->Get("Yanjianyu")<<endl;
    
    
    delete db;
    cout<<"Close tow times" <<endl;
    sleep(10);
 
    cout<<"Opene the database"<<endl;
    db = (new SIMPLE_DB::database);
    db->compaction_call_back = SIMPLE_DB::compaction;

    cout<<"Show the result of get Yanjianyu" <<endl;
    cout<<db->Get("Yanjianyu")<<endl;;

    

    delete db;

    cout<<"Close Three times" <<endl;
    sleep(4);
    return 0;
}
