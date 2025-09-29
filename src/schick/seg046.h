#if !defined(__BORLANDC__)
namespace M302de {
#endif

void status_show(Bit16u);
void status_show_spell(struct struct_hero*, signed short, unsigned short, unsigned short,
				unsigned short, unsigned short);
void status_show_skill(struct struct_hero*, signed short, unsigned short, unsigned short,
				unsigned short, unsigned short);
void status_show_skills(struct struct_hero*);

#if !defined(__BORLANDC__)
}
#endif
