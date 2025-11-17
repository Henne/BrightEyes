#include <STDIO.H>
#include <DOS.H>

struct date sdate;

int main()
{
	getdate(&sdate);
	printf("DATE= %d-%d-%d\n", sdate.da_day, sdate.da_mon, sdate.da_year);

	sdate.da_year = 1994;
	sdate.da_mon = 1;
	sdate.da_day = 12;

	setdate(&sdate);

	sdate.da_year = 0;
	sdate.da_mon = 0;
	sdate.da_day = 0;

	getdate(&sdate);
	printf("DATE= %d-%d-%d\n", sdate.da_day, sdate.da_mon, sdate.da_year);

	return 0;
}
