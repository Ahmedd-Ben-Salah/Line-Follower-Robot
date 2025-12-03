# Line Follower Robot

A fully autonomous line-following robot built using an ESP32 and an array of 8 IR sensors for track detection. The project required tight integration between hardware and software to achieve smooth, high-speed navigation.

## Overview

This project demonstrates a complete solution for competitive robotic line following. The robot can track and follow lines—the most common challenge in robotics competitions—while maintaining high speed and stability.

- **Microcontroller**: ESP32
- **Sensors**: 8 IR sensors arranged in an array for precise track detection
- **Competition**: Developed for the Robots League at ESSTH (3rd place finish)

## Hardware

- **IR Sensor Array**: Designed and calibrated for optimal line detection and cross detection
- **Motor Control**: Implemented using H-bridge drivers and PWM signals for precise speed and direction
- **Optimization**: Tuned for a balance of speed and stability, enabling sharp turns and smooth navigation

## Software

- **PID Control Algorithm**: Ensures accurate line following, quick response to turns, and robust cross/white-line detection on a black surface
- **Features**:
  - Sharp turn handling (including 90° turns)
  - Cross and fork detection
  - High-speed section performance tuning
  
- **PID Tuning**: Experimented with multiple strategies for optimal response in dynamic track conditions

## Skills Gained

- PID control and tuning
- Sensor data processing and calibration
- Motor control via PWM and H-bridge
- Real-time embedded system problem-solving

## Achievements

🏅 This robot was built for the Robots League competition at ESSTH, where our team proudly secured **3rd place**.

---

## Getting Started

### Requirements

- ESP32 development board
- 8 IR sensors
- H-bridge motor drivers (compatible with ESP32)
- Motors (with wheels for your robot platform)
- 3D printed or assembled chassis

### Setup

1. Connect the IR sensor array to the ESP32's GPIO pins.
2. Set up H-bridge drivers and connect motors as per the circuit schematic.
3. Flash the provided code to the ESP32 using the Arduino IDE or esptool.py.
4. Power up your robot on a black surface with a white line track.

### Code Structure

- `/hardware/` - Schematics and calibration guides
- `/firmware/` - Source code for ESP32
- `/docs/` - Project documentation and PID tuning results

 

## Contributing

Feel free to open issues or submit pull requests for suggestions, improvements, or bug fixes.

## Contributors

- Ahmed Ben Salah
- Anas Abd Elmalek Cherif
- Aziz Abbes
- Nassim Mhamdi

## License

This project is offered under the MIT License.

## Contact

For questions, reach out via GitHub issues or [Ahmedd-Ben-Salah](https://github.com/Ahmedd-Ben-Salah).
