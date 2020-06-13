#pragma once

#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "config.h"
#include "memtable.h"
#include "flush.h"
#include "utils.h"
#include "db.h"
#include "run_map.h"
#include "bloomfilter.h"


//因为写的效率很低，所以设置成了回调的形式，方便以后重写

namespace SIMPLE_DB{
    
    class database;
    
    void compaction(database *db);
}
