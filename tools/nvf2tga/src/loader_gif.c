#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <packer.h>
#include <loader.h>
#include <format.h>
#include <magick/api.h>
#include <assert.h> //TODO

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

int dump_gif(ImageSet* img) {
	InitializeMagick(NULL);

	// Globale Bilddaten initialisieren
	//printf("Globale Bilddaten initialisieren\n");
	ImageInfo* info = CloneImageInfo(0);
	ExceptionInfo exception;
	Image* imagelist = NewImageList();
	Image* frame;
	/*Image* frame = AllocateImage(info);
	
	AppendImageToList(&imagelist, frame);
	printf("Globale Bilddaten initialisieren\n");
	imagelist->columns = img->globalWidth;
	imagelist->rows    = img->globalHeight;
	imagelist->depth   = 8;
	imagelist->colors  = 256;
	imagelist->storage_class = PseudoClass;

	// Globale Palette setzen
	if (img->globalPalette != NULL) {
		printf("Setze globale Palette.\n");
		AllocateImageColormap(imagelist, 256);
		PixelPacket* colormap = (PixelPacket*)malloc(256 * sizeof(PixelPacket));
		for (int j=0;   j < 256;   j++) {
			colormap[j].red   = img->globalPalette[j*3 + 0] << 2;
			colormap[j].green = img->globalPalette[j*3 + 1] << 2;
			colormap[j].blue  = img->globalPalette[j*3 + 2] << 2;
			colormap[j].opacity = 255;
		}
		ReplaceImageColormap(imagelist, colormap, 256);
		}*/
	
	//printf("Konstruiere Frames\n");
	// Frames konstruieren
	for (int i=0;   i < img->frameCount;   i++) {
		//printf("Frame parametrisieren\n");
		ImageInfo* finfo = CloneImageInfo(0);
		if (1 || i==0) {
			frame = AllocateImage(finfo);
		} else {
			/*AllocateNextImage(finfo, frame);
			  frame = frame->next;*/
			Image* oldframe = frame;
			frame = AllocateImage(finfo);
			oldframe->next = frame;
			frame->dispose = PreviousDispose;
		}
		frame->columns = img->frames[i]->width;
		frame->rows    = img->frames[i]->height;
		frame->depth   = 8;
		frame->colors  = 256;
		frame->storage_class = PseudoClass;
		frame->delay   = img->frames[i]->delay * 5;
		//printf("made frame: %dx%d, %d colors\n", frame->columns, frame->rows, frame->colors);
		
		// Lokale Farbpalette
		if (img->globalPalette == NULL) {
			//printf("lokale Farbpalette\n");
			AllocateImageColormap(frame, 256);
			PixelPacket* colormap = (PixelPacket*)malloc(256 * sizeof(PixelPacket));
			for (int j=0;   j < 256;   j++) {
				colormap[j].red   = img->globalPalette[j*3 + 0];
				colormap[j].green = img->globalPalette[j*3 + 1];
				colormap[j].blue  = img->globalPalette[j*3 + 2];
				colormap[j].opacity = 255;
			}
			ReplaceImageColormap(frame, colormap, 256);
		} else {
			//printf("Setze globale Palette auch lokal.\n");
			AllocateImageColormap(frame, 256);
			PixelPacket* colormap = (PixelPacket*)malloc(256 * sizeof(PixelPacket));
			for (int j=0;   j < 256;   j++) {
				colormap[j].red   = img->globalPalette[j*3 + 0] << 2;
				colormap[j].green = img->globalPalette[j*3 + 1] << 2;
				colormap[j].blue  = img->globalPalette[j*3 + 2] << 2;
				colormap[j].opacity = 255;
			}
			ReplaceImageColormap(frame, colormap, 256);
		}

		//printf("Pixel schreiben aus %p\n", img->frames[i]->pixels);
		SetImagePixels(frame,
			       img->frames[i]->x0,
			       img->frames[i]->y0,
			       img->frames[i]->width,
			       img->frames[i]->height);
		ImportPixelAreaInfo doof;
		//printf("frame %d: %dx%d@%dx%d, data at %p\n", i,
		//       img->frames[i]->width,
		//       img->frames[i]->height,
		//       img->frames[i]->x0,
		//       img->frames[i]->y0,
		//       img->frames[i]->pixels);
		ImportImagePixelArea(frame,
				     IndexQuantum, 8,
				     img->frames[i]->pixels,
				     NULL, &doof);
		//printf("imported %d pixels\n", doof.bytes_imported);
		AppendImageToList(&imagelist, frame);
	}
	//printf("Bild schreiben\n");
	info->adjoin = MagickTrue;
	strcpy(imagelist->filename, "BLA.GIF");
	WriteImage(info, imagelist);
	//printf("Aufräumen\n");
	DestroyImageList(imagelist);
      	DestroyMagick();
	//printf("magick destroyed\n");
	return 1;
}
