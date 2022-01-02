

#pragma once

#include "file.h"
#include "bufHashTbl.h"
#include<iostream>

namespace badgerdb {

class BufMgr; // Declaring it since we use it

class BufDesc {

 public:

	friend class BufMgr;  // Declaring Function!!!

 private: 

      File* file;
      PageId pageNo;
      FrameId	frameNo;
      int pinCnt;
      bool dirty;   // Is the Page Dirty
      bool valid;   // An Actual Pa
      bool refbit;


  void Clear()
	{
    pinCnt = 0;
		file = NULL;
		pageNo = Page::INVALID_NUMBER;  // Clear this Class 
    dirty = false;
    refbit = false;
		valid = false;
  };

  void Set(File* filePtr, PageId pageNum)
	{ 
		file = filePtr; // Set filePtr to File in DB!
    pageNo = pageNum; // Set: 
    pinCnt = 1;
    dirty = false;
    valid = true;    // Setting the Frame as VALID!
    refbit = true;    // refbit

  }

  void Print()
	{
		if(file)
		{
			std::cout << "file:" << file->filename() << " ";
			std::cout << "pageNo:" << pageNo << " ";
		}
		else
    {
      std::cout << "file:NULL ";
    }
			

		std::cout << "valid:" << valid << " ";
		std::cout << "pinCnt:" << pinCnt << " ";
		std::cout << "dirty:" << dirty << " ";
		std::cout << "refbit:" << refbit << "\n";
  }

  BufDesc()
	{
    // Clear the Description of the Buffer Manager!
  	Clear();
  }
};


struct BufStats
{
    int accesses;
    int diskreads;
    int diskwrites;

    void clear()
    { // Clear everything
      std::cout << "Clear " << std::endl;
      accesses = diskreads = diskwrites = 0;
    }
        
    BufStats()
    {
      // Intinalize
      // Print??? 
      std::cout << "Number of Accesses: "<< accesses << std::endl;
      std::cout << "Number of Diskreaads: " << diskreads << std::endl;
      std::cout << "Number of Diskwrites: " << diskwrites << std::endl;

      clear();
    }
};


class BufMgr 
{
  private:

      FrameId clockHand;
      std::uint32_t numBufs;
      BufHashTbl *hashTable;    // Hash Table mapping (File, Page) to Frame Number
      BufDesc *bufDescTable;    // Keep track of the state of each frame in the Buffer Pool
      BufStats bufStats;      // Statistics about the Buffer Pool usage

      void advanceClock(); // Allocate a free frame using the clock algorithm
      void allocBuf(FrameId & frame); // Advance clock to the next frame in the buffer pool

  public:

      Page* bufPool;
      BufMgr(std::uint32_t bufs);
    
      ~BufMgr(); 

      void readPage(File* file, const PageId PageNo, Page*& page);
      void unPinPage(File* file, const PageId PageNo, const bool dirty);
      void allocPage(File* file, PageId &PageNo, Page*& page); 
      void flushFile(const File* file);
      void disposePage(File* file, const PageId PageNo);
      void  printSelf();

      BufStats &getBufStats()
      {
        return bufStats;
      }

      void clearBufStats() 
      {
        bufStats.clear();  // Clear the Stats
      }
};

}
