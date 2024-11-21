import serial
import time
from pygame import mixer

# Initialize Serial communication (adjust COM port and baud rate as necessary)
ser = serial.Serial('COM9', 31250)  # Standard MIDI baud rate
time.sleep(2)  # Optional: Allow some time for the Serial connection to stabilize

# Initialize the audio system
mixer.init()

# Load sound files
sound_files = ["C.mp3", "D.mp3", "E.mp3", "F.mp3", "G.mp3", "A.mp3", "B.mp3"]
sounds = [mixer.Sound(file) for file in sound_files]

# Function to play a sound based on MIDI note and velocity
def play_sound(note, velocity):
    # Map MIDI note (e.g., 60 = Middle C) to sound index
    note_offset = 60  # Adjust this based on your mapping
    sound_index = note - note_offset
    if 0 <= sound_index < len(sounds):
        sound = sounds[sound_index]
        # Map MIDI velocity (0–127) to volume (0.0–1.0)
        volume = velocity / 127.0
        sound.set_volume(volume)
        sound.play()
    else:
        print(f"Note {note} is out of range")

# Main loop to read and process MIDI messages
while True:
    if ser.in_waiting > 0:  # Check for available data
        try:
            # Read one byte at a time
            byte = ser.read(1)
            status = ord(byte)  # Convert byte to integer

            # If it's a Note On or Note Off message
            if 0x80 <= status <= 0x9F:  # MIDI channel messages
                # Read additional bytes for note and velocity
                note = ord(ser.read(1))
                velocity = ord(ser.read(1))
                print(f"Received: {status} {note} {velocity}")

                if status & 0xF0 == 0x90 and velocity > 0:  # Note On
                    play_sound(note, velocity)
                elif status & 0xF0 == 0x80 or (status & 0xF0 == 0x90 and velocity == 0):  # Note Off
                    print(f"Note Off: {note}")

        except Exception as e:
            print(f"Error reading MIDI: {e}")
