#ifndef WebServer_h
#define WebServer_h

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>


ESP8266WebServer server(80);

char *ssid = "BristleBot_0000";
const char *password = "uclbristlebot";

bool loadFromSpiffs(String path){
  String dataType = "text/plain";
  if(path.endsWith("/")) path += "index.html";

  if(path.endsWith(".src")) path = path.substring(0, path.lastIndexOf("."));
  else if(path.endsWith(".htm")) dataType = "text/html";
  else if(path.endsWith(".html")) dataType = "text/html";
  else if(path.endsWith(".css")) dataType = "text/css";
  else if(path.endsWith(".js")) dataType = "application/javascript";
  else if(path.endsWith(".png")) dataType = "image/png";
  else if(path.endsWith(".gif")) dataType = "image/gif";
  else if(path.endsWith(".jpg")) dataType = "image/jpeg";
  else if(path.endsWith(".ico")) dataType = "image/x-icon";
  else if(path.endsWith(".xml")) dataType = "text/xml";
  else if(path.endsWith(".pdf")) dataType = "application/pdf";
  else if(path.endsWith(".zip")) dataType = "application/zip";
  File dataFile = SPIFFS.open(path.c_str(), "r");
  if (server.hasArg("download")) dataType = "application/octet-stream";
  if (server.streamFile(dataFile, dataType) != dataFile.size()) {}
  dataFile.close();
  return true;
}

void handleNotFound(){
  if(loadFromSpiffs(server.uri())) return;
  String message = "File Not Detected\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " NAME:"+server.argName(i) + "\n VALUE:" + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  Sprintln(message);
}

void configureAccessPoint() {
  WiFi.mode(WIFI_AP);

  // Set SSID based on device MAC address
  String mac = WiFi.macAddress();
  Sprintln("MAC address: " + mac);
  ssid[11] = mac.charAt(12);
  ssid[12] = mac.charAt(13);
  ssid[13] = mac.charAt(15);
  ssid[14] = mac.charAt(16);
  Sprint("SSID: ");
  Sprintln(ssid);
  Sprint("Password: ");
  Sprintln(password);
  
  WiFi.softAP(ssid, password);
  IPAddress myIP = WiFi.softAPIP();
  Sprintln("AP IP address: " + myIP);
}

#endif

