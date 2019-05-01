# elec327-final-project-smart-lamp
Author: Shuaicheng Li, Chuang Yu, Liwen Jiang; main.c is the code for this project. A screenshot of the brd file is brd.jpg, and a screenshot of schematic is called schematic.png. The PCB of this project is designed in Altium, and the two generated files are PCB.PcdDoc and Sheet1.SchDoc. The required long and short video are named as 327video_long and 327_short. (.mp4)

# Introduction
People need lamps in almost all kinds of places: offices, classrooms, labs etc. Lamps enable us to
work comfortably, and a good lamp can make a person work even more efficiently. As
technology develops, lamps are not only limited to lightning tools: many lamps have more
complex functions like charging, digital display, etc., and as lamps become multifunctional, they
can help with people’s work even more. Therefore, inspired by this idea and trend, we decided to
develop a smart lamp with MSP430 and various sensors: apart from lightning, our lamp is able to
read the room temperature and report to the user through an LCD screen, and can light up/ turn
off automatically by using an infrared motion detector to detect whether there is anyone around.
If there is no one around for a short period of time, the LED lamp will turn off automatically. In
this way, users can know various specs about the room by looking at the LCD screen on the
lamp, and the automatic on/off function helps to prevent energy waste if the user forgot to turn
off the lamp and saves people from manually pushing the switch. Moreover, our project has a
photosensitive module which can detect the environmental light intensity. When the environment
gets dark enough, the LED will turn on automatically. Lastly, our project can further be used as a
reading lamp which can protect user’s vision because the ultrasonic module can tell the distance
between the person and the lamp. If the person gets too close to the lamp (<30cm), the buzzer
will be triggered. If the distance is smaller than 10 cm for a period of time, the lamp will turn off
to protect user’s vision.

# Video
We demonstrated our design [here](https://youtu.be/8P2KgN4l940).

We did a demo [here](https://youtu.be/dvHzbnER3Q4).

# Code
Our code is insider this [repo](https://github.com/tomjlw/elec327-final-project-smart-lamp/) feel free to look at it
