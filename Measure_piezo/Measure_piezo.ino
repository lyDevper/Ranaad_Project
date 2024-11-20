char piezoPins[7] = { A0, A1, A2, A3, A4, A5, A6 };

struct Data {
  float voltage;
  float timeStamp;
};

float cycleTime = 200;                     // microsec
const int nSample = 50;                    //
float samplingTime = cycleTime * nSample;  // microsec

Data dataArr[nSample];
int idx = 0;
float peak = 0;

float lowerCut = 0.10;  // V
boolean isActive = false;
float timeStamp;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

float value;
float voltage;

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(A0);
  voltage = value * 5.0 / 1023.0;

  if (voltage > lowerCut && !isActive) {
    isActive = true;
    timeStamp = micros();
  }

  if (isActive) {
    if (voltage > peak)
      peak = voltage;
    dataArr[idx].voltage = voltage;
    dataArr[idx].timeStamp = micros() - timeStamp;
    idx++;

    if (micros() - timeStamp > samplingTime || idx >= nSample) {  // || voltage < lowerCut) {
      isActive = false;
      Serial.println(peak);
      Serial.println(idx);
      for (int i = 0; i < nSample; i++) {
        Serial.print(i);
        Serial.print("\t");
        Serial.print(dataArr[i].voltage);
        Serial.print("\t");
        Serial.println(dataArr[i].timeStamp);
      }

      idx = 0;
      peak = -1;
      for (int i = 0; i < nSample; i++) {
        dataArr[i] = {};
      }
    }
  }



  //Serial.print(value);
  //Serial.print("\t");
  //Serial.println(voltage);
  //delayMicroseconds(cycleTime);
}
