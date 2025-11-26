#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
// Đã xóa thư viện ArduinoJson

const char* ssid = "Hoàn";
const char* password = "11223347";

WebServer server(80);
#define DHTPIN 4
#define DHTTYPE DHT11
#define LED_PIN 2

DHT dht(DHTPIN, DHTTYPE);
bool ledState = false;

// HTML Interface (Giữ nguyên vì JS vẫn nhận được chuỗi định dạng JSON)
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 HTTP Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>body{font-family:sans-serif; text-align:center;} .btn{padding:10px 20px; font-size:16px; margin: 10px; cursor: pointer;}</style>
</head>
<body>
  <h1>ESP32 Dashboard</h1>
  <p>Temp: <span id="temp">--</span> &deg;C</p>
  <p>Humi: <span id="humi">--</span> %</p>
  <p>LED Status: <span id="led">OFF</span></p>
  <button class="btn" onclick="toggleLed(1)">ON</button>
  <button class="btn" onclick="toggleLed(0)">OFF</button>
<script>
  // Polling dữ liệu mỗi 2 giây
  setInterval(function() {
    fetch('/api/sensor').then(response => response.json()).then(data => {
      document.getElementById("temp").innerHTML = data.temperature;
      document.getElementById("humi").innerHTML = data.humidity;
      document.getElementById("led").innerHTML = data.led_state ? "ON" : "OFF";
    }).catch(err => console.log(err));
  }, 2000);

  function toggleLed(state) {
    fetch('/api/led?state=' + state, { method: 'POST' });
  }
</script>
</body></html>
)rawliteral";

void handleRoot() {
  server.send(200, "text/html", index_html);
}

// GET /api/sensor
// Thay đổi: Tự tạo chuỗi JSON thủ công
void handleSensor() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  // Kiểm tra lỗi sensor
  if (isnan(t)) t = 0;
  if (isnan(h)) h = 0;
  
  // Tạo chuỗi JSON thủ công
  // Định dạng: {"temperature": 25.5, "humidity": 60.2, "led_state": 0}
  String jsonResponse = "{";
  jsonResponse += "\"temperature\":" + String(t) + ",";
  jsonResponse += "\"humidity\":" + String(h) + ",";
  jsonResponse += "\"led_state\":" + String(ledState);
  jsonResponse += "}";
  
  server.send(200, "application/json", jsonResponse);
}

// POST /api/led?state=0|1
void handleLed() {
  if (server.hasArg("state")) {
    int state = server.arg("state").toInt();
    ledState = state;
    digitalWrite(LED_PIN, ledState);
    // Trả về chuỗi JSON thủ công đơn giản
    server.send(200, "application/json", "{\"status\":\"success\"}");
  } else {
    server.send(400, "application/json", "{\"status\":\"error\", \"message\":\"Missing state\"}");
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  dht.begin();
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nIP Address: " + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/api/sensor", HTTP_GET, handleSensor);
  server.on("/api/led", HTTP_POST, handleLed);
  
  server.begin();
}

void loop() {
  // Reconnect WiFi
  if ((WiFi.status() != WL_CONNECTED)) {
    WiFi.disconnect(); 
    WiFi.reconnect();
    delay(5000); // Chờ một chút trước khi thử lại để tránh spam
  }
  server.handleClient();
}