#import serial
import time
from pygame import mixer

time.sleep(2)

# เริ่มต้นการตั้งค่าระบบเสียง
mixer.init()

# โหลดไฟล์เสียงทั้งหมดไว้ล่วงหน้า
sound_files = ["C.mp3", "D.mp3", "E.mp3", "F.mp3", "G.mp3", "A.mp3", "B.mp3"]
sounds = [mixer.Sound(file) for file in sound_files]

# ฟังก์ชันสำหรับปรับความดังเสียง
def play_sound(note, volume_level):
    if 0 <= note < len(sound_files):
        sound_play = sounds[note]
        # ปรับระดับเสียง (0.0 ถึง 1.0)
        sound_play.set_volume(volume_level / 100.0)  
        # เล่นเสียง
        sound_play.play()
        print(f"Playing sound from {sound_play} at volume {volume_level}")
    else:
        print("Invalid Note")

volume_level = 75

for note in range(7):
    print(f"Playing note {note}")
    play_sound(note, volume_level)
time.sleep(1.5)

# tuples for note to be played at the same time
music = [(0, 1), (1, 2), (2, 3), (3, 4), (4, 5), (5, 6), (2, 4), (3, 5), (4, 6), (0, 2), (1, 3), (1, 6), (2, 5), (3, 4)]
for note in music:
    print(f"Playing notes {note[0]} and {note[1]}")
    play_sound(note[0], volume_level)
    play_sound(note[1], volume_level)
    time.sleep(1.5)