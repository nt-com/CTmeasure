import serial

MLX_RESOLUTION = 0.02
MLX_KELVIN_CONSTANT = 273.15

ser = serial.Serial(
	port='/dev/ttyUSB0',\
	baudrate=9600,\
	parity=serial.PARITY_NONE,\
	stopbits=serial.STOPBITS_ONE,\
	bytesize=serial.EIGHTBITS,\
	timeout=1
)

def main():

	global MLX_RESOLUTION
	global MLX_KELVIN_CONSTANT

	while(1):
		line = []
		line = ser.readline()
		if len(line) > 3:
			temp = line[:2]
			temp = hex(int(temp.encode('hex'), 16))
			temp = int(temp, 16)
			# see MLX90614 datasheet for this calculation
			temp = float(temp) * MLX_RESOLUTION - MLX_KELVIN_CONSTANT
			print(temp)


main()
