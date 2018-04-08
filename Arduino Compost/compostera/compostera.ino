#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            7         // Pin which is connected to the DHT sensor.
#define DHTTYPE           DHT22     // DHT 22 (AM2302)

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

int sensor_pin = A0;

int output_value ;

int smokeA0 = A5;

void setup() {
  Serial.begin(9600);
  
  // Initialize Humidity
  dht.begin();
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  dht.humidity().getSensor(&sensor);
  // Set delay between sensor readings based on sensor details.
  delayMS = sensor.min_delay / 1000;

  // Moisture
  Serial.begin(9600);

  // Smoke
  pinMode(smokeA0, INPUT);
}

void loop() {
  // Delay between measurements.
  delay(delayMS);
  // Get temperature event and print its value.
  sensors_event_t event;  
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println("Error reading temperature!");
  }
  else {
    Serial.print("Temperature: ");
    Serial.print(event.temperature);
    Serial.println(" °C");
  }
  // Get humidity event and print its value.
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println("Error reading humidity!");
  }
  else {
    Serial.print("Humidity: ");
    Serial.print(event.relative_humidity);
    Serial.println("%");
  }
  
  // Gas sensor
  int mq2Sensor = analogRead(smokeA0);
  float voltaje = mq2Sensor * (5.0 / 1023.0); //Convertimos la lectura en un valor de voltaje
  Serial.print("MQ2 Gas Signal: ");
  Serial.println(mq2Sensor);
  Serial.print("MQ2 Gas Voltage Signal: ");
  Serial.println(voltaje);
  
  // Ground mositure
  output_value= analogRead(sensor_pin);
  Serial.print("Moisture value: ");
  Serial.println(output_value);
  Serial.println("------------------------------------");
}
