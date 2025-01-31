/***********************************************************************
  Shows p1mon MQTT data on the OLED display.

  Is based on the Adafruit MQTT Library ESP32 Adafruit IO SSL/TLS example
  Written by Tony DiCola for Adafruit Industries.
  Modified by Brent Rubell for Adafruit Industries
  MIT license, all text above must be included in any redistribution
 **********************************************************************/

#include <WiFi.h>
#include "WiFiClientSecure.h"
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

#include "json.hpp"

using json = nlohmann::json;

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#include "credentials.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);



/************************* WiFi Access Point *********************************/

//#define WLAN_SSID "xxxxx"
//#define WLAN_PASS "yyyyy"

/************************* MQTT Setup *********************************/

//#define MQTT_SERVER      "vvvvv"

// Using port 8883 for MQTTS
//#define MQTT_SERVERPORT  8883
//#define MQTT_USERNAME "zzzzz"
//#define MQTT_PASS      "wwwwww"

/************ Global State (you don't need to change this!) ******************/

// WiFiFlientSecure for SSL/TLS support
WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, MQTT_SERVERPORT, MQTT_USERNAME, MQTT_PASS);
Adafruit_MQTT_Subscribe p1mon = Adafruit_MQTT_Subscribe(&mqtt, "p1monitor/smartmeter");

// Used key
// /etc/ssl/certs/ISRG_Root_X1.pem
const char* isrg_root_ca = \
"-----BEGIN CERTIFICATE-----\n"
"MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw\n"
"TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh\n"
"cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4\n"
"WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu\n"
"ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY\n"
"MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc\n"
"h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+\n"
"0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U\n"
"A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW\n"
"T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH\n"
"B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC\n"
"B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv\n"
"KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn\n"
"OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn\n"
"jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw\n"
"qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI\n"
"rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV\n"
"HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq\n"
"hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL\n"
"ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ\n"
"3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK\n"
"NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5\n"
"ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur\n"
"TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC\n"
"jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc\n"
"oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq\n"
"4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA\n"
"mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d\n"
"emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=\n"
"-----END CERTIFICATE-----\n";


/****************************** Feeds ***************************************/

// Setup a feed called 'test' for publishing.
Adafruit_MQTT_Publish test = Adafruit_MQTT_Publish(&mqtt, "/feeds/test");

/*************************** Sketch Code ************************************/

void setup()
{
   Serial.begin(115200);
   delay(10);

   Serial.println(F("p1monitor IO MQTTS (SSL/TLS) Example"));

   // Start I2C Communication SDA = 5 and SCL = 4 on Wemos Lolin32 ESP32 with built-in SSD1306 OLED
   Wire.begin(5, 4);

   if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false))
   {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;);
   }
   delay(2000); // Pause for 2 seconds

   // Clear the buffer.

   display.setCursor(5, 5);
   display.setTextColor(WHITE, BLACK);
   display.setTextSize(2);
   display.setTextWrap(true);

   display.clearDisplay();
   display.println("31/ 1/2025");
   display.display();

   // Connect to WiFi access point.
   Serial.println(); Serial.println();
   Serial.print("Connecting to ");
   Serial.println(WLAN_SSID);

   delay(1000);

   WiFi.begin(WLAN_SSID, WLAN_PASS);
   delay(2000);

   while (WiFi.status() != WL_CONNECTED)
   {
      delay(500);
      Serial.print(".");
   }
   Serial.println();

   Serial.println("WiFi connected");
   Serial.println("IP address: "); Serial.println(WiFi.localIP());

   // Set ISRG root CA
   client.setCACert(isrg_root_ca);
   mqtt.subscribe(&p1mon);
}



/*
   MQTT json to be parsed

   {
      "ts": "2025-01-29 21:16:03", 
      "f182": 5116.819, 
      "f181": 5865.626, 
      "k170": 0.98,                      // current power 
      "f140": 0.061, 
      "f140ts": "2025-01-29 21:16:03", 
      "f160": 8.216, 
      "f160ts": "2025-01-25 19:15:00"
   }
 */

uint32_t x=0;
bool first = true;
double kmin = 0.0;
double kmax = 0.0;

void loop()
{
   // Ensure the connection to the MQTT server is alive (this will make the first
   // connection and automatically reconnect when disconnected).  See the MQTT_connect
   // function definition further below.
   MQTT_connect();

   // Now we can publish stuff!
   /*
     only for test
   Serial.print(F("\nSending val "));
   Serial.print(x);
   Serial.print(F(" to test feed..."));
   if (! test.publish(x++))
   {
      Serial.println(F("Failed"));
   } 
   else
   {
      Serial.println(F("OK!"));
   }

   // wait a couple seconds to avoid rate limit
   delay(2000);
    */

   Adafruit_MQTT_Subscribe *subscription;
   while ((subscription = mqtt.readSubscription(5000)))
   {
      // check if its the slider feed
      if (subscription == &p1mon) 
      {
         Serial.print(F("p1mon: "));
         Serial.println((char *)p1mon.lastread);

         json msg = json::parse(p1mon.lastread);
         Serial.println(msg["ts"].template get<std::string>().c_str());
         Serial.println(msg["k170"].template get<double>());

         double k170 = msg["k170"].template get<double>();
         if (first)
         {
            kmin = k170;
            kmax = k170;
            first = false;
         }
         else
         {
            if (k170 < kmin)
            {
               kmin = k170;
            }
            if (k170 > kmax)
            {
               kmax = k170;
            }
         }

         // current power
         char buf[25];
         sprintf(buf, "mo %5.3lf", k170);
         Serial.println(buf);

         const int dx = 5;
         const int dy = 5;
         const int h = 18;
         display.clearDisplay();
         display.setCursor(dx, dy + 0*h);
         display.println(buf);

         // minimum power
         sprintf(buf, "mi %5.3lf", kmin);
         display.setCursor(dx, dy + 1*h);
         display.println(buf);

         // maximum power
         sprintf(buf, "ma %5.3lf", kmax);
         display.setCursor(dx, dy + 2*h);
         display.println(buf);

         display.display();
      }
   }

   // ping the server to keep the mqtt connection alive
   if (!mqtt.ping())
   {
      mqtt.disconnect();
   }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect()
{
   int8_t ret;

   // Stop if already connected.
   if (mqtt.connected()) {
     return;
   }

   Serial.print("Connecting to MQTT... ");

   uint8_t retries = 3;
   while ((ret = mqtt.connect()) != 0)
   { // connect will return 0 for connected
      Serial.println(mqtt.connectErrorString(ret));
      Serial.println("Retrying MQTT connection in 5 seconds...");
      mqtt.disconnect();
      delay(5000);  // wait 5 seconds
      retries--;
      if (retries == 0)
      {
         // basically die and wait for WDT to reset me
         while (1);
      }
   }

   Serial.println("MQTT Connected!");
}

