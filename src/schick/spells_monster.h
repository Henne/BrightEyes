/* 0x20 */
signed int MON_cast_spell(struct enemy_sheet*, signed char);

/* 0x25 */
void mspell_verwandlung(void);

/* 0x2a */
void mspell_bannbaladin(void);

/* 0x2f */
void mspell_boeser_blick(void);

/* 0x34 */
void mspell_horriphobus(void);

/* 0x39 */
void mspell_axxeleratus(void);

/* 0x3e */
void mspell_balsam(void);

/* 0x43 */
void mspell_blitz(void);

/* 0x48 */
void mspell_eisenrost(void);

/* 0x4d */
void mspell_fulminictus(void);

/* 0x52 */
void mspell_ignifaxius(void);

/* 0x57 */
void mspell_plumbumbarum(void);

/* 0x5c */
void mspell_saft_kraft(void);

/* 0x61 */
void mspell_armatrutz(void);

/* 0x66 */
void mspell_paralue(void);

/* 0x6b */
/* should be static */
void MON_do_spell_damage(const signed int);

/* 0x70 */
/* unused */
signed int MON_get_target_PA(void);

/* 0x75 */
/* unused */
signed int MON_get_target_RS(void);

/* 0x7a */
/* should be static */
signed int MON_get_spell_cost(const signed int, const signed int);

/* 0x7f */
/* should be static */
signed int MON_test_attrib3(const struct enemy_sheet*, const signed int, const signed int, const int, signed char);

/* 0x84 */
/* should be static */
signed int MON_test_talent(const struct enemy_sheet*, const signed int, signed char);

/* 0x89 */
/* should be static */
void MON_sub_ae(struct enemy_sheet*, signed int);
