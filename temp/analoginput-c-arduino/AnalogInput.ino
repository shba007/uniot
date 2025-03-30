const int moistureSensorPin = 34;

int moistureSensorValue = 0;

void setup()
{
	Serial.begin(115200);
	delay(1000);
}

void loop()
{
	moistureSensorValue = analogRead(moistureSensorPin);

	Serial.print("\nSensor Value: ");
	Serial.print(moistureSensorValue);

	moistureSensorValue = map(analogRead(moistureSensorPin), 4095, 500, 0, 100);

	Serial.print("\tMap Sensor Value: ");
	Serial.print(moistureSensorValue);

	delay(100);
}
