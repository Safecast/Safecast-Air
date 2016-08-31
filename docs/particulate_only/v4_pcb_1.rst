Reflow soldering SMT components
===================================================

There are several surface mount (SMT) components in the kit. These should be soldered onto the board first before the through-hole components. For this step you will need solder paste and tweezers. We use a toaster oven for reflow soldering.
If you are new to reflow soldering, there are online resources and tutorials. 

Overview of steps
---------------------

* Place a small amount of solder paste onto the pads for the component you are working on
* Use tweezers to pick up, place and align the component onto the solder paste
* If necessary, use a magnifying lens to check the component placement on the board
* Repeat steps for all components before reflow soldering 
* Once all of the components below are placed you can begin reflow soldering 


Voltage divider resistors for the alphasense 4-electrode gas sensors
-------------------------------------------------------------------------- 

R1-R23 odd-numbered resistors (68K) and R2-R24 even-numbered resistors (20K) are paired. For example, R1 & R2 are paired, R3 & R4 etc. Each resistor pair is for one of the 12 analog inputs. The resistor pair form a voltage divider for scaling 0-5V sensor output to 0-1.2V analog input range (using 1.2V internal reference voltage).


* 20K (0.1%) resistors (R2,4,6,8,10,12,14,16,18,20,22,24)
* 68K (0.1%) resistors (R1,3,5,7,9,11,13,15,17,19,21,23)

.. figure:: _static/v4_1a.png
   :align:  center


.. figure:: _static/v4_1b.png
   :align:  center

   
   
MOSFEFT power switch components
--------------------------------------------

* P-Channel MOSFET (Q1)
* 22 Ohm resistor (R25) 
* 10K resistor (R26) 


.. figure:: _static/v4_1h.png
   :align:  center


   
Circuity for toggle switch
----------------------------
 
* 20K Resistor (R27)
* 12K Resistor (R28)
* 0.1 uF capactor (C1) 
* Schmitt buffer (U8)



.. figure:: _static/v4_1g.png
   :align:  center


I2C pull-up resistors
------------------------------------------

* 4.7K resistors (R29 and R30) 



.. figure:: _static/v4_1e.png
   :align:  center

   
   
After the last components have been placed, the board is ready to go into the reflow oven. This usually takes aapprox. 5 mins.


   
Images of the board after reflow soldering
--------------------------------------------


.. figure:: _static/v4_1i.png
   :align:  center

|

.. figure:: _static/v4_1j.png
   :align:  center


|

.. figure:: _static/v4_1k_l.png
   :align:  center




