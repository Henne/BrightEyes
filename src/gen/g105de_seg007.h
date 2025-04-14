#if defined (__BORLANDC__)
struct driver_request {
	char dummy1;		/*@0x00 */
	char subunit;		/*@0x01 */
	char function;		/*@0x02 */
	unsigned short status;  /*@0x03 */
	char dummy2[8];		/*@0x05 */
	char redbook;		/*@0x0d 0 or 1 */
	void *ptr;		/*@0x0e */
	unsigned short dummy4;	/*@0x12 0,1,7,b */
	unsigned short dummy6;	/*@0x14 0 or 2 */
	char dummy7[6];		/*@0x16 */
};

extern struct driver_request far req[9];
extern unsigned char far cd_buf1[804];
extern unsigned char far cd_buf2[20];

#endif
