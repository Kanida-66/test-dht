// ESP8266 + DHT22 - Send data to Node.js server via HTTP POST
// Read temperature and humidity every 30 seconds

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <DHT.h>

// Wi-Fi Settings
const char* ssid = "IoT-Sunksun";    // ชื่อ Wi-Fi
const char* password = "Y8L5f76w"; // รหัส Wi-Fi

// Server Settings
const char* serverUrl = "http://192.168.1.105:3000/api/sensor"; // เปลี่ยนเป็น IP ของคอมพิวเตอร์

// DHT22 Settings
#define DHTPIN 14      // GPIO2 (D4 บน NodeMCU)
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    delay(1000);

    // เริ่มต้น DHT22
    dht.begin();
    Serial.println("DHT22 initialized");

    // เชื่อมต่อ Wi-Fi
    Serial.print("Connecting to WiFi");
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("WiFi connected!");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
}

void loop() {
    // อ่านค่าจากเซ็นเซอร์
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // ตรวจสอบว่าอ่านค่าได้หรือไม่
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        delay(2000);
        return;
    }

    // แสดงค่าบน Serial Monitor
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.print(" °C | Humidity: ");
    Serial.print(humidity);
    Serial.println(" %");

    // ส่งข้อมูลไปยัง Server
    if (WiFi.status() == WL_CONNECTED) {
        WiFiClient client;
        HTTPClient http;

        http.begin(client, serverUrl);
        http.addHeader("Content-Type", "application/json");

        // สร้าง JSON payload
        String payload = "{\"temperature\":" + String(temperature) +
                        ",\"humidity\":" + String(humidity) + "}";

        int httpCode = http.POST(payload);

        if (httpCode > 0) {
            Serial.print("HTTP Response: ");
            Serial.println(httpCode);

            if (httpCode == 200) {
                String response = http.getString();
                Serial.println("Data sent successfully!");
                Serial.println(response);
            }
        } else {
            Serial.print("HTTP Error: ");
            Serial.println(http.errorToString(httpCode));
        }

        http.end();
    } else {
        Serial.println("WiFi disconnected!");
    }

    // รอ 30 วินาทีก่อนอ่านค่าใหม่
    delay(30000);
}