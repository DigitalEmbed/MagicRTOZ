#pragma once

#if (defined(__AVR_ATmega328__) || defined(__AVR_ATmega328P__))

    #include "./config.h"

    #include <alloca.h>
    #include <assert.h>

    #include <avr/boot.h>
    #include <avr/cpufunc.h>
    #include <avr/fuse.h>
    #include <avr/interrupt.h>
    #include <avr/io.h>
    #include <avr/lock.h>
    #include <avr/pgmspace.h>
    #include <avr/power.h>
    #include <avr/sfr_defs.h>
    #include <avr/signature.h>
    #include <avr/sleep.h>
    #include <avr/version.h>
    #include <avr/wdt.h>

    #include <compat/deprecated.h>
    #include <compat/ina90.h>

    #include <ctype.h>
    #include <errno.h>
    #include <inttypes.h>
    #include <math.h>
    #include <setjmp.h>
    #include <stdint.h>
    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include <time.h>

    #include <util/atomic.h>
    #include <util/crc16.h>
    #include <util/delay.h>
    #include <util/delay_basic.h>
    #include <util/parity.h>
    //#include <util/setbaud.h>
    #include <util/twi.h>

#endif