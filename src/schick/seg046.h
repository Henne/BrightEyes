#if !defined(__BORLANDC__)
namespace M302de {
#endif

void status_show(const signed int);
void status_show_spell(const struct struct_hero*, const signed int, const signed int, const signed int,
			const signed int, const signed int);
void status_show_skill(const struct struct_hero*, const signed int, const signed int, const signed int,
			const signed int, const signed int);
void status_show_skills(const struct struct_hero*);

#if !defined(__BORLANDC__)
}
#endif
