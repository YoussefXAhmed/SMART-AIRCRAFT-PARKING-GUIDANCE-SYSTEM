# SMART-AIRCRAFT-PARKING-GUIDANCE-SYSTEM

📌 Project Overview
This project presents a smart embedded system that simulates aircraft ground parking guidance.  
It replaces the traditional human aircraft marshaller with an automated electronic system using distance sensing, visual indicators, sound alerts, interrupts, and timers.

---

🎯 Objectives
- Automate aircraft parking guidance
- Measure distance accurately using ultrasonic sensing
- Provide clear visual and audio feedback
- Implement emergency safety using interrupts
- Use non-blocking timers for real-time operation

---

🧠 System Logic
| Distance Range | Indicator | LCD Message | Action |
|---------------|----------|-------------|--------|
| > 30 cm | None | OUT | Aircraft far |
| 30 – 20 cm | White LED | CONTINUE | Safe to move |
| 20 – 10 cm | Yellow LED | SLOW | Reduce speed |
| 10 – 5 cm | Red LED + Buzzer | WARNING | Close to stop |
| ≤ 5 cm | Red LED + High Buzzer | STOP | Immediate stop |

---

🔧 Hardware Components
- Arduino Uno
- Ultrasonic Sensor (HC-SR04)
- LCD 16×2 (I2C)
- White, Yellow, Red LEDs
- Buzzer
- Push Button
- Resistors & Wires

---

⏱ Timer Implementation
A software timer using `millis()` reads the ultrasonic sensor every 100 ms.
This ensures:
- Non-blocking execution
- Fast response
- Reliable interrupt handling

---

🚨 Interrupt Mechanism
A push button is connected as an external interrupt.
When pressed:
- Red LED turns ON
- High-frequency buzzer activates
- System enters emergency STOP mode
- All normal logic is overridden

---

🧩 Circuit Explanation
- Ultrasonic sensor measures front distance
- LEDs indicate movement status
- LCD displays distance and guidance
- Buzzer gives warning sounds
- Push button triggers emergency stop

---

👨‍💻 Team Members
- Youssef Ahmed  
- Ahmed Mohamed  
- Jozeph Hanna  
- Omar Saleh  

---
🎓 Academic Information
- Course: Embedded Systems  
- Institution: Arab Academy for Science, Technology & Maritime Transport  
- Academic Year: 2024 / 2025

---

## 🚀 Future Enhancements
- Add wireless communication
- Multiple ultrasonic sensors
- Data logging
- Mobile application integration
