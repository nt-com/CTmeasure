import serial
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation

MLX_RESOLUTION = 0.02
MLX_KELVIN_CONSTANT = 273.15

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

ser = serial.Serial(
    port='/dev/ttyUSB0',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=1)

# This function is called periodically from FuncAnimation
def animate(i, xs, ys):

	global MLX_RESOLUTION
	global MLX_KELVIN_CONSTANT

    # Read temperature (Celsius) from TMP102
	line = ser.readline()
	print(line)
	if len(line) > 3:
			temp = line[:2]
			temp = hex(int(temp.encode('hex'), 16))
			temp = int(temp, 16)
			# see MLX90614 datasheet for this calculation
			temp_c = float(temp) * MLX_RESOLUTION - MLX_KELVIN_CONSTANT
			print(temp_c)	

			# Add x and y to lists
			xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
			ys.append(temp_c)

			# Limit x and y lists to 40 items
			xs = xs[-40:]
			ys = ys[-40:]

			mean = 0.0
			for i in ys:
				mean += i
			mean = mean / len(ys)
			mean_value = [mean for i in range(len(xs))]

			# Draw x and y lists
			ax.clear()
			ax.plot(xs, ys, 'go', linewidth=2)
			ax.plot(xs, mean_value, 'r-', linewidth=4)

			plt.tick_params(
				axis='x',          # changes apply to the x-axis
				which='both',      # both major and minor ticks are affected
				bottom=False,      # ticks along the bottom edge are off
				top=False,         # ticks along the top edge are off
				labelbottom=False) # labels along the bottom edge are off
						# Format plot
			#plt.xticks(rotation=45, ha='right')
			plt.subplots_adjust(bottom=0.30)
			plt.ylabel('Temperature (deg C)')

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=1)
plt.show()
