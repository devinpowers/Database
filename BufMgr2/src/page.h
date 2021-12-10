
#pragma once

#include <cstddef>
#include <stdint.h>
#include <memory>
#include <string>

#include "types.h"

namespace badgerdb {


struct PageHeader {
 
  std::uint16_t free_space_lower_bound;

  std::uint16_t free_space_upper_bound;
  SlotId num_slots;

  SlotId num_free_slots;

  PageId current_page_number;

  PageId next_page_number;

  bool operator==(const PageHeader& rhs) const {
    return num_slots == rhs.num_slots &&
        num_free_slots == rhs.num_free_slots &&
        current_page_number == rhs.current_page_number &&
        next_page_number == rhs.next_page_number;
  }
};


struct PageSlot {

  bool used;

  std::uint16_t item_offset;


  std::uint16_t item_length;
};

class PageIterator;


class Page {
 public:

  static const std::size_t SIZE = 8192;

  static const std::size_t DATA_SIZE = SIZE - sizeof(PageHeader);

  static const PageId INVALID_NUMBER = 0;


  static const SlotId INVALID_SLOT = 0;


  Page();

 
  RecordId insertRecord(const std::string& record_data);

  std::string getRecord(const RecordId& record_id) const;

  void updateRecord(const RecordId& record_id, const std::string& record_data);

 
  void deleteRecord(const RecordId& record_id);


  bool hasSpaceForRecord(const std::string& record_data) const;

 
  std::uint16_t getFreeSpace() const { return header_.free_space_upper_bound -
                                              header_.free_space_lower_bound; }


  PageId page_number() const { return header_.current_page_number; }


  PageId next_page_number() const { return header_.next_page_number; }


  PageIterator begin();


  PageIterator end();

 private:

  void initialize();


  void set_page_number(const PageId new_page_number) {
    header_.current_page_number = new_page_number;
  }

 
  void set_next_page_number(const PageId new_next_page_number) {
    header_.next_page_number = new_next_page_number;
  }

 
  void deleteRecord(const RecordId& record_id,
                    const bool allow_slot_compaction);

 
  PageSlot* getSlot(const SlotId slot_number);

 
  const PageSlot& getSlot(const SlotId slot_number) const;


  SlotId getAvailableSlot();

 
  void insertRecordInSlot(const SlotId slot_number,
                          const std::string& record_data);


  void validateRecordId(const RecordId& record_id) const;

  bool isUsed() const { return page_number() != INVALID_NUMBER; }

  PageHeader header_;

  std::string data_;

  friend class File;
  friend class PageIterator;
  friend class PageTest;
  friend class BufferTest;
};

static_assert(Page::SIZE > sizeof(PageHeader),  "Page size must be large enough to hold header and data.");
static_assert(Page::DATA_SIZE > 0, "Page must have some space to hold data.");

}
