
#include <DHT.h>
 
#define DHTPIN 14     // GPIO14 = D5 บน NodeMCU
#define DHTTYPE DHT22
 
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Serial.begin(115200);
  dht.begin();
}
 
void loop() {
  delay(2000);
 
  float humidity    = dht.readHumidity();
  float temperature = dht.readTemperature();
 
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println("อ่านค่าไม่ได้!");
    return;
  }
 
  Serial.print("ความชื้น: ");
  Serial.print(humidity);
  Serial.print(" %  |  อุณหภูมิ: ");
  Serial.print(temperature);
  Serial.println(" °C");
}
