amdmsrt
=======

amdmsrt (amdmsrtweaker-lnx) is a Linux port of the AmdMsrTweaker tool written by Martin Kinkelin and extended by Marcus Pollice.

It has so far only been tested on a Brazos system where reading and settings values appears to be working fine (I can change the voltage), but multiplicator calculation is broken. I believe this to be a bug of the original program, but this needs further testing.

Feel free to try it on other processors supported by AmdMsrTweaker (executing without parameters will only read values). Before using it you have to load the msr, pci and cpuid modules and the program has to be executed as root.
