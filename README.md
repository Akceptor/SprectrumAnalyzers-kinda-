# SprectrumAnalyzers (kinda)

Connect the CC1101 module to the ESP32 as follows:
| CC1101 Pin | ESP32 Pin |
|------------|-----------|
| GND        | GND       |
| VCC        | 3.3V      |
| SCK        | GPIO 18   |
| MISO       | GPIO 19   |
| MOSI       | GPIO 23   |
| CSN        | GPIO 5    |
| GDO0       | GPIO 2    |

Connect the Adafruit compatible I2C display as follows:
| Display Pin | ESP32 Pin |
|-------------|-----------|
| GND         | GND       |
| VCC         | 3.3V      |
| SDA         | GPIO 21   |
| SCL         | GPIO 22   |


## Software Setup

1. Install the Arduino IDE (if not already installed)
2. Add ESP32 board support to Arduino IDE:
   - Open Arduino IDE
   - Go to File > Preferences
   - In "Additional Boards Manager URLs" add: `https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json`
   - Go to Tools > Board > Boards Manager
   - Search for ESP32 and install

3. Install required libraries:
   - Go to Sketch > Include Library > Manage Libraries
   - Search for and install "ELECHOUSE CC1101" library
   - Search for and install "Adafruit SSD1306" library
   - Search for and install "Adafruit GFX" library
