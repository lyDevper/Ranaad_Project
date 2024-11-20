// main function
int notePosition[7] = {0, 1, 0, 0, 0, 0, 0}; // พิน A0 ถึง A6
float volumeLevel = 20.00;

//WhatNote function
int noteCount = 7;
int notePlay = -1;

void setup() {
  Serial.begin(9600);  // เริ่มต้นการเชื่อมต่อ Serial
}

void loop() {
  WhatNoteandSentPlay(notePosition, volumeLevel);
  delay(1000);
}

void WhatNoteandSentPlay(int note[], float volume) {
  // ตรวจสอบโน้ตที่เล่น
  for (int i = 0; i < noteCount; i++) {
    if (note[i] == 1) { // ถ้าค่าของโน้ตที่ตำแหน่ง i เป็น 1
      notePlay = i;  // กำหนดหมายเลขโน้ตที่เล่น
      break;  // หยุดการวนลูปเมื่อเจอโน้ตที่ต้องการ
    }
  }
  Serial.print(notePlay);
  Serial.print(",");
  Serial.println(volume);
}


