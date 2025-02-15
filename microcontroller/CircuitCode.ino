#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Wi-Fi credentials 
const char* ssid = "iPhone";
const char* password = "ncsk8507";

// API endpoint: server listens on port 3000 at /capture
const char* apiEndpoint = "localhost:3000/capture";

// Pin definitions
#define IR_SENSOR 1       // IR sensor (input)
#define ALERT_LED 2       // Alert LED (output)
#define STATUS_LED 4      // Status LED (output)

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println();

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait for Wi-Fi connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Not connected");
  }

  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Set pin modes
  pinMode(IR_SENSOR, INPUT);
  pinMode(ALERT_LED, OUTPUT);
  pinMode(STATUS_LED, OUTPUT);

  // Initial LED state: status LED on, alert LED off
  digitalWrite(ALERT_LED, LOW);
  digitalWrite(STATUS_LED, HIGH);
}

void sendCaptureRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    WiFiClient wifiClient;  // Create a WiFiClient instance
    
    Serial.print("Sending capture request to: ");
    Serial.println(apiEndpoint);
    
    // Updated API call with WiFiClient and URL
    http.begin(wifiClient, apiEndpoint);  
    
    int httpCode = http.GET();  
    
    if (httpCode > 0) {
      Serial.print("HTTP GET code: ");
      Serial.println(httpCode);
      String payload = http.getString();
      Serial.println("Response:");
      Serial.println(payload);
    } else {
      Serial.print("HTTP GET request failed, error: ");
      Serial.println(http.errorToString(httpCode));
    }
    http.end(); 
  } else {
    Serial.println("WiFi not connected");
  }
}

void state_2() {
  // Activate alert: alert LED on, status LED off
  digitalWrite(ALERT_LED, HIGH);
  digitalWrite(STATUS_LED, LOW);
  
  // Call your server's /capture endpoint
  sendCaptureRequest();
  
  // Delay to prevent repeated triggering
  delay(5000);
}

void state_1() {
  // Normal state: alert LED off, status LED on
  digitalWrite(ALERT_LED, LOW);
  digitalWrite(STATUS_LED, HIGH);
}

void loop() {
  Serial.println("Listening for new signals...");
  
  if (digitalRead(IR_SENSOR) == HIGH) {
    state_2();

    while (1) {
      digitalWrite(ALERT_LED, HIGH);
      digitalWrite(STATUS_LED, HIGH);
    }
  } else {
    state_1();
  }
  
  delay(500); // Short delay before next loop
}

