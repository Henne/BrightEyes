/*
 * ACE Loader
 *
 * Author: Henne_NWH <henne@nachtiwndheim.de>
 * License: GPLv3
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <dump.h>
#include <format.h>

static void do_ass(struct ace_header *ace, const char *buf, size_t len)
{
	struct ass_header ass;
	unsigned long i, datalen;
	char *pal;

	if (len < sizeof(ass)) {
		fprintf(stderr, "Buffer to small for ASS Header\n");
		return;
	}
	ass.celwidth = get_sshort(buf);
	ass.celheight = get_sshort(buf + 2);
	ass.amount = buf[4];
	ass.playmode = buf[5];

	printf("\t%03dx%03d\tAmount: %03u\tMode: %03u\n",
	       ass.celwidth, ass.celheight, ass.amount, ass.playmode);

	datalen = sizeof(ass);
	for (i = 0; i < ass.amount; i++) {
		struct cel_header cel;

		cel.size = get_sint(buf + datalen);
		cel.xoffset = get_sshort(buf + datalen + 4);
		cel.yoffset = get_sshort(buf + datalen + 6);
		cel.width = get_sshort(buf + datalen + 8);
		cel.height = get_sshort(buf + datalen + 10);
		cel.compression = buf[datalen + 12];
		cel.action = buf[datalen + 13];

/*		printf("\t\tSize: %08x\tCompression: %x\n",
		       cel.size, cel.compression);
*/
		datalen += sizeof(cel) + cel.size;
	}

	if (datalen + 2 > len) {
		fprintf(stderr, "ASS File is to small for a palette\n");
		return;
	}

	if (len != datalen + 256 * 3) {
		fprintf(stderr, "ASS File has not the expected size\n");
		return;
	}
	pal = (char *)buf + datalen;

	datalen = sizeof(ass);
	for (i = 0; i < ass.amount; i++) {
		struct cel_header cel;
		char fname[100];
		char *data;

		cel.size = get_sint(buf + datalen);
		cel.xoffset = get_sshort(buf + datalen + 4);
		cel.yoffset = get_sshort(buf + datalen + 6);
		cel.width = get_sshort(buf + datalen + 8);
		cel.height = get_sshort(buf + datalen + 10);
		cel.compression = buf[datalen + 12];
		cel.action = buf[datalen + 13];

		sprintf(fname, "CEL%03lu.TGA", i);
		data = malloc(cel.width * cel.height);
		switch (cel.compression) {
		case 0x32:	/* PP20 */
			ppdepack((unsigned char*)(buf + datalen + sizeof(cel)),
				 (unsigned char*)data,
				 cel.size, cel.width * cel.height);
			dump_tga(fname, cel.width, cel.height, data, 256, pal);
			break;
		case 0x1:	/* RLE */
			un_rle((unsigned char*)(buf + datalen + sizeof(cel)),
					(unsigned char*)data, cel.size);
			dump_tga(fname, cel.width, cel.height, data, 256, pal);
			break;
		default:
			fprintf(stdout, "Unknown ACE Compression %x\n",
				cel.compression);
		}
		free(data);

		datalen += sizeof(cel) + cel.size;
	}

}

static void do_seq(struct ace_header *ace, const char *buf, size_t len)
{
	unsigned datalen=0,i,j;
	struct seq_header * seqs;

	if (len < ace->sequences*sizeof(struct seq_header)) {
		fprintf(stderr, "Buffer to small for SEQ Headers\n");
		return;
	}

	seqs=malloc(ace->sequences*sizeof(struct seq_header));
	if (!seqs) {
		fprintf(stderr, "Failed to allocate seqs\n");
		return;
	}

	for (i = 0; i < ace->sequences; i++) {
		struct seq_header *seq=&seqs[i];

		seq->offset = get_sint(buf + datalen);
		seq->label = get_sshort(buf + datalen + 4);
		seq->celwidth = get_sshort(buf + datalen + 6);
		seq->celheight = get_sshort(buf + datalen + 8);
		seq->hotspotx = get_sshort(buf + datalen + 10);
		seq->hotspoty = get_sshort(buf + datalen + 12);
		seq->amount = buf[datalen + 14];
		seq->playmode = buf[datalen + 15];

		printf("\tSequence: %d\tOffset: %d\n", i, seq->offset);

		datalen += sizeof(seq);
	}

	for (i=0; i < ace->sequences; i++)
		for (j=0; j < seqs[i].amount; j++) {
			struct cel_header cel;

			cel.size = get_sint(buf+datalen);

			datalen += sizeof(cel) + cel.size;
	}

	if (len < datalen) {
		fprintf(stderr, "ACE-SEQ file is to small for CEL Headers\n");
		return;
	}

	for (i=0; i < ace->sequences; i++)
	{
	}

	free(seqs);
}

void process_ace(const char *buf, size_t len)
{
	struct ace_header ace;

	if (!buf) {
		fprintf(stderr, "%s() got NULL Ptr\n", __func__);
		return;
	}

	if (len < 8) {
		fprintf(stderr, "Buffer is too small for ACE header\n");
		return;
	}

	strncpy(ace.label, buf, 4);
	ace.version = get_sshort(buf + 4);
	ace.sequences = buf[6];
	ace.speed = buf[7];

	if (strncmp(ace.label, "ACE\0", 4)) {
		fprintf(stderr, "No ACE Signature\n");
		return;
	}

	if (ace.version != 1) {
		fprintf(stderr, "ACE Version is not 1\n");
		return;
	}

	if (ace.sequences > 250) {
		fprintf(stderr, "ACE sequences %u is not a valid value\n",
			ace.sequences);
		return;
	}
	if (ace.speed > 99) {
		fprintf(stderr, "ACE speed %u is not a valid value\n",
			ace.speed);

	}

	printf("Sequences: %03u\tSpeed: %02u\n", ace.sequences, ace.speed);

	if (ace.sequences == 1)
		do_ass(&ace, buf + sizeof(ace), len - sizeof(ace));
	else
		do_seq(&ace, buf + sizeof(ace), len - sizeof(ace));

	return;
}
