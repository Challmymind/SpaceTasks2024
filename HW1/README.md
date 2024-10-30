# Overview
Current sensing circuit to operate on a 24V line with a maximum of 50A. Based on HED. PS. I'm using HED as hall effect device.

## Table of contents
* [Overview](#overview)
* [Rationale](#rationale)
  * [HED](#hed)
  * [Substractor and divider](#substractor-and-divider)
  * [Filter](#filter)
* [LTSpice](#ltspice)
* [Layout](#layout)
* [Reference](#reference)

# Rationale
My main circuit consists of HED, separated voltage divider, substractor and low pass filter.
I have made some more or less reasonable assumptions:
1. 5V power supplies are quite popular, so I assume I can power my circuit with them.
2. There is probably a pretty good (clean) 3V3 power signal if you are using an ADC with a 0-3V3 range.
3. The power lines I used already had some kind of ESD protection elsewhere (like TVS diode).

## HED
Why HED? This is because traditional sensing with a sense resistor would waste a lot of power. I could also use transformers, which require no external power supply, unlike the Hall effect sensor, but then the measured line must be AC.
The Hall effect solution also has another major advantage in compare to sense resistor: Galvanised insulation so no loading effect of any kind and improved safety. I have selected a unidirectional Hal sensor. For a while I thought
I could get away without a differential amplifier, but even with this design I needed it as a substractor (with the resistor I still wanted to do high-side sensing, which is why I needed it anyway). My chosen HED outputs voltage in the range <0V5,3V5>
for <0A,50A>, so I had to scale the signal a bit. [Here is a link to what I used](https://octopart.com/datasheet/acs780llrtr-050u-t-allegro+microsystems-75430442)

## Substractor and divider
As all Hall devices have some DC offset, and I wanted a linear scale from 0 to 3V3, I had to get rid of it. That's why I use a differential amplifier as a substractor to get the offset. It forces me to use very low tolerance resistors to make sure the divider isn't a source of noise. I am powering the HED using 5V so I used the same line with voltage divider to make offset to be substracted. I separated the divider from the circuit with a voltage follower to make sure it wouldn't affect the opamp operation (resistance loading). This part fixes my signal in the range <-0V3,3V6> because I'm using ADC power lines to power the opamp and using saturation as clamping. For the allowed range of amperage on the input <0A,50A> it generates voltage in the range <0V,3V3>. In an emergency, the 3V6 voltage shouldn't exceed the maximum power limits. Without another supply, it's not possible to get better clamping with diodes anyway (Vfwd is too large even for Schottky). I could use an OpAmp with clamp feature, but it's more expensive, but it's possible.

## Filter
At the end of the output I also added a low pass filter with popular values and ran a noise simulation in LTspice. The results seemed good, so I left it in the design. I thought it might make the ADC readings less susceptible to some induced noise or fluctuations from the power line. I could also put low-pass filters on inputs of the differential amplifier, but to work properly the amplifier needs the same impedance on each side, and the tolerance of capacitors makes them much harder to match than resistors.

# LTSpice
I simulated part of my circuit using DC sweep and noise simulations to check if I met the requirements. Here is screen from spice. Some results are placed in the Altium schematic.
![spice](https://github.com/Challmymind/SpaceTasks2024/blob/main/HW1/preview_sim.png)
[Spice file](https://github.com/Challmymind/SpaceTasks2024/blob/main/HW1/Scaling_circuit.asc)

# Layout
I didn't have time to make another PCB, but here are the main things I would consider if I were to make one.

* Ensure that the hall sensor is as far away as possible from other lines/tracks that could induce a strong magnetic field.
* Try to position the sensor plates (line between them) parallel to other lines/tracks that could induce a strong magnetic field.
* Ensure that the 50A trace/polygon is wide enough to avoid thermal problems.
* The power line should approach the sensor from the desired angle range of 0° to 85°.
* Ensure that there is no excess copper under the sensor.
* Make big ground plane for better thermal.
* Ideally use one plane as a power plane and separate it into sections for each voltage
* Keep the trace to the ADC as short as possible.

# Reference
[Selected_Current_Sensing_Circuits](https://bibliotekanauki.pl/articles/274787.pdf) <br>
[Non-Intrusive Hall-Effect Current-Sensing Techniques Provide Safe, Reliable Detection and Protection for Power Electronics ](https://www.allegromicro.com/en/insights-and-innovations/technical-documents/hall-effect-sensor-ic-publications/non-intrusive-hall-effect-current-sensing-techniques-for-power-electronics)<br>
[Hall Effect Current Sensing: Open-Loop and Closed-Loop Configurations](https://www.allaboutcircuits.com/technical-articles/hall-effect-current-sensing-open-loop-and-closed-loop-configurations/) <br>
