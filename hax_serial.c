/*
 * HAX Serial IO implimentations (non-hardware specific)
 */

#include "hax.h"

void puth(uint16_t data) {
	uint8_t i, digit;
	
	for (i = 0; i < 4; ++i) {
		/* Each four-bit chunk of a binary number is a hex digit. */
		digit  = (uint8_t) ((data & 0xF000) >> 12);
		data <<= 4;
		
		if (digit < 10) {
			_putc('0' + digit);
		} else {
			_putc('A' + digit - 10);
		}
	}
}

void _puti(uint16_t data) {
	char buf[6];
	uint8_t i;
	
	/* Generate the output in reverse order (i.e. the left-most digit is the
	 * most significant digit).
	 */
	for (i = 0; data >= 0; ++i) {
		buf[i] = '0' + (data % 10);
		data  /= 10;
	}
	
	/* Reverse the string prior to sending it to the serial port. */
	for (++i; i > 0; --i) {
		_putc(buf[i - 1]);
	}
}

#if 0
void putf(float data) {
	uint32_t frac;
	uint8_t exp = (uint8_t) ( ( data << 1 ) >> 1) >> 23 ) ;
	
	if (exp == 0xFF) {
		putc('N');
		putc('a');
		putc('N');
		return;
	}
	
	uint32_t frac = (uint32_t) ( data << 9 ) >> 9;

	if ( data >> 31 ) { // bit 31 is the sign bit.
		putc('-');
	}
	
	/* TODO: Do something with the fraction and exponent. */
}
#endif

void _puts(char __rom const *data) {
	for (; *data; ++data) {
		_putc(*data);
	}
}

// This is much nicer with inline functions.
#define AA(n) ( (0xF & n) + '0' )
// If the calculated character is not in the numeric range, push it into the
//  Capital letter range ( 7 away in ascii )
#define H2ASCI( z ) ( AA(z) > '9' )?( AA(z) + 7) : ( AA(z) )
#define PUTH4( y ) _putc( H2ASCI(y) ) 
#define PUTH8( x ) ( PUTH4( (x) >> 4 ) , PUTH4( (x) & 0xF ) )

void _puth(uint8_t data) {
	PUTH8(data);
}

void _puth2(uint16_t data) {
	PUTH8( data >> 8 );
	PUTH8( data & 0xFF );
}

