#include <WiFi.h>
#include <HTTPClient.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

// Định nghĩa chân kết nối và loại cảm biến
#define DHTPIN 4 // Chân GPIO4 của ESP32 kết nối với DATA của DHT22
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Thông tin kết nối WiFi
char ssid[] = "Wokwi-GUEST";
char password[] = "";
// API Key của ThingSpeak
const char *api_key = "K3QJKP3V3ZGH4DT0";

void setup()
{
  Serial.begin(115200);
  dht.begin();
  Serial.println("Khởi động ESP32 và DHT22...");

  // Kết nối WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Đang kết nối WiFi...");
  }
  Serial.println("Kết nối WiFi thành công!");
}

void loop()
{
  // Đọc dữ liệu từ DHT22
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Kiểm tra nếu đọc lỗi
  if (isnan(temperature) || isnan(humidity))
  {
    Serial.println("Không đọc được dữ liệu từ DHT22!");
    return;
  }

  // In dữ liệu ra Serial Monitor
  Serial.print("Nhiệt độ: ");
  Serial.print(temperature);
  Serial.print("°C, Độ ẩm: ");
  Serial.print(humidity);
  Serial.println("%");

  // Gửi dữ liệu lên ThingSpeak
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    String url = String("https://api.thingspeak.com/update?api_key=") + api_key +
                 "&field1=" + String(temperature) +
                 "&field2=" + String(humidity);
    http.begin(url);
    int httpCode = http.GET();
    if (httpCode > 0)
    {
      Serial.println("Dữ liệu đã gửi!");
    }
    else
    {
      Serial.println("Lỗi khi gửi dữ liệu.");
    }
    http.end();
  }
  else
  {
    Serial.println("WiFi không kết nối.");
  }

  delay(15000); // Cập nhật dữ liệu mỗi 15 giây
}