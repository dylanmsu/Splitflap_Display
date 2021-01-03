# Splitflap_Display
software to digtally drive a public splitflap display using arduino. The display is meant do be deployed in a steamtrain station where it reads the data from their server.
This project is still in development and requires major improvements and code cleanup.

## this repository contains:
* arduino library
* kicad schematic and pcb desgin
* basic documentation about the splitflap display modules

## PCBs
The driver PCBs can drive 8 splitflap modules at the same time, for that it uses 4 SN754410NE chips (L293D in version 4) that are driven by two 74HC595 shift registers. One shift registers drives the H-bridges, the other enables the outputs of the 8 H-Bridges so they dont waste unnecessary power. The communication protocol is quite simple. You have two serial data inputs in the input connector, one is to enable the individual H-Bridges (DiE, ClkE, LaE), the other is to drive the individual H-bridges (Di, Clk, La). <br> To actually drive splitflap modules with it, open the arduino sketch named TestDriver. Connect the splitflap modules solenoid connections to the H-Bridge output pairs. Then connect the serial pins to the arduino pins listed in the sketch. and lastly connect 20v to the Vdd input connector. Upload the sketch, the splitflaps should be working.

* V1: first version, fully made in kicad with through hole components
* V4: latest version, made in EasyEDA with mostly SMD components

## authors
* Dylan Missuwe
* Tijl Desmet
