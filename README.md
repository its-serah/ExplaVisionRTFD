ExplaVisionRTFD

Overview

ExplaVisionRTFD is a smart system that integrates IOT with an Server and Cloud Storage. It leverages an ESP8266 microcontroller and a Node.js backend to capture images upon detecting motion, upload them to AWS S3, and analyze them as needed in future works.

System Components

Node.js Server: Handles image capture from a webcam and uploads images to AWS S3.

ESP8266 Microcontroller: Monitors an IR sensor and triggers image capture upon motion detection.

AWS S3: Stores captured images for further processing.

Requirements

Before running the project, ensure the following dependencies are installed:

Node.js Server Requirements

Node.js and npm installed

Required npm packages:

npm install express fs path node-webcam node-fetch

Microcontroller (ESP8266) Requirements

Arduino IDE installed

ESP8266 board package installed in Arduino IDE

Required libraries:

ESP8266WiFi.h

ESP8266HTTPClient.h

Setup and Execution

1. Setting Up the Node.js Server

Clone the repository:

git clone https://github.com/its-serah/ExplaVisionRTFD.git
cd ExplaVisionRTFD

Install dependencies:

npm install

Run the server:

node server.js

The server will start at http://localhost:3000.

2. Configuring the ESP8266 Microcontroller

Open the Arduino IDE.

Install the necessary libraries (ESP8266WiFi.h, ESP8266HTTPClient.h).

Update the Wi-Fi credentials in the code:

const char* ssid = "your-SSID";
const char* password = "your-password";

Upload the code to the ESP8266.

Open the serial monitor and ensure the device connects to Wi-Fi.

How It Works

The ESP8266 monitors an IR sensor for motion detection.

Upon detecting motion, the ESP8266 sends an HTTP request to the /capture endpoint of the Node.js server.

The server captures an image using a connected webcam and saves it locally.

The image is then uploaded to an AWS S3 bucket.

The ESP8266 receives confirmation of the successful upload and logs the response.

Expected Output

Images are captured and stored in AWS S3.

The following image demonstrates a successful capture:



Troubleshooting

Node.js Server Issues

Ensure all dependencies are installed (npm install).

Check if the webcam is correctly connected and accessible.

If the image fails to upload, verify AWS S3 credentials and permissions.

ESP8266 Issues

If Wi-Fi does not connect, verify the SSID and password.

Ensure the API endpoint in the code matches the Node.js server’s address.

If the sensor does not detect motion, check wiring and IR sensor functionality.

Contributions

Feel free to contribute by submitting pull requests or reporting issues in the repository.

License

This project is licensed under the MIT License.
