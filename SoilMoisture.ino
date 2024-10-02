#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

const char* account_sid = "your_twilio_account_sid";
const char* auth_token = "your_twilio_auth_token";
const char* whatsapp_number = "whatsapp:+your_twilio_whatsapp_number"; // Twilio sandbox WhatsApp number
const char* recipient_whatsapp_number = "whatsapp:+recipient_phone_number"; // Recipient's WhatsApp number

const String twilio_url = "https://api.twilio.com/2010-04-01/Accounts/" + String(account_sid) + "/Messages.json";

// Soil moisture sensor
const int moistureSensorPin = A0;
const int threshold = 1700; // Adjust this value as per your sensor's range

void setup() {
  Serial.begin(115200);
  
  // Initialize WiFi connection
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  int moistureLevel = analogRead(moistureSensorPin);
  
  if (moistureLevel < threshold) {
    sendWhatsAppMessage();
    delay(60000); // Delay to prevent multiple messages, adjust as necessary
  }
  
  delay(10000); // Delay before next reading
}

void sendWhatsAppMessage() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String message = "The soil moisture level is low. Please check the plant.";
    
    http.begin(twilio_url);
    http.addHeader("Authorization", "Basic " + String(base64::encode(account_sid + ":" + auth_token)));
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    
    String body = "To=" + String(recipient_whatsapp_number) + "&From=" + String(whatsapp_number) + "&Body=" + message;
    
    int httpCode = http.POST(body);
    
    if (httpCode > 0) {
      String response = http.getString();
      Serial.println("WhatsApp message sent successfully");
      Serial.println(response);
    } else {
      Serial.println("Failed to send WhatsApp message");
    }
    
    http.end();
  }
}
