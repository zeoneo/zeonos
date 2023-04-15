## Introduction

This is toy/hobby operating system for arm32 target.
Mostly it will be tested on raspberry pi 3b+.
In future I can try support for arm64 and other recent RPIs like RPI 4

I will be using my code and expericence from following my repositories

- Bare Metal Code https://github.com/zeoneo/rpi3b-bare-metal    
    - Contains most of the init code like interrupts, paging, uart devices etc
- https://github.com/zeoneo/rpi-3b-wifi
    - More related to wifi connection using wpa_supplicant

Raspi2b -> bcm2836 has cortexA7 cores
Raspi3b -> bcm2837 has cortexA53 cores -> in 32bit mode cortexa-53 (armv8-a arch) is same as cortex-a7 (armv7 arch). Hence while emulating on qemu
we will use Machine = raspbi2b and cpu = cortex-a7

## TODO List
[] - Create project structure and build scripts
[] - 
