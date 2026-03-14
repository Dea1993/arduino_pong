compile:
	arduino-cli compile -b arduino:renesas_uno:unor4wifi ./arduino_pong.ino

upload:
	arduino-cli upload -b arduino:renesas_uno:unor4wifi -p /dev/ttyACM0

upload_verbose:
	arduino-cli upload -b arduino:renesas_uno:unor4wifi -p /dev/ttyACM0 -v

monitor:
	arduino-cli monitor -p /dev/ttyACM0

run:
	make upload
	make monitor
