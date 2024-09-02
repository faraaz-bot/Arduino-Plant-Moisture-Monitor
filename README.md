# Arduino-Plant-Moisture-Monitor
Soil Moisture Monitor with WhatsApp Alerts
This project uses an ESP8266 microcontroller to monitor soil moisture levels and send WhatsApp alerts when the moisture level drops below a specified threshold. The alert system is implemented using the Twilio API, allowing you to receive notifications directly to your WhatsApp.

Components Used
ESP8266 WiFi Module: The main microcontroller responsible for reading the soil moisture sensor and sending alerts.
Soil Moisture Sensor: Analog sensor that measures the moisture content in the soil.
Twilio API: Used for sending WhatsApp messages when the soil moisture is low.
Features
Soil Moisture Monitoring: Continuously reads the soil moisture level from the connected sensor.
WhatsApp Alerts: Automatically sends a WhatsApp message if the soil moisture level falls below the defined threshold, alerting you to water your plants.
WiFi Connectivity: Uses the ESP8266's built-in WiFi to connect to the internet and send alerts.
Twilio Integration: Leverages Twilio's API to send WhatsApp messages.
