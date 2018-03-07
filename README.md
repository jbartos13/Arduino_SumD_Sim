# Arduino SumD Simulator
---

The SumD Simulator is Arduino code that will read a button and Arm and Disarm a Betaflight Flight Controller. The code was used to enable the flightcontroller to be uses as an inexpensive IMU data collection device.

Code works by using an Adruino Promini 5v connected to the UART3 pins on an Tinyfish FC, however the compatibilty could be extended to to any Betaflight FC that has an open UART connector.

Major portions of this code have been modified from [Openlrsng](https://openlrsng.org).

JB

3/6/18
