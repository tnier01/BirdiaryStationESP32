/*
Configuration file for Birdiary Station using SEEED XIAO ESP32S3 (Sense)
*/

// SD 
#define SD_Data_Pin 21 
#define Avi_File_Path "/video.avi"
#define Wav_File_Path "/audio.wav" 

// WiFi 
#define WiFi_SSID "..."
#define WiFi_Password "..."

// Server 
#define Server_Host "wiediversistmeingarten.org"
#define Server_Port 443
#define Server_Environment_Endpoint "/api/environment/"
#define Server_Movement_Endpoint "/api/movement/"
#define Server_Station_Id "999" // A Birdiary Station instance can be created here: https://www.wiediversistmeingarten.org/view/createstation

// DHT 
#define DHT_Pin 9 

// Time
#define Time_Zone "CET-1CEST,M3.5.0,M10.5.0/3"

// HX711 
/*
Set Terminal_Weight and Calibration_Weight to 0, during calibration. 
- Threshold_Weight = Threshold above which a weight measurement is recognised as movement. 
- Threshold_Taring = Threshold above which a weight measurement is recognised as false weight, triggers taring of balance. 
*/
#define HX711_Data_Pin 1
#define HX711_CLK_Pin 2
#define Terminal_Weight 542700 
#define Calibration_Weight 500
#define Threshold_Weight 5.00
#define Threshold_Taring 1.00

// Audio 
#define Audio_Recording_Duration 6 
#define Audio_CLK_Pin 42
#define Audio_Data_Pin 41

// Tasks 
/*
Specify in min the frequency in which a certain process should be carried out. 
- Environment_Interval = Interval in which environment data is sent to the server. 
- Taring_Interval = Interval in which the balance is checked whether it still provides valid values. 
*/
#define Environment_Interval 2 
#define Taring_Interval 2 
