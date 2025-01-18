#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <IRsend.h>

// Replace these with your Wi-Fi credentials
const char* ssid = "yourwifi";
const char* password = "yourwifipass";

// Define IR LED pin
const uint16_t kIrLedPin = 0;

// Create an instance of the IRsend class
IRsend irsend(kIrLedPin);

// Create an instance of the server at port 80
ESP8266WebServer server(80);

// Raw data sequences for the IR commands
const uint16_t playRawData[] = {8970, 4460, 608, 522, 608, 1626, 558, 1700, 608, 1650, 612, 520, 608, 1626, 558, 1702, 606, 548, 608, 1626, 558, 572, 556, 572, 556, 600, 556, 1698, 610, 520, 606, 524, 606, 1652, 608, 626, 608, 522, 606, 522, 606, 1652, 606, 628, 606, 522, 608, 522, 606, 548, 606, 1522, 558, 1700, 606, 1628, 556, 600, 556, 1594, 606, 1628, 558, 1700, 606, 1602, 684, 39498, 8968, 2178, 558};
const uint16_t stopRawData[] = {8920, 4510, 558, 572, 556, 1700, 576, 1658, 558, 1702, 556, 574, 556, 1700, 602, 1634, 558, 600, 554, 1700, 576, 554, 602, 528, 574, 582, 600, 1634, 556, 574, 554, 574, 554, 1702, 558, 1700, 576, 554, 576, 554, 574, 582, 602, 634, 598, 530, 572, 556, 574, 580, 574, 556, 598, 1636, 556, 1702, 602, 1658, 600, 1530, 556, 1700, 574, 1660, 556, 1674, 612, 39548, 8918, 2248, 600};
const uint16_t rewindRawData[] = {8928, 4502, 566, 564, 564, 1668, 532, 1726, 566, 1694, 566, 564, 564, 1670, 530, 1728, 566, 590, 564, 1670, 532, 598, 508, 622, 508, 648, 530, 1726, 566, 564, 564, 566, 562, 1696, 564, 1564, 530, 1674, 566, 564, 564, 1694, 566, 670, 564, 566, 566, 564, 564, 592, 564, 670, 562, 620, 564, 1668, 532, 626, 530, 1620, 540, 1696, 532, 1726, 566, 1642, 616, 25760, 140, 13642, 8896, 2272, 564};
const uint16_t pauseRawData[] = {8928, 4502, 566, 564, 564, 1668, 532, 1726, 566, 1694, 566, 564, 564, 1670, 530, 1728, 566, 590, 564, 1670, 532, 598, 508, 622, 508, 648, 530, 1726, 566, 564, 564, 566, 562, 1696, 564, 1564, 530, 1674, 566, 564, 564, 1694, 566, 670, 564, 566, 566, 564, 564, 592, 564, 670, 562, 620, 564, 1668, 532, 626, 530, 1620, 540, 1696, 532, 1726, 566, 1642, 616, 25760, 140, 13642, 8896, 2272, 564};

// HTML content for the webpage with renamed inputs
const char* htmlPage = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <title>Nwgat's VHS Controller</title>
</head>
<body>
  <h1>Control IR LED</h1>
  <form action="/play" method="get">
    <input type="submit" value="Play">
  </form>
  <form action="/pause" method="get">
    <input type="submit" value="Pause">
  </form>
  <form action="/rewind" method="get">
    <input type="submit" value="Rewind">
  </form>
</body>
</html>
)rawliteral";

// Function to handle sending IR signal for play
void handlePlay() {
  irsend.sendRaw(playRawData, sizeof(playRawData) / sizeof(playRawData[0]), 38);  // Send raw data for Play
  server.send(200, "text/html", "<h1>IR Signal for Play Sent!</h1>");
}

// Function to handle sending IR signal for stop
void handleStop() {
  irsend.sendRaw(stopRawData, sizeof(stopRawData) / sizeof(stopRawData[0]), 38);  // Send raw data for Stop
  server.send(200, "text/html", "<h1>IR Signal for Stop Sent!</h1>");
}

// Function to handle sending IR signal for pause
void handlePause() {
  irsend.sendRaw(pauseRawData, sizeof(pauseRawData) / sizeof(pauseRawData[0]), 38);  // Send raw data for Pause
  server.send(200, "text/html", "<h1>IR Signal for Pause Sent!</h1>");
}

// Function to handle sending IR signal for rewind
void handleRewind() {
  irsend.sendRaw(rewindRawData, sizeof(rewindRawData) / sizeof(rewindRawData[0]), 38);  // Send raw data for Rewind
  server.send(200, "text/html", "<h1>IR Signal for Rewind Sent!</h1>");
}

// Function to handle the root URL
void handleRoot() {
  server.send(200, "text/html", htmlPage);
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  // Initialize IRsend
  irsend.begin();
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set up web server routes
  server.on("/", handleRoot);
  server.on("/play", handlePlay);
  server.on("/pause", handlePause);
  server.on("/rewind", handleRewind);
  
  // Start the server
  server.begin();
  Serial.println("Server started");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Handle client requests
  server.handleClient();
}
