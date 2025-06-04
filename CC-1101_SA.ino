#include <SPI.h>
#include <Wire.h>
#include <ELECHOUSE_CC1101_SRC_DRV.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CC1101_SCK  18
#define CC1101_MISO 19
#define CC1101_MOSI 23
#define CC1101_CS   5

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Frequency scan settings
#define START_FREQ  300.0    // MHz
#define END_FREQ    928.0    // MHz
#define STEP_FREQ   5.0      // MHz
#define SCAN_DELAY  20       // ms delay per scan point

// LED indicator
#define LED_PIN 17

void setup() {
  Serial.begin(115200);
  while (!Serial);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  SPI.begin(CC1101_SCK, CC1101_MISO, CC1101_MOSI, CC1101_CS);
  ELECHOUSE_cc1101.setSpiPin(CC1101_SCK, CC1101_MISO, CC1101_MOSI, CC1101_CS);
  ELECHOUSE_cc1101.Init();

  ELECHOUSE_cc1101.setCCMode(1);
  ELECHOUSE_cc1101.setModulation(0);
  ELECHOUSE_cc1101.setPA(10);
  ELECHOUSE_cc1101.setRxBW(101);
  ELECHOUSE_cc1101.setSyncMode(7);
  ELECHOUSE_cc1101.setAdrChk(0);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 not found!");
    while (1);
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Spectrum Analyzer");
  display.display();

  Serial.println("ESP32 + CC1101 Spectrum Analyzer");
  delay(1000);
}

void loop() {
  int barHeights[SCREEN_WIDTH] = {0};
  int index = 0;

  display.clearDisplay();

  for (float freq = START_FREQ; freq <= END_FREQ && index < SCREEN_WIDTH; freq += STEP_FREQ, index++) {
    ELECHOUSE_cc1101.setMHZ(freq);
    ELECHOUSE_cc1101.SetRx();
    delay(SCAN_DELAY);

    int rssi = ELECHOUSE_cc1101.getRssi();
    int barHeight = map(rssi, -110, -40, 0, SCREEN_HEIGHT - 10);
    barHeight = constrain(barHeight, 0, SCREEN_HEIGHT - 10);
    barHeights[index] = barHeight;

    // TODO Better serial output
    Serial.print(freq, 1);
    Serial.print(" MHz | ");
    for (int i = 0; i < barHeight / 2; i++) Serial.print("|");
    Serial.print(" ");
    Serial.print(rssi);
    Serial.println(" dBm");
  }

  for (int i = 0; i < index; i++) {
    int barHeight = barHeights[i];
    display.drawLine(i, SCREEN_HEIGHT - 1, i, SCREEN_HEIGHT - 1 - barHeight, SSD1306_WHITE);
  }

  // TODO Print max freq/rssi
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.printf("%.0f", START_FREQ);

  display.setCursor(SCREEN_WIDTH - 6 * String((int)END_FREQ).length(), 0);
  display.printf("%.0f", END_FREQ);

  display.display();
  delay(100);
}
