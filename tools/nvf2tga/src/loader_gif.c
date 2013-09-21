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
#include <magick/api.h>

int sanitycheck_gif(const char* buf, size_t len) {
    // TODO
    return 1;
}

ImageSet* process_gif(const char *buf, size_t len) {
    InitializeMagick(NULL);
    ImageInfo* info = CloneImageInfo(0);
    ExceptionInfo exception;
    Image* image = ReadImage(info, &exception);
    DestroyImage(image);
    DestroyImageInfo(info);
    DestroyExceptionInfo(&exception);
    DestroyMagick();
    return NULL;
}

int dump_gif(ImageSet* img, char* prefix) {
    InitializeMagick(NULL);

    // Globale Bilddaten initialisieren
    ImageInfo* info = CloneImageInfo(0);
    ExceptionInfo exception;
    Image* imagelist = NewImageList();
    Image* frame;
	
    // Frames konstruieren
    for (int i=0;   i < img->frameCount;   i++) {
	ImageInfo* finfo = CloneImageInfo(0);
	if (1 || i==0) {
	    frame = AllocateImage(finfo);
	} else {
	    AllocateNextImage(finfo, frame);
	    frame = frame->next;
	    frame->dispose = PreviousDispose;
	}
	frame->columns = img->globalWidth;
	frame->rows    = img->globalHeight;
	frame->depth   = 8;
	frame->colors  = 256;
	frame->storage_class = PseudoClass;
	frame->delay   = img->frames[i]->delay / 10;
		
	// Farbpalette setzen
	if (img->globalPalette == NULL) {
	    //printf("lokale Farbpalette\n");
	    AllocateImageColormap(frame, 256);
	    PixelPacket* colormap = (PixelPacket*)malloc(256 * sizeof(PixelPacket));
	    for (int j=0;   j < 256;   j++) {
		colormap[j].red   = img->globalPalette[j].r << 2;
		colormap[j].green = img->globalPalette[j].g << 2;
		colormap[j].blue  = img->globalPalette[j].b << 2;
		colormap[j].opacity = 255;
	    }
	    ReplaceImageColormap(frame, colormap, 256);
	    free(colormap);
	} else {
	    //printf("Setze globale Palette auch lokal.\n");
	    AllocateImageColormap(frame, 256);
	    PixelPacket* colormap = (PixelPacket*)malloc(256 * sizeof(PixelPacket));
	    for (int j=0;   j < 256;   j++) {
		colormap[j].red   = img->globalPalette[j].r << 2;
		colormap[j].green = img->globalPalette[j].g << 2;
		colormap[j].blue  = img->globalPalette[j].b << 2;
		colormap[j].opacity = 255;
	    }
	    ReplaceImageColormap(frame, colormap, 256);
	    free(colormap);
	}
		
	printf("frame %d: %dx%d@%dx%d (%s)\n", i,
	       img->frames[i]->width, img->frames[i]->height,
	       img->frames[i]->x0,    img->frames[i]->y0,
	       img->frames[i]->comment);
	int pixelcount = 0;
	for (int y=0; y<img->frames[i]->height; y++) {
	    SetImagePixels(frame,
			   img->frames[i]->x0,     y+img->frames[i]->y0,
			   img->frames[i]->width,  1);
	    ImportPixelAreaInfo importinfo;
	    ImportImagePixelArea(frame, IndexQuantum, 8,
				 img->frames[i]->pixels + pixelcount,
				 NULL, &importinfo);
	    pixelcount += importinfo.bytes_imported;
	}
	// Kommentar setzen
	//SetImageAttribute(frame, "label", img->frames[i]->comment);
	AppendImageToList(&imagelist, frame);
    }
	
    // Datei schreiben & Aufräumen
    info->adjoin = MagickTrue;
    sprintf(imagelist->filename, "%s.gif", prefix);
    WriteImage(info, imagelist);
    DestroyImageList(imagelist);
    DestroyMagick();
    return 1;
}
