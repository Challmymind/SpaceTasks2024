# Overview
PCB project for AP63300 converter (24V-5V 3A). The components selected should be able to withstand normal operation and protect the circuit in an emergency.
I concentrated on making sure the circuit would survive potential ESD, voltage spikes and short circuits. PCB layout designed according to AP63300 datasheet best practices and some general rules:
* decoupling capacitors should be as close as possible to the decoupled device
* the ground plane should be as large as possible to improve grounding and thermal performance
* high current traces should be as wide as possible (ideally thicker copper could be used)
* components connected to the high voltage track should have good DC bias parameters

## Table of contents
* [Overview](#overview)
* [Components](#components)
  * [Fuse](#fuse)
  * [Diode](#diode)
  * [Capacitors](#capacitors)
* [Layout](#layout)

# Components
Reasons for the components I have chosen.

## Fuse
When the circuit is powered but no external output is connected, the AP63300 outputs 2.9mA, enough to light a diode (13% of maximum brightness), generting 14.5mW with an efficiency of around 5%.
This means that the circuit (when supplied with 24V) draws 12mA. When the circuit is delivering maximum current (3A), the efficiency is around 90%. Calculating the input current in this case, we get about 700mA.
That's why I decided to limit the input current to 1A with a fuse to protect the output devices from overloading. The AP63300 has built-in overcurrent protection.

## Diode
I'm using the same diode as in the original schematic because its clamping voltage is 33V, which is good considering the maximum input voltage of the AP63300 is 35V. 

## Filter
At the end of the output I also added a low pass filter with popular values and ran a noise simulation in LTspice. The results seemed good, so I left it in the design. I thought it might make the ADC readings less susceptible to some induced noise or fluctuations from the power line. I could also put low-pass filters on inputs of the differential amplifier, but to work properly the amplifier needs the same impedance on each side, and the tolerance of capacitors makes them much harder to match than resistors.

# Capacitors
The capacitors on the high side were difficult to choose because I had to find capacitors with good DC bias parameters. There were better than I placed in the final project, but the prices and availability made me lower my expectations.
That's still a lot better than the 90% loss of capacity at 24V that I found in very few products.

# Layout
I tried to use mostly polygons because I knew that power related boards are hard to balance thermally. I tried to follow every "best practice" that the AP63300 offered.
It was also my first time using Altium, so I spent half a day just getting a feel for it before committing to the final layout.
