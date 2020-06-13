#SIMPLE_DB

**SIMPLE_DB 是一个基于LSM-Tree的嵌入式数据库**

##简介
在实习期间，对[RocksDB](https://github.com/facebook/rocksdb "RocksDB")学习之后，想要自己实现一个类似的基于LSM-Tree的嵌入式数据库，在实习的业余时间完成。

## 开发语言和环境
使用C++语言，Centos 7系统开发，IDE为Vscode，使用g++ 4.8.5往上。

## 安装测试的方法
    git clone https://github.com/YJYandHCX/SIMPLE_DB
	make
	cp ./libSimple_db.a ./examples
	g++ db_test.cc libSimple_db.a -lpthread -std=c++11 -o test
	./test
