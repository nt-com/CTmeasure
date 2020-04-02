# CTMeasure

Contactless Temperature Measurement.

NOT A MEDICAL DEVICE.

See the doc folder for a nice PDF documentation. Read this first!

#### Usage

STM32 Version:

(1) Hook up the electronics, I added a schematic below.

(2) Get the software to the microcontroller. Import the project to the OpenSTM32 IDE or the IDE of your choice and flash it.

(3) Connect to the bluetooth module and start the UI program (plot_rt.py) as SUDO to be able to access the rfcomm device. You may have to change the device name in the python program. Otherwise, If you don't need a graphical output, connect via screen, minicom, etc.

Watch the data on the screen.

ATMega328p Version:

go to /src/ folder compile with make do sudo ./flash_script when it asks you what to flash type app and press enter the software should be running on the microcontroller now go to src and start either plot_serial.py or read_serial.py (probably need to do this as root)
