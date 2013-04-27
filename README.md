amdmsrt
=======

amdmsrt (amdmsrtweaker-lnx) is a Linux port of the AmdMsrTweaker tool written by Martin Kinkelin and extended by Marcus Pollice.

It has been tested with a 14h (AMD E-350) and a 15h (AMD A10-5700) processor, but should work with all processors supported by AmdMsrTweaker. (Fam 10h-15h)

Changes to frequency will not be reflected by /proc/cpuinfo, but a quick benchmark such as "openssl speed sha1" should show a speed difference.

Download
--------

You can download statically compiled executables from bintray. After downloading and extracting, you might have to make the file executable. You can do this with "chmod +x amdmsrt-amd64" (or "chmod +x amdmsrt-i386") in a shell or in the file properties if you use a graphical file manager.

* [32 bit executable](http://dl.bintray.com/content/johkra/amdmsrt/amdmsrt-i386.zip?direct) (sha1: cacf4a6ce93ee4dd9dadec43dd77687c5283acc8)
* [64 bit executable](http://dl.bintray.com/content/johkra/amdmsrt/amdmsrt-amd64.zip?direct) (sha1: d2c1b7b23a05555f38194c7228cd8732877b280e)

Compilation
-----------

Clone the repo and make sure you have gcc and make installed. Then execute "make" in the cloned directory. Optionally copy the file "amdmsrt" to a directory in $PATH such as /usr/bin or /usr/local/bin.

Usage
-----

Make sure you have the msr and cpuid modules loaded. ("modprobe msr"/"modprobe cpuid" as root) The program will otherwise exit with a corresponding error message.

See the file "readme.txt" for usage examples. The program has to executed as root and the executable is called "amdmsrt".

TODO
----

- Proper documentation
- Find a way to modify GPU p-states for Trinity. (10W more in idle on Linux compared to Windows, probably caused by missing Linux PowerPlay support for Trinity with the open drivers.)
