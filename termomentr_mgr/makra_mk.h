#ifndef MAKRA_H_INCLUDED
#define MAKRA_H_INCLUDED
// Makra upraszczaj¹ce dostêp do portów
// *** PORT
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)
// *** PIN
#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)
// *** DDR
#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)

#define ROTATE_RIGHT(x) x=(x>>1)|(x<<7)
#define ROTATE_LEFT(x) x=(x<<1)|(x>>7)

#ifndef cbi
#define cbi(reg,bit)	reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg,bit)	reg |= (_BV(bit))
#endif

#ifndef outb
#define	outb(addr, data)	addr = (data)
#endif
#ifndef inb
#define	inb(addr)			(addr)
#endif
#ifndef outw
#define	outw(addr, data)	addr = (data)
#endif
#ifndef inw
#define	inw(addr)			(addr)
#endif

#define MIN(a,b)			((a<b)?(a):(b))
#define MAX(a,b)			((a>b)?(a):(b))
#define ABS(x)				((x>0)?(x):(-x))

typedef unsigned char  u08;
typedef   signed char  s08;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned long  u32;
typedef   signed long  s32;
typedef unsigned long long u64;
typedef   signed long long s64;

enum {Left=0, Right};
enum {Long=0,Short};
enum {DisplayOFF=0, DisplayON};
enum {pon=0,wto,sro,czw,pia,sob,nie};

#define nop() {asm volatile("nop"::);}

#endif
