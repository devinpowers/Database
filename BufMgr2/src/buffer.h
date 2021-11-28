
#pragma once

#include "file.h"
#include "bufHashTbl.h"

namespace badgerdb {


class BufMgr;

class BufDesc {

	friend class BufMgr;

 private:

  File* file;

  PageId pageNo;

  FrameId	frameNo;

  int pinCnt;

  bool dirty;


  bool valid;

  bool refbit;


  void Clear()
	{
    pinCnt = 0;
		file = NULL;
		pageNo = Page::INVALID_NUMBER;
    dirty = false;
    refbit = false;
		valid = false;
  };

  void Set(File* filePtr, PageId pageNum)
	{ 
		file = filePtr;
    pageNo = pageNum;
    pinCnt = 1;
    dirty = false;
    valid = true;
    refbit = true;
  }

  void Print()
	{
		if(file)
		{
			std::cout << "file:" << file->filename() << " ";
			std::cout << "pageNo:" << pageNo << " ";
		}
		else
			std::cout << "file:NULL ";

		std::cout << "valid:" << valid << " ";
		std::cout << "pinCnt:" << pinCnt << " ";
		std::cout << "dirty:" << dirty << " ";
		std::cout << "refbit:" << refbit << "\n";
  }

  BufDesc()
	{
  	Clear();
  }
};


struct BufStats
{

  int accesses;

  int diskreads;

  int diskwrites;

  void clear()
  {
		accesses = diskreads = diskwrites = 0;
  }
      
  BufStats()
  {
		clear();
  }
};


class BufMgr 
{
 private:

  FrameId clockHand;

  std::uint32_t numBufs;
	
  BufHashTbl *hashTable;

  BufDesc *bufDescTable;

  BufStats bufStats;

  void advanceClock();

  void allocBuf(FrameId & frame);

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


  BufStats & getBufStats()
  {
		return bufStats;
  }


  void clearBufStats() 
  {
		bufStats.clear();
  }
};

}
