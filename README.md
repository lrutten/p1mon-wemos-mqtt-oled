# Wemos Lolin32 ESP32 with OLED


This board subscribes to the `p1mon` MQTT messages.
These messages originate from the Raspberry Pi which is connected
to the P1 port of the smartmeter.

This Raspberry Pi runs PHP software which every 10s relays the received power info to
the Mosquitto MQTT server.
The PHP software is open source and can be downloaded from:

* [https://www.ztatz.nl/](https://www.ztatz.nl/)

The received MQTT messages are built up as a JSON object.
The `nlohmann` C++ library is used to decode the received JSON message.
This library consists only of 1 file: `json.hpp`.


These `platformio` libraries are used in this project:


~~~~
[env:lolin32]
platform = espressif32
board = lolin32
framework = arduino
lib_deps = 
	adafruit/Adafruit MQTT Library@^2.5.9
	adafruit/Adafruit SSD1306@^2.5.13
~~~~

