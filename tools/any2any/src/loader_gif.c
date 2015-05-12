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


int gif_readfunc(GifFileType* gif_file, GifByteType* outBuffer, int bytes) {
    const char* inBuffer = gif_file->UserData;
    memcpy(outBuffer, inBuffer, bytes);
    return bytes;
}

ImageSet* process_gif(const char *buf, size_t len) {
    int error;
    GifFileType* gif = DGifOpen((char*)buf, &gif_readfunc, &error);
    if (error != GIF_OK) {
	printf("GIF reading error %d: %s\n", error, GifErrorString(error));
	return NULL;
    }
    printf("gif read, now converting\n", error);
    ImageSet* imgset = (ImageSet*)malloc(sizeof(ImageSet));
    imgset->width  = gif->SWidth;
    imgset->height = gif->SHeight;
    ColorMapObject* gif_colormap = gif->SColorMap;
    if (gif_colormap != NULL) {
	gif_colormap->ColorCount;
	imgset->palette = (Color*)calloc(256, sizeof(Color));
	int shift = 8 - gif_colormap->BitsPerPixel;
	if (shift < 1 || shift > 7) {
	    fprintf(stderr, "Error opening gif: invalid palette BPP %d\n", gif_colormap->BitsPerPixel);
	}
	for (int i=0;  i < 256;  i++) {
	    if (i <= gif_colormap->ColorCount) {
		imgset->palette[i].r = gif_colormap->Colors[i].Red   >> shift;
		imgset->palette[i].g = gif_colormap->Colors[i].Green >> shift;
		imgset->palette[i].b = gif_colormap->Colors[i].Blue  >> shift;
	    } else {
		//gset->palette[i] = Color{0,0,0};
	    }
	}
    } else {
	imgset->palette = NULL;
    }
    printf("palette ready\n");
    GifImageDesc gif_img = gif->Image;
    if (gif_img.Interlace) fprintf(stderr, "Error reading GIF: interlaced mode not supported.\n");
    imgset->mainPixels = NULL;
    imgset->seqCount = 1;
    imgset->sequences = (Sequence*)calloc(imgset->seqCount, sizeof(Sequence));
    Sequence* seq = &(imgset->sequences[0]);
    seq->name = "S001";
    seq->frameCount = gif->ImageCount;
    seq->frames     = (Frame*)calloc(seq->frameCount, sizeof(Frame));
    seq->defaultDelay = 100;
    seq->imgCount = gif->ImageCount;
    seq->img = (MyImage*)calloc(seq->imgCount, sizeof(MyImage));
    for (int i=0;  i < gif->ImageCount;  i++) {
	GifImageDesc gif_desc = gif->SavedImages[i].ImageDesc;
	if (gif_desc.Interlace) fprintf(stderr, "Error reading GIF: interlaced mode not supported.\n");
	seq->frames[i].index = i;
	// TODO: Delay aus GifExtensionBlock lesen
	seq->frames[i].delay = 100;
	seq->img[i].x0 = gif_desc.Left;
	seq->img[i].y0 = gif_desc.Top;
	seq->img[i].width = gif_desc.Width;
	seq->img[i].height = gif_desc.Height;
	seq->img[i].palette = NULL; // TODO: local color map
	// TODO: Muss ich hier konvertieren???
	seq->img[i].pixels = gif->SavedImages[i].RasterBits;
    }
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
