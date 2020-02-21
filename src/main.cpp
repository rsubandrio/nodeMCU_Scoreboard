#include <Arduino.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
// #include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <HJS589.h>
#include <fonts/ElektronMart6x8.h>
#include "../include/BigNumber.h"
#include "../include/index_html.h"

const char* ssid     = "rio_scoreboard";
const char* password = "rio_keren";

// const byte DNS_PORT = 53;
IPAddress APIP(10, 4, 19, 82);
IPAddress GWIP(10, 4, 19, 1);
IPAddress NMIP(255, 255, 255, 0);

// DNSServer dnsServer;
ESP8266WebServer server(80);

#define pin_A 16
#define pin_B 12
#define pin_sclk 0
#define pin_clk 14
#define pin_r 13
#define pin_noe 15

#define DISPLAYS_WIDE 1
#define DISPLAYS_HIGH 1
// SPIDMD dmd(DISPLAYS_WIDE, DISPLAYS_HIGH, pin_noe, pin_A, pin_B, pin_sclk); // DMD controls the entire display
HJS589 dmd(DISPLAYS_WIDE, DISPLAYS_HIGH); // DMD controls the entire display
// HJS589 P10 FUNGSI TAMBAHAN UNTUK NODEMCU ESP8266

void ICACHE_RAM_ATTR refresh() { 
  dmd.refresh();
  timer0_write(ESP.getCycleCount() + 40000);  
}

void Disp_init() {  
  dmd.start();
  timer0_attachInterrupt(refresh);
  timer0_write(ESP.getCycleCount() + 40000);
  dmd.clear();
}
// const char *PARAM_SCORE_ONE = "l";
// const char *PARAM_SCORE_TWO = "r";

// byte score1 = 0;
// byte score2 = 0;
// byte scored_before = 0;
void notFound()
{
  server.send(404, "text/plain", F("Not Found"));
}

void setup() {
  Serial.begin(115200);

  // inisiasi spiffs
  // if (!SPIFFS.begin())
  // {
  //   Serial.println("Gak bisa lanjut, SPIFFS Error");
  //   return;
  // }

  Serial.println("Setting AP (Access Point)…");
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(APIP, GWIP, NMIP);
  WiFi.softAP(ssid, password);
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

  // dnsServer.setErrorReplyCode(DNSReplyCode::NoError);
  // dnsServer.start(DNS_PORT, "*", APIP);
  // dnsServer.

  // Serial.println(WiFi.localIP());
  // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  //   request->send(SPIFFS, "/index.html", "text/html");
  // });
  server.on("/", [] () {
    server.send_P(200, "text/html", index_html);
  });

  server.on("/update", [] () {
    dmd.drawText(0, 0, server.arg("l"));
    dmd.drawText(19, 0, server.arg("r"));
    if (server.hasArg("gameover") && server.arg("gameover").toInt() == 1)
    {
      dmd.clear();
      dmd.setFont(ElektronMart6x8);
      dmd.drawText(0, 0, F("GAME"));
      dmd.drawText(5, 9, F("OVER"));
      delay(1000);
      dmd.clear();
      dmd.setFont(BigNumber);
      dmd.drawText(0, 0, server.arg("l"));
      dmd.drawRect(15,0,16,31);
      dmd.drawText(19, 0, server.arg("r"));
      delay(1000);
      dmd.clear();
      dmd.setFont(ElektronMart6x8);
      dmd.drawText(0, 0, F("GAME"));
      dmd.drawText(5, 9, F("OVER"));
      delay(1000);
      dmd.clear();
      dmd.setFont(BigNumber);
      dmd.drawText(0, 0, server.arg("l"));
      dmd.drawRect(15,0,16,31);
      dmd.drawText(19, 0, server.arg("r"));
      delay(1000);
      dmd.clear();
      dmd.drawText(0, 0, F("00"));
      dmd.drawRect(15,0,16,31);
      dmd.drawText(19, 0, F("00"));
    }
    server.send(200, "application/json", "{\"status\":\"ok\"}");
  });

  server.onNotFound(notFound);

  server.begin();

  Disp_init();
  dmd.setBrightness(100);
  dmd.setFont(BigNumber);
  dmd.drawText(0, 0, F("00"));
  dmd.drawText(19, 0, F("00"));
  dmd.drawRect(15,0,16,31);
}

void loop() {
  // dnsServer.processNextRequest();
  server.handleClient();
}
