#include <MIDI.h>

// Create a MIDI instance
MIDI_CREATE_DEFAULT_INSTANCE();

const int nNote = 7;
const char piezoPins[7] = { A0, A1, A2, A3, A4, A5, A6 };
bool disablePins[7] = { 0, 0, 0, 0, 0, 0, 0 };

const int nSample = 50;      // max sample size
float samplingTime = 10000;  // microsec

float lowerCut = 0.100;   // V
float offTime = 100000;  // microsec // min time before next hit
float gain = 1.5;
float offset = 12;

struct PinState {
  bool isActive = false;
  float peak = 0;
  float timeStampHit;
  float dataArr[nSample];
  int idx = 0;
};
PinState pinStates[nNote];

float noteValues[nNote] = { 0, 0, 1, 1, 1, 1, 1 };

void setup() {
  //Serial.begin(115200);       // Start serial communication
  MIDI.begin(MIDI_CHANNEL_OMNI); // Initialize MIDI on all channels
}

float value;
float voltage;

void loop() {
  for (int i = 0; i < nNote; i++) {
    noteValues[i] = 0;
  }
  readPiezoes();
  //sendNotePlay();       // For Serial output
  sendNotePlay_MIDI();  // For MIDI output
}

void readPiezoes() {
  for (int i = 0; i < nNote; i++) {
    if (disablePins[i]) {
      continue;
    }

    float value = analogRead(piezoPins[i]);
    float voltage = value * 5.0 / 1023.0;

    if (voltage > lowerCut && !pinStates[i].isActive) {
      if (micros() - pinStates[i].timeStampHit <= offTime) {
        continue;
      }
      pinStates[i].isActive = true;
      pinStates[i].timeStampHit = micros();
    }

    if (pinStates[i].isActive) {
      pinStates[i].peak = max(pinStates[i].peak, voltage);
      pinStates[i].dataArr[pinStates[i].idx] = voltage;
      pinStates[i].idx++;

      if (micros() - pinStates[i].timeStampHit > samplingTime || pinStates[i].idx >= nSample) {
        noteValues[i] = voltToVolume(pinStates[i].peak);

        pinStates[i].isActive = false;
        pinStates[i].idx = 0;
        pinStates[i].peak = 0;
        for (int j = 0; j < nSample; j++) {
          pinStates[i].dataArr[j] = 0;
        }
      }
    }
  }
}

float voltToVolume(float volt) {
  float percent = volt * 100.0 / 5.0;
  return min(percent * gain + offset, 100.0);
}

// Existing Serial output function
void sendNotePlay() {
  for (int i = 0; i < nNote; i++) {
    if (noteValues[i] > 0) {
      Serial.print(i);  // Note to play
      Serial.print(",");
      Serial.println(noteValues[i]);  // Volume
    }
  }
}

// New MIDI output function
void sendNotePlay_MIDI() {
  for (int i = 0; i < nNote; i++) {
    if (noteValues[i] > 0) {
      int midiNote = 60 + i; // Map to MIDI notes (C4 + index)
      int velocity = map(noteValues[i], 0, 100, 0, 127); // Scale to MIDI velocity range (0-127)

      MIDI.sendNoteOn(midiNote, velocity, 1); // Send Note On message
      MIDI.sendNoteOff(midiNote, 0, 1);      // Immediately send Note Off message
    }
  }
}
