// const int ledPin = 13;

/* const int moistureSensorPin = D15; */
const int luminositySensorPin = A0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // pinMode(ledPin, OUTPUT);
}

void loop() {
/*   int moistureSensorValue = analogRead(moistureSensorPin);
  int moistureValue = map(moistureSensorValue, 106, 482, 100, 0); // map(moistureSensorValue, 400, 535, 100, 0); */
  int luminositySensorValue = analogRead(luminositySensorPin);
  int luminosityValue = map(luminositySensorValue, 14, 1014, 100, 0);

  // put your main code here, to run repeatedly:
  // digitalWrite(ledPin, HIGH);
  Serial.print(luminositySensorValue);
  Serial.print(" Value -> ");
  Serial.print(luminosityValue);
  Serial.println("%");
  delay(2000);
}
