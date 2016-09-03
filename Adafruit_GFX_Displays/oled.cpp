/*
 * oled.cpp
 *
 *  Created on: Sep 1, 2016
 *      Author: Mike Mehr
 */

#define ARDUINO 10000 // MLM - joking around but needed

extern "C" {
#include "oled.h"
#include "spi.h"
#include "stdio.h"
}

#include "Adafruit_SSD1306.h"

Adafruit_SSD1306 OLED(OLED_DC, OLED_RST, OLED_CS);
void debugRAM(void);

// set up the pins on the LPC-1769 via the class into the SPI implementation (doesn't work for I2C for now)
// will return 0 if cannot instantiate the class, 1 if it's good to go
int oled_init(void) {
	// set up the I/O pins on the LPC board via CMSIS
	spi_pinConfig(OLED_DC, 0); // use GPIO (func.0)
	spi_pinConfig(OLED_RST, 0);
	spi_pinConfig(OLED_CS, 0);
	// NOTE: assumes SPI0 is already set up for device sharing, so we only need to add these 3 pins to the mix
	OLED.init(OLED_DC, OLED_RST, OLED_CS);
	return 0;
}

int oled_begin(int reset) {
	// perform the reset if asked, return result
	// ALSO NOTE: this sets up the pin directions on DC and CS, and RST if a reset is requested
	// it assumes SPI has already been set up by someone else (i.e., the BMPE driver)
	// it then sets up SPI transfer mode and sends a series of commands to the device to initialize it
	OLED.begin(SSD1306_SWITCHCAPVCC, SSD1306_I2C_ADDRESS, (reset==1), true);
	debugRAM();
	OLED.display(); // send RAM buffer to the hardware (initially set up with logo)
	return 0;
}

void oled_clearDisplay(void) {
	OLED.clearDisplay(); // RAM buffer only
	OLED.display(); // to the hardware
	return;
}

void oled_invertDisplay(int inverted) {
	OLED.invertDisplay(inverted); // RAM buffer only
	OLED.display(); // to the hardware
	return;
}

void debugRAM() {
	printf("Pixel RAM contents:\n");
    for (uint16_t i=0; i<(SSD1306_LCDWIDTH); i++) {
        for (uint16_t j=0; j<(SSD1306_LCDHEIGHT); j++) {
        	int pixel = OLED.getPixel(i,j);
        	printf("%s", pixel? ".": " ");
        }
        printf("\n");
    }
}
/*
ATTEMPTED ADAFRUIT LOGO RECORDED FROM RAM:
Unfortunately it did NOT show up like this when displayed by the display() function.
---
Pixel RAM contents:
                    ......
                    ........                  ..
                    ..........           .......
                    ...........        .........
                    ...........       ..........
                    ............     ...........
                    ............    ............
                    .............  .............
                    ............................
                    ............................
                    ............................
                    ............................
                     ......  ..................
                     ......   ........  .......
                      .....   .......   ......
                       .....   .....    .....
                        ....   ....    .....
                     ........  ....   .....
                  ............ .... .........
                 ..............................
               ..................................
              ............    .......   ...........
             ...........      .......      .........
            ...........     ..... ....     ..........
           .....................  ...................
          ......................   ...................
           .....................   ...................
             ...................   ...................
                ................   ...................
                          ......  .....  ............
                          ....... .....    .........
                          ..............     .....
                          ..............
                          ..............
                          ..............
                          ...............
                          ..............
                           .............
                           .............
                            ...........
                            ..........
                             .......
                              .....
                 .......
                .........           ..   ...........
               ....   ....          ..   ...........
               ..       ..
               ..       ..
               ..       ..
               ..       ..                ..........
                ..     ..                ...........
               ...........                ..........
               ...........                ..
                                         ..
                                         ..
                  .....                  ...
                 .......                  ..........
               ....   ...                  .........
               ..       ..
               ..       ..
               ..       ..                  ......
               ..       ..                 ........
                ..     ..                 ...    ...
       ..................                ...      ..
       ...................               ..        ..
       ..................                ..        ..
                                         ..       ..
                   ...                    ..     ..
                 .......         ...................
                ...   ...        ...................
               ...     ...        ..................
               ..       ..
               ..       ..
               ..       ..               ........
                ..     ..                ...........
                ...   ...                        ...
               ...........                        ..
               ...........                         ..
                                                  ..
                                          ..........
               ..                        ..........
       ...................                .......
       ...................
      ..       ..                          ..     .
      ..       ..                         ....   ...
                                         ......   ..
               ...........               ..  ..    ..
               ...........               ..  ...  ..
                ...                      ...  ......
               ..                         .    ....
               ..
               ..
               .                         ..
                                     ...............
               ........               ..............
               ..........                ..
                       ...               ..
                        ..                ..........
                        ..               ...........
                        ..                ..........
                ..........                ..
               ..........                ..
               ........                  ..
                                         ..

          ..   ..........
         ...   ...........          ..   ...........
          .    ..........           ..   ...........


               ..                             ..
           ...............                 ........
           ...............                .........
           ..............                ...  .   ..
               ..                        ..   .   ..
                                         ..   .    ..
                                         ..   .   ...
                                         ...  .   ..
                                          .....  ..
                                           ....  ..
                                              .
                                           ..     .
                                          ....   ...
                                         .. ...   ..
                                         ..  ...   ..
                                         ...  ..  ..
                                          ..  ......
                                               ....
*/
