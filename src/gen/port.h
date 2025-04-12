#ifndef PORT_H
#define PORT_H

typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned long Bit32u;
typedef signed long Bit32s;

typedef Bit8u* RealPt;

#if defined(__BORLANDC__)

#define host_readw(p) (*(Bit16u*)(p))
#define host_readd(p) (*(Bit32u*)(p))

#define host_readws(p) *(Bit16s*)(p)
#define host_readds(p) *(Bit32s*)(p)

#define host_writew(p, d)       (*(Bit16u*)(p) = d)
#define host_writed(p, d)       (*(Bit32u*)(p) = d)

#define host_writews(p, d)       (*(Bit16s*)(p) = d)
#define host_writeds(p, d)       (*(Bit32s*)(p) = d)

#else

#define __abs__(v) abs(v)

static inline Bit16s host_readws(Bit8u* p)
{
	return (Bit16s)host_readw(p);
}

static inline Bit32s host_readds(Bit8u* p)
{
	return (Bit32s)host_readd(p);
}

static inline Bit16s host_writews(Bit8u* p, Bit16s val)
{
	return (*(Bit16s*)(p) = val);
}

#endif

#endif
