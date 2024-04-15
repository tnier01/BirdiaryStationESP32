# GetStarted 

This GetStarted Manual contains the software and hardware requirements, as well as instructions for setting up the Birdiary Script for station operation with an ESP32. 

## Hardware 
The following hardware components are recommended to operate the ESP32 as a birdiary station. 

### Hardware required
- [Seeed Studio XIAO ESP32S3 Sense](https://www.seeedstudio.com/XIAO-ESP32S3-Sense-p-5639.html)
    -  A microcontroller that already includes a camera sensor (OV2640), as well as a digital microphone, WiFi, 8MB PSRAM, 8MB Flash.
    -  The Birdiary ESP32 application is optimised for this microcontroller. The application generally also works with other ESP32 microcontrollers (additional sensors may then have to be provided, attention should be paid to ensuring that sufficient PSRAM is available and adjustments to the software may have to be made).
- [DHT22](https://www.berrybase.de/dht22-digitaler-temperatur-und-luftfeuchtessensor-mit-breakout-board-jumperkabeln-f-f)
- [M5Stack (HX711)](https://www.berrybase.de/m5stack-mini-wiegeeinheit-hx711-waegezellen-verstaerker) and [seeed Grove adapter cable](https://www.berrybase.de/seeed-grove-adapterkabel-4-pin-dupont-female-4-pin-grove-stecker-5er-pack)
- [strain gauge, 1 kg](https://www.berrybase.de/dehnungsmessstreifen-waegezelle-1kg)
- A perch or other landing option is required so that the bird can land on the strain gauge and can therefore also be weighed. The recommended perch can be modelled with the [Birdiary DIY instructions](https://docs.google.com/document/d/1ItowLull5JF3irzGtbR-fCmgelG3B7DSaU1prOeQXA4/edit#heading=h.vr18r8juhbra). 
- [USB-C cable](https://www.berrybase.de/usb-c-3.1-generation-1-kabel-c-stecker-c-stecker-schwarz) for connection to your computer and power supply 
- Dupont cabel 
    - The Seeed Studio XIAO ESP32S3 Sense contains only one GND slot. The GND cable from the DHT22 and the M5 stack must therefore be connected to this slot. We recommend using two Dupont cables for this and joining them at one end. 
    - e.g. you could use this cable as starting point: [Dupont cable 2 pin](https://www.berrybase.de/kabel-mit-dupont-steckverbinder-awg26-35cm-2-pin)
- [micro SD card](https://www.berrybase.de/sandisk-extreme-micro-sdhc-a1-uhs-i-u3-speicherkarte-adapter-32gb)
    - For the temporarly storage of the recorded content (video and audio) a micro SD card is required. 
- [2x Pin Strip](https://www.berrybase.de/stiftleiste-1x-7-polig-rm-2-54-gerade) and soldering equipment 

### Hardware preparations 
![Pin Map XIAO ESP32S3 (Sense) Birdiary Station](/PinMap.png "Pin Map XIAO ESP32S3 (Sense) Birdiary Station")

- Seeed Studio XIAO ESP32S3 Sense 
    - If the Seeed Studio XIAO ESP32S3 Sense was ordered via the linked [shop](https://www.seeedstudio.com/XIAO-ESP32S3-Sense-p-5639.html), the expansion board (including camera sensor, microphone, micro SD slot) must still be plugged onto the chip board and the WiFi antenna must be attached. A detailed [manual](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#hardware-preparation) is available. 
    - In order to be able to connect the cables from the sensors (balance, DHT22) to the ESP32, the corresponding pins must be reachable on the ESP32. To do this, the pin strips must be soldered to the underside of the board so that the connectors can be plugged into the board from below. 
- Connecting balance and ESP32 
    - Connecting strain gauge and perch 
        - The perch must be attached to the strain gauge with a screw. The recommended installation method is described in the [Birdiary DIY instructions](https://docs.google.com/document/d/1ItowLull5JF3irzGtbR-fCmgelG3B7DSaU1prOeQXA4/edit#heading=h.r44s1od4mln0).
    - Connecting M5Stack and strain gauge 
        - The strain gauge cables can be clamped in the M5 stack. A PH0x60mm screwdriver is required. 
            - The green cable of the strain gauge has to be connected to A+ of the M5Stack. 
            - The white cable of the strain gauge has to be connected to A- of the M5Stack. 
            - The black cable of the strain gauge has to be connected to E- of the M5Stack. 
            - The red cable of the strain gauge has to be connected to E+ of the M5Stack. 
        - The wide part of the adapter cable is plugged into the M5 stack, the individual cables into the corresponding pins of the ESP32. 
    - Connecting ESP32 and M5Stack, as well as DHT22 
        - The individual data cables need to be connected to the corresponding pins of the ESP32. The corresponding pins are defined in the [config.h](/main/config.h). You can simply use the default or define the pins your way. Besides the [pin map](PinMap.png) in this repository, a additional [pin map](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#hardware-overview) of the XIAO ESP32S3 Sense is available. 
        - The ```5V``` pin of the M5Stack must be connected to the ```5V``` pin of the ESP32, the ```+``` pin of the DHT22 must be connected to the ```3V3``` pin of the ESP32. 
        - As already described, there is only one ```GND``` pin on the XIAO ESP32S3 Sense, which must be shared between balance (```GND``` pin) and DHT22 (```-``` pin). We recommend using two Dupont cables, joining them at one end and plug this end into the GND pin of the XIAO ESP32S3 Sense.
- microSD  
    - To use the microSD you need to use the correct format. A [HowTo](https://wiki.seeedstudio.com/xiao_esp32s3_sense_filesystem/#prepare-the-microsd-card) is available. 
    - Connecting microSD card and ESP32 
        - Plug in the microSD into the slot below the camera. 
- Connecting ESP32 and local computer  
    - You can connect the XIAO ESP32S3 Sense with the USB-C cable to your local computer for data transfer and power supply. 

## Software 
The following software should be used and is required to operate the ESP32 as a birdiary station. 

- The script is Arduino code and can be transferred to the ESP32 via Arduino. Therefore the Arduino IDE is required. 
    - [Instructions](https://docs.espressif.com/projects/arduino-esp32/en/latest/installing.html) for installing the Arduino IDE on your local computer is provided by ESPRESSIF. 
        - These instructions must be followed accordingly. 
        - It is reqiured to use the [development release link](https://espressif.github.io/arduino-esp32/package_esp32_dev_index.json) as additional boards manager URL 
        - It is then recommended to select and install version 3.0.0-alpha3 of [esp32 by Espressif Systems](https://github.com/espressif/arduino-esp32) in the Arduino Boards Manager. Newer releases may also work, but the functionality of the Birdiary script cannot be guaranteed. 
        - Select your board and port. A [HowTo](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#software-preparation) is available, have a look at Step 4.
        - Download/Clone the [BirdiaryStationESP32 repository](https://github.com/tnier01/BirdiaryStationESP32) and open the file [main.ino](/main/main.ino) in Arduino. 
- Download the following libraries if they are not already available. You can therefore use the Arduino Library Manager.  
    - [<ArduinoJson.h>](https://arduinojson.org/?utm_source=meta&utm_medium=library.properties) for JSON 
    - [<AM2302-Sensor.h>](https://github.com/hasenradball/AM2302-Sensor) for DHT22
    - ["HX711.h"](https://github.com/bogde/HX711) for HX711, can be found as *HX711 Arduino Library*
    - [<RunningMedian.h>](https://github.com/RobTillaart/RunningMedian) for calculating median of by the HX711 recorded values 
    - [<TaskScheduler.h>](https://github.com/arkhipenko/TaskScheduler) for automatizing tasks like tracking and sending an environment 
- You need to activate PSRAM of the ESP32. A [HowTo](https://wiki.seeedstudio.com/xiao_esp32s3_camera_usage/#turn-on-the-psram-option) is available.   
- [config.h](/main/config.h)
    - In the ```config.h``` file, you are required to enter your WiFi credentials. There are several additional configuration options available, but you may also choose to use the default settings. 
    - You need to create a [Birdiary Station instance](https://www.wiediversistmeingarten.org/view/createstation) and insert the corresponding ID in the ```config.h``` as variable ```Server_Station_Id```. If you visit your station you can find the ID in the ```URL```: https://www.wiediversistmeingarten.org/view/station/*EXAMPLE-BIRDIARY-STATION-ID*.
    ```Server_Station_Id``` = ```EXAMPLE-BIRDIARY-STATION-ID```
- Upload the program to the ESP32 and check in the Serial Monitor whether everything works. A [HowTo](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/#run-your-first-blink-program) for uploading a program is available. 
- Calibration of the balance 
    - A calibration weight half the value range of the strain gauge should be used.  
    - Load the strain gauge with the calibration weight and read the value displayed in the serial monitor. 
    - In order to proceed, define the weight of the calibration weight as ```Calibration_Weight``` and the previously displayed value in the serial monitor as ```Terminal_Weight``` in grams in the [config.h](/main/config.h).
    - The modified program should then be re-uploaded to the ESP32.  

## Concluding Remarks 
- Unfortunately, there are currently only instructions for the hardware design of the birdhouse based on the Raspberry Pi ([Birdiary Manual](https://docs.google.com/document/d/1ItowLull5JF3irzGtbR-fCmgelG3B7DSaU1prOeQXA4/)). Nevertheless, you can use these instructions as a starting point if you also want to build a birdhouse adapted for ESP32. It is likely that adjustments will be necessary to attach the sensors. 
- In comparison to previous implementations of the birdiary script, the following limitations exist: 
    - Recording duration of a Movement is currently limited to about 255 seconds.
    - The video is an image recorded at the beginning of the movement.
    - The audio is recorded at the beginning of the movement and is limited to a duration which can be set in the ```config.h```. 
- If you would like to support us in minimising these limitations, we would be delighted to receive your [contribution](https://github.com/tnier01/BirdiaryStationESP32/tree/main?tab=readme-ov-file#suggest-feature). 
- If you encounter any issues or have questions, please contact the Birdiary Team via [info@wiediversistmeingarten.org](mailto:info@wiediversistmeingarten.org). 

## Further Documentation 
Furthermore, the following documentation is recommended: 

- [Seeed Studio Documentation](https://wiki.seeedstudio.com/xiao_esp32s3_getting_started/) about the Seeed Studio XIAO ESP32S3 Sense including many examples. 
- Forum 
    - [Arduino Forum](https://forum.arduino.cc/)
    - [Seeed Studio Forum](https://forum.seeedstudio.com/)
    - [ESPRESSIF ESP32 Forum](https://www.esp32.com/viewforum.php?f=19&sid=1b42a5d2c46c207feea2ee31573ec992)
