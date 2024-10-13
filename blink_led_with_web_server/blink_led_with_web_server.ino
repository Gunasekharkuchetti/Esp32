#include <WiFi.h>

// Replace with your network credentials
const char* ssid = "Guna";
const char* password = "12345678";

// Set GPIO pin for LED
const int ledPin = 2;

// Create an instance of WiFiServer on port 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Current state of the LED
String ledState = "OFF";

// Timer for controlling updates
unsigned long currentTime = millis();
unsigned long previousTime = 0;
const long timeoutTime = 2000;  // Timeout of 2 seconds

void setup() {
  // Initialize the GPIO pin as an output
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);  // LED off

  // Start serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Start the server
  server.begin();
  Serial.println(WiFi.localIP());  // Print the IP address
}

void loop() {
  // Listen for incoming clients
  WiFiClient client = server.available();

  if (client) {
    Serial.println("New Client.");
    String currentLine = "";  // String to hold incoming data from client
    currentTime = millis();
    previousTime = currentTime;

    // While the client is connected
    while (client.connected() && currentTime - previousTime <= timeoutTime) {
      currentTime = millis();
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        header += c;

        if (c == '\n') {  // If the end of the HTTP request is reached
          if (currentLine.length() == 0) {
            // Send HTTP response
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            // Web page content
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<style>html { font-family: Helvetica; display: inline-block; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");
           
            // Body
            client.println("<body><h1>ESP32 Web Server</h1>");
           
            // Check if LED is ON or OFF
            if (ledState == "ON") {
              client.println("<p>LED is ON</p>");
              client.println("<a href=\"/ledoff\"><button class=\"button button2\">OFF</button></a>");
            } else {
              client.println("<p>LED is OFF</p>");
              client.println("<a href=\"/ledon\"><button class=\"button\">ON</button></a>");
            }
           
            client.println("</body></html>");

            // The HTTP response ends with a blank line
            client.println();
           
            // Control the LED
            if (header.indexOf("GET /ledon") >= 0) {
              Serial.println("LED ON");
              ledState = "ON";
              digitalWrite(ledPin, HIGH);  // Turn LED ON
            } else if (header.indexOf("GET /ledoff") >= 0) {
              Serial.println("LED OFF");
              ledState = "OFF";
              digitalWrite(ledPin, LOW);  // Turn LED OFF
            }
           
            break;
          } else {  // If a newline is found, reset the current line
            currentLine = "";
          }
        } else if (c != '\r') {  // Add every character to the currentLine if it is not a carriage return
          currentLine += c;
        }
      }
    }
    // Clear the header variable
    header = "";

    // Close the client connection
    client.stop();
    Serial.println("Client Disconnected.");
  }
}