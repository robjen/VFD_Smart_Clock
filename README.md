# VFD_Smart_Clock

A Series of 'socket' designs and firmware running on ESP8266 for IoT smart clock

Sockets are 1-4 digits in all sorts of size based on:
* 4 digits IV4/17 16 segment tube
* 1 digit 5x7 IV26 7 point tubes type 1 (fully independant dots) and type 3  (1-2-1-2-1 )
* 2 digits 2x 5x7 IV25 7 point tubes type 1 and type 3 (yes found these on the internet named them as above)
* LED lighting under/behind each tube with a WS2812 RGB 
* HV module on each board so they are fully independant and run from a single 5V supply
* 4 digital lines for easy control passed though each board SPI-clk SPI-data SPI-load BLANK LED-data

Any of the types of socket can be chained, but you might have to mess about with the firmware if you want mix-n-match different types

Firmware provides:
* clock display 
* NTP sync
* auto DST based on timezone
* hidden 'phrases'
* RSS news reader (BBC)
* Ability to add user messages
* Web interface for configuration
* a load of animated transitions and effects
* a few LED effects (more coming soon)

loads of new features coming soon

its a bit of a mess right now but in progress



