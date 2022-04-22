#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal.h>

#define PIN 2	 		
#define SENSOR A0		
#define NUMPIXELS 1 	

const int motor = 13;
const int LedRed = 12;
const int LedGreen = 11;

LiquidCrystal lcd(3, 4, 5, 6, 7, 8);

#define dryThreshold 50 	 
#define wetThreshold 200 	 
#define thresholdCenter (dryThreshold + wetThreshold)/2    
#define crossFade 20 		 

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int delayval = 100; 	

int redColor = 0;
int greenColor = 0;
int blueColor = 0;


int sensorValue  = 0;
int transitionValue = 0;

void setup() 
{
  	pixels.begin();
  	pinMode(SENSOR,INPUT);
	Serial.begin(9600);
  	lcd.begin(16, 2);
  	lcd.print("   Automated");
  	lcd.setCursor(0,1);
 	lcd.print("Irrigation System");
	pinMode(motor, OUTPUT);
  	pinMode(LedRed, OUTPUT);
  	pinMode(LedGreen, OUTPUT);
  	delay(4600);
  	lcd.clear();
  	lcd.print("Blue=WET, Red=DRY");
  	lcd.setCursor(0,1);
  	lcd.print("Water Pump =");
}

void loop()
{
	sensorValue = analogRead(SENSOR);
	transitionValue = map(sensorValue,0,1023,0,255);
	setColor();
    pixels.setPixelColor(0,redColor,greenColor,blueColor);
    pixels.show();
    delay(delayval); 
  	
  	if (redColor)
  	{
    	digitalWrite(motor, HIGH);
    	digitalWrite(LedRed, LOW);
    	digitalWrite(LedGreen, HIGH);
      	lcd.setCursor(13,1);
    	lcd.print("ON ");
  	}
  	else 
  	{
  	 	digitalWrite(motor, LOW);
  	  	digitalWrite(LedRed, HIGH);
    	digitalWrite(LedGreen, LOW);
        lcd.setCursor(13,1);
    	lcd.print("OFF");
  }
   delay(1000);
}

void setColor()
{
	redColor = ((transitionValue <= dryThreshold + crossFade) && (transitionValue >= 0 ))? map(transitionValue,0,dryThreshold + crossFade,255,0) : 0;
	blueColor = (transitionValue >= wetThreshold - crossFade && transitionValue <= 255)? map(transitionValue,wetThreshold - crossFade,255,0,255):0; 
  	if(transitionValue >= dryThreshold && transitionValue <= thresholdCenter)
  		greenColor = map(transitionValue,dryThreshold,thresholdCenter,0,255);

  	else if(transitionValue > thresholdCenter && transitionValue < wetThreshold)
    	greenColor = map(transitionValue,dryThreshold,thresholdCenter,255,0);

    else
    	greenColor = 0;              
}
