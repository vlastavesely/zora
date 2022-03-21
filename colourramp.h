#ifndef __ZORA_COLOURRAMP_H
#define __ZORA_COLOURRAMP_H

void colourramp_interpolate_temperature(unsigned short *r, unsigned short *g,
					unsigned short *b, unsigned int len,
					unsigned int temperature);

#endif /* __ZORA_COLOURRAMP_H */
