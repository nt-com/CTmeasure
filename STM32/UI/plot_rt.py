import serial
import datetime as dt
import matplotlib.pyplot as plt
import matplotlib.animation as animation

# Create figure for plotting
fig = plt.figure()
ax = fig.add_subplot(1, 1, 1)
xs = []
ys = []

ser = serial.Serial(
    port='/dev/rfcomm0',\
    baudrate=9600,\
    parity=serial.PARITY_NONE,\
    stopbits=serial.STOPBITS_ONE,\
    bytesize=serial.EIGHTBITS,\
        timeout=1)

# This function is called periodically from FuncAnimation
def animate(i, xs, ys):


    # Read temperature (Celsius) from TMP102
	line = ser.readline()
	print(line)
	if len(line) > 2:
			temp_c = float(line)
			# Add x and y to lists
			xs.append(dt.datetime.now().strftime('%H:%M:%S.%f'))
			ys.append(temp_c)

					# Limit x and y lists to 20 items
			xs = xs[-40:]
			ys = ys[-40:]

					# Draw x and y lists
			ax.clear()
			ax.plot(xs, ys, 'r-o', linewidth=3)
			plt.tick_params(
				axis='x',          # changes apply to the x-axis
				which='both',      # both major and minor ticks are affected
				bottom=False,      # ticks along the bottom edge are off
				top=False,         # ticks along the top edge are off
				labelbottom=False) # labels along the bottom edge are off
						# Format plot
			#plt.xticks(rotation=45, ha='right')
			plt.subplots_adjust(bottom=0.30)
			plt.title('MLX Temperature over Time')
			plt.ylabel('Temperature (deg C)')

# Set up plot to call animate() function periodically
ani = animation.FuncAnimation(fig, animate, fargs=(xs, ys), interval=1)
plt.show()
