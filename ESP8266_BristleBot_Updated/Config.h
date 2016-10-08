#include <ArduinoJson.h>

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

  // We don't use String here because ArduinoJson library requires the input
  // buffer to be mutable. If you don't use ArduinoJson, you may as well
  // use configFile.readString instead.
  configFile.readBytes(buf.get(), size);

  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& json = jsonBuffer.parseObject(buf.get());

  if (!json.success()) {
    Serial.println("Failed to parse config file");
    return false;
  }

  drive.left.maximum(json["motor"]["leftMax"]);
  drive.right.maximum(json["motor"]["rightMax"]);
  heartbeat_enable = json["led"]["red"];
  sensors.left.threshold = json["proximity"]["leftThr"];
  sensors.right.threshold = json["proximity"]["rightThr"];

  json.printTo(S);
  return true;
}

bool saveConfig() {
  StaticJsonBuffer<200> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();
  JsonObject& led = root.createNestedObject("led");
  JsonObject& motor = root.createNestedObject("motor");
  JsonObject& proximity = root.createNestedObject("proximity");
  motor["leftMax"] = drive.left.maximum();
  motor["rightMax"] = drive.right.maximum();
  led["red"] = heartbeat_enable;
  proximity["leftThr"] = sensors.left.threshold;
  proximity["rightThr"] = sensors.right.threshold;

  File configFile = SPIFFS.open("/config.json", "w");
  if (!configFile) {
    Sprintln("Failed to open config file for writing");
    return false;
  }

  root.printTo(configFile);
  root.printTo(S);
  Sprintln();
  return true;
}

