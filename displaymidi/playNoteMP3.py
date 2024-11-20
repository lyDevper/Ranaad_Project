import serial
import time
from pygame import mixer

# กำหนดค่าการเชื่อมต่อ Serial
ser = serial.Serial('COM7', 115200)  # เปลี่ยน COM เป็นพอร์ตที่ Arduino เชื่อมต่ออยู่
time.sleep(2)  # รอเวลาเพื่อให้ Serial พร้อมใช้งาน >> จำเป็นมั้ย

# เริ่มต้นการตั้งค่าระบบเสียง
mixer.init()

# ฟังก์ชันสำหรับปรับความดังเสียง
def play_sound(pin, volume_level):
    sound_files = ["C.mp3", "D.mp3", "E.mp3", "F.mp3", "G.mp3", "A.mp3", "B.mp3"]
    # ตรวจสอบว่าหมายเลขปุ่มอยู่ในช่วงที่กำหนด (0-6)
    if 0 <= pin < len(sound_files):
        sound_file = sound_files[pin]
        print(f"Playing sound from {sound_file} at volume {volume_level}")
        # โหลดไฟล์เสียง
        mixer.music.load(sound_file)
        # ปรับระดับเสียง
        mixer.music.set_volume(volume_level / 100.0)  # ความดังอยู่ในช่วง 0.0 - 1.0
        # เล่นเสียง
        mixer.music.play()
        # รอจนกว่าเพลงจะเล่นเสร็จ
        while mixer.music.get_busy():  # เช็คว่าเพลงยังเล่นอยู่หรือไม่
            pass
    else:
        print("Invalid Note")

while True:
    if ser.in_waiting > 0:  # Check if data is available from Arduino
        try:
            # Read data from Serial
            dataRead = ser.readline().decode('utf-8').strip()
            print(f"Received data: {dataRead}")
            
             # แยกข้อมูล Note และ Volume โดยใช้เครื่องหมายจุลภาคเป็นตัวคั่น
            note_data, volume_data = dataRead.split(",")  # แยกข้อมูล Note และ Volume

            # แปลงข้อมูลที่แยกได้เป็นประเภทข้อมูลที่เหมาะสม
            note = int(note_data)  # ใช้ค่าตัวเลขแรกเป็นหมายเลขพิน
            volume_level = float(volume_data)  # ใช้ค่าตัวเลขหลังเป็นระดับเสียง
            print(f"Pin: {note}, Volume Level: {volume_level}")
            
            # Call the function to play sound based on note and volume
            play_sound(note, volume_level)
        
        except UnicodeDecodeError:
            print("Error: Unable to decode data from Serial.")
