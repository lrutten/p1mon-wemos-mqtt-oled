help:
	echo help

init:
	pio project init --board lolin32

pkg: pkg-gfx pkg-ssd1306 pkg-mqtt

pkg-gfx:
	pio pkg install --library "adafruit/Adafruit GFX Library^1.11.11"

pkg-ssd1306:
	pio pkg install --library "adafruit/Adafruit SSD1306@^2.5.13"

pkg-mqtt:
	pio pkg install --library "adafruit/Adafruit MQTT Library@^2.5.9"
	#pio pkg install --library "knolleary/PubSubClient@^2.8"
	#pio pkg install --library "hideakitai/MQTTPubSubClient@^0.3.2"

run:
	pio run -t upload

monitor:
	pio device monitor -b 115200

clean-all:
	rm -rvf include
	rm -rvf lib
	rm -rvf platformio.ini
	rm -rvf src
	rm -rvf test




