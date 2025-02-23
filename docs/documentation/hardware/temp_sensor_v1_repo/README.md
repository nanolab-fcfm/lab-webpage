# ESP8266 Temperature Sensor

This project reads temperature data from a **MAX31865 RTD sensor** (PT100) and a **Dallas Temperature Sensor (DS18B20)** using an ESP8266. The data is transmitted over Serial communication upon request.

## 📦 Features
- **Read PT100 temperature** via MAX31865.
- **Read DS18B20 temperature** using OneWire.
- **Serial communication** for data requests.
- **Device identification response**.

## 🔧 Requirements
- **ESP8266** microcontroller.
- **MAX31865 RTD module**.
- **PT100 RTD sensor**.
- **DS18B20 temperature sensor**.
- **USB-to-Serial Converter** (e.g., FTDI, CH340G).
- **Arduino IDE** with necessary libraries:
  - `Adafruit MAX31865`
  - `DallasTemperature`
  - `OneWire`

## 🚀 Commands & Usage
### **Temperature Commands**
| Command   | Description |
|-----------|------------|
| `R`       | Request timestamped temperatures from both sensors. |
| `*IDN?`   | Request device identification. |

## 🔧 Wiring
| ESP8266 Pin | MAX31865 | DS18B20 |
|------------|----------|---------|
| GPIO15 (D2) | CS       | -       |
| MISO       | MISO     | -       |
| MOSI       | MOSI     | -       |
| SCK        | SCK      | -       |
| 3.3V       | VIN      | VCC     |
| GND        | GND      | GND     |
| GPIO2 (D4) | -        | DATA    |

**Note:** A **4.7kΩ pull-up resistor** is required between VCC and DATA for DS18B20.

## 📌 Example Usage
```
R       # Request temperatures
*IDN?   # Get device identification
```

## 🔧 Troubleshooting
### **Permission Denied (Linux)**
If you get `permission denied` on `/dev/ttyUSB0`, run:
```bash
sudo usermod -aG dialout $USER
```
Then **restart the terminal**.

## 📌 Future Improvements
- Implement error handling for sensor failures.
- Add Wi-Fi support for remote data logging.
- Integrate with cloud platforms for real-time monitoring.

## 📜 License
This project is open-source under the **MIT License**.

---
**Author:** ROSATECH

