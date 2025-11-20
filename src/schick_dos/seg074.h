/* 0x20 */
void show_automap(void);
/* 0x25 */
signed int select_teleport_dest(void);
/* 0x2a */
/* should be static */
signed int is_discovered(const signed int, const signed int);
/* 0x2f */
/* should be static */
void render_automap(const signed int);
/* 0x34 */
/* should be static */
void draw_automap_square(const signed int, const signed int, const signed int, const signed int);
/* 0x39 */
/* should be static */
void draw_automap_entrance(const signed int, const signed int, const signed int);
/* 0x3e */
/* should be static */
void draw_automap_to_screen(void);
/* 0x43 */
/* should be static */
uint16_t get_mapval_small(const signed int, const signed int);
/* 0x48 */
/* should be static */
uint16_t get_mapval_large(const signed int, const signed int);
/* 0x4d */
/* should be static */
signed int is_group_in_prison(const signed int);
/* 0x52 */
/* should be static */
signed int get_maploc(const signed int, const signed int);
