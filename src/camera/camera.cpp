#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "esp_camera.h"
#include <Adafruit_NeoPixel.h> 


// LED controls?
Adafruit_NeoPixel pixels(16, 38, NEO_GRB + NEO_KHZ800);

// Wi-Fi credentials
const char* ssid = "167 Prall Street";
const char* password = "$$$$$$";

// Create a web server on port 80
WebServer server(80);

// WROVER-KIT PIN Map (adjust if needed)
#define CAM_PIN_PWDN    -1  // power down is not used
#define CAM_PIN_RESET   9   // software reset will be performed
#define CAM_PIN_XCLK    1
#define CAM_PIN_SIOD    6
#define CAM_PIN_SIOC    7

#define CAM_PIN_D9      46
#define CAM_PIN_D8      45
#define CAM_PIN_D7      37
#define CAM_PIN_D6      21
#define CAM_PIN_D5      42
#define CAM_PIN_D4      41
#define CAM_PIN_D3      40
#define CAM_PIN_D2      39
#define CAM_PIN_VSYNC   8
#define CAM_PIN_HREF    5
#define CAM_PIN_PCLK    18

// Camera configuration (using a lower resolution for better reliability)
static camera_config_t camera_config = {
  .pin_pwdn       = CAM_PIN_PWDN,
  .pin_reset      = CAM_PIN_RESET,
  .pin_xclk       = CAM_PIN_XCLK,
  .pin_sccb_sda   = CAM_PIN_SIOD,
  .pin_sccb_scl   = CAM_PIN_SIOC,

  .pin_d7         = CAM_PIN_D9,
  .pin_d6         = CAM_PIN_D8,
  .pin_d5         = CAM_PIN_D7,
  .pin_d4         = CAM_PIN_D6,
  .pin_d3         = CAM_PIN_D5,
  .pin_d2         = CAM_PIN_D4,
  .pin_d1         = CAM_PIN_D3,
  .pin_d0         = CAM_PIN_D2,
  .pin_vsync      = CAM_PIN_VSYNC,
  .pin_href       = CAM_PIN_HREF,
  .pin_pclk       = CAM_PIN_PCLK,

  .xclk_freq_hz   = 20000000, // 20 MHz clock
  .ledc_timer     = LEDC_TIMER_0,
  .ledc_channel   = LEDC_CHANNEL_0,

  .pixel_format   = PIXFORMAT_JPEG,  // JPEG format

  // Use a smaller resolution for testing (QVGA or VGA)
  .frame_size     = FRAMESIZE_QVGA,
  .jpeg_quality   = 12,              // Quality 0-63 (lower means higher quality)
  .fb_count       = 1,               // Number of frame buffers
  .grab_mode      = CAMERA_GRAB_WHEN_EMPTY
};

//
// Initialize the camera
//
esp_err_t camera_init() {
  // If a power-down pin is defined, ensure the camera is powered up.
  if (CAM_PIN_PWDN != -1) {
    pinMode(CAM_PIN_PWDN, OUTPUT);
    digitalWrite(CAM_PIN_PWDN, LOW);
  }

  // Initialize the camera with the configuration.
  esp_err_t err = esp_camera_init(&camera_config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x\n", err);
    return err;
  }
  Serial.println("Camera initialized successfully!");
  return ESP_OK;
}

//
// HTTP handler for capturing and serving an image
//
void handleCapture() {
  // Capture an image from the camera
  camera_fb_t *fb = esp_camera_fb_get();
  if (!fb) {
    server.send(500, "text/plain", "Camera capture failed");
    return;
  }

  // Get the underlying WiFiClient from the server
  WiFiClient client = server.client();
  
  // Build and send the HTTP response headers manually
  client.print("HTTP/1.1 200 OK\r\n");
  client.print("Content-Type: image/jpeg\r\n");
  client.print("Content-Length: " + String(fb->len) + "\r\n");
  client.print("Content-Disposition: inline; filename=capture.jpg\r\n");
  client.print("Access-Control-Allow-Origin: *\r\n");
  client.print("Connection: close\r\n\r\n");  // Blank line to end headers
  
  // Send the image data (binary JPEG)
  size_t bytesWritten = client.write(fb->buf, fb->len);
  Serial.printf("Sent %d bytes (expected %d bytes)\n", bytesWritten, fb->len);

  // Release the frame buffer
  esp_camera_fb_return(fb);
  
  // Close the connection after data is sent
  client.stop();
}

//
// HTTP handler for a simple homepage with the captured image
//
void handleRoot() {
  String html = "<html><head><meta http-equiv='refresh' content='5' /></head><body>";
  html += "<h1>ESP32 Camera</h1>";
  html += "<p>Image will refresh every 5 seconds.</p>";
  html += "<img src='/capture' style='width:100%;max-width:600px' />";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

//
// Standard Arduino setup() function
//
// void setup() {
//   Serial.begin(115200);
//   // Wait for serial port to be ready (optional)
//   while (!Serial) {
//     delay(10);
//   }

//   pixels.begin();
//   for(int i=0; i<16; i++) { // For each pixel...
//     // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
//     // Here we're using a moderately bright green color:
//     pixels.setPixelColor(i, pixels.Color(0, 150, 0));

//     pixels.show();   // Send the updated pixel colors to the hardware.

//     delay(5); // Pause before next pass through loop
//   }

//   Serial.println("\n--- Starting Camera Web Server ---");

//   // Initialize the camera
//   if (camera_init() != ESP_OK) {
//     Serial.println("Camera initialization failed. Halting.");
//     while (true) {
//       delay(1000);
//     }
//   }

//   // Connect to Wi-Fi
//   Serial.printf("Connecting to Wi-Fi network: %s\n", ssid);
//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(500);
//     Serial.print(".");
//   }
//   Serial.println("");
//   Serial.print("Wi-Fi connected. IP address: ");
//   Serial.println(WiFi.localIP());

//   // Define HTTP endpoints
//   server.on("/", HTTP_GET, handleRoot);
//   server.on("/capture", HTTP_GET, handleCapture);

//   // Start the server
//   server.begin();
//   Serial.println("HTTP server started");
// }

//
// Standard Arduino loop() function
//
// void loop() {
//   // Handle incoming HTTP client requests
//   server.handleClient();
// }
