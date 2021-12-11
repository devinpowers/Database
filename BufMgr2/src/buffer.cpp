

#include <memory>
#include <iostream>
#include "buffer.h"
#include "exceptions/buffer_exceeded_exception.h"
#include "exceptions/page_not_pinned_exception.h"
#include "exceptions/page_pinned_exception.h"
#include "exceptions/bad_buffer_exception.h"
#include "exceptions/hash_not_found_exception.h"

namespace badgerdb { 

BufMgr::BufMgr(std::uint32_t bufs) : numBufs(bufs) {

	// Constructor for the Buffer Manager!!
	// Setting up the Buffer Manager for us to use!!!

	bufDescTable = new BufDesc[bufs]; // Creates a Buffer Desription Table the size of our buffer!

  for (FrameId i = 0; i < bufs; i++) 
  {
  	bufDescTable[i].frameNo = i;       // For each element in the array set a frame number and the frame to false
  	bufDescTable[i].valid = false;
  }

  bufPool = new Page[bufs];

  int htsize = ((((int) (bufs * 1.2))*2)/2)+1;   // Hash Function?? 


  hashTable = new BufHashTbl (htsize);  // allocate the buffer hash table

  clockHand = bufs - 1;   // Set the Clock Hand to the size of our Buffer

}


BufMgr::~BufMgr() {
}

void BufMgr::advanceClock()
{	
	// advance clock to the next frame in the buffer pool
	clockHand = (clockHand +1 ) % numBufs;
	
}

void BufMgr::allocBuf(FrameId & frame) 
{
	
}

	
void BufMgr::readPage(File* file, const PageId pageNo, Page*& page)
{	// Reads Page! 

	try{
		// Case 1: Page is in the Buffer Pool
		FrameId frame;
		hashTable-> lookup(file, pageNo, frame);
		// Set the Appropriate refbit
		bufDescTable[frame].refbit = true;
		bufDescTable[frame].pinCnt++;
		bufStats.accesses++;
		// Return a pointer to the frame containing the page
		page = &bufPool[frame];
	}
	catch(HashNotFoundException e){
		// Case 2: Page is NOT in the Buffer pool

		FrameId frame;
		allocBuf(frame);
		//
	}


}


void BufMgr::unPinPage(File* file, const PageId pageNo, const bool dirty) 
{
}

void BufMgr::flushFile(const File* file) 
{
}

void BufMgr::allocPage(File* file, PageId &pageNo, Page*& page) 
{
}

void BufMgr::disposePage(File* file, const PageId PageNo)
{
    
}

void BufMgr::printSelf(void) 
{
  BufDesc* tmpbuf;
	int validFrames = 0;
  
  for (std::uint32_t i = 0; i < numBufs; i++)
	{
  	tmpbuf = &(bufDescTable[i]);
		std::cout << "FrameNo:" << i << " ";
		tmpbuf->Print();

  	if (tmpbuf->valid == true)
    	validFrames++;
  }

	std::cout << "Total Number of Valid Frames:" << validFrames << "\n";
}

}
