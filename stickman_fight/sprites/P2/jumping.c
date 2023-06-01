#ifdef __AVR__
#include <avr/io.h>
#include <avr/pgmspace.h>
#elif defined(ESP8266)
#include <pgmspace.h>
#elif defined(__IMXRT1052__) || defined(__IMXRT1062__)
// PROGMEM is defefind for T4 to place data in specific memory section
#undef PROGMEM
#define PROGMEM
#else
#define PROGMEM
#endif
static const unsigned char jumpingP2[] PROGMEM =
  "\000\000\000\000\000\000\000\000\000\000\000\000\377\377\377\377\377\377\377\377\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\377\377\377\377\377\377\377\377\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\377\377\377\377\377\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\377\377\377\377\377\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000Q\214Q\214\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000,c,cQ\214Q\214"
  "\377\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000,c,cQ\214Q\214\377\377\377\377"
  "\000\000\000\000\000\000\000\000,c,c,c\000\000\000\000Q\214Q\214\000\000\000\000\377\377\377\377\000\000\000\000"
  ",c,c,c\000\000\000\000Q\214Q\214\000\000\000\000\377\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000Q\214Q\214\000\000\000\000\377\377\377\377\000\000\000\000\000\000\000\000\000\000,c,cQ\214Q\214"
  "\000\000\000\000\377\377\377\377\000\000\000\000\000\000\000\000\000\000,c,c\377\377\377\377\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000,c,c\377\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000,c,c\377\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000,c,c"
  "\377\377\377\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\377\377"
  "\377\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\377\377\377\377\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"
  "\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000"//,
//};
;
