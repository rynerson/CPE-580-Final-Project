# CPE-580-Final-Project
This was my final project from Embedded Systems Design in the Fall of 2024. The goal of the project was to extend a previous lab assignment. 
The original lab assignment was to display the switch data of a tm4c123gh6pm launchpad to a website. The original lab involved creating a website on a hosting platform, setting up a esp8266 WiFi module with AT commands,
and sending the data to the website with a POST query. In my final project, I needed to send the switch data from two tm4c123gh6pm launchpads to the site. This required extensive editing of the PHP script. I introduced new variables for the
other board. I also kept track of the output and used regexes to find and replace outdated information. When I wrote to the file, I used the argument LOCK_EX to ensure that nothing else could write to the file while something was being written.

TOOLS USED:
tm4c123gh6pm launchpad (x2)
esp8266 WiFi modules (x2)
wires
laptop
Webhosting platform (I used Awardspace, but would not recommend it)
PuTTY

You will need to host the html file on a web hosting site. You will need to wire up the esp8266's according to the picture given in the lab report. You will then need to build and run both projects on two separate tiva boards. 
