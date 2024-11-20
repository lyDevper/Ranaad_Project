char piezoPins[7] = { A0, A1, A2, A3, A4, A5, A6 };
float value;
float voltage;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  value = analogRead(A0);
  voltage = value * 5 / 1023;
  Serial.print(value);
  Serial.print("\t");
  Serial.println(voltage);
  delay(0.1);
}
