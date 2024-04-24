# Coastal Echoes 
## Introduction
Coastal Echoes is an innovative project combining real-time environmental data with interactive visualization techniques to present dynamic coastal scenes. It utilizes real-time tidal and wind data to animate coastal landscapes visually, allowing the physical reproduction of natural phenomena such as wave motion and beach coverage. This project serves as both a functional tool for marine enthusiasts and an artistic decoration.
## Project Goals
The main objective of Coastal Echoes is to integrate real-time environmental data with Internet of Things technology to generate dynamic representations of coastal environments, helping target users such as shell collectors, surfers, and beachgoers understand and visualize tidal and wind impacts in real-time.
## Features
* Real-Time Data Visualization: Using tide and wind data to illustrate their effects on wave motion and beach coverage.
* Physical Prototype: Features a motorized mechanism that simulates ocean wave movements correlated with real-time wind speed data.
* LED Visualization: An LED strip displays tide levels with color gradients, enhancing visual feedback.
* User-Friendly Interface: Designed to be intuitive and accessible for all user types.
## Getting Started
### Prerequisites
* Before starting with the installation of the Coastal Echoes project, ensure that you have the following:
* Access to the internet and a suitable environment to run Python scripts (e.g., Google Colab, Jupyter Notebook, or a local Python environment).
* Two APIs for real-time environmental data:
* Wind speed API
* Tide value API
* MQTT broker setup to publish and subscribe to topics.
* Arduino IDE installed on your computer for programming the Arduino Uno WiFi Rev2.
* The following libraries installed in your Arduino IDE:
* MQTT for handling MQTT connections.
* Servo to control the servo motors.
* WiFiNINA for network connectivity on Arduino.
* A Prusa 3D printer or access to one for printing the required parts.
* The 3D model source files for printing parts, available in the GitHub repository.
## Installation
### Software Setup
1.Clone the repository to your local machine or development environment:
```bash
git clone https://github.com/ucfnchb/0021_Group-Prototype.git
```
2.Navigate to the cloned directory and locate the Python scripts for retrieving data from the environmental APIs. These scripts are designed to publish data to MQTT topics. Run these scripts in your chosen environment (local or cloud-based).
3.Locate the Arduino Code folder in the repository, which contains the scripts for the Arduino. You will upload these scripts to the Arduino board in later steps.
### Hardware Assembly
1.Print the required 3D parts using the model source files provided in the repository's 3D_Models folder.
2.Assemble the hardware according to the assembly diagrams and instructions found in the Diagrams folder. This includes fitting together the 3D-printed parts and the electronic components.
3.Install the LED strips, servo motors, and the Arduino Uno WiFi Rev2 on the assembled hardware.
### Arduino Configuration
1.Open the Arduino IDE and install the necessary libraries (MQTT, Servo, and WiFiNINA) via the Library Manager. 
2.Open the Arduino scripts provided in the Arduino Code folder. 
3.Configure the scripts with the correct MQTT topics that the Python scripts are publishing to.
https://github.com/ucfnchb/0021_Group-Prototype/tree/main/API
4.Upload the scripts to your Arduino board, ensuring that it's connected to your computer via USB.
https://github.com/ucfnchb/0021_Group-Prototype/tree/main/Arduino%20Code
5.Connect the Arduino to the local Wi-Fi network as instructed in the script comments.
6.Once the setup is complete, the Arduino board should start receiving data from MQTT and controlling the LED strips and servo motors accordingly, reflecting the real-time environmental data in the physical prototype.
### Usage
* USB Power
* Connect the device to a WiFi network.
* The device will automatically start receiving data and simulating the environmental conditions of the NewHeaven coast.
## Prototype Design
The design process involved meticulous planning and execution, focusing on creating a responsive visualization that is both accurate and pleasing to the eye. For detailed design descriptions, see DesignDocs.
## Future Work
Future enhancements will focus on integrating predictive features to forecast unfavorable tidal and wind conditions. We are also exploring options to enhance the device's durability and functionality over prolonged use.
## Contact
