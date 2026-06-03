# 📞 GL868 ESP32 Voice Call System

A button-triggered GSM voice call system built on the ESP32 using the `GL868_ESP32` library. Press a physical button to dial a number — supports both outgoing and incoming calls with full audio routing configuration.

---

## ✨ Features

- **One-button dialing** — press a physical button to initiate a voice call
- **Incoming call auto-answer** — automatically answers incoming RING events
- **Audio routing on connect** — configures speaker, mic gain, and sidetone exactly once per call (no looping)
- **Clean call lifecycle** — detects `NO CARRIER`, `BUSY`, and `NO ANSWER` to reset state
- **GeoLinker integration** — runs `GeoLinker.update()` passively when no call is active

---

## 🛒 Hardware Requirements

| Component | Details |
|-----------|---------|
| ESP32 Dev Board | Any standard ESP32 |
| GL868 GSM Module | Connected via `Serial2` |
| Push Button | Connected to GPIO 4 (configurable) |
| SIM Card | With active voice plan |
| Speaker + Mic | Connected to GL868 audio pins |

---

## 🔌 Wiring

```
ESP32 GPIO 4  →  Button  →  GND     (INPUT_PULLUP, active LOW)
ESP32 TX2     →  GL868 RX
ESP32 RX2     →  GL868 TX
ESP32 GND     →  GL868 GND
ESP32 3.3V/5V →  GL868 VCC (check your module's voltage)
```

> Refer to your specific GL868 breakout board's datasheet for audio jack and power pinout.

---

## ⚙️ Configuration

Edit these values at the top of the sketch before flashing:

```cpp
#define DEVICE_ID    "your_device_id"   // GeoLinker device ID
#define API_KEY      "your_api_key"     // GeoLinker API key

#define BUTTON_PIN   4                  // GPIO pin for the dial button

const char* phoneNumber = "+91----------"; // Number to dial on button press
```

---

## 📦 Dependencies

Install via **Arduino Library Manager** or **PlatformIO**:

| Library | Purpose |
|---------|---------|
| `GL868_ESP32` | Modem control, GSM, GeoLinker APIs |

---

## 🚀 Getting Started

1. **Clone or download** this repository
2. Open the `.ino` file in Arduino IDE (or add to a PlatformIO project)
3. Install the `GL868_ESP32` library
4. Fill in your `DEVICE_ID`, `API_KEY`, and `phoneNumber`
5. Select your ESP32 board and correct COM port
6. Flash and open the Serial Monitor at **115200 baud**
7. Wait for `Network connected` → `Ready`
8. Press the button to dial!

---

## 📟 Serial Monitor Output

```
GL868 Voice System
Powering modem...
Waiting for network...
Network connected
Ready

Executing Voice Dial Sequence...
Dialing: ATD+91----------; 
MO CONNECTED
[Audio Patch] Call connected. Configuring audio routing ONCE.

NO CARRIER
Call Disconnected.
```

---

## 🔊 Audio AT Commands Used

| Command | Effect |
|---------|--------|
| `AT+CHFA=1` | Route audio to handsfree/external speaker |
| `AT+CLVL=90` | Set speaker volume (0–100) |
| `AT+CMIC=0,12` | Set main mic gain |
| `AT+FMMUTE=0` | Unmute audio |
| `AT+SIDET=1` | Enable sidetone (hear yourself speak) |

Adjust `AT+CLVL` and `AT+CMIC` values to tune volume and mic sensitivity for your hardware.

---

## 🐛 Troubleshooting

| Symptom | Likely Cause | Fix |
|---------|-------------|-----|
| Stuck on `Searching network...` | No SIM or poor signal | Check SIM insertion and antenna |
| No audio on call | Wrong `CHFA` routing | Try `AT+CHFA=0` for handset mode |
| Button does nothing | Wrong GPIO or wiring | Verify `BUTTON_PIN` and pull-up |
| Modem power fails | Insufficient current | Use a dedicated 2A+ power supply |
| Echo on call | Sidetone too high | Set `AT+SIDET=0` to disable |

---

## 📄 License

MIT License — free to use, modify, and distribute.

---

## 🤝 Contributing

Pull requests are welcome! For major changes, please open an issue first to discuss what you'd like to change.
