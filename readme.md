# Mixed Reality Chemistry Simulation

This project implements a Mixed Reality (MR) framework for chemistry education, specifically focusing on titration. It uses passive haptics through a 3D-printed "skeletal" burette and active environmental sensing to bridge the gap between virtual simulations and physical laboratory skills.

## System Overview

The system utilizes a Meta Quest 3 headset and a custom wireless peripheral built with an ESP8266/ESP32. By mapping a physical potentiometer to a virtual stopcock and using a Time-of-Flight (ToF) sensor to detect the presence of a collection vessel, the simulation enforces both fine motor skill development and laboratory safety protocols.

## Repository Structure

The repository is organized into two primary components:

* **Arduino/**: Contains the firmware for the hardware peripheral.
* **Unity/**: Contains the MR application and sensor data integration logic.

## Hardware Components

* **Microcontroller**: ESP8266 (NodeMCU) or ESP32.


* **Input Sensor**: 10k Linear Potentiometer (to simulate the burette valve).


* **Environmental Sensor**: VL53L0X Time-of-Flight (ToF) sensor (to detect the flask/vessel).


* **Chassis**: 3D-printed burette housing mounted on a standard retort stand.



## Setup and Installation

### Arduino Configuration

1. Navigate to the `Arduino/` directory.
2. Open `sketch_jan12a.ino` in the Arduino IDE.
3. Install the following dependencies via the Library Manager:
* `Adafruit_VL53L0X`
* `ArduinoJson`


4. Update the `ssid`, `password`, and `targetIP` (the IP address of your Meta Quest 3) in the source code.
5. Flash the code to your microcontroller.

### Unity Configuration

1. Open the `Unity/` project in Unity 6.


2. The project includes the **Meta XR All-in-One SDK** for Passthrough and interaction features.


3. Open the main scene containing the `UDPReceiver` script.


4. Ensure the `port` in the `UDPReceiver` component matches the `targetPort` defined in the Arduino sketch (default is 5000).

## Technical Implementation

### Data Pipeline

1. **Acquisition**: The ESP8266 reads the potentiometer's analog voltage (0-1023) and the ToF sensor's distance in millimeters.
2. **Serialization**: Data is packed into a JSON payload: `{"pot": value, "dist": value}`.
3. **Transport**: The payload is sent via UDP over WiFi to the headset at approximately 60Hz to minimize latency.


4. **Reception**: A background thread in Unity listens for UDP packets, which are then parsed by `JsonUtility` to update the virtual state.



### Safety Logic

The simulation monitors the distance data from the ToF sensor. If the valve is opened while the `dist` value indicates no vessel is present (e.g., distance exceeds 15cm), the system triggers a "Spill Event" and provides a visual warning to the user.

### Spatial Alignment

To align the virtual twin with the physical hardware, a 3-point calibration routine is used. Users touch the Quest 3 controller to the physical nozzle to instantiate a spatial anchor, ensuring the virtual stopcock aligns with the physical potentiometer.