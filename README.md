# ESP32 IoT Project: Blynk & Web Server

Dự án IoT cơ bản sử dụng ESP32 để giám sát nhiệt độ, độ ẩm (DHT11) và điều khiển LED thông qua App Blynk và Web Server cục bộ.

## 🔌 Chuẩn bị Phần cứng

| Thiết bị | Chân ESP32 | Ghi chú |
| :--- | :--- | :--- |
| **DHT11 Sensor** | **GPIO 4** | Chân tín hiệu (Data) |
| **LED** | **GPIO 2** | LED tích hợp (Onboard) |
| **Cáp Micro USB** | - | Nạp code và cấp nguồn |

---

## 🛠️ Cài đặt Thư viện (Arduino IDE)

Trước khi nạp code, hãy cài đặt các thư viện sau trong **Library Manager** (`Ctrl+Shift+I`):

1.  `DHT sensor library` (bởi Adafruit).
2.  `Adafruit Unified Sensor` (Thư viện phụ thuộc bắt buộc).
3.  `Blynk` (Chỉ cần cho Bài 1).

> **Lưu ý:** Bài 2 đã được tối ưu để **không cần cài** thư viện `ArduinoJson`.

---

## 📱 Bài 1: Blynk (IoT Cloud)

Kết nối ESP32 với ứng dụng Blynk trên điện thoại để giám sát và điều khiển từ xa qua Internet.

### Cấu hình trên App Blynk/Console
1.  Tạo Template mới (Datastream):
    * **V0 (Virtual Pin):** Nhiệt độ (Temperature) - Đơn vị: Double.
    * **V1 (Virtual Pin):** Độ ẩm (Humidity) - Đơn vị: Double.
    * **V2 (Virtual Pin):** Nút nhấn (Switch) - Điều khiển LED (0/1).
2.  Copy **Template ID**, **Device Name**, và **Auth Token**.

### Cấu hình Code
* Thay thế 3 dòng đầu tiên trong code bằng thông tin lấy từ Blynk Console.
* Điền `ssid` và `pass` (Tên và mật khẩu WiFi).

### Vận hành
* Mở App Blynk trên điện thoại.
* Nhiệt độ/Độ ẩm cập nhật mỗi **2 giây**.
* Nhấn nút trên App -> LED trên mạch sáng/tắt.

---

## 🌐 Bài 2: Web Server (HTTP)

ESP32 tự tạo một trang web nội bộ để hiển thị thông số và cung cấp API.

### Cấu hình Code
* Điền `ssid` và `pass` (Tên và mật khẩu WiFi).
* Không cần cài thêm thư viện JSON (xử lý thủ công chuỗi String).

### Vận hành
1.  Nạp code và mở **Serial Monitor** (Baud 115200).
2.  Lấy địa chỉ IP được in ra (Ví dụ: `192.168.1.100`).
3.  **Truy cập Dashboard:**
    * Mở trình duyệt (Chrome/Safari) và gõ địa chỉ IP.
    * Giao diện web hiện ra, tự động cập nhật số liệu.
4.  **Sử dụng API (Dành cho Postman/App khác):**
    * **Lấy dữ liệu (GET):** `http://<IP>/api/sensor`
        * *Trả về:* `{"temperature": 30.5, "humidity": 70.0, "led_state": 0}`
    * **Điều khiển LED (POST):** `http://<IP>/api/led?state=1` (1 là Bật, 0 là Tắt).
    * Đảm bảo điện thoại/laptop và ESP32 đang bắt chung một mạng WiFi.
