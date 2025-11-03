#if !defined(__BORLANDC__)
namespace M302de {
#endif

#if defined(__BORLANDC__)
//0x20
void unused_store(signed short);
//0x25
Bit8u* unused_load(signed short);
#endif
//0x2a
void load_area_description(const signed int);
//0x2f
#if defined(__BORLANDC__)
void unused_load_archive_file(const signed int, const unsigned short, const Bit32u);
#endif
//0x34
void prepare_dungeon_area(void);
//0x39
void load_map(void);
//0x3e
void load_dungeon_ddt(void);
//0x43
Bit8u* load_city_textures(const signed int, const signed int, const signed int, const signed int);
//0x48
void prepare_city_area(void);
//0x4d
void load_special_textures(const signed int);
//0x52
void call_load_buffer(void);
//0x75
void prepare_area(const signed int);
//0x5c
void call_load_area(const signed int);
//0x61
void load_npc(const signed int);
//0x66
void save_npc(const signed int);
/* 0x6b */
void load_splashes(void);
/* 0x70 */
void load_informer_tlk(const signed int);
//0x75
void load_tlk(const signed int);
//0x7a
void load_fightbg(const signed int);

#if !defined(__BORLANDC__)
}
#endif
