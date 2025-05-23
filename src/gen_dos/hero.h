struct struct_attribs {
	signed char normal;
	signed char current;
	signed char mod;
}
;

#pragma pack ( 1 )
struct struct_hero {
	/* Offset 0x00 */
	char name[16];
#if defined(NEWHERO)
	/* Offset 0x10 */
	char alias[16];
#endif
	/* Offset 0x20 */
	signed char items;
	signed char typus;
	signed char sex;
	unsigned char height;
	signed short weight;
	signed char god;
	signed char level;
	signed long ap;
	signed long money;
	/* Offset 0x30 */
	signed char rs1;
	signed char rs2;
	signed char unkn1;
	unsigned char bp_left;
	struct struct_attribs attrib[14];
	/* Offset 0x5e */
	signed short le;
	/* Offset 0x60 */
	signed short le_max;
	signed short ae;
	signed short ae_max;
	signed char mr;

	signed char atpa_base;
	signed char at_weapon[7];
	signed char pa_weapon[7];
	signed char w_at_mod;
	signed char w_pa_mod;
	signed char w_type;
	signed char atpa_mod;
	unsigned char le_malus;

	signed char unkn2[4];
	signed char hunger;
	/* Offset 0x80 */
	signed char thirst;
	signed char unkn3;
	signed char direction;
	signed char actions_left;
	signed char unkn4[3];
	signed char unkn5;
	signed char group;
	signed char unkn12;
	signed char position;
	/* Offset 0x8b */
	signed long unkn6;
	signed long unkn7;
	signed char unkn8[3];
	signed char rounds_blinded;
	signed char rounds_eclipt;
	signed char unkn9[8];
	/* Offset 0xa0 */
	signed char axxel;
	signed char drunk;
	signed char unkn10[8];
	signed char status;
	signed char status2;
	signed short unkn11;
	/* Not figured out yet, but reserve space */
	signed char sick[8][5];
	signed char poison[10][5];

	/* Offset 0x108 */
	signed char skills[52];
	signed char skill_incs;
	/* Offset 0x13d */
	signed char spells[86];
	signed char spell_incs;
	signed char spell_school;
	signed char staff_level;

	/* Offset 0x196 */
	/* Not figured out yet, but reserve space */
	signed char inventory[23][14];
	signed short load;

	unsigned char pic[1024];
};
