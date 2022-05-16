#include "./config.h"

#if (defined(__AVR_ATtiny85__))
  #pragma message "uC: ATtiny85"
#endif

#if (defined(__AVR_ATmega328__))
  #pragma message "uC: ATmega328"
#endif

#if (defined(__AVR_ATmega328P__))
  #pragma message "uC: ATmega328P"
#endif

#if (ENABLE_GPIO == 1)
  #pragma message "GPIO: enabled!"
#else
  #pragma message "GPIO: disabled!"
#endif