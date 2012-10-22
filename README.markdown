# UDP Test

Written May, 2009  
By Ralph Lin  
University of Washington  
Neurobotics Lab  
Demo for RSS2009  

## Description:

This program is used to test UDP socket communications to 
communicate between the EMG computer (Windows) to robot controller
(RT Ubuntu) computer for the human-in-the-loop RSS 2009 demo (emgClassification.cpp).

It consists of two paired programs, dserver.cpp and dclient.cpp
which receives and send messages with no error checking.

## Syntax:

* dserver PortNumber  
where PortNumber is the port where dserver should listen for
packets from dclient

* dclient ServerName PortNumber  
where ServerName is host name running dserver and PortNumber is 
the port where to use for communications