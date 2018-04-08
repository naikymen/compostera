int sensor_pin = A0;

int output_value ;

void setup() {

Serial.begin(9600);

Serial.println("Reading From the Sensor ...");

delay(2000);

}

void loop() {

output_value= analogRead(sensor_pin);
 Serial.println(output_value);

if(output_value<700)
 {

Serial.println("Mositure");

}

delay(1000);

}
