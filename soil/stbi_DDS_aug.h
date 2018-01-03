#include <stdio.h>

/*
	adding DDS loading support to stbi
*/

#ifndef HEADER_STB_IMAGE_DDS_AUGMENTATION
#define HEADER_STB_IMAGE_DDS_AUGMENTATION

///	A bunch of DirectDraw Surface structures and flags
typedef struct {
	unsigned int    dwMagic;
	unsigned int    dwSize;
	unsigned int    dwFlags;
	unsigned int    dwHeight;
	unsigned int    dwWidth;
	unsigned int    dwPitchOrLinearSize;
	unsigned int    dwDepth;
	unsigned int    dwMipMapCount;
	unsigned int    dwReserved1[11];

	//  DDPIXELFORMAT
	struct {
		unsigned int    dwSize;
		unsigned int    dwFlags;
		unsigned int    dwFourCC;
		unsigned int    dwRGBBitCount;
		unsigned int    dwRBitMask;
		unsigned int    dwGBitMask;
		unsigned int    dwBBitMask;
		unsigned int    dwAlphaBitMask;
	}               sPixelFormat;

	//  DDCAPS2
	struct {
		unsigned int    dwCaps1;
		unsigned int    dwCaps2;
		unsigned int    dwDDSX;
		unsigned int    dwReserved;
	}               sCaps;
	unsigned int    dwReserved2;
} DDS_header;

//	is it a DDS file?
extern int      stbi_dds_test_memory      (unsigned char *buffer, int len);

extern unsigned char *stbi_dds_load             (char *filename,           int *x, int *y, int *comp, int req_comp);
extern unsigned char *stbi_dds_load_from_memory (unsigned char *buffer, int len, int *x, int *y, int *comp, int req_comp);
#ifndef STBI_NO_STDIO
extern int      stbi_dds_test_file        (FILE *f);
extern unsigned char *stbi_dds_load_from_file   (FILE *f,                  int *x, int *y, int *comp, int req_comp);
#endif

//
//
////   end header file   /////////////////////////////////////////////////////
#endif // HEADER_STB_IMAGE_DDS_AUGMENTATION
