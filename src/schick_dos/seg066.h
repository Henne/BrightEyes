/* 0x20 */
signed int enter_location(const signed int);
/* 0x25 */
signed int enter_location_daspota(void);
/* 0x2a */
signed int get_border_index(const unsigned char);
/* 0x2f */
void do_special_buildings(void);
/* 0x34 */
void TLK_eremit(const signed int);
/* 0x39 */
void do_town(void);
/* should be static */
/* 0x3e */
void town_update_view(void);
/* 0x43 */
void town_building_textures(void);
/* should be static */
/* 0x48 */
void town_direction_change(void);
/* 0x4d */
void load_town_texture(signed int, signed int, signed int, signed int);
/* 0x52 */
void town_water_and_grass(void);
/* 0x57 */
void town_do_step(const signed int);
/* 0x5c */
void town_fade_and_colors(void);
/* 0x61 */
void town_update_house_count(void);
/* 0x66 */
void town_set_vis_field_tex(void);
/* 0x6b */
void town_set_vis_field_vals(void);
/* 0x70 */
signed int town_step(void);
