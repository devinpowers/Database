
#pragma once

#include <fstream>
#include <string>
#include <map>
#include <memory>

#include "page.h"

namespace badgerdb {

class FileIterator;


struct FileHeader {
 
  PageId num_pages;
  
  PageId first_used_page;

  PageId num_free_pages;

  PageId first_free_page;

  bool operator==(const FileHeader& rhs) const {
    return num_pages == rhs.num_pages &&
        num_free_pages == rhs.num_free_pages &&
        first_used_page == rhs.first_used_page &&
        first_free_page == rhs.first_free_page;
  }
};


class File {
 public:
  
  static File create(const std::string& filename);


  static File open(const std::string& filename);

  
  static void remove(const std::string& filename);

  
  static bool isOpen(const std::string& filename);

  static bool exists(const std::string& filename);

  File(const File& other);

  File& operator=(const File& rhs);

  ~File();

  Page allocatePage();

  Page readPage(const PageId page_number) const;

  void writePage(const Page& new_page);

  void deletePage(const PageId page_number);

  const std::string& filename() const { return filename_; }

  FileIterator begin();

  FileIterator end();

 private:

  static std::streampos pagePosition(const PageId page_number) {
    return sizeof(FileHeader) + ((page_number - 1) * Page::SIZE);
  }

  File(const std::string& name, const bool create_new);

  void openIfNeeded(const bool create_new);

  void close();

  Page readPage(const PageId page_number, const bool allow_free) const;

  
  void writePage(const PageId page_number, const Page& new_page);

  void writePage(const PageId page_number, const PageHeader& header,
                 const Page& new_page);

 
  FileHeader readHeader() const;

  void writeHeader(const FileHeader& header);

  PageHeader readPageHeader(const PageId page_number) const;

  typedef std::map<std::string,
                   std::shared_ptr<std::fstream> > StreamMap;
  typedef std::map<std::string, int> CountMap;

  static StreamMap open_streams_;
 
  static CountMap open_counts_;

  std::string filename_;

  std::shared_ptr<std::fstream> stream_;

  friend class FileIterator;
  friend class FileTest;
};

}
