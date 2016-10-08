
void updateSettings(char *data) {

  StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(data);

  if (json.success()) {
    json.prettyPrintTo(Serial);
    drive.left.maximum(json["drive"]["leftMax"]);
    drive.right.maximum(json["drive"]["rightMax"]);
    drive.delayLeft(json["drive"]["delayLeft"]);
    drive.delayRight(json["drive"]["delayRight"]);
    drive.delay180(json["drive"]["delay180"]);
    
    red.state(json["led"]["red"]["state"]);
    blue.state(json["led"]["blue"]["state"]);
    ir.state(json["led"]["ir"]["state"]);
    
    sensors.left.threshold = json["proximity"]["leftThreshold"];
    sensors.right.threshold = json["proximity"]["rightThreshold"];
    sensors.cycles = json["proximity"]["cycles"];
    sensors.level = json["proximity"]["level"];
    sensors.rate = json["proximity"]["rate"];

    File configFile = SPIFFS.open("/config.json", "w");
    if (configFile) json.printTo(configFile);
    else  Serial.println("Failed to open config file for writing");
  }
  else Serial.println("Failed to parse config file");
}

bool loadConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  updateSettings(buf.get());
}

bool sendConfig(uint8_t num) {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  String configData = configFile.readString();
  Sprintln(configData);
  webSocket.sendTXT(num, configData);
}

bool saveConfig() {
  File configFile = SPIFFS.open("/config.json", "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    return false;
  }

  size_t size = configFile.size();
  if (size > 1024) {
    Serial.println("Config file size is too large");
    return false;
  }

  // Allocate a buffer to store contents of the file.
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

   StaticJsonBuffer<500> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (json.success()) { 
    JsonObject& led = json["led"];
    JsonObject& redset = json["red"];
    int test = redset["state"];
    int test2 = json["led"]["red"]["state"];
 Serial.println(test);
 Serial.println(test2);
 redset.set("state",4);


  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Sprintln("Failed to open config file for writing");
    return false;
  }

  json.printTo(configFile);
  json.prettyPrintTo(Serial);
  Serial.println();
  }

  return true;
}


