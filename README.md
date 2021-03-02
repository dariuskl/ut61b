UT61B Monitoring Utility for Linux
====================================

A very simple utility that will connect to the UT61B (probably other models
will work fine) and output its measurements on the console.

It would be very easy to adapt this to write a log or interface it to something
else. The code is pretty straight-forward.

To enable the serial communication on the UT61B, long-press the REL button.


## Open points

- The serial communications IC in the UT-DT04 serial cable requires an
  autosuspend, initially. This can be achieved by running the enclosed script
  `autosuspend.sh`. It will go through the connected USB devices and look for
  1a86:e0008 to perform the autosuspend. It is based off a script taken from
  [1].
  However, I'd much prefer, if that could be done by the utility itself and
  I'll look into how to do that at some point.

[1]: http://erste.de/UT61/index.html
