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

float noteValues[nNote] = { 0, 0, 0, 0, 0, 0, 0 };

void setup() {
  Serial.begin(115200);
}

float value;
float voltage;

void loop() {
  for (int i = 0; i < nNote; i++) {
    noteValues[i] = 0;
  }
  readPiezoes();
  sendNotePlay();
  /*
  for (int i = 0; i < nNote; i++) {
    Serial.print(noteValues[i]);
    Serial.print(",");
  }
  Serial.println();
  */
}

void readPiezoes() {
  // to be called in 1 cycle
  for (int i = 0; i < nNote; i++) {
    if (disablePins[i]) {
      continue;
    }

    float value = analogRead(piezoPins[i]);
    float voltage = value * 5.0 / 1023.0;

    // first hit; start of signal
    if (voltage > lowerCut && !pinStates[i].isActive) {
      if (micros() - pinStates[i].timeStampHit <= offTime) {
        continue;
      }
      pinStates[i].isActive = true;
      pinStates[i].timeStampHit = micros();
    }

    // during signal change
    if (pinStates[i].isActive) {
      pinStates[i].peak = max(pinStates[i].peak, voltage);
      pinStates[i].dataArr[pinStates[i].idx] = voltage;
      pinStates[i].idx++;

      // end of signal
      if (micros() - pinStates[i].timeStampHit > samplingTime || pinStates[i].idx >= nSample) {
        noteValues[i] = voltToVolume(pinStates[i].peak);

        pinStates[i].isActive = false;
        // reset for next hit
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
  // discetize into 10 levels starting from lowerCut
  /*
  if (percent < 10) {
    return 0;
  }
  else if (percent < 20) {
    return 20;
  } 
  else if (percent < 30) {
    return 30;
  } 
  else if (percent < 40) {
    return 40;
  } 
  else if (percent < 50) {
    return 50;
  } 
  else if (percent < 60) {
    return 60;
  } 
  else if (percent < 70) {
    return 70;
  } 
  else if (percent < 80) {
    return 80;
  } 
  else if (percent < 90) {
    return 90;
  } 
  else {
    return 100;
  }
  */
  return min(percent * gain + offset, 100.0);
}

void sendNotePlay() {
  for (int i = 0; i < nNote; i++) {
    if (noteValues[i] > 0) {
      Serial.print(i);  // note to play
      Serial.print(",");
      Serial.println(noteValues[i]);  // volume
    }
  }
}