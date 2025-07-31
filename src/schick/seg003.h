#if !defined(__BORLANDC__)
namespace M302de {
#endif

signed short update_direction(unsigned char);

void move(void);

void door_frame(signed short, signed short, signed short, signed short);

void loot_corpse(Bit8u*, char*, Bit8s*);

void no_way(void);

#if !defined(__BORLANDC__)
}
#endif
