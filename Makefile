PORT ?= /dev/ttyACM0
BOARD = arduino:renesas_uno:unor4wifi
SKETCH = arduino_pong.ino

compile:
	arduino-cli compile -b $(BOARD) $(SKETCH)

upload:
	arduino-cli upload -b $(BOARD) -p $(PORT)

upload_verbose:
	arduino-cli upload -b $(BOARD) -p $(PORT) -v

monitor:
	arduino-cli monitor -p $(PORT)

run:
	make compile
	make upload
	make monitor
