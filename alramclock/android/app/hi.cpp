#include "DHT.h"

#define DHTPIN 2            // Pin where the DHT11 is connected
#define DHTTYPE DHT11       // DHT 11

#define RELAYPIN_BULB 7     // Pin where the IN2 of the relay controlling the bulb is connected
#define RELAYPIN_FAN 10      // Pin where the IN1 of the relay controlling the fan is connected
#define RELAYPIN_PUMP 9     // Pin where the relay controlling the water pump is connected

const int moisturePin = A0;  // Pin for the moisture sensor
DHT dht(DHTPIN, DHTTYPE);    // Declare DHT object

void setup() {
  Serial.begin(9600);
  dht.begin();  // Initialize DHT sensor
  
  // Set the relay pins as outputs
  pinMode(RELAYPIN_BULB, OUTPUT);
  pinMode(RELAYPIN_FAN, OUTPUT);
  pinMode(RELAYPIN_PUMP, OUTPUT);

  // Turn off the bulb, fan, and water pump initially (assuming active-low relays)
  digitalWrite(RELAYPIN_BULB, LOW); 
  digitalWrite(RELAYPIN_FAN, LOW);
  digitalWrite(RELAYPIN_PUMP, LOW); // Water pump relay default off
}

void loop() {
  // Read moisture level from the sensor
  int moistureLevel = analogRead(moisturePin);
  
  // Read temperature and humidity from DHT11 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  
  // Check if any reads failed
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // Print temperature and humidity readings to Serial Monitor
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" *C");
  }

  // Print the moisture level to the Serial Monitor
  Serial.print("Moisture Level: ");
  Serial.println(moistureLevel);

  // Control the water pump based on moisture level
  if (moistureLevel >= 500 && moistureLevel < 700) {
    digitalWrite(RELAYPIN_PUMP, LOW); // Turn on the water pump (active-high relay)
    Serial.println("Water pump ON");
  } else {
    digitalWrite(RELAYPIN_PUMP, HIGH);  // Turn off the water pump (active-high relay)
    Serial.println("Water pump OFF");
  }

  // Control the bulb based on temperature
  if (temperature > 20) {
    digitalWrite(RELAYPIN_BULB, LOW);  // Turn on the bulb (active-low relay)
    Serial.println("Temperature is below 30°C. Turning on the bulb.");
  } else {
    digitalWrite(RELAYPIN_BULB, HIGH); // Turn off the bulb
    Serial.println("Temperature is 30°C or higher. Turning off the bulb.");
  }

  // Control the fan based on temperature
  if (temperature > 20) {
    digitalWrite(RELAYPIN_FAN, HIGH);   // Turn on the fan (active-low relay)
    Serial.println("Temperature is above 30°C. Turning on the fan.");
  } else {
    digitalWrite(RELAYPIN_FAN, LOW);  // Turn off the fan
    Serial.println("Temperature is 30°C or below. Turning off the fan.");
  }

  // Wait for 2 seconds before the next reading
  delay(2000);
}