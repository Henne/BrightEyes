#ifndef PORT_H
#define PORT_H

typedef unsigned char Bit8u;
typedef signed char Bit8s;
typedef unsigned short Bit16u;
typedef signed short Bit16s;
typedef unsigned long Bit32u;
typedef signed long Bit32s;

typedef Bit8u* RealPt;
typedef Bit8u* PhysPt;

#if defined(__BORLANDC__)

#ifdef __cplusplus
#define INTCAST void interrupt (*)(...)
#else
#define INTCAST void interrupt (*)()
#endif

typedef Bit8u huge * HugePt;

#define Real2Host(p) ((Bit8u*)(p))

#define F_PADA(p, o) (*((HugePt*)p) += o)
#define F_PADD(p, o) ((HugePt)(p) + o)
#define F_PSUB(p1, p2) ((HugePt)(p1) - (HugePt)(p2))

#define H_PADD(p, o) F_PADD(p, o)

#define reloc_gen (0)

#define mem_readb(p) *(signed char*)(p)
#define mem_writeb(p, d) *(unsigned char*)(p) = d

#define host_readb(p) (*(Bit8u*)(p))
#define host_readw(p) (*(Bit16u*)(p))
#define host_readd(p) (*(Bit32u*)(p))

#define host_readbs(p) *(Bit8s*)(p)
#define host_readws(p) *(Bit16s*)(p)
#define host_readds(p) *(Bit32s*)(p)

#define host_writeb(p, d)       (*(Bit8u*)(p) = d)
#define host_writew(p, d)       (*(Bit16u*)(p) = d)
#define host_writed(p, d)       (*(Bit32u*)(p) = d)

#define host_writebs(p, d)       (*(Bit8s*)(p) = d)
#define host_writews(p, d)       (*(Bit16s*)(p) = d)
#define host_writeds(p, d)       (*(Bit32s*)(p) = d)

#else

typedef Bit8u* HugePt;

#define INTCAST RealPt

#define __abs__(v) abs(v)

static inline Bit8s host_readbs(Bit8u* p)
{
	return (Bit8s)host_readb(p);
}

static inline Bit16s host_readws(Bit8u* p)
{
	return (Bit16s)host_readw(p);
}

static inline Bit32s host_readds(Bit8u* p)
{
	return (Bit32s)host_readd(p);
}

static inline Bit8s host_writebs(Bit8u* p, Bit8s val)
{
	return (*(Bit8s*)(p) = val);
}

static inline Bit16s host_writews(Bit8u* p, Bit16s val)
{
	return (*(Bit16s*)(p) = val);
}

#endif

#endif
