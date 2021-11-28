

#pragma once

#include <cassert>
#include "file.h"
#include "page.h"
#include "types.h"

namespace badgerdb {


class PageIterator {
 public:

  PageIterator()
      : page_(NULL) {
    current_record_ = {Page::INVALID_NUMBER, Page::INVALID_SLOT};
  }


  PageIterator(Page* page)
      : page_(page)  {
    assert(page_ != NULL);
    const SlotId used_slot = getNextUsedSlot(Page::INVALID_SLOT /* start */);
    current_record_ = {page_->page_number(), used_slot};
  }


  PageIterator(Page* page, const RecordId& record_id)
      : page_(page),
        current_record_(record_id) {
  }

 
	inline PageIterator& operator++() {
    assert(page_ != NULL);
    const SlotId used_slot = getNextUsedSlot(current_record_.slot_number);
    current_record_ = {page_->page_number(), used_slot};

		return *this;
  }

	inline PageIterator operator++(int) {
		PageIterator tmp = *this;   // copy ourselves

    assert(page_ != NULL);
    const SlotId used_slot = getNextUsedSlot(current_record_.slot_number);
    current_record_ = {page_->page_number(), used_slot};

		return tmp;
  }
 
	inline bool operator==(const PageIterator& rhs) const {
    return page_->page_number() == rhs.page_->page_number() &&
        current_record_ == rhs.current_record_;
  }

	inline bool operator!=(const PageIterator& rhs) const {
    return (page_->page_number() != rhs.page_->page_number()) || 
        (current_record_ != rhs.current_record_);
  }

 
	inline std::string operator*() const {
		return page_->getRecord(current_record_); 
	}

  
  SlotId getNextUsedSlot(const SlotId start) const {
    SlotId slot_number = Page::INVALID_SLOT;
    for (SlotId i = start + 1; i <= page_->header_.num_slots; ++i) {
      const PageSlot* slot = page_->getSlot(i);
      if (slot->used) {
        slot_number = i;
        break;
      }
    }
    return slot_number;
  }

 private:

  Page* page_;
  RecordId current_record_;

};

}
