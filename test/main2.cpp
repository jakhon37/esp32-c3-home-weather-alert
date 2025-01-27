// #include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "DHT.h"

// Define the DHT sensor type
// #define DHTTYPE DHT11
#define DHTTYPE    // DHT 22  (AM2302), AM2321

// WiFi credentials
const char* ssid     = "U+NetEFDA";
const char* password = "5D2A1A9EM#";

// Forward declarations for the handler functions and SendHTML
void handle_OnConnect();
void handle_NotFound();
String SendHTML(String Temperaturestat, String Humiditystat);

// Create a web server object that listens for HTTP requests on port 80
WebServer server(80);

// DHT setup
uint8_t DHTPin = 10;
DHT dht(DHTPin, DHTTYPE);

// Rename or undefine DHT's debug macros to avoid collisions
#undef DEBUG_PRINT
#undef DEBUG_PRINTLN

// Define your own debug macros
#define MY_DEBUG_PRINT(x)   Serial.print(x)
#define MY_DEBUG_PRINTLN(x) Serial.println(x)

float Temperature = 0.0;
float Humidity    = 0.0;

void handle_Data() {
  Temperature = dht.readTemperature();
  Humidity = dht.readHumidity();
  String json = "{";
  json += "\"temperature\":"; json += Temperature; json += ",";
  json += "\"humidity\":"; json += Humidity;
  json += "}";
  server.send(200, "application/json", json);
}
void setup() {
  Serial.begin(115200);
  delay(25);

  dht.begin();

  MY_DEBUG_PRINTLN("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(50);
    MY_DEBUG_PRINT(".");
  }

  MY_DEBUG_PRINTLN("WiFi connected!");
  MY_DEBUG_PRINT("IP Address: ");
  MY_DEBUG_PRINTLN(WiFi.localIP());
  server.on("/data", handle_Data);

  server.on("/", handle_OnConnect);
  server.onNotFound(handle_NotFound);

  server.begin();
  MY_DEBUG_PRINTLN("HTTP server started");
}

unsigned long lastDHTRead = 0;
const unsigned long readInterval = 2000; // 2 seconds

void loop() {
  server.handleClient();
    // Read sensor every 2 seconds
  if (millis() - lastDHTRead > readInterval) {
    lastDHTRead = millis();
    Temperature = dht.readTemperature();
    Humidity = dht.readHumidity();
  }
}

// Handler for root URL
void handle_OnConnect() {
  Temperature = dht.readTemperature();
  Humidity    = dht.readHumidity();

  if (isnan(Temperature) || isnan(Humidity)) {
    MY_DEBUG_PRINTLN("Failed to read from DHT sensor!");
    // Just call SendHTML with "N/A" strings
    server.send(200, "text/html", SendHTML("N/A", "N/A"));
  } else {
    MY_DEBUG_PRINT("Temperature: ");
    MY_DEBUG_PRINTLN(Temperature);
    MY_DEBUG_PRINT("Humidity: ");
    MY_DEBUG_PRINTLN(Humidity);

    // Convert float to String (you can add optional decimal places)
    String tempString = String(Temperature, 1); // 1 decimal place
    String humString  = String(Humidity, 1);

    server.send(200, "text/html", SendHTML(tempString, humString));
  }
}


// Handler for 404
void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}



// HTML generator
// String SendHTML(String Temperaturestat, String Humiditystat) {
//   String ptr = "<!DOCTYPE html> <html>\n";
//   // ... (rest of your HTML)
//   ptr += "</html>\n";
//   return ptr;
// }
String SendHTML(String Temperaturestat, String Humiditystat) {
  String page = "<!DOCTYPE html>\n";
  page += "<html lang='en'>\n";
  page += "<head>\n";
  page += "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
  page += "  <meta charset='UTF-8'>\n";
  page += "  <title>ESP32 Weather Station</title>\n";

  // --- Import Google Font (optional) ---
  // Requires active internet connection
  page += "  <link href='https://fonts.googleapis.com/css2?family=Poppins:wght@400;700&display=swap' rel='stylesheet'>\n";

  // --- Import Font Awesome (for icons) ---
  // Requires active internet connection
  page += "  <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css' integrity='sha512-...' crossorigin='anonymous' referrerpolicy='no-referrer' />\n";

  // --- Import Chart.js (optional for chart) ---
  // Requires active internet connection
  page += "  <script src='https://cdn.jsdelivr.net/npm/chart.js'></script>\n";

  // --- Main CSS styling ---
  page += "  <style>\n";
  page += "    * {\n";
  page += "      box-sizing: border-box;\n";
  page += "    }\n";
  page += "    body {\n";
  page += "      margin: 0;\n";
  page += "      padding: 0;\n";
  page += "      font-family: 'Poppins', sans-serif;\n";
  page += "      background: linear-gradient(to right, #36D1DC, #5B86E5);\n";  
  page += "      color: #ffffff;\n";
  page += "      display: flex;\n";
  page += "      flex-direction: column;\n";
  page += "      align-items: center;\n";
  page += "      justify-content: flex-start;\n";
  page += "      min-height: 100vh;\n";
  page += "      padding-top: 30px;\n";
  page += "    }\n";
  page += "    h1 {\n";
  page += "      font-size: 2.5rem;\n";
  page += "      margin-bottom: 20px;\n";
  page += "      text-shadow: 1px 1px 2px rgba(0,0,0,0.3);\n";
  page += "    }\n";
  page += "    .container {\n";
  page += "      display: flex;\n";
  page += "      flex-wrap: wrap;\n";
  page += "      justify-content: center;\n";
  page += "      gap: 20px;\n";
  page += "      max-width: 1200px;\n";
  page += "      width: 100%;\n";
  page += "      margin: 0 auto;\n";
  page += "    }\n";

  // Card styling with “glass” effect
  page += "    .card {\n";
  page += "      background: rgba(255, 255, 255, 0.2);\n";
  page += "      backdrop-filter: blur(6px);\n"; // “Frosted glass” effect in modern browsers
  page += "      border-radius: 12px;\n";
  page += "      box-shadow: 0 8px 20px rgba(0, 0, 0, 0.2);\n";
  page += "      width: 300px;\n";
  page += "      padding: 20px;\n";
  page += "      text-align: center;\n";
  page += "      transition: transform 0.3s ease;\n";
  page += "    }\n";
  page += "    .card:hover {\n";
  page += "      transform: translateY(-5px);\n";
  page += "    }\n";
  page += "    .icon {\n";
  page += "      font-size: 2rem;\n";
  page += "      margin-bottom: 10px;\n";
  page += "      display: block;\n";
  page += "    }\n";
  page += "    .card h2 {\n";
  page += "      margin-bottom: 10px;\n";
  page += "      font-size: 1.5rem;\n";
  page += "    }\n";
  page += "    .card p {\n";
  page += "      font-size: 1.2rem;\n";
  page += "      margin: 5px 0;\n";
  page += "      transition: color 0.3s;\n";
  page += "    }\n";

  // (Optional) a subtle fade-in for updated values
  page += "    .fade {\n";
  page += "      animation: fadeIn 0.5s;\n";
  page += "    }\n";
  page += "    @keyframes fadeIn {\n";
  page += "      from { opacity: 0; }\n";
  page += "      to { opacity: 1; }\n";
  page += "    }\n";

  // Chart container
  page += "    .chart-container {\n";
  page += "      background: rgba(255, 255, 255, 0.2);\n";
  page += "      backdrop-filter: blur(6px);\n";
  page += "      box-shadow: 0 8px 20px rgba(0, 0, 0, 0.2);\n";
  page += "      border-radius: 12px;\n";
  page += "      width: 90%;\n";
  page += "      max-width: 600px;\n";
  page += "      padding: 20px;\n";
  page += "      margin: 20px;\n";
  page += "    }\n";
  page += "    #myChart {\n";
  page += "      width: 100%;\n";
  page += "      max-height: 300px;\n";
  page += "    }\n";
  
  page += "  </style>\n";
  page += "</head>\n";

  // --- Body Content ---
  page += "<body>\n";
  page += "  <h1>ESP32 Weather Station</h1>\n";
  page += "  <div class='container'>\n";

  // Temperature Card
  page += "    <div class='card'>\n";
  page += "      <i class='fas fa-thermometer-half icon'></i>\n";
  page += "      <h2>Temperature</h2>\n";
  page += "      <p id='tempValue'>" + Temperaturestat + " &deg;C</p>\n";
  page += "    </div>\n";

  // Humidity Card
  page += "    <div class='card'>\n";
  page += "      <i class='fas fa-tint icon'></i>\n";
  page += "      <h2>Humidity</h2>\n";
  page += "      <p id='humValue'>" + Humiditystat + " %</p>\n";
  page += "    </div>\n";

  page += "  </div> <!-- end .container -->\n";

  // ---- Optional Real-Time Chart Container ----
  page += "  <div class='chart-container'>\n";
  page += "    <canvas id='myChart'></canvas>\n";
  page += "  </div>\n";

  // --- JavaScript: Fetch logic + Chart.js Setup ---
  page += "  <script>\n";

  // We'll keep arrays of data in JavaScript to show a timeline chart
  page += "    let tempData = [];\n"; 
  page += "    let humData = [];\n";
  page += "    let timeLabels = [];\n";

  // Grab current time (HH:MM:SS) as a label 
  page += "    function getTimeLabel() {\n";
  page += "      const now = new Date();\n";
  page += "      return now.toLocaleTimeString();\n";
  page += "    }\n";

  // Chart.js initialization
  page += "    const ctx = document.getElementById('myChart').getContext('2d');\n";
  page += "    const myChart = new Chart(ctx, {\n";
  page += "      type: 'line',\n";
  page += "      data: {\n";
  page += "        labels: timeLabels,\n";
  page += "        datasets: [\n";
  page += "          {\n";
  page += "            label: 'Temperature (°C)',\n";
  page += "            data: tempData,\n";
  page += "            borderColor: 'rgba(255, 99, 132, 1)',\n";
  page += "            backgroundColor: 'rgba(255, 99, 132, 0.2)',\n";
  page += "            fill: true,\n";
  page += "            tension: 0.1\n";
  page += "          },\n";
  page += "          {\n";
  page += "            label: 'Humidity (%)',\n";
  page += "            data: humData,\n";
  page += "            borderColor: 'rgba(54, 162, 235, 1)',\n";
  page += "            backgroundColor: 'rgba(54, 162, 235, 0.2)',\n";
  page += "            fill: true,\n";
  page += "            tension: 0.1\n";
  page += "          }\n";
  page += "        ]\n";
  page += "      },\n";
  page += "      options: {\n";
  page += "        responsive: true,\n";
  page += "        maintainAspectRatio: false,\n";
  page += "        scales: {\n";
  page += "          y: {\n";
  page += "            beginAtZero: false\n";
  page += "          }\n";
  page += "        }\n";
  page += "      }\n";
  page += "    });\n";

  // Function to fetch new data from /data
  page += "    function updateReadings() {\n";
  page += "      fetch('/data')\n";
  page += "        .then(response => response.json())\n";
  page += "        .then(json => {\n";
  page += "          // Update text on the page\n";
  page += "          const tempVal = document.getElementById('tempValue');\n";
  page += "          const humVal = document.getElementById('humValue');\n";
  page += "          tempVal.innerHTML = `${json.temperature} °C`;\n";
  page += "          humVal.innerHTML = `${json.humidity} %`;\n";

  // Optional fade animation for the new value
  page += "          tempVal.classList.add('fade');\n";
  page += "          humVal.classList.add('fade');\n";
  page += "          setTimeout(() => {\n";
  page += "            tempVal.classList.remove('fade');\n";
  page += "            humVal.classList.remove('fade');\n";
  page += "          }, 500);\n";

  // Add the new values to the chart data
  page += "          const currentTime = getTimeLabel();\n";
  page += "          timeLabels.push(currentTime);\n";
  page += "          tempData.push(json.temperature);\n";
  page += "          humData.push(json.humidity);\n";

  // Keep chart arrays from growing too large
  page += "          if(timeLabels.length > 20) {\n"; // keep last 20 data points
  page += "            timeLabels.shift();\n";
  page += "            tempData.shift();\n";
  page += "            humData.shift();\n";
  page += "          }\n";

  // Update the chart
  page += "          myChart.update();\n";
  page += "        });\n";
  page += "    }\n";

  // Refresh data every 5 seconds
  page += "    setInterval(updateReadings, 5000);\n";

  // --- Optional: fetch & populate initial data once the page loads ---
  page += "    window.onload = function() {\n";
  page += "      updateReadings();\n";
  page += "    };\n";

  page += "  </script>\n";

  page += "</body>\n";
  page += "</html>\n";

  return page;
}
