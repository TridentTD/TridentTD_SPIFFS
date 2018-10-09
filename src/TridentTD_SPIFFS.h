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
    void openFile(String filename);
    void readFile(String filename);
    void closeFile();
    // Print methods:
    size_t write(uint8_t) override;
    size_t write(const uint8_t *buf, size_t size) override;
    void flush() override;

    // Stream methods:
    int available() override;
    int read() override;
    int peek() override;

    size_t readBytes(char *buffer, size_t length);
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
    String verion = "1.0.0";
};
}

// #if defined (ESP32)
extern fs::TridentTD_SPIFFS td_SPIFFS;
// #endif

#endif //__TRIDENTTD_SPIFFS_H__
