
URL:# https://github.com/StudioShemp/Garage-Door-Controller

Garage-Door-Controller.
ESP32 Garage Door controller with Blynk integration and IFTTT/Google Home capability
ESP32 Sketch for controlling my garage door.

In the Blynk App:
V7 is configured as a push switch - 0/1    (press to open / close door)

V3 is configured as an Advanced LCD      (shows door state - open/closed)

V5 is configured as a Value Display          (shows the distance in cm from the ceiling to the garage door - <40cm when open)

The ESP32 module triggers a relay via GPO Pin 34 

It measures distance using a standard ultrasonic sensor module connected to pins 14 (trigger) and 25 (echo)

Using IFTTT as a Google assistant & webhook combo to trigger the door:

In IFTTT This select Google Assistant > Say a simple phrase

What do you want to say :
"Operate the Garage Door"

In response:
"OK, Operating the garage door"

Within IFTTT That :
Webhooks

URL:
https://blynk-cloud.com/<your-blynk-project-auth-code>/update/V7

Method:
PUT

Content Type:
application/json

Body :
["1"]


note that you may want to use your country's blynk-cloud.com IP address in both the sketch and the web hook. e.g. in Australia 
nslookup blynk-cloud.com
returns the IP address 45.55.96.146

so - your sketch would become:

Blynk.begin(auth, ssid, pass, "45.55.96.146", 8442);

and your webhook would be

URL:
https://45.55.96.146/<your-blynk-project-auth-code>/update/V7
