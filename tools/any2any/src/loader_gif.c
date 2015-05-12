/*
 * GIF loader
 *
 * Loads/Dumps a GIF file to/from an ImageSet structure.
 *
 * Authors: Henne_NWH <henne@nachtwindheim.de>
 *          Hendrik <hermes9@web.de>
 * License: GPLv3
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <format.h>
//#include <magick/api.h>
#include <gif_lib.h>

int sanitycheck_gif(const char* buf, size_t len) {
    // TODO
    return 1;
}

ImageSet* process_gif(const char *buf, size_t len) {
    //DGifSlurp();
    return NULL;
}

void gif_addLoopExtensionBlock(GifFileType* file) {
    GifByteType extensionblock[15];
    strcpy(extensionblock, "NETSCAPE2.0");
    extensionblock[11] = 0x03; //
    extensionblock[12] = 0x01; // loop
    extensionblock[13] = 0x00;
    extensionblock[14] = 0x00; // ... indefinitely
    EGifPutExtension(file, APPLICATION_EXT_FUNC_CODE, 15, &extensionblock[0]);
}

void gif_addGraphicsExtensionBlock(GifFileType* file, uint8_t disposalMode, uint16_t delay_ms) {
    GraphicsControlBlock gcb;
    GifByteType extensionblock[4];
    gcb.DisposalMode = disposalMode;
    gcb.UserInputFlag= false;
    gcb.DelayTime    = delay_ms / 10;
    gcb.TransparentColor = -1;
    EGifGCBToExtension(&gcb, &extensionblock[0]);
    EGifPutExtension(file, GRAPHICS_EXT_FUNC_CODE, 4, &extensionblock[0]);
}
int dump_gif(ImageSet* img, char* prefix) {
    char fname[100];
    GifFileType* file;
    ColorMapObject* gif_colormap;
    GifColorType* colormap = (GifColorType*)malloc(256 * sizeof(GifColorType));

    if (img->mainPixels != NULL) {
	// GIF öffnen
	sprintf(fname, "%s.gif", prefix);
	file = EGifOpenFileName(fname, false, NULL);
	EGifSetGifVersion(file, true);

	// Globale Farbpalette & Screendesc setzen
	for (int j=0;   j < 256;   j++) {
	    colormap[j].Red   = img->palette[j].r;
	    colormap[j].Green = img->palette[j].g;
	    colormap[j].Blue  = img->palette[j].b;
	}
	gif_colormap = GifMakeMapObject(256, colormap);
	EGifPutScreenDesc(file, img->width, img->height, 8, 0, gif_colormap);
	
	// ImageDesc setzen und Bild schreiben
	EGifPutImageDesc(file, 0, 0, img->width, img->height, false, NULL);
	for (int y=0; y<img->height; y++) {
	    EGifPutLine(file, img->mainPixels + (y*img->width), img->width);
	}
	EGifSpew(file);
	EGifCloseFile(file, NULL);
	
	printf("main image: %dx%d\n", img->width, img->height);
    }
    // Frames konstruieren
    for (int s=0;  s < img->seqCount;  s++) {
	Sequence* seq = &img->sequences[s];
	//sprintf(fname, "%s-%02d.gif", prefix, s);
	sprintf(fname, "%s-%s.gif", prefix, seq->name);
	file = EGifOpenFileName(fname, false, NULL);
	EGifSetGifVersion(file, true);

	// Globale Farbpalette & Screendesc setzen
	colormap = (GifColorType*)malloc(256 * sizeof(GifColorType));
	for (int j=0;   j < 256;   j++) {
	    colormap[j].Red   = img->palette[j].r;
	    colormap[j].Green = img->palette[j].g;
	    colormap[j].Blue  = img->palette[j].b;
	}
	gif_colormap = GifMakeMapObject(256, colormap);
	EGifPutScreenDesc(file, img->width, img->height, 8, 0, gif_colormap);

	gif_addLoopExtensionBlock(file);


	if (img->mainPixels != NULL) {
	    // ImageDesc setzen und Bild schreiben
	    //gif_addGraphicsExtensionBlock(file, DISPOSE_DO_NOT, seq->defaultDelay*2);
	    EGifPutImageDesc(file, 0, 0, img->width, img->height, false, NULL);
	    for (int y=0; y<img->height; y++) {
		EGifPutLine(file, img->mainPixels + (y*img->width), img->width);
	    }
	}

	// TODO: frames berücksichtien
	for (int i=0;   i < seq->imgCount;   i++) {
	    MyImage* frame = &seq->img[i];
	    /*
	    // ggf. lokale Farbpalette setzen
	    if (frame->palette != NULL) {
		colormap = (GifColorType*)malloc(256 * sizeof(GifColorType));
		for (int j=0;   j < 256;   j++) {
		    colormap[j].Red   = img->palette[j].r << 2;
		    colormap[j].Green = img->palette[j].g << 2;
		    colormap[j].Blue  = img->palette[j].b << 2;
		}
		gif_colormap = GifMakeMapObject(256, colormap);
		}*/
	    
	    printf("frame %d:%d geometry %dx%d@%dx%d\n", s, i,
		   frame->width, frame->height,
		   frame->x0,    frame->y0);

	    // TODO: Kommentar setzen
	    //gif_addCommentExtensionBlock(file, bla);
	    gif_addGraphicsExtensionBlock(file, DISPOSE_BACKGROUND, seq->defaultDelay*2);
	    
	    // ImageDesc setzen und Bild schreiben
	    EGifPutImageDesc(file, frame->x0, frame->y0, frame->width, frame->height, false, NULL);
	    for (int y=0;  y < frame->height;  y++) {
		EGifPutLine(file, frame->pixels + (y*frame->width), frame->width);
	    }
	}
	EGifSpew(file);
	EGifCloseFile(file, NULL);
    }
    
    // Datei schreiben & Aufräumen
    free(colormap);
    return 1;
}
