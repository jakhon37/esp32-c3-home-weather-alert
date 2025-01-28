#include "WebGui.h"

WebGui::WebGui(uint16_t port) : server(port), currentTemperature(0.0), currentHumidity(0.0) {}

void WebGui::begin() {
    // Define HTTP routes
    server.on("/", std::bind(&WebGui::handleRoot, this));
    server.on("/data", std::bind(&WebGui::handleData, this));
    server.onNotFound(std::bind(&WebGui::handleNotFound, this));

    // Start the server
    server.begin();
    Serial.println("HTTP server started");
}

void WebGui::handleClient() {
    server.handleClient();
}

void WebGui::setSensorData(float temperature, float humidity) {
    currentTemperature = temperature;
    currentHumidity = humidity;
}

String WebGui::generateHTML() {
    
    String page = "<!DOCTYPE html>\n";
    page += "<html lang='en'>\n";
    page += "<head>\n";
    page += "  <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n";
    page += "  <meta charset='UTF-8'>\n";
    page += "  <title>ESP32 Weather Station</title>\n";

    // Import Google Font
    page += "  <link href='https://fonts.googleapis.com/css2?family=Poppins:wght@400;700&display=swap' rel='stylesheet'>\n";

    // Import Font Awesome for Icons
    page += "  <link rel='stylesheet' href='https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0-beta3/css/all.min.css' integrity='sha512-...' crossorigin='anonymous' referrerpolicy='no-referrer' />\n";

    // Import Chart.js
    page += "  <script src='https://cdn.jsdelivr.net/npm/chart.js'></script>\n";

    // CSS Styles
    page += "  <style>\n";
    page += "    * { box-sizing: border-box; margin: 0; padding: 0; }\n";
    page += "    body { font-family: 'Poppins', sans-serif; background: linear-gradient(to right, #36D1DC, #5B86E5); color: #ffffff; display: flex; flex-direction: column; align-items: center; justify-content: flex-start; min-height: 100vh; padding-top: 30px; }\n";
    page += "    h1 { font-size: 2.5rem; margin-bottom: 20px; text-shadow: 1px 1px 2px rgba(0,0,0,0.3); }\n";
    page += "    .container { display: flex; flex-wrap: wrap; justify-content: center; gap: 20px; max-width: 1200px; width: 100%; margin: 0 auto; }\n";
    page += "    .card { background: rgba(255, 255, 255, 0.2); backdrop-filter: blur(6px); border-radius: 12px; box-shadow: 0 8px 20px rgba(0, 0, 0, 0.2); width: 300px; padding: 20px; text-align: center; transition: transform 0.3s ease; }\n";
    page += "    .card:hover { transform: translateY(-5px); }\n";
    page += "    .icon { font-size: 2rem; margin-bottom: 10px; display: block; }\n";
    page += "    .card h2 { margin-bottom: 10px; font-size: 1.5rem; }\n";
    page += "    .card p { font-size: 1.2rem; margin: 5px 0; transition: color 0.3s; }\n";
    page += "    .fade { animation: fadeIn 0.5s; }\n";
    page += "    @keyframes fadeIn { from { opacity: 0; } to { opacity: 1; } }\n";
    page += "    .chart-container { background: rgba(255, 255, 255, 0.2); backdrop-filter: blur(6px); box-shadow: 0 8px 20px rgba(0, 0, 0, 0.2); border-radius: 12px; width: 90%; max-width: 600px; padding: 20px; margin: 20px; }\n";
    page += "    #myChart { width: 100%; max-height: 300px; }\n";
    page += "  </style>\n";
    page += "</head>\n";

    // Body Content
    page += "<body>\n";
    page += "  <h1>ESP32 Home Weather Station</h1>\n";
    page += "  <div class='container'>\n";

    // Temperature Card
    page += "    <div class='card'>\n";
    page += "      <i class='fas fa-thermometer-half icon'></i>\n";
    page += "      <h2>Temperature</h2>\n";
    page += "      <p id='tempValue'>" + String(currentTemperature,1) + " &deg;C</p>\n";
    page += "    </div>\n";

    // Humidity Card
    page += "    <div class='card'>\n";
    page += "      <i class='fas fa-tint icon'></i>\n";
    page += "      <h2>Humidity</h2>\n";
    page += "      <p id='humValue'>" + String(currentHumidity,1) + " %</p>\n";
    page += "    </div>\n";

    page += "  </div> <!-- end .container -->\n";

    // Chart Container
    page += "  <div class='chart-container'>\n";
    page += "    <canvas id='myChart'></canvas>\n";
    page += "  </div>\n";

    // JavaScript
    page += "  <script>\n";
    page += "    let tempData = [];\n";
    page += "    let humData = [];\n";
    page += "    let timeLabels = [];\n";

    page += "    function getTimeLabel() {\n";
    page += "      const now = new Date();\n";
    page += "      return now.toLocaleTimeString();\n";
    page += "    }\n";

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

    page += "    function updateReadings() {\n";
    page += "      fetch('/data')\n";
    page += "        .then(response => response.json())\n";
    page += "        .then(json => {\n";
    page += "          const tempVal = document.getElementById('tempValue');\n";
    page += "          const humVal = document.getElementById('humValue');\n";
    page += "          tempVal.innerHTML = `${json.temperature} °C`;\n";
    page += "          humVal.innerHTML = `${json.humidity} %`;\n";

    page += "          tempVal.classList.add('fade');\n";
    page += "          humVal.classList.add('fade');\n";
    page += "          setTimeout(() => {\n";
    page += "            tempVal.classList.remove('fade');\n";
    page += "            humVal.classList.remove('fade');\n";
    page += "          }, 500);\n";

    page += "          const currentTime = getTimeLabel();\n";
    page += "          timeLabels.push(currentTime);\n";
    page += "          tempData.push(json.temperature);\n";
    page += "          humData.push(json.humidity);\n";

    page += "          if(timeLabels.length > 20) {\n";
    page += "            timeLabels.shift();\n";
    page += "            tempData.shift();\n";
    page += "            humData.shift();\n";
    page += "          }\n";

    page += "          myChart.update();\n";
    page += "        });\n";
    page += "    }\n";

    page += "    setInterval(updateReadings, 5000);\n";

    page += "    window.onload = function() {\n";
    page += "      updateReadings();\n";
    page += "    };\n";

    page += "  </script>\n";

    page += "</body>\n";
    page += "</html>\n";

    return page;
}

void WebGui::handleRoot() {
    if (isnan(currentTemperature) || isnan(currentHumidity)) {
        server.send(200, "text/plain", "Sensor error: data not available");
        return;
    }
    String htmlContent = generateHTML();
    server.send(200, "text/html", htmlContent);
}

void WebGui::handleData() {
    String json = "{";
    json += "\"temperature\": " + String(currentTemperature, 1) + ",";
    json += "\"humidity\": " + String(currentHumidity, 1);
    json += "}";
    server.send(200, "application/json", json);
}

void WebGui::handleNotFound() {
    server.send(404, "text/plain", "Not found");
}
