#include <Arduino.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <WiFi.h>

#define height 135
#define width 240

#define b1_pin 35
#define b2_pin 0
#define bdel 119

#define backgnd TFT_BLACK
#define primary TFT_WHITE
#define secondary TFT_WHITE
#define font 2

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite scr = TFT_eSprite(&tft);

bool b1;
bool b2;
byte cur_ch = 1;
byte i2;

int t_netw_count;
int ch_netw_count;

unsigned long del;

void setup() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    scr.createSprite(240, 135);

    pinMode(b1_pin, INPUT_PULLUP);
    pinMode(b2_pin, INPUT_PULLUP);

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);
    t_netw_count = WiFi.scanNetworks();

    scr.fillSprite(backgnd);
    scr.setTextColor(primary, backgnd);
}


void loop(){
    b1 = !digitalRead(b1_pin);
    b2 = !digitalRead(b2_pin);

    if(b1 == 1 && millis() - del >= bdel && cur_ch != 13){
        cur_ch++;
        del = millis();
    } else if(b2 == 1 && millis() - del >= bdel && cur_ch != 1){
        cur_ch--;
        del = millis();
    } 
    if (b1 == 1 && b2 == 1){
        WiFi.scanDelete();
        t_netw_count = WiFi.scanNetworks();
        del = millis();
    }

    scr.fillSprite(backgnd);
    ch_netw_count = 0;

    for (int i = 0; i < t_netw_count; i++) {
        if(WiFi.channel(i) == cur_ch){
            ch_netw_count++;
            String net_info = String(WiFi.SSID(i)) + ": " + String(WiFi.RSSI(i)) + " dBm " + (WiFi.encryptionType(i) == WIFI_AUTH_OPEN ? " " : "*");        
            scr.drawString(net_info, 0, (7*font*ch_netw_count)+4, font);
        }

        delay(10);
    }

    
    scr.drawLine(0, 7*font+2, width, 7*font+2, secondary);
    String top = String("CH ") + cur_ch + String("  APS ") + ch_netw_count + String("  (") + t_netw_count + String(")");
    scr.drawString(top, 0, 0, font);
    
    scr.pushSprite(0,0);

}