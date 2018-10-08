#include "TridentTD_SPIFFS.h"

#if defined (ESP32)
#include <vfs_api.h>
#endif

#if defined(ESP8266)
#include <FSImpl.h>
#endif

using namespace fs;


TridentTD_SPIFFS::TridentTD_SPIFFS(FSImplPtr impl)
#if defined (ESP32)
  : SPIFFSFS(impl)
#endif
#if defined (ESP8266)
  : FS(impl)
#endif
{}

#if defined (ESP8266)
bool TridentTD_SPIFFS::begin() {
  if (!_impl) {
      return false;
  }
  return _impl->begin();
}

bool TridentTD_SPIFFS::begin(bool formatOnFail) {
  if(!this->begin()){
    if( formatOnFail) {
      this->format();
    }
  }
}

size_t TridentTD_SPIFFS::totalBytes(){
  FSInfo info;
  this->info(info);
  return info.totalBytes;
}

size_t TridentTD_SPIFFS::usedBytes(){
  FSInfo info;
  this->info(info);
  return info.usedBytes;
}

#endif

void TridentTD_SPIFFS::listDir(String dirname) {
#if defined (ESP32)
  _listDir( *this, dirname.c_str(), 0 );
#elif defined (ESP8266)
  Serial.printf("Listing directory: %s\r\n", dirname.c_str());

  Dir root = this->openDir(dirname);
  // if (!root) {
  //   Serial.println("[td_SPIFFS] failed to open directory");
  //   return;
  // }
  while (root.next()) {
    String fileName = root.fileName();
    Serial.print("  FILE: ");
    Serial.print(root.fileName());
    Serial.print("\tSIZE: ");
    Serial.println(root.fileSize());
  }
#endif
}


#if defined (ESP32)
void TridentTD_SPIFFS::_listDir(fs::FS &fs, const char *dirname, uint8_t levels ) {
  Serial.printf("Listing directory: %s\r\n", dirname);

  File root = fs.open(dirname);
  if (!root) {
    Serial.println("[td_SPIFFS] failed to open directory");
    return;
  }
  if (!root.isDirectory()) {
    Serial.println("[td_SPIFFS] not a directory");
    return;
  }

  File file = root.openNextFile();
  while (file) {
    if (file.isDirectory()) {
      Serial.print("  DIR : ");
      Serial.println(file.name());
      if (levels) {
        _listDir(fs, file.name(), levels - 1);
      }
    } else {
      Serial.print("  FILE: ");
      Serial.print(file.name());
      Serial.print("\tSIZE: ");
      Serial.println(file.size());
    }
    file = root.openNextFile();
  }
}
#endif

void TridentTD_SPIFFS::readFile(String path) {
  Serial.printf("Reading file: %s\r\n", path.c_str());

#if defined (ESP32)
  File file = this->open(path.c_str());
  if (!file || file.isDirectory()) {
    Serial.println("[td_SPIFFS] failed to open file for reading");
    return;
  }
#elif defined (ESP8266)
  File file = this->open(path.c_str(), "r");
  if (!file) {
    Serial.println("[td_SPIFFS] failed to open file for reading");
    return;
  }  
#endif

  Serial.println("[td_SPIFFS] read from file:");
  while (file.available()) {
    Serial.write(file.read());
  }
}

void TridentTD_SPIFFS::readFiletoStream(String path, Stream &stm) {
  Serial.printf("Reading file: %s\r\n", path.c_str());

#if defined (ESP32)
  File file = this->open(path.c_str());
  if (!file || file.isDirectory()) {
    Serial.println("[td_SPIFFS] failed to open file for reading");
    return;
  }
#elif defined (ESP8266)
  File file = this->open(path.c_str(), "r");
  if (!file) {
    Serial.println("[td_SPIFFS] failed to open file for reading");
    return;
  }  
#endif

  Serial.println("[td_SPIFFS] read from file:");
  while (file.available()) {
    stm.write(file.read());
  }
}

void TridentTD_SPIFFS::writeFile(String path, String message) {
  Serial.printf("Writing file: %s\r\n", path.c_str());

#if defined (ESP32)
  File file = this->open(path.c_str(), FILE_WRITE);
#elif defined (ESP8266)
  File file = this->open(path.c_str(), "w");
#endif

  if (!file) {
    Serial.println("[td_SPIFFS] failed to open file for writing");
    return;
  }


  if (file.print(message)) {
    Serial.println("[td_SPIFFS] file written");
  } else {
    Serial.println("[td_SPIFFS] frite failed");
  }
}

void TridentTD_SPIFFS::appendFile(String path, String message) {
  Serial.printf("Appending to file: %s\r\n", path.c_str());

#if defined (ESP32)
  File file = this->open(path.c_str(), FILE_APPEND);
#elif defined (ESP8266)
  File file = this->open(path.c_str(), "a");
#endif

  if (!file) {
    Serial.println("[td_SPIFFS] failed to open file for appending");
    return;
  }
  if (file.print(message)) {
    Serial.println("[td_SPIFFS] message appended");
  } else {
    Serial.println("[td_SPIFFS] append failed");
  }
}

void TridentTD_SPIFFS::renameFile(String path1, String path2) {
  Serial.printf("Renaming file %s to %s\r\n", path1.c_str(), path2.c_str());
  if (this->rename(path1.c_str(), path2.c_str())) {
    Serial.println("[td_SPIFFS] file renamed");
  } else {
    Serial.println("[td_SPIFFS] rename failed");
  }
}

void TridentTD_SPIFFS::deleteFile(String path) {
  Serial.printf("Deleting file: %s\r\n", path.c_str());
  if (this->remove(path.c_str())) {
    Serial.println("[td_SPIFFS] file deleted");
  } else {
    Serial.println("[td_SPIFFS] delete failed");
  }
}

#if defined (ESP8266)
#include "spiffs_api.h"

#ifdef ARDUINO
extern "C" uint32_t _SPIFFS_start;
extern "C" uint32_t _SPIFFS_end;
extern "C" uint32_t _SPIFFS_page;
extern "C" uint32_t _SPIFFS_block;

#define SPIFFS_PHYS_ADDR ((uint32_t) (&_SPIFFS_start) - 0x40200000)
#define SPIFFS_PHYS_SIZE ((uint32_t) (&_SPIFFS_end) - (uint32_t) (&_SPIFFS_start))
#define SPIFFS_PHYS_PAGE ((uint32_t) &_SPIFFS_page)
#define SPIFFS_PHYS_BLOCK ((uint32_t) &_SPIFFS_block)

#ifndef SPIFFS_MAX_OPEN_FILES
#define SPIFFS_MAX_OPEN_FILES 5
#endif  //SPIFFS_MAX_OPEN_FILES
#endif // ARDUINO

TridentTD_SPIFFS td_SPIFFS = TridentTD_SPIFFS(FSImplPtr(new SPIFFSImpl(
                                                         SPIFFS_PHYS_ADDR,
                                                         SPIFFS_PHYS_SIZE,
                                                         SPIFFS_PHYS_PAGE,
                                                         SPIFFS_PHYS_BLOCK,
                                                         SPIFFS_MAX_OPEN_FILES)));
#endif // ESP8266


#if defined (ESP32)
TridentTD_SPIFFS td_SPIFFS = TridentTD_SPIFFS(FSImplPtr(new VFSImpl()));
#endif // ESP32


