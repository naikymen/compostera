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

// PPM
float mq2_slope = -1.511508654;
float mq2_intercept = 4.331343678;
float mq2_V0 = 4.90974609375;
float mq2_R0 = 33.6625714285715;
float mq2_RL = 2.47;
float Vref = 5.27;
int bitsADC = 10;


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
  Serial.print("MQ2 Gas ADC Signal: ");
  Serial.println(mq2Sensor);
  float mq2_Vs = (Vref - ((mq2Sensor * Vref) / pow(2, bitsADC))) ;
  float mq2_Rs = (mq2_RL * (mq2_Vs / (Vref - mq2_Vs)));
  float rs_ro = (mq2_Rs / mq2_R0);
  float log10ppm = ((mq2_slope * rs_ro) + mq2_intercept);
  float ppm = pow(10, log10ppm);
  Serial.print("MQ2 CH4 PPMs: ");
  Serial.print(ppm);
  Serial.println(" ppm");
  
  // 2D array
  //byte my2dArray[2][3]={
  //  {99,2,3},
  //  {2,3,4}
  //};
  //Serial.print(my2dArray[0][0]);
  
  // Ground mositure
  output_value= analogRead(sensor_pin);
  Serial.print("Solid moisture EC (% of max): ");
  Serial.println(output_value/1023);
  Serial.println("------------------------------------");
}
