#include <TridentTD_SPIFFS.h>

void setup() {
  Serial.begin(115200); Serial.println();
  // เริ่มต้นใช้งาน หากไม่ได้ ให้ format
  td_SPIFFS.begin(FORMAT_IF_FAILED);

  // แสดง file ใน SPIFFS ทั้งหมด
  td_SPIFFS.listDir();

  // เขียนข้อมูลลง file ใหม่
  td_SPIFFS.writeFile ("/hello.txt", "Hello ");

  // เขิยนข้อมูลต่อของเดิม ลงที่ file 
  td_SPIFFS.appendFile("/hello.txt", "World!\r\n");

  // อ่านข้อมูลจาก file
  td_SPIFFS.readFile  ("/hello.txt");

  // เปลี่ยนชื่อ file
  td_SPIFFS.renameFile("/hello.txt", "/foo.txt");

  // อ่านข้อมูลจาก file
  String data = td_SPIFFS.readFile  ("/foo.txt");
  Serial.println(data);

  // แสดง file ใน SPIFFS ทั้งหมด
  td_SPIFFS.listDir();

  // แสดงขนาด SPIFFS ท้งหมดและ ที่ใช้ไป
  Serial.printf("Total : %d bytes ; Used : %d bytes\n",td_SPIFFS.totalBytes(), td_SPIFFS.usedBytes());

  // ลบ file ทิ้ง
  if( td_SPIFFS.exists("/foo.txt") )
    td_SPIFFS.deleteFile("/foo.txt");
}

void loop() {

}


