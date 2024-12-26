#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define BLYNK_PRINT Serial
#include <Blynk.h>
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
 

 
#define REPORTING_PERIOD_MS 1000

/*
 * #define BLYNK_TEMPLATE_ID "TMPL3zvw2Gi6i"
#define BLYNK_TEMPLATE_NAME "Heart beat"
#define BLYNK_AUTH_TOKEN "UkQVnx8W_6y48N3jg4_N4VmEZLIcg5KL"
 */

 
char auth[] = "UkQVnx8W_6y48N3jg4_N4VmEZLIcg5KL";             // You should get Auth Token in the Blynk App.
char ssid[] = "Moshi";                                     // Your WiFi credentials.
char pass[] = "shikayoyo";
 
// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;
 
float BPM, SpO2;
uint32_t tsLastReport = 0;
 

 
void onBeatDetected()
{
    Serial.println("Beat Detected!");
    
}
 
void setup()
{
    Serial.begin(9600);    
    pinMode(16, OUTPUT);
    Blynk.begin(auth, ssid, pass);
 
    Serial.print("Initializing Pulse Oximeter..");
 
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
        
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 
    // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
     //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
}

 
void loop()
{
    pox.update();
    Blynk.run();
 
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
     if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");
 
        Blynk.virtualWrite(V0, BPM);
        Blynk.virtualWrite(V1, SpO2);
        if(BPM > 76)
        {
        if(BPM < 60){
          Blynk.logEvent("bpm_low");
         Serial.println("BPM Low");
         }
         if( SpO2 < 80){
          Blynk.logEvent("spo2_low");
          Serial.println("SpO2 Low");

         }
         if(BPM > 140){
          Blynk.logEvent("bpm_high");
          Serial.println("BPM High");          
         }
        }
        
       

        tsLastReport = millis();
    }
}
