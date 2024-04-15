/*
  Script for running Birdiary Station on ESP32 
*/
#include <config.h>
#include <WiFiClientSecure.h>  // library for http post multipart/form-data (early loading needed)

// SD Card
#include "FS.h"
#include "SD.h"
#include "SPI.h"
const int SD_PIN_CS = SD_Data_Pin;
bool sd_sign = false;
const char *aviFilePath = Avi_File_Path;
const char *wavFilePath = Wav_File_Path;
File videoFile;
File audioFile;

void setupSD() {
  // Initialize the SD card
  if (!SD.begin(SD_PIN_CS)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  uint8_t cardType = SD.cardType();
  // determine if the type of SD card is available
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached!");
    return;
  }
  Serial.print("(SD Card Type: ");
  if (cardType == CARD_MMC) {
    Serial.print("MMC");
  } else if (cardType == CARD_SD) {
    Serial.print("SDSC");
  } else if (cardType == CARD_SDHC) {
    Serial.print("SDHC");
  } else {
    Serial.print("UNKNOWN");
  }
  Serial.println(")");
  sd_sign = true;

  // delete video and audio file if they are existing from previous executions
  if (SD.exists(aviFilePath) == 1) {
    SD.remove(aviFilePath);
  }
  if (SD.exists(wavFilePath) == 1) {
    SD.remove(wavFilePath);
  }
}

// Wifi
#include <WiFi.h>
#include <HTTPClient.h>
const char *ssid = WiFi_SSID;
const char *password = WiFi_Password;

// Function to establish WiFi connection
void connectToWifi() {
  // Connect Wifi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("(Wating for Wifi)");
  }
  Serial.print("(Connected to WiFi network with IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.println(")");
}

// HTTP post multipart/form-data
const char host[] = Server_Host; // your server address
const uint16_t port = Server_Port;
const String MOVEMENT_ENDPOINT = Server_Movement_Endpoint;
const String STATION_ID = Server_Station_Id;

// Function for receiving the response after a HTTP POST multipart/form-data
/*
Some further hints and explanations concerning the HTTP POST multipart/form-data function can be found in a forum discussion (https://forum.arduino.cc/t/sending-video-avi-and-audio-wav-files-with-arduino-script-from-esp32s3-via-http-post-multipart-form-data-to-server/1234706/8) 
Thanks for your support J-M-L (https://forum.arduino.cc/u/J-M-L), kenb4 (https://forum.arduino.cc/u/kenb4)
*/
int statusFromResponse(WiFiClient &client) {
  size_t p = 0;
  size_t available = 0;
  // wait for first byte
  while (client.connected()) {
    if (available = client.available()) {
      break;
    }
    delay(1);
    p++;
  }
  if (!available) {
    Serial.println("not connected");
    return 0;
  }
  Serial.printf("waited %d for %d available --->\n", p, available);

  int code = -1;
  size_t t = 0;
  char buf[200];

  while (client.connected() && available) {
    p = client.read((uint8_t *)buf, std::min(available, sizeof(buf) - 1));
    if (t == 0) {
      sscanf(buf, "HTTP/%*s %d", &code);
    }
    t += p;
    buf[p] = 0;
    Serial.print(buf);
    available = client.available();
  }
  Serial.printf("<---\n%d total\n", t);
  return code;
}

// Function for sending a beforehand recorded movement
/*
Some further hints and explanations concerning the HTTP POST multipart/form-data function can be found in a forum discussion (https://forum.arduino.cc/t/sending-video-avi-and-audio-wav-files-with-arduino-script-from-esp32s3-via-http-post-multipart-form-data-to-server/1234706/8) 
Thanks for your support J-M-L (https://forum.arduino.cc/u/J-M-L), kenb4 (https://forum.arduino.cc/u/kenb4)
*/
void sendMovement(String json) {
  if (SD.exists("/video.avi") == 1 && SD.exists("/audio.wav") == 1) {

    videoFile = SD.open("/video.avi", FILE_READ);
    String videoLength = String(videoFile.size());
    audioFile = SD.open("/audio.wav", FILE_READ);
    String audioLength = String(audioFile.size());

    if ((WiFi.status() == WL_CONNECTED)) {

      WiFiClientSecure wifi;
      wifi.setInsecure();

      if (wifi.connect(host, port)) {
        Serial.println("Sending Movement!");

        wifi.println("POST " + MOVEMENT_ENDPOINT + STATION_ID + " HTTP/1.1");

        wifi.print("Host: ");
        wifi.println(host);

        String boundary = "..--..--MyOwnBoundary" + String(random(0xEFFFFF) + 0x100000, HEX);
        wifi.println("Content-Type: multipart/form-data; boundary=" + boundary);
        wifi.println("Transfer-Encoding: chunked");
        wifi.println();  // end of request headers (will be more headers for each part)

        auto stringPart = [&](const String &name, const String &value) {
          // RFC 1342 §7.2.1 says CRLF before two hyphens is considered part of boundary
          String part = "\r\n--" + boundary + "\r\n";
          part += "Content-Disposition: form-data; name=\"";
          part += name;
          part += "\"\r\n\r\n";  // CRLF to end header, and CRLF before content
          part += value;

          // Serial.printf("part %s len %d\n%s\n", name, part.length(), part.c_str());
          wifi.println(part.length(), HEX);
          wifi.println(part);
        };

        auto filePart = [&](const String &name, File &file, const String &filename, const String &type) {
          String part = "\r\n--" + boundary + "\r\n";
          part += "Content-Disposition: form-data; name=\"";
          part += name;
          part += "\"; filename=\"";
          part += filename;
          part += "\"\r\nContent-Type: ";
          part += type;
          part += "\r\n\r\n";  // CRLF to end header, and CRLF before content

          wifi.println(part.length(), HEX);
          wifi.println(part);

          uint8_t buf[1 << 12];  // has to fit in stack; maybe use global buffer instead
          for (size_t r = file.read(buf, sizeof(buf)); r > 0;) {
            wifi.println(r, HEX);
            wifi.write(buf, r);
            wifi.println();
            Serial.printf("(Wrote %d of %s, now position %d)\n", r, file.name(), file.position());
            r = file.read(buf, sizeof(buf));
          }
        };

        auto endPart = [&]() {
          String part = "\r\n--" + boundary + "--\r\n";  // -- on both ends marks terminating boundary

          wifi.println(part.length(), HEX);
          wifi.println(part);
          wifi.println(0);  // last chunk
          wifi.println();
        };

        stringPart("json", json);
        filePart("videoKey", videoFile, aviFilePath + 1, "video/avi");  // + 1 to skip leading slash
        filePart("audioKey", audioFile, wavFilePath + 1, "audio/wav");
        endPart();

        Serial.printf("Movement sent with status: %d\n", statusFromResponse(wifi));

        videoFile.close();
        audioFile.close();

        wifi.stop();
      } else {
        Serial.println("The movement was not sent due to missing server connection.");
      }
    } else {
      Serial.println("The movement was not sent due to missing Wifi connection.");
    }
  } else {
    Serial.println("The movement was not sent due to a problem with the files.");
  }
}

// Constructing JSON
#include <ArduinoJson.h>

// DHT
#include <AM2302-Sensor.h>
constexpr unsigned int DHTPIN{ DHT_Pin };
AM2302::AM2302_Sensor am2302{ DHTPIN };

// TimeStamp
#include "time.h"
#include "sntp.h"
const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
const char *time_zone = Time_Zone;

// Function to get local time
String printLocalTime() {
  struct tm timeinfo;
  //2024-03-02 13:39:38.645436
  while (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    delay(5000);
    printLocalTime();
  }
  char time[20];
  strftime(time, 20, "%F %H:%M:%S", &timeinfo);
  String timestamp = String(time);
  return (timestamp);
}

// Callback function (get's called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("(Got time adjustment from NTP)");
  printLocalTime();
}

// HX711
#include "HX711.h"
HX711 scale;
#include <RunningMedian.h>                        // calculate median of measured weight values
RunningMedian valuesWeight = RunningMedian(255);  // Storage Format for weight values whereby max is 255 values
const int LOADCELL_DAT_PIN = HX711_Data_Pin;
const int LOADCELL_CLK_PIN = HX711_CLK_Pin;
const int terminal_weight = Terminal_Weight;
const int calibration_weight = Calibration_Weight;
const float thresholdWeight = Threshold_Weight;
const float thresholdTaring = Threshold_Taring;

// Function to setup the balance
void setupBalance() {
  scale.begin(LOADCELL_DAT_PIN, LOADCELL_CLK_PIN);
  int balanceReferenceUnit;
  // following if clause is needed for initial calibration of the balance
  if (calibration_weight != 0) {
    balanceReferenceUnit = terminal_weight / calibration_weight;
    scale.set_scale(balanceReferenceUnit);
  }
  scale.tare();
}

// Function for taring the balance in case of producing not vaild values.
void balanceTaring() {
  if (terminal_weight != 0 && calibration_weight != 0) {
    Serial.println("Starting balance taring");
    float taringWeight = scale.get_units();
    if (taringWeight < thresholdWeight) {
      while (taringWeight < -thresholdTaring || taringWeight > thresholdTaring) {
        Serial.println("(Measured weight (" + String(taringWeight) + " g) not in valid range. Balance is tared.)");
        scale.tare();
        delay(3000);
        taringWeight = scale.get_units();
      }
      if (taringWeight > -thresholdTaring && taringWeight < thresholdTaring) {
        Serial.println("(Measured weight (" + String(taringWeight) + " g) in valid range. No taring of balance needed.)");
      }
    }

    Serial.println("Ending balance taring");
  }
}

// Audio
/*
- documentation: https://docs.espressif.com/projects/arduino-esp32/en/latest/api/i2s.html
- example: https://github.com/espressif/arduino-esp32/blob/c17a688b628ac80fc997c9cedf16bdd37085de20/libraries/ESP_I2S/examples/Record_to_WAV/Record_to_WAV.ino 
*/
#include <ESP_I2S.h>
I2SClass I2S;
int RECORDING_DURATION = Audio_Recording_Duration;
int SAMPLING_RATE = 16000;
int clk = Audio_CLK_Pin;
int din0 = Audio_Data_Pin;

void recordAudio() {
  I2S.setPinsPdmRx(clk, din0, -1, -1, -1);
  I2S.begin(I2S_MODE_PDM_RX, SAMPLING_RATE, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, -1);

  uint8_t *wav_buffer;
  size_t wav_size;

  audioFile = SD.open(wavFilePath, FILE_WRITE);

  Serial.println("(Recording Audio)");
  wav_buffer = I2S.recordWAV(RECORDING_DURATION, &wav_size);

  // Write the audio data to the file
  if (audioFile.write(wav_buffer, wav_size) != wav_size) {
    Serial.println("Failed to write audio data to file!");
    return;
  } else {
    Serial.println("(Audio saved)");
  }

  I2S.end();
  audioFile.close();
}

// Video
#include "esp_camera.h"
#define CAMERA_MODEL_XIAO_ESP32S3
#include "camera_pins.h"
bool camera_sign = false;

// Function to setup the camera
void setupCamera() {
  // Initialize the camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
  camera_sign = true;
}

// Function for recording a video
/*
Currently, a image (MJPEG) is recorded and stored as AVI. 
*/
void recordVideo() {

  // Camera & SD available
  if (camera_sign && sd_sign) {

    videoFile = SD.open(aviFilePath, FILE_WRITE);
    if (!videoFile) {
      Serial.println("Error opening video file!");
      return;
    }
    Serial.println("(Recording Video)");

    // Recording of test image needed to overwrite old buffer (otherwise old content will be recorded)
    camera_fb_t *test = esp_camera_fb_get();
    esp_camera_fb_return(test);

    camera_fb_t *fb = esp_camera_fb_get();
    videoFile.write(fb->buf, fb->len);
    esp_camera_fb_return(fb);


    videoFile.close();
    Serial.println("(Video saved)");
  }
}

// Environment
const String ENVIRONMENT_ENDPOINT = Server_Environment_Endpoint;

// Function to send an environment
void sendEnvironment(String environment) {
  // Check WiFi connection status
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // HTTP Post application/json
    http.begin("https://" + String(host) + ENVIRONMENT_ENDPOINT + STATION_ID);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(environment);
    if (httpResponseCode == 200) {
      Serial.println("The following environment data has been sent (HTTP Response Code (" + String(httpResponseCode) + "):");
    } else {
      Serial.println("The following environment data was NOT sent (HTTP Response Code (" + String(httpResponseCode) + "):");
    }
    Serial.println(environment);
    http.end();
  } else {
    Serial.println("WiFi Disconnected, Restarting Script with Setup!");
    setup();
  }
}

// Function to track an environment
StaticJsonDocument<96> trackEnvironment() {
  if (WiFi.status() == WL_CONNECTED) {
    // measurement TimeStamp
    String timestamp = printLocalTime();

    // measurement DHT
    float temperature = am2302.get_Temperature();
    float humidity = am2302.get_Humidity();

    StaticJsonDocument<96> environment;
    environment["date"] = timestamp;
    environment["temperature"] = String(temperature);
    environment["humidity"] = String(humidity);

    return (environment);
  } else {
    Serial.println("WiFi Disconnected, Restarting Script with Setup!");
    setup();
  }
}

// Function to handle the complete process of environments
void taskEnvironmentFunction() {
  StaticJsonDocument<96> environment = trackEnvironment();
  String output;

  serializeJson(environment, output);
  sendEnvironment(output);
}

// Tasks
#include <TaskScheduler.h>
const int environmentIntervalMin = Environment_Interval;
const int environmentIntervalMS = environmentIntervalMin * 60 * 1000;
const int taringIntervalMin = Taring_Interval;
const int taringIntervalMS = taringIntervalMin * 60 * 1000;
Task taskEnvironment(environmentIntervalMS, TASK_FOREVER, &taskEnvironmentFunction);
Task taskBalanceTaring(taringIntervalMS, TASK_FOREVER, &balanceTaring);
Scheduler runner;

// Tracking Movement
String movementStartingTimestamp;
String movementEndingTimestamp;

//Function to handle the movement process
void trackMovement() {
  float currentWeight = scale.get_units();

  if (currentWeight > thresholdWeight && valuesWeight.getCount() == 0) {
    Serial.println("Movement recognized!");

    movementStartingTimestamp = printLocalTime();

    valuesWeight.add(currentWeight);

    recordVideo();
    delay(100);
    recordAudio();

  } else if (currentWeight > thresholdWeight && valuesWeight.isFull() != true) {
    Serial.println("Movement ongoing! (Currently measured weight: " + String(currentWeight) + " g)");
    valuesWeight.add(currentWeight);

    delay(1000);  // delay of 1 sek. (There are only 255 measurements per movement possible, therefore we want to limit it.)

  } else if (currentWeight < thresholdWeight && valuesWeight.getCount() >= 1 || valuesWeight.isFull() == true) {
    Serial.println("Movement ended!");

    movementEndingTimestamp = printLocalTime();

    float weightAverage = valuesWeight.getMedian();
    valuesWeight.clear();

    StaticJsonDocument<96> movement;
    movement["start_date"] = movementStartingTimestamp;
    movement["end_date"] = movementEndingTimestamp;
    movement["audio"] = "audioKey";
    movement["video"] = "videoKey";
    movement["weight"] = weightAverage;
    movement["environment"] = trackEnvironment();

    String json;
    serializeJson(movement, json);
    Serial.println("The following movement was tracked: ");
    Serial.println(json);

    // Sending Movement
    sendMovement(json);

    SD.remove(aviFilePath);
    SD.remove(wavFilePath);

  } else {
    Serial.println("Waiting for movement! (Currently measured weight: " + String(currentWeight) + " g)");
  }
}

// Setup Function
void setup() {
  Serial.begin(115200);
  Serial.println("Starting Birdiary");

  // Setup HX711
  Serial.println("Setup HX711");
  setupBalance();

  // Setup Wifi
  Serial.println("Setup Wifi");
  connectToWifi();

  // Setup DHT
  Serial.println("Setup DHT");
  am2302.begin();
  delay(3000);

  // Setup TimeStamp
  Serial.println("Setup Time");
  sntp_set_time_sync_notification_cb(timeavailable);
  sntp_servermode_dhcp(1);
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  // Setup Video
  Serial.println("Setup Video");
  setupCamera();

  // Setup SD
  Serial.println("Setup SD");
  setupSD();

  // Setup Tasks
  Serial.println("Setup Tasks");
  runner.init();
  runner.addTask(taskEnvironment);
  runner.addTask(taskBalanceTaring);
  taskEnvironment.enable();
  taskBalanceTaring.enable();
}

// Loop Function
void loop() {
  // running tasks which should be executed in a certain time interval
  runner.execute();

  // handle movements recognized via balance
  trackMovement();
}
