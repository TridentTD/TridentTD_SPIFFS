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
class TridentTD_SPIFFS : public SPIFFSFS 
#endif
#if defined (ESP8266)
class TridentTD_SPIFFS : public FS 
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
    void listDir(String dirname = "/");
    void readFile(String path);
    void readFiletoStream(String path, Stream &stm);

    void writeFile(String path, String message);
    void appendFile(String path, String message);
    void renameFile(String path1, String path2);
    void deleteFile(String path);
  private:
#if defined (ESP32)
    void _listDir(fs::FS &fs, const char *dirname="/", uint8_t levels = 0);
#endif
};
}

// #if defined (ESP32)
extern fs::TridentTD_SPIFFS td_SPIFFS;
// #endif

#endif //__TRIDENTTD_SPIFFS_H__
