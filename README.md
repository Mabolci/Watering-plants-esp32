# Watering-plants-esp32
This is part of submission for university subject with the shortest acronime possible -- UMPANUMiW

# Main objectives
Software for microcontroller able to communicate via MQQT with Domoticz in order monitor moisture for a certain plant, amout of water in the container and water it if nececery

# Hardware used
ESP 32 (devkit v1), soil moisture sencor, water level sensor and cabels

# Usage
In the code, replace `"SSID"` and `"PASSWORD"` with your network's ssid and password, than, replace `"MQTT_USERNAME"`, `"MQTT_PASSWORD"` and `"MQTT_SERVER"` as well. Sert proper port via variable `MQTT_PORT`. Connect water sensor data pin to pin `36` and power pin to pin `17`. Next connect analog pin from moisture sencor to pin `39`. Finally connect the moisture sencor to 3.3V and both sencor to GND, flash microcontroller with the main file