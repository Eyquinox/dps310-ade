# DPS310 read regs
## Author
Arthur DELAHOUSSE

## Overview

DPS310 is a thermometer/barometer chip.

It exposes registers through a I2C bus.

A real usage requires to write some registers over I2C protocol (optional setup),
and then read some other registers to compute temperature and pressure from them.

Below is a real registers dump of such sensor, discovered and printed in a
human form by `i2cdump` Linux tool. At this point, chip is already initialized,
no need to write any register, just to read results.

```text
$ i2cdump 1 0x76
No size specified (using byte-data access)
WARNING! This program can confuse your I2C bus, cause data loss and worse!
I will probe file /dev/i2c-1, address 0x76, mode byte
Continue? [Y/n]
     0  1  2  3  4  5  6  7  8  9  a  b  c  d  e  f    0123456789abcdef
00: f8 01 90 02 34 f2 26 a0 c7 04 00 00 00 10 00 00    ????4?&???...?..
10: 0d ee d6 12 e4 bf 43 c0 fb cb 03 7d e8 9e 00 05    ??????C????}??.?
20: fd bd 00 00 29 53 04 70 c4 00 00 00 00 00 00 00    ??..)S?p?.......
30: 65 17 00 66 c3 0a 00 00 06 00 00 00 00 00 00 01    e?.f??..?......?
40: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
50: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
60: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
70: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
80: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
90: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
d0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
e0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
f0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00    ................
```

You can read official component datasheet from [Infineon website][1].

## Features
* Read temperature from fake dps310 dump
* Convert temperature to human readable
* Print on console temperature
* Send it to MQTT topic /thprobe/ade/temperature every 5 minutes

## Project structure

* main.c : Application entry point, reads temperature and send it via MQTT.
* dps310.c / dps310.h : Sensor processing module, converts raw register values to real temperature.
* fakedps310.c : Dump of DPS310 register for testing.
* mqttlink.c / mqttlink.h : MQTT module using Eclipse Paho C library.
* Makefile : Build and run targets.

# Installation
## Prerequisites
GCC (or any C99/C11-compatible compiler)
Eclipse Paho MQTT C client library
Install on Debian/Ubuntu:

```shell
sudo apt-get update
sudo apt-get install libpaho-mqtt3c-dev
```

## Compilation

A quick and dirty build is provided via the Makefile:

Build the project :
```shell
make
```

Run the program (build if necessary) :
```shell
make buildrun
```

The default target (make) compiles all sources into the mqttlink binary, with the Paho MQTT (-lpaho-mqtt3c) lib.

## Usage
After building, run:
```shell
./thprobe
```

Press Ctrl+C to stop.

# Know limitations
* Error managment has not been yet implemented on dps310 module -> Ensure that the dump is correct (if you change it)
* Use data from dps310 dump -> Update code if you want to read it directly from real dps310
* Doesn't use TLS encryption when sending temperature to MQTT server

[1]: https://www.infineon.com/cms/en/product/sensor/pressure-sensors/pressure-sensors-for-iot/dps310/
