#ifndef __TRIDENTTD_SPIFFS_H__
#define __TRIDENTTD_SPIFFS_H__

#include <Arduino.h>
#include <Stream.h>
#include <FS.h>

#if defined (ESP32)
#include <SPIFFS.h>
#endif

#define FORMAT_IF_FAILED    true

namespace fs
{
#if defined (ESP32)
class TridentTD_SPIFFS : public SPIFFSFS , public Stream
#endif
#if defined (ESP8266)

#define FILE_READ       "r"
#define FILE_WRITE      "w"
#define FILE_APPEND     "a"

class TridentTD_SPIFFS : public FS , public Stream
#endif
{
  public:
    TridentTD_SPIFFS(FSImplPtr impl);
#if defined (ESP8266)
    bool begin();
    bool begin(bool formatOnFail);
    size_t totalBytes();
    size_t usedBytes();
#endif

    //---------------------------------------
    int8_t openFile(String filename);   // เปิดไฟล์สำหรับเขียน
    int8_t readFile(String filename);   // เปิดไฟล์สำหรับอ่าน
    void closeFile();                   // ปิดไฟล์
    // Print methods:
    size_t write(uint8_t) override;     // เขียนทีละ byte
    size_t write(const uint8_t *buf, size_t size) override;  // เขียนทีละ size
    void flush() override;

    // Stream methods:
    int available() override;           // เช็คว่าเหลือเท่าไหร่ตอนนี้
    int read() override;                // อ่านทีละ byte ออกมา พร้อมกับตำแหน่งเลื่อนไปตัวถัดไป
    int peek() override;                // เหมือน read() แต่ยังไม่ขยับตำแหน่งในการอ่าน

    size_t readBytes(char *buffer, size_t length);  // อ่านออกมาทีละหลาย byte
    size_t position() const;            // ตำแหน่งปัจจุบันของ file ที่เปิดอยู่
    bool   feof();                      // ตำแหน่งขณะนี้สิ้นสุด file หรือไม่
    void   rewind();                    // กลับไปเริ่มต้นไฟล์ใหม่
    //---------------------------------------

    void listDir(String dirname = "/");
    String readFiletoString(String path);
    void readFiletoStream(String path, Stream &stm);

    void writeFile(String path, String message);
    void appendFile(String path, String message);
    void renameFile(String path1, String path2);
    void deleteFile(String path);
    size_t filesize(String path);
  private:
#if defined (ESP32)
    void _listDir(fs::FS &fs, const char *dirname="/", uint8_t levels = 0);
#endif
    File   _file;
    size_t _size();
    int8_t _bFileOpened = 0;
    String verion = "1.0.0";
};
}

// #if defined (ESP32)
extern fs::TridentTD_SPIFFS td_SPIFFS;
// #endif

#endif //__TRIDENTTD_SPIFFS_H__
