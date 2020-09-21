#include <OneWire.h>
#include <DallasTemperature.h>
#include <LiquidCrystal_I2C.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define TEMPERATURE_PRECISION 12
#define PIN_RELAY 3

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);

int numberOfDevices; // Number of temperature devices found

DeviceAddress tempDeviceAddress; // We'll use this variable to store a found device address
LiquidCrystal_I2C lcd(0x27,16,2); //Initialize the lcd I2C address
String strOut;//declare the string which will be used to output 

void setup()
{
  pinMode(PIN_RELAY, OUTPUT); // Set pin 3 as output mode_relay module
  //initialize the 1602 lcd
  lcd.init();
  lcd.backlight();
  // start serial port
  Serial.begin(9600);
  //Serial.println("Dallas Temperature IC Control Library Demo");
  // Start up the library
  sensors.begin();
  
  // Grab a count of devices on the wire
  numberOfDevices = sensors.getDeviceCount();
  
  // locate devices on the bus
  Serial.print("Locating devices...");
  
  Serial.print("Found ");
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  //Serial.print("Parasite power is: "); 
  //if (sensors.isParasitePowerMode()) Serial.println("ON");
  //else Serial.println("OFF");
  
  // Loop through each device, print out address
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
        {
                Serial.print("Found device ");
                Serial.print(i, DEC);
                Serial.print(" with address: ");
                printAddress(tempDeviceAddress);
                Serial.println();
                
                Serial.print("Setting resolution to ");
                Serial.println(TEMPERATURE_PRECISION,DEC);
                
                // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
                sensors.setResolution(tempDeviceAddress, TEMPERATURE_PRECISION);
                
                Serial.print("Resolution actually set to: ");
                Serial.print(sensors.getResolution(tempDeviceAddress), DEC); 
                Serial.println();
        }else{
                Serial.print("Found ghost device at ");
                Serial.print(i, DEC);
                Serial.print(" but could not detect address. Check power and cabling");
        }

        lcd.setCursor(2, 0);
        String igem = "GSU iGEM";
        igem += "   T";
        igem += (i+1);
        lcd.print(igem);
        lcd.setCursor(1, 1);
        lcd.print("Initializing..");
        delay(250);
        lcd.clear();
  }

}

// function to print the temperature for a device
// NO LONGER USED, SAVED FOR BACKUP
void printTemperature(DeviceAddress deviceAddress)
{
  // method 1 - slower
  //Serial.print("Temp C: ");
  //Serial.print(sensors.getTempC(deviceAddress));
  //Serial.print(" Temp F: ");
  //Serial.print(sensors.getTempF(deviceAddress)); // Makes a second call to getTempC and then converts to Fahrenheit

  // method 2 - faster
  float tempC = sensors.getTempC(deviceAddress);
  Serial.print("Temp C: ");
  Serial.print(tempC);
  strOut += tempC;
  strOut += " °C";
  for (int i=0;i<strOut.length();i++)
    {
    lcd.write(strOut.charAt(i));
    }
  strOut = "";
  //Serial.print(" Temp F: ");
  //Serial.println(DallasTemperature::toFahrenheit(tempC)); // Converts tempC to Fahrenheit
}

void loop()
{ 
  // call sensors.requestTemperatures() to issue a global temperature 
  // request to all devices on the bus
  Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.println("DONE");
  
  
  // Loop through each device, print out temperature data
  for(int i=0;i<numberOfDevices; i++)
  {
    // Search the wire for address
    if(sensors.getAddress(tempDeviceAddress, i))
    {
        // Output the device ID
        Serial.print("Temperature for device: ");
        Serial.println(i,DEC);
                
        // It responds almost immediately. Let's print out the data
        float tempC = sensors.getTempC(tempDeviceAddress);
        Serial.print("Temp C: ");
        Serial.print(tempC);

        lcd.setCursor(2, 0);
        strOut = ("Tank");
        int ii = i+1;
        strOut += ii;
        strOut += " Temp:";
        lcd.print(strOut);
        strOut = "";
        lcd.setCursor(2, 1);
        strOut = tempC;
        strOut += " ~C";
        lcd.print(strOut);

        if(tempC <24)
        {
          digitalWrite(PIN_RELAY,HIGH);//trun on the heaters
        }
        else if(tempC>25.5)
        {
          digitalWrite(PIN_RELAY,LOW);//turn off the heaters
        }
        
        delay(1200);
        lcd.clear();
        strOut = "";
    }
    //else ghost device! Check your power requirements and cabling    
  }
  delay(500);
  Serial.println(":");
  Serial.println("~~");
  Serial.println("~~");
}

// function to print a device address
void printAddress(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
