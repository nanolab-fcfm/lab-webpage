# ESP8266 Temperature Controller

This project allows controlling an ESP8266-based device through serial communication. The system simulates button presses to adjust temperature settings on an external device.

## 📦 Features
- **Set Current Temperature** (`SCTxxx`)
- **Set Target Temperature** (`STTxxx`)
- **Start Temperature Adjustment** (`GO`)
- **Query Current Temperature** (`RCT`)
- **Query Target Temperature** (`RTT`)
- **Manual Button Press Commands** (`A`, `AA`, `B`, `BB`, `AB`)

## 🔧 Requirements
- **ESP8266** microcontroller
- **USB-to-Serial Converter** (e.g., FTDI, CH340G)
- **Serial Terminal Software** (Minicom, Screen, Picocom, or Python `pyserial`)

## 🚀 Commands & Usage
### **Temperature Commands**
| Command   | Description |
|-----------|------------|
| `SCTxxx`  | Set **current** temperature to `xxx` °C. Example: `SCT30` (sets to 30°C) |
| `STTxxx`  | Set **target** temperature to `xxx` °C. Example: `STT100` (target = 100°C) |
| `RCT`     | Request **current** temperature |
| `RTT`     | Request **target** temperature |
| `GO`      | Start adjusting temperature from `CT` to `TT` |

### **Manual Button Presses**
| Command | Action |
|---------|--------|
| `A`     | Short press button A (100 ms) |
| `AA`    | Long press button A (500 ms) |
| `B`     | Short press button B (100 ms) |
| `BB`    | Long press button B (500 ms) |
| `AB`    | Short press both A and B simultaneously |

## 🔧 Troubleshooting
### **Permission Denied (Linux)**
If you get `permission denied` on `/dev/ttyUSB0`, run:
```bash
sudo usermod -aG dialout $USER
```
Then **restart the terminal**.

## 📌 Example Usage
```
SCT25   # Set current temperature to 25°C
STT80   # Set target temperature to 80°C
RCT     # Get current temperature
RTT     # Get target temperature
GO      # Start automatic temperature adjustment
```


