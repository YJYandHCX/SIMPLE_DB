#target = cache_test
deps = utils.h config.h LRU_cache.h bloomfilter.h memtable.h run_map.h compaction.h  db.h flush.h run_map.h log_lib.h threadpool.h
objects = utils.o LRU_cache.o bloomfilter.o memtable.o compaction.o flush.o  db.o run_map.o log_lib.o threadpool.o 



simple_db.so : $(objects)

	ar crv libSimple_db.a $(objects) 

LRU_cache.o : $(deps)
	g++ -c LRU_cache.cc -lpthread -std=c++11

utils.o : $(deps)
	g++ -c utils.cc -lpthread -std=c++11 

memtable.o : $(deps)

	g++ -c memtable.cc -lpthread -std=c++11
compaction.o : $(deps)
	g++ -c compaction.cc -lpthread -std=c++11

bloomfilter.o : $(deps)
	g++ -c bloomfilter.cc -lpthread -std=c++11 



flush.o : $(deps)
	g++ -c flush.cc  -std=c++11 

run_map.o : $(deps)
	g++ -c run_map.cc  -std=c++11 

threadpool.o : $(deps)
	g++ -c threadpool.cc  -lpthread -std=c++11 

log_lib.o : $(deps)
	g++ -c log_lib.cc  -lpthread -std=c++11 

db.o : $(deps)
	g++ -c db.cc  -lpthread -std=c++11 

.PHONY : clean

clean :

	rm target $(objects) 
