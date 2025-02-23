#include <SPI.h>
#include <Adafruit_MAX31865.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// MAX31865 configuration
#define MAX31865_CS_PIN 4    // GPIO15 (was D2)
#define RTD_NOMINAL 100.0     // PT100
#define RREF 430.0            // Reference resistor
#define ONE_WIRE_BUS 2  // D4 corresponds to GPIO2 on the ESP8266

Adafruit_MAX31865 max31865 = Adafruit_MAX31865(MAX31865_CS_PIN);
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

void setup() {
  // Initialize Serial communication at 115200 baud
  Serial.begin(115200);

  // Initialize MAX31865
  max31865.begin(MAX31865_2WIRE);

  Serial.println("ESP8266 is ready.");
  sensors.begin();
  Serial.println("Dallas sensor is ready.");
}

void loop() {
  // Check if data is available on Serial
  if (Serial.available() > 0) {
    // Read the incoming byte
    char received = Serial.read();

    // If the received byte is 'R', send the temperature
    if (received == 'R') {
      // Read temperature
      float temperature = max31865.temperature(RTD_NOMINAL, RREF);
      sensors.requestTemperatures();
      
      // Get current time in milliseconds since the program started
      unsigned long currentTime = millis();

      // Send timestamp and temperature over Serial
      Serial.print(currentTime);
      Serial.print(",");
      Serial.print(temperature, 2);
      Serial.print(",");
      Serial.println(sensors.getTempCByIndex(0));
    } else if (command == "*IDN?") {
        Serial.println("ROSATECH,TSN100,P0000002,1.0.1");
    }
  }

  // Small delay to prevent excessive CPU usage
  delay(10);
}


