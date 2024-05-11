#include <MQ135.h>

#include <MQ2.h>
#include <DHT.h>
#include <DHT_U.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// time in ms to print the data to the serial
unsigned long time1 = 0;

// dust sensor
// the measure pin of the dust sensor
int dustmesurepin = A3;
// the led pin of the dust sensor
int ledpower = 3;
// declare the volt 
float vomesure;
float calcvoltage;
// declare the dust value
float dust;


// set buzzer pin
int buzzer = 9;

// Define the analog pin for the MQ135 sensor
const int mq135Pin = A1;
MQ135 mq135_sensor(mq135Pin);
#define co2Zero     20 
float co2lvl;

// the AQi 
float sensorvalue;


// Define the type of the DHT sensor
#define Type DHT11
// initialize the sense pin
int sensPin = 2;
// tell the dht library the sensepin and the type of the sensor
DHT HT(sensPin, Type);
// declare the temp and humid variables
float tempc;
float humid;

// Set the I2C address (check your module documentation)
#define I2C_ADDR 0x27

// Set the LCD dimensions (usually 16x2)
#define LCD_COLS 16
#define LCD_ROWS 2

// seth the analog pin for the gas sensor
int Analog_Input = A0;
// declare the gases
int lpg, co, smoke;

// start the mq2 sensor
MQ2 mq2(Analog_Input);

// start the lcd screen
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COLS, LCD_ROWS);

void setup() {

  // set the pin mode of the buzzer
  pinMode(buzzer, OUTPUT);

  Serial.begin(9600);  // Initialize serial communication for debugging
  // delay so that the serial doesn't print random numbers
  delay(1500);
  // print the columns names that will be written in the csv file
  Serial.println("Date&Time, Temperature, Humidity, LPG, CO, Smoke, AQI, Dust");
  // set the time between each reading
  time1 = millis();


  lcd.begin(LCD_COLS, LCD_ROWS);  // Initialize the LCD with the specified columns and rows
  lcd.backlight();  // Turn on the backlight

  // set the pin of the mq135 sensor to input 
  pinMode(mq135Pin, INPUT);

  // tell the dht lib to begin
  HT.begin();

  // begin the mq2 
  mq2.begin();
}

void loop() {
  // Read from the dht sensor the temp and humid
  tempc = HT.readTemperature();
  humid = HT.readHumidity(); 


  // float rzero = mq135_sensor.getRZero();
  // float correctedRZero = mq135_sensor.getCorrectedRZero(tempc, humid);
  // float resistance = mq135_sensor.getResistance();
  // float ppm = mq135_sensor.getPPM();
  // float correctedPPM = mq135_sensor.getCorrectedPPM(tempc, humid);

  // Serial.print("MQ135 RZero: ");
  // Serial.print(rzero);
  // Serial.print("\t Corrected RZero: ");
  // Serial.print(correctedRZero);
  // Serial.print("\t Resistance: ");
  // Serial.print(resistance);
  // Serial.print("\t PPM: ");
  // Serial.print(ppm);
  // Serial.print("\t Corrected PPM: ");
  // Serial.print(correctedPPM);
  // Serial.println("ppm");

  // int co2now[10];                               //int array for co2 readings
  // int co2raw = 0;                               //int for raw value of co2
  // int co2ppm = 0;                               //int for calculated ppm
  // int zzz = 0;                                  //int for averaging

  // for (int x = 0;x<10;x++)  //samplpe co2 10x over 2 seconds
  // {                   
  //   co2now[x]=analogRead(A1);
  //   delay(200);
  // }

  // for (int x = 0;x<10;x++)  //add samples together
  // {                     
  //   zzz=zzz + co2now[x];  
  // }
  
  // co2raw = zzz/10;                            //divide samples by 10
  // co2ppm = co2raw - co2Zero;                 //get calculated ppm

  // Serial.print("AirQuality=");
  // Serial.print(co2raw);  // prints the value read
  // Serial.println(" PPM");
  // delay(50);             

  // get the output value of the mq135 sensor
  sensorvalue = analogRead(mq135Pin);

  // co2lvl = sensorvalue - 120;
  // co2lvl = map(co2lvl, 0, 1024, 400, 5000);
  // Serial.println("co2 level:");
  // Serial.println(co2lvl);

  // Serial.println(sensorvalue);

  // set tthe cursor of the lcd to the first pixel of the screen 
  lcd.setCursor(0, 0);
  // print to the lcd screen
  lcd.print("Temperature: ");
  
  // go to the next row of the screen
  lcd.setCursor(0, 1);
  // lcd.print("   ");
  // lcd.setCursor(0, 1);
  // print the value of the tempreature
  lcd.print(tempc);
  // print "C" like "temp: 20C"
  lcd.print("C  ");
  // keep displaying this for 4s
  delay(4000);
  // reset the position of writing to the start of the screen
  lcd.setCursor(0, 0);
  // clear the previous writings
  lcd.print("             ");
  // reset the position again
  lcd.setCursor(0, 0);
  // write the humidity
  lcd.print("Humidity: ");
  lcd.setCursor(0, 1);
  lcd.print("            ");
  lcd.setCursor(0, 1);
  humid = HT.readHumidity(); 
  lcd.print(humid);
  lcd.print("%");


  // if the humidity is more than 80% turn on the buzzer
  if (humid > 80.00){
  // turn on the buzzer with value of 10
  analogWrite(buzzer, 10);
  // keep the buzzer running for 3s
  delay(3000);
  // turn off the buzzer
  analogWrite(buzzer, 0);
  // set cursor of the lcd screen to the start and notify the user
  lcd.setCursor(0, 0);
  lcd.print("Humidity is high");
  }
  // if tempreature is more than 40C trun on the alarm
  if (tempc > 40.00){
  // keep the buzzer working for 3s
  analogWrite(buzzer, 10);
  delay(3000);
  // turn it off
  analogWrite(buzzer, 0);
  // alert the user
  lcd.setCursor(0, 0);
  lcd.print("Temp is high");
  }

  delay(4000);  // Wait for a moment to make the display readable

  float* values= mq2.read(false); //set it false if you don't want to print the values in the Serial
  //lpg = values[0];
  lpg = mq2.readLPG();
  //co = values[1];
  co = mq2.readCO();
  //smoke = values[2];
  smoke = mq2.readSmoke();
  // clear the screen and print the values of lpg co and smoke
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,0);
  lcd.print("LPG:");
  lcd.print(lpg);
  lcd.print(" CO:");
  lcd.print(co);
  lcd.setCursor(0,1);
  lcd.print("                  ");
  lcd.setCursor(0,1);
  lcd.print("SMOKE:");
  lcd.print((smoke*100)/1000000);
  lcd.print(" %");
  // keep them displayed for 4s
  delay(4000);

  // if any of the gases if detected alert the user
  if (lpg > 5.00 || lpg < 0.00){
    analogWrite(buzzer, 10);
    delay(3000);
    analogWrite(buzzer, 0);
    lcd.setCursor(0, 0);
    lcd.print("                 ");
    lcd.setCursor(0, 0);
    lcd.print("gas is high");
  }
  if (co > 10.00 || co < -10.00){
    analogWrite(buzzer, 10);
    delay(3000);
    analogWrite(buzzer, 0);
    lcd.setCursor(0, 0);
    lcd.print("                 ");
    lcd.setCursor(0, 1);
    lcd.print("         ");
    lcd.setCursor(0, 0);
    lcd.print("gas is high");
    delay(5000);

  }
  if ((smoke*100)/1000000 > 10.00 || (smoke*100)/1000000 < -10.00){
    analogWrite(buzzer, 15);
    lcd.setCursor(0, 0);
    delay(4000);
    analogWrite(buzzer, 0);
    lcd.setCursor(0, 0);
    lcd.print("                 ");
    lcd.setCursor(0, 1);
    lcd.print("         ");
    lcd.setCursor(0, 0);
    lcd.print("smoke is high");
    delay(5000);
  }
  
  // set the cursor back and print the aqi
  lcd.setCursor(0, 0);
  lcd.print("               ");
  lcd.setCursor(0, 0);
  lcd.print("Air quality:");
  lcd.print(sensorvalue);
  lcd.print("             ");
  lcd.setCursor(0, 1);
  lcd.print("          ");

  // if the aqi is more than 30 and less than 60 then the air quality is moderate
  if (sensorvalue > 30.00 && sensorvalue < 60.00){
    lcd.setCursor(0, 1);
    lcd.print("Moderate");
    // alert the user for 3s
    analogWrite(buzzer, 5);
    delay(3000);
    analogWrite(buzzer, 0);
  } else if (sensorvalue > 60.00){
    // if the value is more than 60 then the air quality is bad and aleart the user
    lcd.setCursor(0, 1);
    lcd.print("Bad!!");
    // increase the sound of the buzzer than the moderate aqi alarm
    analogWrite(buzzer, 10);
    delay(4000);
    analogWrite(buzzer, 0);
  } else if (sensorvalue < 30.00 && sensorvalue > 0.00){
    // if the value is less than 30 then the air is good
    lcd.setCursor(0, 1);
    lcd.print("Good!!");
  }
  // keep the readings displayed for 4s
  delay(4000);
  // set the cursor back and clear the screen
  lcd.setCursor(0, 0);
  lcd.print("                 ");

  // THE DUST SENSOR
  // turn of the led of the dust led sensor
  digitalWrite(ledpower, LOW);
  // delay for the sensor to measure the particles in the air 
  delayMicroseconds(280); 
  //delta time 40 sleep time 9680
  // read the voltage outputed from the sensor
  vomesure = analogRead(dustmesurepin);
  delayMicroseconds(40);
  // turn on the led
  digitalWrite(ledpower, HIGH);
  delayMicroseconds(9680);
  // convert the value from voltage to analog
  calcvoltage = vomesure * (5.0 / 1024.0);
  // the eq of the dust
  dust = 170 * calcvoltage - 0.1;
  // print the dust value to the screen
  lcd.setCursor(0, 0);
  lcd.print("             ");
  lcd.setCursor(0, 0);
  lcd.print("Dust: ");
  lcd.print(dust);
  lcd.setCursor(0, 1);
  lcd.print("                 ");
  lcd.setCursor(0, 1);
  // the values sheet of the sensor
  if (dust < 50.00){
    lcd.print("Excelent");
  } else if (dust > 51.00 && dust < 100.00){
    lcd.print("Average");
  } else if (dust > 101.00 && dust < 150.00){
    lcd.print("Light");
  } else if (dust > 151.00 && dust < 200.00){
    lcd.print("Moderate");
  } else if (dust > 201.00 && dust < 300.00){
    lcd.print("Heavy!!");
    analogWrite(buzzer, 10);
    delay(4000);
    analogWrite(buzzer, 0);
  } else if (dust > 300.00){
    lcd.print("Serious!!");
    analogWrite(buzzer, 15);
    delay(4000);
    analogWrite(buzzer, 0);
  }
  delay(4000);

  // print to the serial that saves the readings to the gile evrey 1s but it will be longer becuase of the delay of the previous code
  if((unsigned long)millis() - time1 > 1000)
  {
    // print coma for the formatting of the csv file
    Serial.print(",");
    // print to the serial the value of temperature
    Serial.print(tempc);
    Serial.print(",");
    // the humidity
    Serial.print(humid);
    Serial.print(",");
    // the gases lpg, co, smoke
    Serial.print(lpg);
    Serial.print(",");
    Serial.print(co);
    Serial.print(",");
    Serial.print(smoke);
    Serial.print(",");
    // the AQI
    Serial.print(sensorvalue);
    Serial.print(",");
    // the dust but print it in a new line so that the next reading is written on the next row
    Serial.println(dust);
    // reset the time
    time1 = millis();
  }

}

