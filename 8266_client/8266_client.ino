/*
   Copyright (c) 2015, Majenko Technologies
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

 * * Redistributions of source code must retain the above copyright notice, this
     list of conditions and the following disclaimer.

 * * Redistributions in binary form must reproduce the above copyright notice, this
     list of conditions and the following disclaimer in the documentation and/or
     other materials provided with the distribution.

 * * Neither the name of Majenko Technologies nor the names of its
     contributors may be used to endorse or promote products derived from
     this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
   ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
   (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
   LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
   ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/* Create a WiFi access point and provide a web server on it. */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>



#define LED 2

/* Set these to your desired credentials. */
const char *ssid = "AT8266";
const char *password = "asdfghjkl";
String host("192.168.4.1");
uint16_t port(333);


/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
WiFiClient wificlinet;
void led_flash(int value = 10000, int sum_val = 20000);//led闪烁
void led_flash(int value, int sum_val)
{
    static int i = 0;
    i++;
    if(i < value)
    {
        digitalWrite(LED, HIGH);
    }
    else
    {
        digitalWrite(LED, LOW);
    }
    if (i >= sum_val)    i = 0;
}
void led_init()
{
    pinMode(LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
}
void setup() {
    delay(1000);
    Serial.begin(115200);
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    led_init();
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    
    while(wificlinet.connect(host, port) == 0);
    Serial.println("Connected successful");
}

void loop() {
    uint8_t val;
    led_flash();
    while(!WiFi.isConnected())//连接wifi
    {
        WiFi.begin(ssid, password);
    }
    while(wificlinet.status() == 0)//连接tcp服务端
    {
        wificlinet.connect(host, port);
    }
    if(Serial.available())//读串口
    {
        val = Serial.read();
        wificlinet.write(val);
    }
    while (wificlinet.available())//读网络消息
    {
        char val = wificlinet.read();//read()同arduino
        Serial.print(val);
    }

}
