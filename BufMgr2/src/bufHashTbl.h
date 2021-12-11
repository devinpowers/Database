
#pragma once

#include "file.h"

namespace badgerdb {

struct hashBucket {
	
    File *file;
    PageId pageNo;
    FrameId frameNo;
    hashBucket*   next;
};

class BufHashTbl
{
  private:

    int HTSIZE;
    hashBucket**  ht;
    
    int	 hash(const File* file, const PageId pageNo);

 public:

    BufHashTbl(const int htSize);  // constructor

    ~BufHashTbl(); // destructor
    
    void insert(const File* file, const PageId pageNo, const FrameId frameNo);

    void lookup(const File* file, const PageId pageNo, FrameId &frameNo);

    void remove(const File* file, const PageId pageNo);  
};

}


/*
    BUffer


*/
