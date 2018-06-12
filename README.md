# Chicken coop project ("Kurník 21. století")
Chicken coop controller board (Eagle schematics) + arduino SW (for arduino and attiny + modified dependent libraries)

## HW comment
* Arduino nano + Attiny85 (for battery protection)
* LCD with nice user friendly menu
* External temp probe + automatic fan controller
* Set your GPS location (via menu) and you are ready to go! (actual Sunrise and Sunset is automaticly calculated every day + you can add user delay time)
* RTC has build in automatic SW correction (cheap RTCs can be running up to 30s late every day) + automatic summer/winter time
* External LED for door status + Data output via Serial 
* H-bridge + motor from screwdriver for door opening (build in safety mechanism for sensor failure) Door position is controlled by Magnetic Sensor Window Alarm Switch (I have tried mechanical and also Hall-sensor switches, but this one is more realible)
* It has also manual override function for doors
* Solar panel / main voltage power supply + UPS 6V battery (up to 2 days of running on battery backup) + Hibernation mode for low backup voltage on battery
* Double sided PCB can be easily prepared at home conditions (I have made many prototype versions)

Inspired by [Arduino chicken project by user RobotChicken](http://www.instructables.com/id/Arduino-Chicken-Coop-Controller/)

## Photos

![Whole box](photos/case01.jpg?raw=true "Whole box")

![Internal menu](photos/case02.jpg?raw=true "Internal menu")

![Actual state of door HW](photos/door01.jpg?raw=true "Actual state of door HW")

![Motor detail](photos/motor.jpg?raw=true "Motor detail")

![PCB01](photos/pcb01.jpg?raw=true "PCB01")

![PCB02](photos/pcb02.jpg?raw=true "PCB02")

![PCB03](photos/pcb03-side-detail.jpg?raw=true "PCB03")
