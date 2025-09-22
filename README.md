# SolarDirectionClass (for solar panels)
A class to calculate the direction and elevation of a solar panel to point it accurately to the position of the sun  
It can also be used to prove that the earth is NOT flat :-)
    
Please note that calculations are only valid for dates between 1901 and 2099, due to an approximation used in the Julian Day calculation.   

The functions used in the Solar class were copied from spreadsheets published on the website of the Global Monitoring Laboratory:  
https://gml.noaa.gov/grad/solcalc/calcdetails.html  
I noticed this text on the noaa site so leave the comment below intact:  
  
=============================================================================================================  
The information on government servers are in the public domain, unless specifically annotated otherwise, and may be used freely by the public
so long as you do not:  
1) claim it is your own (e.g. by claiming copyright for NOAA information – see next paragraph),  
2) use it in a manner that implies an endorsement or affiliation with NOAA,  
or  
3) modify it in content and then present it as official government material.  
You also cannot present information of your own in a way that makes it appear to be official government information.
  
=============================================================================================================
  
An example of how to use this class can be found in the main function at the end of main.cpp  

Modification, 2025-09-19:  
An axtra function was added to the class to determine whether the sun is up or down on a given date/time. I needed this to switch off a battery charger and to switch on an inverter during the night.  
  
Good luck,  
Hennie Peters  
Zevenaar, Netherlands
