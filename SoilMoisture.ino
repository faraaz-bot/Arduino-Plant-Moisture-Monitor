#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// WiFiManager class to encapsulate WiFi-related operations
class WiFiManager {
  private:
    const char* ssid;
    const char* password;

  public:
    WiFiManager(const char* ssid, const char* password) : ssid(ssid), password(password) {}

    void connect() {
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
      }
      Serial.println("Connected to WiFi");
    }
};

// TwilioManager class to handle WhatsApp messaging through Twilio API
class TwilioManager {
  private:
    const char* account_sid;
    const char* auth_token;
    const char* whatsapp_number;
    const char* recipient_whatsapp_number;

  public:
    TwilioManager(const char* sid, const char* token, const char* fromNumber, const char* toNumber) 
      : account_sid(sid), auth_token(token), whatsapp_number(fromNumber), recipient_whatsapp_number(toNumber) {}

    void sendWhatsAppMessage(const String& message) {
      if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        String twilio_url = "https://api.twilio.com/2010-04-01/Accounts/" + String(account_sid) + "/Messages.json";
        
        http.begin(twilio_url);
        http.addHeader("Authorization", "Basic " + String(base64::encode(account_sid + ":" + auth_token)));
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");
        
        String body = "To=" + String(recipient_whatsapp_number) + "&From=" + String(whatsapp_number) + "&Body=" + message;
        int httpCode = http.POST(body);

        if (httpCode > 0) {
          Serial.println("WhatsApp message sent successfully");
        } else {
          Serial.println("Failed to send WhatsApp message");
        }
        
        http.end();
      }
    }
};

// Abstract base class for Sensors
class Sensor {
  protected:
    int pin;  // Analog pin for the sensor

  public:
    Sensor(int pin) : pin(pin) {}
    virtual int readSensor() = 0;  // Pure virtual function to enforce implementation in derived classes
    virtual bool isBelowThreshold() = 0;  // Check if sensor value is below threshold
};

// MoistureSensor class derived from the base Sensor class
class MoistureSensor : public Sensor {
  private:
    int threshold;

  public:
    MoistureSensor(int pin, int threshold) : Sensor(pin), threshold(threshold) {}

    // Override to implement sensor reading
    int readSensor() override {
      return analogRead(pin);  // Read moisture level
    }

    // Override to implement threshold check
    bool isBelowThreshold() override {
      return readSensor() < threshold;
    }
};

void setup() {
  Serial.begin(115200);

  // Create WiFiManager instance
  WiFiManager wifi("your_SSID", "your_PASSWORD");
  wifi.connect();

  // Create TwilioManager instance
  TwilioManager twilio("your_twilio_account_sid", "your_twilio_auth_token", 
                       "whatsapp:+your_twilio_whatsapp_number", "whatsapp:+recipient_phone_number");
  
  // Create MoistureSensor instance
  MoistureSensor moistureSensor(A0, 1700);  // Pin A0, threshold 1700
  
  // Send a message if the sensor detects low moisture
  if (moistureSensor.isBelowThreshold()) {
    twilio.sendWhatsAppMessage("The soil moisture level is low. Please check the plant.");
  }
}

void loop() {
  delay(60000);  // Run once every minute (adjust as necessary)
}
