
#pragma once

#include <cassert>
#include "file.h"
#include "page.h"
#include "types.h"

namespace badgerdb {


class FileIterator {
 public:
 
  FileIterator()
      : file_(NULL),
        current_page_number_(Page::INVALID_NUMBER) {
  }


  FileIterator(File* file)
      : file_(file) {
    assert(file_ != NULL);
    const FileHeader& header = file_->readHeader();
    current_page_number_ = header.first_used_page;
  }

  FileIterator(File* file, PageId page_number)
      : file_(file),
        current_page_number_(page_number) {
  }

 
	inline FileIterator& operator++() {
    assert(file_ != NULL);
    const PageHeader& header = file_->readPageHeader(current_page_number_);
    current_page_number_ = header.next_page_number;

		return *this;
	}


	inline FileIterator operator++(int)
	{
		FileIterator tmp = *this;   // copy ourselves

    assert(file_ != NULL);
    const PageHeader& header = file_->readPageHeader(current_page_number_);
    current_page_number_ = header.next_page_number;

		return tmp;
	}


	inline bool operator==(const FileIterator& rhs) const {
    return file_->filename() == rhs.file_->filename() &&
        current_page_number_ == rhs.current_page_number_;
  }

	inline bool operator!=(const FileIterator& rhs) const {
    return (file_->filename() != rhs.file_->filename()) ||
        (current_page_number_ != rhs.current_page_number_);
  }

 
	inline Page operator*() const
  { return file_->readPage(current_page_number_); }

 private:

  File* file_;

  PageId current_page_number_;
};

}
