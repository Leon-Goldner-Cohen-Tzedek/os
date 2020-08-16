#ifndef BITS_H
#define BITS_H
//just some bit manipulation macros so that i don't go insane

#define lobyte(x)(short)(x & 0x00ff)
#define hibyte(x)(short)(x & 0xff00)

#endif
