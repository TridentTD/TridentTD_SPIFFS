#include <TridentTD_SPIFFS.h>

void setup() {
  Serial.begin(115200); Serial.println();
  // เริ่มต้นใช้งาน หากไม่ได้ ให้ format
  td_SPIFFS.begin(FORMAT_IF_FAILED);
//  td_SPIFFS.format();   // หากต้องการ format SPIFFS

  // แสดง file ใน SPIFFS ทั้งหมด
  td_SPIFFS.listDir();

  // เขียนข้อมูลลง file ใหม่
  td_SPIFFS.writeFile ("/hello.txt", "Hello ");

  // เขิยนข้อมูลต่อของเดิม ลงที่ file 
  td_SPIFFS.appendFile("/hello.txt", "World!\r\n");

  // อ่านข้อมูลจาก file
  String data = td_SPIFFS.readFiletoString("/hello.txt");
  Serial.println(data);

  // เปลี่ยนชื่อ file
  td_SPIFFS.renameFile("/hello.txt", "/foo.txt");

  // อ่านข้อมูลจาก file
  data = td_SPIFFS.readFiletoString("/foo.txt");
  Serial.println(data);

  // วิธี เขียน file อีกแบบ
  td_SPIFFS.openFile("/json_data.txt");   // เปิด file ที่ต้องการเขียน หากมีอยู่แล้วจะทำการเพิ่มต่อท้าย
  td_SPIFFS.println("{\"temp\": 23.5, \"humid\": 35}");
  td_SPIFFS.println("{\"temp\": 23.0, \"humid\": 36}");
  td_SPIFFS.println("{\"temp\": 24.1, \"humid\": 37}");
  td_SPIFFS.closeFile();

  // วิธีอ่าน file อีกแบบ
  td_SPIFFS.readFile("/json_data.txt");   // อ่าน file ที่ต้องการ
  while( td_SPIFFS.available())
    Serial.print((char) td_SPIFFS.read());
  Serial.println();
  td_SPIFFS.closeFile();

  // แสดงขนาดของ file ที่ต้องการ
  Serial.printf(" Size of /json_data.txt : %d\n", td_SPIFFS.filesize("/json_data.txt"));

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

