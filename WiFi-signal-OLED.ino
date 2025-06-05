#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 
#define OLED_ADDR 0x3C

#define OLED_SDA 21
#define OLED_SCL 22

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int rssi[14]; // index 1–13 used

void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Wi-Fi Spectrum");
  display.display();
  delay(1000);

  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // ensure no connection
}

void loop() {
  for (int i = 1; i <= 13; i++) rssi[i] = -100;

  int n = WiFi.scanNetworks(false, true); // async = false, show hidden = true

  for (int i = 0; i < n; ++i) {
    int ch = WiFi.channel(i);
    int signal = WiFi.RSSI(i);
    if (ch >= 1 && ch <= 13 && signal > rssi[ch]) {
      rssi[ch] = signal;
    }
  }

  for (int i = 1; i <= 13; i++) {
    int signal = rssi[i];
    int barLength = map(signal, -100, -30, 0, 20);
    if (barLength < 0) barLength = 0;

    Serial.printf("   %2d    | %4d dBm | ", i, signal);
    for (int j = 0; j < barLength; j++) Serial.print("█");
    Serial.println();
  }

  display.clearDisplay();
  for (int i = 1; i <= 13; i++) {
    int signal = rssi[i];
    int barHeight = map(signal, -100, -30, 0, SCREEN_HEIGHT - 10);
    if (barHeight < 0) barHeight = 0;

    int x = (i - 1) * 9; // space per bar
    display.drawRect(x, SCREEN_HEIGHT - barHeight, 7, barHeight, WHITE);
    display.setCursor(x, SCREEN_HEIGHT - 8);
    display.setTextSize(1);
    display.print(i);
  }
  display.display();

  delay(400);
}
