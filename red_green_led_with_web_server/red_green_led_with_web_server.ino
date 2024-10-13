#include <WiFi.h>
#include <WebServer.h>

// Replace with your network credentials
const char* ssid = "Guna";
const char* password = "12345678";

// Set GPIO pins for red and green LEDs
const int redLED = 12;
const int greenLED = 14;

// Create a WebServer object on port 80
WebServer server(80);

void setup() {
  // Initialize the red and green LED pins as output
  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);

  // Start the Serial Monitor for debugging
  Serial.begin(115200);

  // Connect to Wi-Fi
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  // Wait for the ESP32 to connect to the Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  // Print the local IP address
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Define the routes for the web server
  server.on("/", handleRoot);        // Route to the main page
  server.on("/redon", handleRedOn);  // Route to turn on the red LED
  server.on("/redoff", handleRedOff);// Route to turn off the red LED
  server.on("/greenon", handleGreenOn); // Route to turn on the green LED
  server.on("/greenoff", handleGreenOff); // Route to turn off the green LED

  // Start the server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  // Handle incoming client requests
  server.handleClient();
}

// Function to display the web page
void handleRoot() {
  String html = "<html>\
    <head><title>ESP32 LED Control</title></head>\
    <body>\
      <h1>ESP32 LED Control</h1>\
      <p><a href=\"/redon\"><button>Red ON</button></a> <a href=\"/redoff\"><button>Red OFF</button></a></p>\
      <p><a href=\"/greenon\"><button>Green ON</button></a> <a href=\"/greenoff\"><button>Green OFF</button></a></p>\
    </body>\
  </html>";

  server.send(200, "text/html", html);
}

// Function to turn on the red LED
void handleRedOn() {
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW); // Optionally turn off the green LED
  server.send(200, "text/html", "<p>Red LED is ON</p><a href=\"/\">Go Back</a>");
}

// Function to turn off the red LED
void handleRedOff() {
  digitalWrite(redLED, LOW);
  server.send(200, "text/html", "<p>Red LED is OFF</p><a href=\"/\">Go Back</a>");
}

// Function to turn on the green LED
void handleGreenOn() {
  digitalWrite(greenLED, HIGH);
  digitalWrite(redLED, LOW); // Optionally turn off the red LED
  server.send(200, "text/html", "<p>Green LED is ON</p><a href=\"/\">Go Back</a>");
}

// Function to turn off the green LED
void handleGreenOff() {
  digitalWrite(greenLED, LOW);
  server.send(200, "text/html", "<p>Green LED is OFF</p><a href=\"/\">Go Back</a>");
}
