

#pragma once

namespace badgerdb {


typedef std::uint32_t PageId;

typedef std::uint16_t SlotId;


typedef std::uint32_t FrameId;


struct RecordId {
  
  PageId page_number;

  SlotId slot_number;

  bool operator==(const RecordId& rhs) const {
    return page_number == rhs.page_number && slot_number == rhs.slot_number;
  }

  bool operator!=(const RecordId& rhs) const {
    return (page_number != rhs.page_number) || (slot_number != rhs.slot_number);
  }
};

}


