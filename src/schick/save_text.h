//0x20
void init_text(void);
//0x25
void split_textbuffer(char **, char*, uint32_t);
//0x2a
void load_ggsts_nvf(void);
/* 0x2f */
signed int load_game_state(void);
/* 0x34 */
signed int save_game_state(void);
/* 0x39 */
signed int read_chr_temp(char*, const signed int, const signed int);
//0x3e
void write_chr_temp(const signed int);
/* 0x43 */
signed int copy_chr_names(uint8_t*, const signed int);
//0x48
void load_in_head(const signed int);
//0x4d
void load_tx2(const signed int);
//0x57
void load_tx(const signed int);
//0x57
void load_ltx(const signed int);
//0x61
void prepare_chr_name(char*, char*);
//0x61
void prepare_sg_name(char*, char*);
//0x66
void load_tempicon(signed int);
