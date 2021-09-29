/*
 Name:		Cayenne_MKR1010.ino
 Created:	9/29/2021 12:13:32 AM
 Author:	VALERY NGWA
*/

#define CAYENNE_PRINT Serial  // Comment this out to disable prints and save space
#include <CayenneMQTTWiFi101.h>
//#include <EEPROM.h>    // Library of Storage
#include <Adafruit_Sensor.h>  // Library of Adafruit sensors
#include "DHT.h"      // Library of DHT sensors
#include <math.h>


// WiFi network info.
char ssid[] = "ssid";
char wifiPassword[] = "wifiPassword";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

//Defining channels
#define Temp_Channel 0
#define Hum_Channel 1
#define Lum_Channel 2


//Defining quantity values

float Temp = 0.0;
float Hum = 0.0;
float Lum = 0.0;
const int thresholdvalue = 10;         //The treshold for which the LED should turn on. Setting it lower will make it go on at more light, higher for more darkness
float Rsensor;             //Resistance of sensor in K

//define pins
#define LIGHT_SENSOR A0    //Grove - Light Sensor is connected to A0 of Arduino
#define DHTPIN 8    // Change the pin to which the DHT is connected
const int ledPin = 12;                       //Connect the LED Grove module to Pin12, Digital 12


#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);



void setup() {
	Serial.begin(9600);
	dht.begin();
	/*for (int i = 0; i < 256; i++) {
		byte valeur = EEPROM.read(i);
		Serial.print(i);
		Serial.print(" = ");
		Serial.println(valeur);
	}*/
	pinMode(ledPin, OUTPUT);         //Set the LED on Digital 12 as an OUTPUT
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
	Read_TempAndHum();
	Read_Luminosity();
	Cayenne.loop();
}

//Function to read temperature and humidity
void Read_TempAndHum() {
	Hum = dht.readHumidity();  // Gets the values of the humidity
	Temp = dht.readTemperature(); // Gets the values of the temperature
	float c = 100 - Hum;
	float b = c / 5;
	float T = Temp - b;
	Serial.print("Temperature = ");
	Serial.print(Temp);
	Serial.print(" *C ");
	Serial.print(" Humidity = ");
	Serial.print(Hum);
	Serial.print(" % ");
	Serial.print(" The value of the Formula = ");
	Serial.print(T);
	delay(2000);               // Delays 2 seconds, as the DHT22 sampling rate is 0.5Hz
}

//function to read luminosity
void Read_Luminosity() {
	int sensorValue = analogRead(LIGHT_SENSOR);
	Rsensor = (float)(1023 - sensorValue) * 10 / sensorValue;
	Serial.println("the analog read data is ");
	Serial.println(sensorValue);
	Serial.println("the sensor resistance is ");
	Serial.println(Rsensor, DEC);   //show the ligth intensity on the serial monitor;
	delay(200);               // Delays 2 seconds, as the DHT22 sampling rate is 0.5Hz
}

//Cayenne function to send data
CAYENNE_OUT_DEFAULT()
{
	Cayenne.virtualWrite(Temp_Channel, Temp, TYPE_TEMPERATURE , UNIT_CELSIUS);
	Cayenne.virtualWrite(Hum_Channel, Hum, TYPE_RELATIVE_HUMIDITY, UNIT_PERCENT);
	Cayenne.virtualWrite(Lum_Channel, Lum, TYPE_LUMINOSITY, UNIT_LUX);
}
