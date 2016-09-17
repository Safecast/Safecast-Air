Version 3.0
=========================

Image of Prototype V0.1 R3 
------------------------------


.. figure:: _static/rev3_1.png
   :align:  center

   
  

Bill Of Materials
--------------------
   
**This is the BOM for prototype V0.1 R3 shown above**

=============  =====  =============================================   ===================     =======================         
Ref            Qty    Description                                     Vendor                  Part #
=============  =====  =============================================   ===================     =======================         
R1-23 (odd)    12     68K resistor (1%)                               Digikey                 S68KCACT-ND
R2-24 (even)   12     20K resistor (1%)                               Digikey                 S20KCACT-ND
R25,R26        2      10k resistor                                    Digikey                 S10KQCT-ND
R27            1      20K resistor                                    Digikey                 S20KQCT-ND
R28            1      12k resistor                                    Digikey                 S12KQCT-ND
R29,R30        2      4.7k resistor (select via I2C requirements)     Digikey                 S4.7KQCT-ND     
C1             1      0.1uf ceramic capacitor                         Digikey                 399-4151-ND

P1, P2         2      Mini shrouded header, 5x2                       Digikey                 WM18560-ND
P3             1      Shrouded header, 5x2                            Digikey                 S9169-ND
P4, P11        2      2x2 pin array                                   Digikey                 609-3225-ND                 
P5,7,8         3      Breakaway header, male pins, 36 pos             Digikey                 3M9457-ND
P6             1      Screw terminal block, 2-pos                     Digikey                 ED2635-ND
P9             1      6-pin JST XH connector                          Digikey                 455-2271-ND
P10            1      5-pin JST XH connector                          Digikey                 455-2270-ND 
S1             1      Switch, 4-direction with center push            Online components       SKQUCAA010
S2             1      Slide switch                                    Digikey                 401-2000-ND

U1             1      Adafruit Ultimate GPS Breakout                  Adafruit                746
U2             1      XBEE                                            Sparkfun                WRL-08742
(or            1      RN-XV WiFly                                     Sparkfun                WRL-10822) 
U2             2      2mm 10pin XBee Socket                           Sparkfun                PRT-08272
U3             1      Monochrome 1.3" 128x64 OLED graphic display     Adafruit                938
U4             1      OpenLog                                         Sparkfun                DEV-09530
U5             1      3.3V, 600mA step down voltage regulator         Pololu                  2106
U6             1      Teensy 3.1                                      Sparkfun                DEV-12646
U6             1      Teensy header                                   Digikey                 WM17453-ND
U1,3,6         3      Breakaway header, female, 36 pos                Digikey                 929974E-01-36-ND
U7             1      Resistor array, isolated                        Digikey                 4608X-2-102LF-ND
U8             1      Schmitt buffer                                  Digikey                 TC7SZ17FU(T5LJFTCT-ND
=============  =====  =============================================   ===================     =======================         
 


Build instructions 
==================================

1) Resistors
-----------------------


Voltage divider resistors for the alphasense 4-electrode gas sensors
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* R1-R23 odd-numbered resistors (68K) and R2-R24 even-numbered resistors (20K) are paired. For example, R1 & R2 are paired, R3 & R4 etc.
* Each resistor pair is for one of the 12 analog inputs.
* The resistor pair form a voltage divider for scaling 0-5V sensor output to 0-1.2V analog input range (using 1.2V internal reference voltage).

Take the 68K resistors are solder these into each of the 12 odd-numbered positions from R1-R23 i.e. R1,3,5,7,9,11,13,15,17,19,21 and 23.

.. figure:: _static/1a.JPG
   :align:  center

Next, take the 20K resistors are solder these into each of the 12 even-numbered positions from R2-R24 i.e. R2,4,6,8,10,12,14,16,18,20,22 and 24.


.. figure:: _static/2b.JPG
   :align:  center

   
   
Pull-up resistors 
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

* R25 and R26 are pull-up resistors for the amphenol particulate sensor (P10). Note, these will be likely removed in the next board revision.
* R29 and R30 are I2C pull-up resistors for the UEXT header 

Solder these resistors to the board as shown in the image below.

.. figure:: _static/5.JPG
   :align:  center

   
   
   
2) Joystick input circuit
-------------------------------------

A 5-position joystick toggle switch is used for accessing device configuration menus. It is not yet enabled in firmware.

In addition to the toggle switch (S1) the following additional components are required for the joystick circuit:

* 20K Resistor (R27)
* 12K Resistor (R28)
* 0.1 uF capactor (C1) 
* Resistor array (U7) 
* Schmitt buffer (U8)

Solder the resistors and capacitors onto the board as shown in the pictures below (no specific order). 

.. figure:: _static/7.JPG
   :align:  center

Solder the toggle switch onto the board.

.. figure:: _static/8.JPG
   :align:  center

   
Solder the Schmitt buffer onto the board (image to be added).

3) Headers for gas, particulate sensors
-----------------------------------------------------



Sensor Headers
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

There are 5 headers available for connecting sensors to the board. 

* **P1 and P2** - Alphasense Gas sensor headers 
* **P9** - OPC-N2 Alphasense particulate sensor 
* **P10** - Amphenol particulate sensor. In the next board revision P10 will be removed and the digital lines can be used for other purposes. May also add a humidity sensor.
* **P3** - UEXT general purpose expansion header. Has 12C, SPI and UART. Can also be used as additional analog inputs e.g. for connecting methane sensor.


Solder the headers onto the board as shown in the images below.

.. figure:: _static/9.JPG
   :align:  center
.. figure:: _static/10.JPG
   :align:  center
.. figure:: _static/11.JPG
   :align:  center

   
   
4) External 5V power  
-------------------------------------

Optional source of higher current power input which can be used as an alternative to powering from the USB connector on the Teensy (500 mA limit). The slide switch (S2) can be used as an on/off switch for the device when using the external 5V power. 


Solder the blue 2-position terminal block onto the board as shown below. Make sure the terminal block is positioned with the connector side facing out.
 
.. figure:: _static/12.JPG
   :align:  center

Solder the switch onto the board as shown below.
   

.. figure:: _static/13b.JPG
   :align:  center


5) Configuration and expansion headers
----------------------------------------------

These pins provide access to additional features on the Teensy and some additional configuration options for the device.

* AOUT (P8) - This header provides access to the onboard DAC (digital to analog converter) on the Teensy.  
* 3V3SEL (P4) - Selects between the 3.3V regulator on the Teensy or the higher current external 3.3V Pololu regulator. You might use the higher current Pololu regulator when using the XBEE or WiFly.
* PROG (P7) - Provides access to the Teensy program signal which reboots the microcontroller into Bootloader mode. (Development option).
* VBUS (P5) - For connecting the external power to the Teensy. If you are using the external power you will want to put a jumper on the pins.
* UEXT VOUT (P11) - Selects the power for the UEXT expansion header, whether it is 5V or 3.3V. For example, to select 5V, place a jumper on the right pins.


Solder the 2x2 pins into positions P4 and P11.

.. figure:: _static/14.JPG
   :align:  center

Take the 36-position male breakaway header and break off 3 sets of 2-position headers. Solder these into positions P5, P7 and P8. Solder with the longer pin facing up.

   
.. figure:: _static/15.JPG
   :align:  center

   
6) OpenLOG and 3V3 Headers
-------------------------------------

Take the same breakaway header from the previous step and break off a 6-position and a 4-position header. Solder these into positions U4 and U5 as shown in the image below. Solder with the shorter pin side facing up.

|

These pin headers will be used to connect the OpenLog SD card data logger and Pololu 3V3 optional power regulator later in the instructions. 


.. figure:: _static/16.JPG
   :align:  center

7) Teensy Headers
-------------------------------------

There are 3 of the 36-position (pos) female breakaway headers. These breakaway headers can be cut into customs lengths using a pair of wire clippers.  

* Take one of the breakaway headers and cut off two 14-position headers. 
* Take a second breakaway header and cut off two 5-position headers and two 7-position headers. 
* Place the custom headers into the board at position U6 as shown in the image below. You may find that you will need to sand the rough edges of the headers where they were cut to get the headers to all fit into the board. 

|

These female headers will be used to connect the Teensy later in the instructions. 


.. figure:: _static/17b.JPG
   :align:  center



8) GPS, LCD and XBEE Headers
-------------------------------------

Use the remainder of the 36-position (pos) female breakaway header to break off the following headers: 

* 8 pos header x 1
* 9 pos header x 1

Solder these into positions U1 (9 pos header) and U3 (8 pos header). 
These female headers will be used to connect the Adafruit Ultimate GPS Breakout and Adafruit Monochrome 1.3" or 0.96" 128x64 OLED graphic display later in the instructions. 



.. figure:: _static/17d.JPG
   :align:  center
   
   

Take the two pre-cut 10-position headers. These shorter headers are ready to use. Solder these into the board at position U2. 

These female headers will be used to connect the XBEE RF module or Sparkfun RN-XV WiFly later in the instructions. 

.. figure:: _static/18b.JPG
   :align:  center

9) Placing large components
-------------------------------------

   
OpenLog SD card data logger
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Place the OpenLog onto the 6-pin header and solder in place as shown in the image below.

.. figure:: _static/19.JPG
   :align:  center

   
Pololu 3V3  power regulator
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Place the Pololu 3V3 module onto the 4-pin header and solder in place as shown in the image below.

.. figure:: _static/20.JPG
   :align:  center

   
   
XBEE/WiFly wireless communication module
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Insert the XBEE RF module or Sparkfun RN-XV WiFly into the sockets as shown in the image below. 
   
.. figure:: _static/21a.JPG
   :align:  center

   
   
Adafruit Ultimate GPS Breakout
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Adafruit GPS Breakout board comes with a 9-pin array. Solder this onto the breakout board as shown in the images below.
   
.. figure:: _static/22a_b.JPG
   :align:  center

Insert the GPS board into the 9-position female header as shown in the image below.

.. figure:: _static/22c.JPG
   :align:  center
   

   
Adafruit OLED graphic display
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

The Adafruit graphic display comes with an 8-pin array. Solder this onto the board as shown in the image below and insert the LCD display into the 8-position female header.

   
.. figure:: _static/23.JPG
   :align:  center

   
10) Teensy board
-------------------------------------


Take the Teensy header and solder onto the Teensy board as shown in the image below.

 
.. figure:: _static/24.JPG
   :align:  center


Program the Teensy with the Safecast Air firmware

|
   
Place the Teensy board into the female headers as shown below.


 
.. figure:: _static/25.JPG
   :align:  center
  
   


