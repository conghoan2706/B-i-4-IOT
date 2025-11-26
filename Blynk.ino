#define BLYNK_TEMPLATE_ID "TMPL6zdIZ4GOu"
#define BLYNK_TEMPLATE_NAME "Hoan"
#define BLYNK_AUTH_TOKEN "BgGDaj5b7Pr2k_nEfxs5-vdNNYjJod4S"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char ssid[] = "Hoàn";
char pass[] = "11223347";

#define DHTPIN 4
#define DHTTYPE DHT11 // Hoặc DHT22
#define LED_PIN 2

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Gửi dữ liệu cảm biến mỗi 2 giây
void sendSensor() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println("Lỗi đọc DHT sensor!");
    return;
  }

  // Gửi lên Blynk (V0: Temp, V1: Humi)
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);

  // Cảnh báo nhiệt độ > 35 độ
  if (t > 35) {
    Blynk.logEvent("temp_alert", "Cảnh báo! Nhiệt độ quá cao: " + String(t) + "°C");
  }
}

// Đồng bộ trạng thái nút nhấn trên Blynk (V2) với LED
BLYNK_WRITE(V2) {
  int pinValue = param.asInt(); 
  digitalWrite(LED_PIN, pinValue);
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  
  // Thiết lập timer gọi hàm sendSensor mỗi 2000ms (2s)
  timer.setInterval(2000L, sendSensor);
}

void loop() {
  // Tự động reconnect nếu mất kết nối được thư viện xử lý
  if (WiFi.status() == WL_CONNECTED) {
     Blynk.run();
  }
  timer.run();
}