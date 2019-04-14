/*
  This is my most recent  Garage Door Opener sketches using
  the ESP32 Devkit C board and the Blynk framework. It measures the
  distance from the garage door using ultrasonic sensors to see if it
  is open. It will also message and remind via Blynk when the door 
  is left open for too long 
*/

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Blynk.h>
/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

char auth[] = "<MY-Blynk-Project-auth-code>";
char ssid[] = "<MY-SSID>";
char pass[] = "<MY-WIFI-PWD>";

WidgetLCD lcd(V3);

#define relayPin 32    //Pin to trigger relay
#define pushPin V7      //Virtual Pin for Blynk button
#define distancePin V5  //Virtual Pin for recording distance 

#define trigPin 14     //Pin to trigger ultrasonic sensor
#define echoPin 25     //Pin to read ultrasonic sensor echo
long duration;
int distance;

int warnCount = 0;
int warnThreshold = 300;  // 300 seconds before first warning (5 mins)

BlynkTimer timer;

void setup()
{
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(relayPin, OUTPUT);       // Pin Connected to the Relay control board
  Serial.begin(115200);

  Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //e.g.
  //Blynk.begin(auth, ssid, pass, "45.55.96.146", 8442);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8442);

  timer.setInterval(1000L, checkDistance);
}


BLYNK_WRITE(pushPin) // reads V7 from the Blynk App/IFTTT webhook and holds it high for half a second, then returns it to low
{
  if (param.asInt() == 1)
  {
    digitalWrite(relayPin, HIGH); //GPIO32  sets the digital relay pin high
    timer.setTimeout(500L, []() {   //commence timer - half a second
      digitalWrite(relayPin, LOW); // GPIO32   sets the digital relay pin low
      Blynk.virtualWrite(pushPin, 0);  //sets the virtual pin V7 low
    });  // END Timer Function
  }
}

// check the distance to the door
void checkDistance()
{

  //Read Ultrasonic
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;

  Blynk.virtualWrite(distancePin, distance);
  if (distance < 40) {
    lcd.print(0, 0, "Door Open  ");
    warnCount++;

  } else {
    lcd.print(0, 0, "Door Closed");
    warnCount = 0;
  }
  if (warnCount > warnThreshold) {
    Blynk.notify("Garage door is open!");
    warnCount = (warnCount - 180);
  }

}


void loop()
{
  Blynk.run();
  timer.run();
}
