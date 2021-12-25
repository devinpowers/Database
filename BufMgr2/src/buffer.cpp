

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
	// Allocates an array for the Buffer Pool and a Corresponding BufDesc Table
	// All Frames will be set up in a clear state
	// The hash table will also start out in an empty state

	bufPool = new Page[bufs];	// Create Buffer Pool!
	bufDescTable = new BufDesc[bufs]; // Creates a Buffer Desription Table the size of our buffer!

  for (FrameId i = 0; i < bufs; i++) 
  { // Set all the Frame Numbers in the Buffer and their current state of being valid to false
  	bufDescTable[i].frameNo = i;       
  	bufDescTable[i].valid = false;
  }


  // Hash Table
  int htsize = ((((int) (bufs * 1.2))*2)/2)+1;   // Hash Function?? 

  hashTable = new BufHashTbl(htsize);  // allocate the buffer hash table

  clockHand = bufs - 1;   // Set the Clock Hand to the size of our Buffer

}


BufMgr::~BufMgr() {
	// Flushes out all dirty pages and deallocates the buffer pool and the BufDesc table.

	for(int i = 0; i < (int)numBufs; i++ ){
\
		if(bufDescTable[i].dirty == true){
			// Must write page and other information to disk
			File* thisFile = bufDescTable[i].file;
			PageId thisPageNo = bufDescTable[i].pageNo;
			Page thisPage = bufPool[thisPageNo];

			bufStats.accesses++;
			thisFile ->writePage(thisPage);
			bufStats.diskwrites++;
			bufDescTable[i].dirty = false;
			hashTable -> remove(thisFile, thisPageNo);
			bufDescTable[i].Clear(); // Clear array at index
		}
	}

	// Deallocate our Memory
	delete [] bufPool;
	delete [] bufDescTable;
	delete hashTable;

}

void BufMgr::advanceClock()
{	// advance clock to the next frame in the buffer pool
	clockHand = (clockHand +1 ) % numBufs;	
	
}

void BufMgr::allocBuf(FrameId & frame) 
{
	int count = 0;
	bool allocated = false;
	// We have to find a empty spot in our Buffer Pool, we must traverse
	while(count <(int)numBufs*2){

		if(bufDescTable[clockHand].valid == true){
			// Valid page to go down
			if (bufDescTable[clockHand].refbit == false ){

				if (bufDescTable[clockHand].pinCnt == 0){
					//
					if (bufDescTable[clockhand].dirty == false){
						//
						allocated = true;
						frame = clockhand;
						break;
					}
					else{

						// FLSUH PAGE TO DISK
						bufDescTable[clockHand].file->writePage(bufPool[clockHand]);
						bufStats.accesses++;
						hashTable->remove(bufDescTable[clockHand].file, bufDescTable[clockHand].pageNo);
						bufDescTable[clockHand].Clear();
						allocated = true;
						frame = clockHand;
						bufStats.diskwrites++;
                 		break;
					}
				}
				else{
					// Adnace
					advanceClock();
					count++;
				}
			}
			else{
				//refbit
				bufDescTable[clockHand].refbit = false;
				advanceClock(); // next
				count++;
			}
		}
		else{
			// Not valid Set
			allocated = true;
			frame = clockHand;
			break;
		}
	}
	if (allocated == false){
		// All Buffer frames are pinned throw BufferExceededException
		throw BufferExceededException();
	}	
}

	
void BufMgr::readPage(File* file, const PageId pageNo, Page*& page)
{	// Reads Page! 

	try{
		// Case 1: Page is in the Buffer Pool
		FrameId frame;
		hashTable-> lookup(file, pageNo, frame); // Use Hashtable to look up and r
		// Set the Appropriate refbit
		bufDescTable[frame].refbit = true; 	// Set as a real page
		bufDescTable[frame].pinCnt++;		 //  Set this as well!
		bufStats.accesses++; 				// Set this as Well!


		// Return a pointer to the frame containing the page
		page = &bufPool[frame];

	}
	catch(HashNotFoundException e){
		// Case 2: Page is NOT in the Buffer pool
		// So we have to grab it from the DB/Disk and put into the Buffer Pool, But also check to see if there is room !!!
		FrameId frame;
		allocBuf(frame); // ALlocate a open buffer page for us
		
		// Call method to read page from the disk and into the uffer pool
		Page Target = file-> readPage(pageNo);
		bufStats.diskreads++;
		bufPool[frame] = Target; // Set page inside the buffer Pool 
		// Insert the page into the 
		hashTable->insert(file, pageNo, frame);
		// return pointer to frame containg ghe frame

		page = &bufPool[frame];

		bufDescTable[frame].Set(file, pageNo);
	}
 

}


void BufMgr::unPinPage(File* file, const PageId pageNo, const bool dirty) 
{
	// UnPinPage

}

void BufMgr::flushFile(const File* file) 
{
	// Flush the Page
	// iterate through the buftable and look for the pages belonging to the specfied file
	for (FrameId i = 0; i < numBufs; i++){
		
		if (bufDescTable[i].file == file){
			PageId pageNo = bufDescTable[i].pageNo;
			// What about the exception and stuff
		}

	}
}

void BufMgr::allocPage(File* file, PageId &pageNo, Page*& page) 
{
	// Allocate an empty page in the specified specified file (Remeber Pages are inside files)
	Page emptyPage = file-> allocatePage();
	pageNo = emptyPage.page_number();

	FrameId frame;

	// Call allocBuff to allocate a buffer pool Frame!!

	allocBuf(frame);
	bufPool[frame] = emptyPage;
	bufStats.accesses++;
	page = &bufPool[frame];

	hashTable->insert(file, pageNo, frame);

	bufDescTable[frame].Set(file, pageNo); // invoke set() to "set" the table up properly!


}

void BufMgr::disposePage(File* file, const PageId PageNo)
{
	try{
		// Get Rid of that shit!!
		FrameId frame;
		hashTable-> lookup(file, PageNo, frame);
		// Free the Frame
		bufDescTable[frame].Clear();

		// Something Something
		hashTable-> remove(file, PageNo);
	}

	catch (HashNotFoundException e){
		// IDK Nothign to return!!
	}

	// Delete the Page from the File
	file->deletePage(PageNo);
}


void BufMgr::printSelf(void) 
{

	// Print the Buffer or something!!!!
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
