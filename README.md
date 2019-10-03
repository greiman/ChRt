#### ChRt - ChibiOS/RT Arduino Library

This is a version of ChibiOS for Arduino.
I am starting a new repository since this version is not backward
compatible with the previous libraries. Both AVR and ARM are now in
a single library.

This version adds support for Teensy 4.0. Teensy 4.0 has support for free 
running tick-less mode and periodic mode.  The default for Teensy 4.0 is
tick-less mode. Edit libraries/ChRt/src/teensy4/chconf_teensy4.h to
select periodic mode or change parameters for Teensy 4.

See this article about ChibiOS tick-less mode:

http://www.chibios.org/dokuwiki/doku.php?id=chibios:articles:tickless

AVR also supports tick-less mode.  The default is periodic mode.  Edit
libraries/ChRt/src/teensy4/chconf_teensy4.h to select tick-less mode.

Due, SAMD, and Teensy 3.x are supported in periodic mode.

The base code for ChRt was written by Giovanni Di Sirio, the author
of ChibiOS/Nil and ChibiOS/RT.

See this site for detailed documentation of ChibiOS/RT 19.1.

http://www.chibios.org/dokuwiki/doku.php

The kernel is version 6.0.3 of ChibiOS/RT from ChibiOS release 19.1.3.

https://osdn.net/projects/chibios/releases

If you are installing from the GitHub repo zips, rename the folder ChRt
before copying it to your Arduino/libraries folder.

Please read libraries/ChRt/doc/ChRt.html for more information.  See the 
Examples section of the html documentation.

Start with the ChBlink example which is traditional for almost every RTOS.

