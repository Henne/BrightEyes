#include "port.h"

#if !defined(__BORLANDC__)
Bit16s bc__read(Bit16u, Bit8u*, Bit16u);
Bit16s bc__close(Bit16u);
Bit16s bc__create(RealPt, Bit16u);
Bit16s bc_write(Bit16s, RealPt, Bit16u);

#else
// __BORLANDC__

#define bc__read _read
#define bc__close _close
#define bc__creat _creat
#define bc_write write

#endif
