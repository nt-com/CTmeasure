# CTmeasure -- Contactless Temperature Measurement

# What this project is not:
# THIS IS NOT A MEDICAL DEVICE
## I am not a medical professional, nor a medical engineer. Just an embedded systems guy trying to help.

## Introduction

The recent COVID-19 infections demand measures regarding the detection of infected individuals to help combat the spread of the virus. Recently the media has reported of instances around the globe where fever is measured in front of public buildings (for instance supermarkets). After registering with the group "HelpfulEngineering" on Slack and reading the contributions of members, I started working on a way to measure temperatures without contact using infrared technology. The credit belongs to theses groups.

## Method

As a basis for infrared temperature measurement I use the MLX90614 sensor, which is widely available as a breakout board for fast prototyping. In addition to the measurement of temperatures, the data must be evaluated. For this, my solution is a bluetooth module capable of sending the sensor data to a PC for plotting and analysis. The bluetooth module used in this project is the HC05, also widely available. 

As a microcontroller I chose the STM32F401RE, which I had available as a prototyping platform. Here, I coded some of the drivers through a course I did a while back and the missing parts by myself. The code does not use any HAL or API, it is written from the ground up to remove all unnecessary parts, hopefully resulting in a small codebase with minimal complexity.

In order to display the measured data on a PC, I connect the bluetooth module as a serial port. Through a python script utilizing the serial and matplotlib library I am able to plot the data in "real-time".

As a result, a system that can measure temperatures without direct skin contact is realized. In addition, because the evaluation of the measurement results is done via bluetooth the person measuring must have no direct contact with the people whos fever shall be measured.

## Remarks

I wanted to push out this project as fast as possible. Thus, it is still in a prototype state. If this idea brings some benefits regarding the battle against COVID-19 I will work on it further and enhance its features. Please, see this project as a baseline for future development.

If it is not suitable for application, at least the readers of this readme know how it doesn't work.

## Technical Details

### MLX90614

The MLX90614 sensor uses I2C to communicate. There are two important things to know about this:

(1) The internal pull-ups of the STM32F4 ARE NOT SUFFICIENT! Use external pull-ups, currently I am using 12kOhms.
(2) The MLX90614 demands a repeated start condition after the register that shall be read is communicated to the sensor

If (1) is not considered, the I2C timing requirements are violated, making repeated start impossible (2), thus the sensor does not respond with any data. This is solved in my code.

### HC05 Bluetooth

The bluetooth module is straight forward. Just let the microcontroller send the measured temperature via UART to the module. Connect it to your PC as a serial port and use the python script to display the data. No problems there. The good thing about bluetooth is that lots of devices speak it. Thus, smartphones, tablets, laptops can connect to the sensor device.

### Software

#### Problems:

(1) I documented the software extensively, however, some parts are not yet documented the way I want them to be. This will be fixed as soon as I get around to it. After it is done I will generate a doxygen documentation.

(2) In the current state a delay is introduced by a loop after each measurement. I am not satisfied with this. What I want is a timer that triggers each measurement via an interrupt. This will be added. 

The software works as intended and is, as far as i can tell, stable (has been running for a couple of hours now).

#### Architecture

The drivers are all encapsulated by the application software, the main function is as small as possible. However, the architecture needs to be modified in my opinion, further uncoupling the concrete drivers from the function of the application itself. This will be done in future implementations.

## Evaluation

First, a measurement of the ambient room temperature:

![alt text](https://github.com/nt-com/CTmeasure/doc/img/ambient.png)

Regarding the accuracy of this measurement, I cannot give any better values. The datasheet of the MLX90614 says the accuracy is at 0.5 degrees celsius, with a resolution of 0.02 degrees celsius. However, the temperature seems plausible. I do not have any other means to measure the ambient temperature other than an old thermometer. 

Second, a measurement of my forehead temperature. The first graphic shows the rise in temperature at the time I put my head against the sensor. The second graphic shows the temperature after about 20-30 seconds in front of the sensor. I was pretty close to the sensor through, however never touching it with my skin. The temperature of about 36 degrees C measured by the prototype was confirmed by a fever thermometer.

![alt text](https://github.com/nt-com/CTmeasure/doc/img/temp1.png)
![alt text](https://github.com/nt-com/CTmeasure/doc/img/temp2.png)

## Conclusion

At the moment the prelimiary results show that the prototype works as intended. However, as the sections before describe, there is still work to be done. One major improvement is a better user interface, displaying the measured temperature more accurately in addition to the changes of the microcontroller software. (Better user interface means not hacked together in 3 minutes).

In further iterations, the software should be ported to a cheaper, smaller microcontroller to reduce the cost of the system. Here, the expensive floating point calculations for converting the raw temperature values of the sensor to a human-readable decimal format can be left to the PC instead of the microcontroller.

## Closing Remarks

Everything I have done can be done using an arduino with the arduino IDE, but I don't really like that stuff. Just so you know, If you want to get this up and running on an arduino, there are great tutorials AND ready to use libraries for the MLX90614 and the HC05 online.

If you want to contact me, drop me a line at nmt@nt-com.org. Stay Healthy, Stay Safe.

# Usage

(1) Hook up the electronics, I added a schematic below. 

(2) Get the software to the microcontroller. Import the project to the OpenSTM32 IDE or the IDE of your choice and flash it.

(3) Connect to the bluetooth module and start the UI program (plot_rt.py) as SUDO to be able to access the rfcomm device. You may have to change the device name in the python program. Otherwise, If you don't need a graphical output, connect via screen, minicom, etc.

Watch the data on the screen.

# Future Work

When I get around to it / people think this project makes sense / works, I will:

(1) add the timer and interrupts

(2) build a nice UI 

(3) port to a smaller microcontroller


