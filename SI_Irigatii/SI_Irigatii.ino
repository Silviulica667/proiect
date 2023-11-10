//Libraries   
#include<Wire.h>
#include<Adafruit_Sensor.h>
#include<DHT.h>
#include<DHT_U.h>
#include<Adafruit_BMP085.h>
#include<LiquidCrystal_I2C.h>

//Pins declaration
#define DHTPIN 2
#define soil_pin A0 
#define gas_pin A1
#define ldr_pin A2
#define valve 3
#define DHTTYPE DHT11

//Sensor object
DHT_Unified dht(DHTPIN, DHTTYPE);
Adafruit_BMP085 bmp;

LiquidCrystal_I2C lcd(0x20,20,4);

//Declare data variables
int soil_value;
int gas_value;
int ldr_value;
uint32_t dedlayMS;
int delayMS;

void setup(){
  Serial.begin(9600);
  Serial.println("Sistem inteligent de irigatii");
  pinMode(valve, OUTPUT);
  lcd.init();
  lcd.backlight();

  dht.begin();
  if(!bmp.begin()){
    Serial.println("Could not find a valid sensor");
    while(1){}
  }
  else{
    Serial.println(F("Pressure sensor initialized"));
  }
  Serial.println(F("DHT11 Begin"));

  //DHT 11 sensor details

  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("----------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print(F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print(F("Driver Ver: ")); Serial.println(sensor.version);
  Serial.print(F("Unique ID: ")); Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value: ")); Serial.print(sensor.max_value); Serial.println(F("C"));
  Serial.print(F("Max Value: ")); Serial.print(sensor.min_value); Serial.println(F("C"));
  Serial.print(F("Max Value: ")); Serial.print(sensor.resolution); Serial.println(F("C"));
  

  //Print DHT11 humidity sensor
  dht.humidity().getSensor(&sensor);
  
  Serial.println(F("Humidity Sensor"));
  Serial.print(F("Sensor Type: ")); Serial.println(sensor.name);
  Serial.print(F("Driver Ver: ")); Serial.println(sensor.version);
  Serial.print(F("Unique ID: ")); Serial.println(sensor.sensor_id);
  Serial.print(F("Max Value: ")); Serial.print(sensor.max_value); Serial.println(F("C"));
  Serial.print(F("Max Value: ")); Serial.print(sensor.min_value); Serial.println(F("C"));
  Serial.print(F("Max Value: ")); Serial.print(sensor.resolution); Serial.println(F("C"));
 

  Serial.println("");
  delayMS = sensor.min_delay / 1000;

  lcd.clear();
}

void loop(){
  delay(delayMS);

  //Get temperature
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)){
    Serial.println(F("Error reading temperature"));
  }
  else{
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("C"));
  }

  //Get humidity
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)){
    Serial.println(F("Error reading humidity"));
  }
  else{
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
  }

  //Get pressure
   Serial.print(F("Pressure: "));
    Serial.print(bmp.readPressure());
    Serial.println(F("Pa"));

  //Get Soil Value
  soil_value = analogRead(soil_pin);
  soil_value = map(soil_value,0,1023,0,100);
  Serial.print("Moisture: ");
  Serial.print(soil_value);
  Serial.println("%");

  if(soil_value<25){
    digitalWrite(valve,HIGH);
  }
  else{
    digitalWrite(valve,LOW);
  }

  //Gas Sensor

  gas_value = analogRead(gas_pin);
  gas_value = map(gas_value,0,1023,100,0);
  Serial.print("Air Quality : ");
  Serial.print(gas_value);
  Serial.println("%");

  //LDR
  ldr_value = analogRead(ldr_pin);
  ldr_value = map(ldr_value, 0 ,1023, 0, 100);
  Serial.print("Light Intensity : ");
  Serial.print(ldr_value);
  Serial.println("%");

  Serial.println();
  Serial.println(F("----------------------------------"));
  Serial.println();

  //LCD display

  lcd.setCursor(0,0);
  lcd.print("T: ");
  lcd.print(event.temperature);
  lcd.print("C H: ");
  lcd.print(event.relative_humidity);
  lcd.print("%");


  lcd.setCursor(0,1);
  lcd.print("P: ");
  lcd.print(bmp.readPressure());
  lcd.print("PA AQ: ");
  lcd.print(gas_value);
  lcd.print("%");

  lcd.setCursor(0,2);
  lcd.print("Soil Moisture: ");
  lcd.print(soil_value);
  lcd.print("%");

  lcd.setCursor(0,3);
  lcd.print("Light Intensity: ");
  lcd.print(ldr_value);
  lcd.print("%");
}  
