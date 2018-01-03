/*
	Jonathan Dummer
	2007-07-26-10.36

	Simple OpenGL Image Library

	Public Domain
	using Sean Barret's stb_image as a base

	Thanks to:
	* Sean Barret - for the awesome stb_image
	* Dan Venkitachalam - for finding some non-compliant DDS files, and patching some explicit casts
	* everybody at gamedev.net
*/

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wincompatible-pointer-types-discards-qualifiers"
#pragma clang diagnostic ignored "-Wcast-qual"
#pragma clang diagnostic ignored "-Wunused-parameter"

#define SOIL_CHECK_FOR_GL_ERRORS 0
#define GLEW_STATIC

#include "glew.h"
#include "wglew.h"

#ifdef _WIN32
//	#define WIN32_LEAN_AND_MEAN
	#include <Windows.h>
	#include <wingdi.h>
	#include <gl/GL.h>
#elif defined(__APPLE__) || defined(__APPLE_CC__)
	/*	I can't test this Apple stuff!	*/
	#include <OpenGL/gl.h>
	#include <Carbon/Carbon.h>
	#define APIENTRY
#else
	#include <GL/gl.h>
	#include <GL/glx.h>
#endif

#include "SOIL.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image.h"
#include "stb_image_write.h"
#include "stbi_DDS_aug_c.h"
#include "image_helper.h"
#include "image_DXT.h"

#include <stdlib.h>
#include <string.h>

/*	error reporting	*/
static const char *result_string_pointer = "SOIL initialized";

/*	for loading cube maps	*/
enum{
	SOIL_CAPABILITY_UNKNOWN = -1,
	SOIL_CAPABILITY_NONE = 0,
	SOIL_CAPABILITY_PRESENT = 1
};
static int has_cubemap_capability = SOIL_CAPABILITY_UNKNOWN;
int query_cubemap_capability( void );
#define SOIL_TEXTURE_WRAP_R					0x8072
//#define SOIL_CLAMP_TO_EDGE					0x812F
//#define SOIL_NORMAL_MAP						0x8511
//#define SOIL_REFLECTION_MAP					0x8512
#define SOIL_TEXTURE_CUBE_MAP				GL_TEXTURE_CUBE_MAP
// #define SOIL_TEXTURE_BINDING_CUBE_MAP		0x8514 
#define SOIL_TEXTURE_CUBE_MAP_POSITIVE_X	GL_TEXTURE_CUBE_MAP_POSITIVE_X
#define SOIL_TEXTURE_CUBE_MAP_NEGATIVE_X	GL_TEXTURE_CUBE_MAP_NEGATIVE_X
#define SOIL_TEXTURE_CUBE_MAP_POSITIVE_Y	GL_TEXTURE_CUBE_MAP_POSITIVE_Y
#define SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Y	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y
#define SOIL_TEXTURE_CUBE_MAP_POSITIVE_Z	GL_TEXTURE_CUBE_MAP_POSITIVE_Z
#define SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Z	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
// #define SOIL_PROXY_TEXTURE_CUBE_MAP			GL_PROXY_TEXTURE_CUBE_MAP
#define SOIL_MAX_CUBE_MAP_TEXTURE_SIZE		0x851C
/*	for non-power-of-two texture	*/
static int has_NPOT_capability = SOIL_CAPABILITY_UNKNOWN;
int query_NPOT_capability( void );
/*	for texture rectangles	*/
static int has_tex_rectangle_capability = SOIL_CAPABILITY_UNKNOWN;
int query_tex_rectangle_capability( void );
#define SOIL_TEXTURE_RECTANGLE_ARB				GL_TEXTURE_RECTANGLE
// #define SOIL_MAX_RECTANGLE_TEXTURE_SIZE_ARB		0x84F8
/*	for using DXT compression	*/
static int has_DXT_capability = SOIL_CAPABILITY_UNKNOWN;
int query_DXT_capability( void );
// #define SOIL_RGB_S3TC_DXT1		GL_COMPRESSED_RGB_S3TC_DXT1_EXT
#define SOIL_RGBA_S3TC_DXT1		GL_COMPRESSED_RGBA_S3TC_DXT1_EXT
#define SOIL_RGBA_S3TC_DXT3		GL_COMPRESSED_RGBA_S3TC_DXT3_EXT
#define SOIL_RGBA_S3TC_DXT5		GL_COMPRESSED_RGBA_S3TC_DXT5_EXT
typedef void (APIENTRY * P_SOIL_GLCOMPRESSEDTEXIMAGE2DPROC) (GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);
static P_SOIL_GLCOMPRESSEDTEXIMAGE2DPROC soilGlCompressedTexImage2D = NULL;
unsigned int SOIL_direct_load_DDS(
		const char *filename,
		unsigned int reuse_texture_ID,
		int flags,
		int loading_as_cubemap, int* width, int* height );
unsigned int SOIL_direct_load_DDS_from_memory(
		const unsigned char *const buffer,
		int buffer_length,
		unsigned int reuse_texture_ID,
		int flags,
		int loading_as_cubemap,
	    int* width,
	    int* height);
/*	other functions	*/
unsigned int
	SOIL_internal_create_OGL_texture
	(
		const unsigned char *const data,
		int width, int height, int channels,
		unsigned int reuse_texture_ID,
		unsigned int flags,
		unsigned int opengl_texture_type,
		unsigned int opengl_texture_target,
		unsigned int texture_check_size_enum
	);

/*	and the code magic begins here [8^)	*/
unsigned int
	SOIL_load_OGL_texture
	(
		const char *filename,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
	)
{
	/*	variables	*/
	unsigned char* img;
	int width, height, channels;
	unsigned int tex_id;
	/*	does the user want direct uploading of the image as a DDS file?	*/
	if( flags & SOIL_FLAG_DDS_LOAD_DIRECT )
	{
		/*	1st try direct loading of the image as a DDS file
			note: direct uploading will only load what is in the
			DDS file, no MIPmaps will be generated, the image will
			not be flipped, etc.	*/
		tex_id = SOIL_direct_load_DDS( filename, reuse_texture_ID, (int)flags, 0, &width, &height );
		if( tex_id )
		{
			/*	hey, it worked!!	*/
			return tex_id;
		}
	}
	/*	try to load the image	*/
	img = SOIL_load_image( filename, &width, &height, &channels, force_channels );
	/*	channels holds the original number of channels, which may have been forced	*/
	if( (force_channels >= 1) && (force_channels <= 4) )
	{
		channels = force_channels;
	}
	if( NULL == img )
	{
		/*	image loading failed	*/
		result_string_pointer = stbi_failure_reason();
		return 0;
	}
	/*	OK, make it a texture!	*/
	tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			reuse_texture_ID, flags,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE );
	/*	and nuke the image data	*/
	SOIL_free_image_data( img );
	/*	and return the handle, such as it is	*/
	return tex_id;
}

/*unsigned int
	SOIL_load_OGL_HDR_texture
	(
		const char *filename,
		int fake_HDR_format,
		int rescale_to_max,
		unsigned int reuse_texture_ID,
		unsigned int flags
	)
{
	unsigned char* img;
	int width, height, channels;
	unsigned int tex_id;

	if( (fake_HDR_format != SOIL_HDR_RGBE) &&
		(fake_HDR_format != SOIL_HDR_RGBdivA) &&
		(fake_HDR_format != SOIL_HDR_RGBdivA2) )
	{
		result_string_pointer = "Invalid fake HDR format specified";
		return 0;
	}

	img = stbi_hdr_load_rgbe( filename, &width, &height, &channels, 4 );

	if( NULL == img )
	{

		result_string_pointer = stbi_failure_reason();
		return 0;
	}

	if( fake_HDR_format == SOIL_HDR_RGBdivA )
	{
		RGBE_to_RGBdivA( img, width, height, rescale_to_max );
	} else if( fake_HDR_format == SOIL_HDR_RGBdivA2 )
	{
		RGBE_to_RGBdivA2( img, width, height, rescale_to_max );
	}

	tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			reuse_texture_ID, flags,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE );

	SOIL_free_image_data( img );
	return tex_id;
}*/

unsigned int
	SOIL_load_OGL_texture_from_memory
	(
		const unsigned char *const buffer,
		int buffer_length,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags,
		int* width,
		int* height
	)
{
	/*	variables	*/
	unsigned char* img;
	int channels;
	unsigned int tex_id;
	/*	does the user want direct uploading of the image as a DDS file?	*/
	if( flags & SOIL_FLAG_DDS_LOAD_DIRECT )
	{
		/*	1st try direct loading of the image as a DDS file
			note: direct uploading will only load what is in the
			DDS file, no MIPmaps will be generated, the image will
			not be flipped, etc.	*/
		tex_id = SOIL_direct_load_DDS_from_memory(
				buffer, buffer_length,
				reuse_texture_ID, (int)flags, 0, width, height);
		if( tex_id )
		{
			/*	hey, it worked!!	*/
			return tex_id;
		}
	}
	/*	try to load the image	*/
	img = SOIL_load_image_from_memory(
					buffer, buffer_length,
					width, height, &channels,
					force_channels );
	/*	channels holds the original number of channels, which may have been forced	*/
	if( (force_channels >= 1) && (force_channels <= 4) )
	{
		channels = force_channels;
	}
	if( NULL == img )
	{
		/*	image loading failed	*/
		result_string_pointer = stbi_failure_reason();
		return 0;
	}
	/*	OK, make it a texture!	*/
	tex_id = SOIL_internal_create_OGL_texture(
			img, *width, *height, channels,
			reuse_texture_ID, flags,
			GL_TEXTURE_2D, GL_TEXTURE_2D,
			GL_MAX_TEXTURE_SIZE );
	/*	and nuke the image data	*/
	SOIL_free_image_data( img );
	/*	and return the handle, such as it is	*/
	return tex_id;
}

unsigned int
	SOIL_load_OGL_cubemap
	(
		const char *x_pos_file,
		const char *x_neg_file,
		const char *y_pos_file,
		const char *y_neg_file,
		const char *z_pos_file,
		const char *z_neg_file,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
	)
{
	/*	variables	*/
	unsigned char* img;
	int width, height, channels;
	unsigned int tex_id;
	/*	error checking	*/
	if( (x_pos_file == NULL) ||
		(x_neg_file == NULL) ||
		(y_pos_file == NULL) ||
		(y_neg_file == NULL) ||
		(z_pos_file == NULL) ||
		(z_neg_file == NULL) )
	{
		result_string_pointer = "Invalid cube map files list";
		return 0;
	}
	/*	capability checking	*/
	if( query_cubemap_capability() != SOIL_CAPABILITY_PRESENT )
	{
		result_string_pointer = "No cube map capability present";
		return 0;
	}
	/*	1st face: try to load the image	*/
	img = SOIL_load_image( x_pos_file, &width, &height, &channels, force_channels );
	/*	channels holds the original number of channels, which may have been forced	*/
	if( (force_channels >= 1) && (force_channels <= 4) )
	{
		channels = force_channels;
	}
	if( NULL == img )
	{
		/*	image loading failed	*/
		result_string_pointer = stbi_failure_reason();
		return 0;
	}
	/*	upload the texture, and create a texture ID if necessary	*/
	tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			reuse_texture_ID, flags,
			SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_POSITIVE_X,
			SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
	/*	and nuke the image data	*/
	SOIL_free_image_data( img );
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image( x_neg_file, &width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image( y_pos_file, &width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image( y_neg_file, &width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image( z_pos_file, &width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image( z_neg_file, &width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	and return the handle, such as it is	*/
	return tex_id;
}

unsigned int
	SOIL_load_OGL_cubemap_from_memory
	(
		const unsigned char *const x_pos_buffer,
		int x_pos_buffer_length,
		const unsigned char *const x_neg_buffer,
		int x_neg_buffer_length,
		const unsigned char *const y_pos_buffer,
		int y_pos_buffer_length,
		const unsigned char *const y_neg_buffer,
		int y_neg_buffer_length,
		const unsigned char *const z_pos_buffer,
		int z_pos_buffer_length,
		const unsigned char *const z_neg_buffer,
		int z_neg_buffer_length,
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
	)
{
	/*	variables	*/
	unsigned char* img;
	int width, height, channels;
	unsigned int tex_id;
	/*	error checking	*/
	if( (x_pos_buffer == NULL) ||
		(x_neg_buffer == NULL) ||
		(y_pos_buffer == NULL) ||
		(y_neg_buffer == NULL) ||
		(z_pos_buffer == NULL) ||
		(z_neg_buffer == NULL) )
	{
		result_string_pointer = "Invalid cube map buffers list";
		return 0;
	}
	/*	capability checking	*/
	if( query_cubemap_capability() != SOIL_CAPABILITY_PRESENT )
	{
		result_string_pointer = "No cube map capability present";
		return 0;
	}
	/*	1st face: try to load the image	*/
	img = SOIL_load_image_from_memory(
			x_pos_buffer, x_pos_buffer_length,
			&width, &height, &channels, force_channels );
	/*	channels holds the original number of channels, which may have been forced	*/
	if( (force_channels >= 1) && (force_channels <= 4) )
	{
		channels = force_channels;
	}
	if( NULL == img )
	{
		/*	image loading failed	*/
		result_string_pointer = stbi_failure_reason();
		return 0;
	}
	/*	upload the texture, and create a texture ID if necessary	*/
	tex_id = SOIL_internal_create_OGL_texture(
			img, width, height, channels,
			reuse_texture_ID, flags,
			SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_POSITIVE_X,
			SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
	/*	and nuke the image data	*/
	SOIL_free_image_data( img );
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image_from_memory(
				x_neg_buffer, x_neg_buffer_length,
				&width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image_from_memory(
				y_pos_buffer, y_pos_buffer_length,
				&width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_POSITIVE_Y,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image_from_memory(
				y_neg_buffer, y_neg_buffer_length,
				&width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image_from_memory(
				z_pos_buffer, z_pos_buffer_length,
				&width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_POSITIVE_Z,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	continue?	*/
	if( tex_id != 0 )
	{
		/*	1st face: try to load the image	*/
		img = SOIL_load_image_from_memory(
				z_neg_buffer, z_neg_buffer_length,
				&width, &height, &channels, force_channels );
		/*	channels holds the original number of channels, which may have been forced	*/
		if( (force_channels >= 1) && (force_channels <= 4) )
		{
			channels = force_channels;
		}
		if( NULL == img )
		{
			/*	image loading failed	*/
			result_string_pointer = stbi_failure_reason();
			return 0;
		}
		/*	upload the texture, but reuse the assigned texture ID	*/
		tex_id = SOIL_internal_create_OGL_texture(
				img, width, height, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP, SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
		/*	and nuke the image data	*/
		SOIL_free_image_data( img );
	}
	/*	and return the handle, such as it is	*/
	return tex_id;
}

unsigned int
	SOIL_load_OGL_single_cubemap
	(
		const char *filename,
		const char face_order[6],
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
	)
{
	/*	variables	*/
	unsigned char* img;
	int width, height, channels, i;
	unsigned int tex_id = 0;
	/*	error checking	*/
	if( filename == NULL )
	{
		result_string_pointer = "Invalid single cube map file name";
		return 0;
	}
	/*	does the user want direct uploading of the image as a DDS file?	*/
	if( flags & SOIL_FLAG_DDS_LOAD_DIRECT )
	{
		/*	1st try direct loading of the image as a DDS file
			note: direct uploading will only load what is in the
			DDS file, no MIPmaps will be generated, the image will
			not be flipped, etc.	*/
		tex_id = SOIL_direct_load_DDS( filename, reuse_texture_ID, (int)flags, 1, &width, &height );
		if( tex_id )
		{
			/*	hey, it worked!!	*/
			return tex_id;
		}
	}
	/*	face order checking	*/
	for( i = 0; i < 6; ++i )
	{
		if( (face_order[i] != 'N') &&
			(face_order[i] != 'S') &&
			(face_order[i] != 'W') &&
			(face_order[i] != 'E') &&
			(face_order[i] != 'U') &&
			(face_order[i] != 'D') )
		{
			result_string_pointer = "Invalid single cube map face order";
			return 0;
		};
	}
	/*	capability checking	*/
	if( query_cubemap_capability() != SOIL_CAPABILITY_PRESENT )
	{
		result_string_pointer = "No cube map capability present";
		return 0;
	}
	/*	1st off, try to load the full image	*/
	img = SOIL_load_image( filename, &width, &height, &channels, force_channels );
	/*	channels holds the original number of channels, which may have been forced	*/
	if( (force_channels >= 1) && (force_channels <= 4) )
	{
		channels = force_channels;
	}
	if( NULL == img )
	{
		/*	image loading failed	*/
		result_string_pointer = stbi_failure_reason();
		return 0;
	}
	/*	now, does this image have the right dimensions?	*/
	if( (width != 6*height) &&
		(6*width != height) )
	{
		SOIL_free_image_data( img );
		result_string_pointer = "Single cubemap image must have a 6:1 ratio";
		return 0;
	}
	/*	try the image split and create	*/
	tex_id = SOIL_create_OGL_single_cubemap(
			img, width, height, channels,
			face_order, reuse_texture_ID, flags
			);
	/*	nuke the temporary image data and return the texture handle	*/
	SOIL_free_image_data( img );
	return tex_id;
}

unsigned int
	SOIL_load_OGL_single_cubemap_from_memory
	(
		const unsigned char *const buffer,
		int buffer_length,
		const char face_order[6],
		int force_channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
	)
{
	/*	variables	*/
	unsigned char* img;
	int width, height, channels, i;
	unsigned int tex_id = 0;
	/*	error checking	*/
	if( buffer == NULL )
	{
		result_string_pointer = "Invalid single cube map buffer";
		return 0;
	}
	/*	does the user want direct uploading of the image as a DDS file?	*/
	if( flags & SOIL_FLAG_DDS_LOAD_DIRECT )
	{
		/*	1st try direct loading of the image as a DDS file
			note: direct uploading will only load what is in the
			DDS file, no MIPmaps will be generated, the image will
			not be flipped, etc.	*/
		tex_id = SOIL_direct_load_DDS_from_memory(
				buffer, buffer_length,
				reuse_texture_ID, (int)flags, 1, &width, &height);
		if( tex_id )
		{
			/*	hey, it worked!!	*/
			return tex_id;
		}
	}
	/*	face order checking	*/
	for( i = 0; i < 6; ++i )
	{
		if( (face_order[i] != 'N') &&
			(face_order[i] != 'S') &&
			(face_order[i] != 'W') &&
			(face_order[i] != 'E') &&
			(face_order[i] != 'U') &&
			(face_order[i] != 'D') )
		{
			result_string_pointer = "Invalid single cube map face order";
			return 0;
		};
	}
	/*	capability checking	*/
	if( query_cubemap_capability() != SOIL_CAPABILITY_PRESENT )
	{
		result_string_pointer = "No cube map capability present";
		return 0;
	}
	/*	1st off, try to load the full image	*/
	img = SOIL_load_image_from_memory(
			buffer, buffer_length,
			&width, &height, &channels,
			force_channels );
	/*	channels holds the original number of channels, which may have been forced	*/
	if( (force_channels >= 1) && (force_channels <= 4) )
	{
		channels = force_channels;
	}
	if( NULL == img )
	{
		/*	image loading failed	*/
		result_string_pointer = stbi_failure_reason();
		return 0;
	}
	/*	now, does this image have the right dimensions?	*/
	if( (width != 6*height) &&
		(6*width != height) )
	{
		SOIL_free_image_data( img );
		result_string_pointer = "Single cubemap image must have a 6:1 ratio";
		return 0;
	}
	/*	try the image split and create	*/
	tex_id = SOIL_create_OGL_single_cubemap(
			img, width, height, channels,
			face_order, reuse_texture_ID, flags
			);
	/*	nuke the temporary image data and return the texture handle	*/
	SOIL_free_image_data( img );
	return tex_id;
}

unsigned int
	SOIL_create_OGL_single_cubemap
	(
		const unsigned char *const data,
		int width, int height, int channels,
		const char face_order[6],
		unsigned int reuse_texture_ID,
		unsigned int flags
	)
{
	/*	variables	*/
	unsigned char* sub_img;
	int dw, dh, sz, i;
	unsigned int tex_id;
	/*	error checking	*/
	if( data == NULL )
	{
		result_string_pointer = "Invalid single cube map image data";
		return 0;
	}
	/*	face order checking	*/
	for( i = 0; i < 6; ++i )
	{
		if( (face_order[i] != 'N') &&
			(face_order[i] != 'S') &&
			(face_order[i] != 'W') &&
			(face_order[i] != 'E') &&
			(face_order[i] != 'U') &&
			(face_order[i] != 'D') )
		{
			result_string_pointer = "Invalid single cube map face order";
			return 0;
		};
	}
	/*	capability checking	*/
	if( query_cubemap_capability() != SOIL_CAPABILITY_PRESENT )
	{
		result_string_pointer = "No cube map capability present";
		return 0;
	}
	/*	now, does this image have the right dimensions?	*/
	if( (width != 6*height) &&
		(6*width != height) )
	{
		result_string_pointer = "Single cubemap image must have a 6:1 ratio";
		return 0;
	}
	/*	which way am I stepping?	*/
	if( width > height )
	{
		dw = height;
		dh = 0;
	} else
	{
		dw = 0;
		dh = width;
	}
	sz = dw+dh;
	sub_img = (unsigned char *)malloc( (size_t)(sz*sz*channels) );
	/*	do the splitting and uploading	*/
	tex_id = reuse_texture_ID;
	for( i = 0; i < 6; ++i )
	{
		int x, y, idx = 0;
		unsigned int cubemap_target = 0;
		/*	copy in the sub-image	*/
		for( y = i*dh; y < i*dh+sz; ++y )
		{
			for( x = i*dw*channels; x < (i*dw+sz)*channels; ++x )
			{
				sub_img[idx++] = data[y*width*channels+x];
			}
		}
		/*	what is my texture target?
			remember, this coordinate system is
			LHS if viewed from inside the cube!	*/
		switch( face_order[i] )
		{
		case 'N':
			cubemap_target = SOIL_TEXTURE_CUBE_MAP_POSITIVE_Z;
			break;
		case 'S':
			cubemap_target = SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
			break;
		case 'W':
			cubemap_target = SOIL_TEXTURE_CUBE_MAP_NEGATIVE_X;
			break;
		case 'E':
			cubemap_target = SOIL_TEXTURE_CUBE_MAP_POSITIVE_X;
			break;
		case 'U':
			cubemap_target = SOIL_TEXTURE_CUBE_MAP_POSITIVE_Y;
			break;
		case 'D':
			cubemap_target = SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
			break;
		}
		/*	upload it as a texture	*/
		tex_id = SOIL_internal_create_OGL_texture(
				sub_img, sz, sz, channels,
				tex_id, flags,
				SOIL_TEXTURE_CUBE_MAP,
				cubemap_target,
				SOIL_MAX_CUBE_MAP_TEXTURE_SIZE );
	}
	/*	and nuke the image and sub-image data	*/
	SOIL_free_image_data( sub_img );
	/*	and return the handle, such as it is	*/
	return tex_id;
}

unsigned int
	SOIL_create_OGL_texture
	(
		const unsigned char *const data,
		int width, int height, int channels,
		unsigned int reuse_texture_ID,
		unsigned int flags
	)
{
	/*	wrapper function for 2D textures	*/
	return SOIL_internal_create_OGL_texture(
				data, width, height, channels,
				reuse_texture_ID, flags,
				GL_TEXTURE_2D, GL_TEXTURE_2D,
				GL_MAX_TEXTURE_SIZE );
}

void check_for_GL_errors(const char* );

#if SOIL_CHECK_FOR_GL_ERRORS
void check_for_GL_errors( const char *calling_location )
{
	/*	check for errors	*/
	GLenum err_code = glGetError();
	while( GL_NO_ERROR != err_code )
	{
		printf( "OpenGL Error @ %s: %i", calling_location, err_code );
		err_code = glGetError();
	}
}
#else
void check_for_GL_errors( const char* calling_location)
{
	/*	no check for errors	*/
}
#endif

unsigned int
	SOIL_internal_create_OGL_texture
	(
		const unsigned char *const data,
		int width, int height, int channels,
		unsigned int reuse_texture_ID,
		unsigned int flags,
		unsigned int opengl_texture_type,
		unsigned int opengl_texture_target,
		unsigned int texture_check_size_enum
	)
{
	/*	variables	*/
	unsigned char* img;
	unsigned int tex_id;
	unsigned int internal_texture_format = 0, original_texture_format = 0;

	int max_supported_size;
	/*	If the user wants to use the texture rectangle I kill a few flags	*/
	if( flags & SOIL_FLAG_TEXTURE_RECTANGLE )
	{
		/*	well, the user asked for it, can we do that?	*/
		if( query_tex_rectangle_capability() == SOIL_CAPABILITY_PRESENT )
		{
			/*	only allow this if the user in _NOT_ trying to do a cubemap!	*/
			if( opengl_texture_type == GL_TEXTURE_2D )
			{
				/*	clean out the flags that cannot be used with texture rectangles	*/
				flags &= ~(unsigned int)(
						SOIL_FLAG_POWER_OF_TWO | SOIL_FLAG_MIPMAPS |
						SOIL_FLAG_TEXTURE_REPEATS
					);
				/*	and change my target	*/
				opengl_texture_target = SOIL_TEXTURE_RECTANGLE_ARB;
				opengl_texture_type = SOIL_TEXTURE_RECTANGLE_ARB;
			} else
			{
				/*	not allowed for any other uses (yes, I'm looking at you, cubemaps!)	*/
				flags &= ~(unsigned int)SOIL_FLAG_TEXTURE_RECTANGLE;
			}

		} else
		{
			/*	can't do it, and that is a breakable offense (uv coords use pixels instead of [0,1]!)	*/
			result_string_pointer = "Texture Rectangle extension unsupported";
			return 0;
		}
	}
	/*	create a copy the image data	*/
	img = (unsigned char*)malloc( (size_t)(width*height*channels) );
	memcpy( img, data, (size_t)(width*height*channels) );
	/*	does the user want me to invert the image?	*/
	if( flags & SOIL_FLAG_INVERT_Y )
	{
		int i, j;
		for( j = 0; j*2 < height; ++j )
		{
			int index1 = j * width * channels;
			int index2 = (height - 1 - j) * width * channels;
			for( i = width * channels; i > 0; --i )
			{
				unsigned char temp = img[index1];
				img[index1] = img[index2];
				img[index2] = temp;
				++index1;
				++index2;
			}
		}
	}
	/*	does the user want me to scale the colors into the NTSC safe RGB range?	*/
	if( flags & SOIL_FLAG_NTSC_SAFE_RGB )
	{
		scale_image_RGB_to_NTSC_safe( img, width, height, channels );
	}
	/*	does the user want me to convert from straight to pre-multiplied alpha?
		(and do we even _have_ alpha?)	*/
	if( flags & SOIL_FLAG_MULTIPLY_ALPHA )
	{
		int i;
		switch( channels )
		{
		case 2:
			for( i = 0; i < 2*width*height; i += 2 )
			{
				img[i] = (img[i] * img[i+1] + 128) >> 8;
			}
			break;
		case 4:
			for( i = 0; i < 4*width*height; i += 4 )
			{
				img[i+0] = (img[i+0] * img[i+3] + 128) >> 8;
				img[i+1] = (img[i+1] * img[i+3] + 128) >> 8;
				img[i+2] = (img[i+2] * img[i+3] + 128) >> 8;
			}
			break;
		default:
			/*	no other number of channels contains alpha data	*/
			break;
		}
	}
	/*	if the user can't support NPOT textures, make sure we force the POT option	*/
	if( (query_NPOT_capability() == SOIL_CAPABILITY_NONE) &&
		!(flags & SOIL_FLAG_TEXTURE_RECTANGLE) )
	{
		/*	add in the POT flag */
		flags |= SOIL_FLAG_POWER_OF_TWO;
	}
	/*	how large of a texture can this OpenGL implementation handle?	*/
	/*	texture_check_size_enum will be GL_MAX_TEXTURE_SIZE or SOIL_MAX_CUBE_MAP_TEXTURE_SIZE	*/
	glGetIntegerv( texture_check_size_enum, &max_supported_size );
	/*	do I need to make it a power of 2?	*/
	if(
		(flags & SOIL_FLAG_POWER_OF_TWO) ||	/*	user asked for it	*/
		(flags & SOIL_FLAG_MIPMAPS) ||		/*	need it for the MIP-maps	*/
		(width > max_supported_size) ||		/*	it's too big, (make sure it's	*/
		(height > max_supported_size) )		/*	2^n for later down-sampling)	*/
	{
		int new_width = 1;
		int new_height = 1;
		while( new_width < width )
		{
			new_width *= 2;
		}
		while( new_height < height )
		{
			new_height *= 2;
		}
		/*	still?	*/
		if( (new_width != width) || (new_height != height) )
		{
			/*	yep, resize	*/
			unsigned char *resampled = (unsigned char*)malloc((size_t)(channels*new_width*new_height) );
			up_scale_image(
					img, width, height, channels,
					resampled, new_width, new_height );
			/*	OJO	this is for debug only!	*/
			/*
			SOIL_save_image( "\\showme.bmp", SOIL_SAVE_TYPE_BMP,
							new_width, new_height, channels,
							resampled );
			*/
			/*	nuke the old guy, then point it at the new guy	*/
			SOIL_free_image_data( img );
			img = resampled;
			width = new_width;
			height = new_height;
		}
	}
	/*	now, if it is too large...	*/
	if( (width > max_supported_size) || (height > max_supported_size) )
	{
		/*	I've already made it a power of two, so simply use the MIPmapping
			code to reduce its size to the allowable maximum.	*/
		unsigned char *resampled;
		int reduce_block_x = 1, reduce_block_y = 1;
		int new_width, new_height;
		if( width > max_supported_size )
		{
			reduce_block_x = width / max_supported_size;
		}
		if( height > max_supported_size )
		{
			reduce_block_y = height / max_supported_size;
		}
		new_width = width / reduce_block_x;
		new_height = height / reduce_block_y;
		resampled = (unsigned char*)malloc((size_t)(channels*new_width*new_height) );
		/*	perform the actual reduction	*/
		mipmap_image(	img, width, height, channels,
						resampled, reduce_block_x, reduce_block_y );
		/*	nuke the old guy, then point it at the new guy	*/
		SOIL_free_image_data( img );
		img = resampled;
		width = new_width;
		height = new_height;
	}
	/*	does the user want us to use YCoCg color space?	*/
	if( flags & SOIL_FLAG_CoCg_Y )
	{
		/*	this will only work with RGB and RGBA images */
		convert_RGB_to_YCoCg( img, width, height, channels );
		/*
		save_image_as_DDS( "CoCg_Y.dds", width, height, channels, img );
		*/
	}
	/*	create the OpenGL texture ID handle
    	(note: allowing a forced texture ID lets me reload a texture)	*/
    tex_id = reuse_texture_ID;
    if( tex_id == 0 )
    {
		glGenTextures( 1, &tex_id );
    }
	check_for_GL_errors( "glGenTextures" );
	/* Note: sometimes glGenTextures fails (usually no OpenGL context)	*/
	if( tex_id )
	{
		/*	and what type am I using as the internal texture format?	*/
		switch( channels )
		{
		case 1:
			original_texture_format = GL_RED;
			internal_texture_format = GL_COMPRESSED_R11_EAC;
			break;
		case 2:
			original_texture_format = GL_RG;
			internal_texture_format = GL_COMPRESSED_RG11_EAC;
			break;
		case 3:
			original_texture_format = GL_RGB;
			internal_texture_format = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
			break;
		case 4:
			original_texture_format = GL_RGBA;
			internal_texture_format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
			break;
		}

		int MIPlevel = 1;
		if (flags & SOIL_FLAG_MIPMAPS) {
			while (((1 << MIPlevel) <= width) || ((1 << MIPlevel) <= height)) {
				++MIPlevel;
			}
		}
		/*  bind an OpenGL texture ID	*/
		glBindTexture( opengl_texture_type, tex_id );
		check_for_GL_errors( "glBindTexture" );
		/*  upload the main image	*/

		glTexStorage2D(opengl_texture_type, MIPlevel, internal_texture_format, width, height);
		glTexSubImage2D(opengl_texture_type, 0, 0, 0, width, height, original_texture_format, GL_UNSIGNED_BYTE, img);
		if(MIPlevel > 1)
			glGenerateMipmap(opengl_texture_type);

		check_for_GL_errors( "glTexImage2D" );
		/*printf( "OpenGL DXT compressor\n" );	*/
		
		/*	are any MIPmaps desired?	*/
		if( flags & SOIL_FLAG_MIPMAPS )
		{
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
			check_for_GL_errors( "GL_TEXTURE_MIN/MAG_FILTER" );
		} else
		{
			/*	instruct OpenGL _NOT_ to use the MIPmaps	*/
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
			check_for_GL_errors( "GL_TEXTURE_MIN/MAG_FILTER" );
		}
		/*	does the user want clamping, or wrapping?	*/
		if( flags & SOIL_FLAG_TEXTURE_REPEATS )
		{
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT );
			if( opengl_texture_type == SOIL_TEXTURE_CUBE_MAP )
			{
				/*	SOIL_TEXTURE_WRAP_R is invalid if cubemaps aren't supported	*/
				glTexParameteri( opengl_texture_type, SOIL_TEXTURE_WRAP_R, GL_REPEAT );
			}
			check_for_GL_errors( "GL_TEXTURE_WRAP_*" );
		} else
		{
			/*	unsigned int clamp_mode = SOIL_CLAMP_TO_EDGE;	*/
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP);
			if( opengl_texture_type == SOIL_TEXTURE_CUBE_MAP )
			{
				/*	SOIL_TEXTURE_WRAP_R is invalid if cubemaps aren't supported	*/
				glTexParameteri( opengl_texture_type, SOIL_TEXTURE_WRAP_R, GL_CLAMP);
			}
			check_for_GL_errors( "GL_TEXTURE_WRAP_*" );
		}
		/*	done	*/
		result_string_pointer = "Image loaded as an OpenGL texture";
	} else
	{
		/*	failed	*/
		result_string_pointer = "Failed to generate an OpenGL texture name; missing OpenGL context?";
	}
	SOIL_free_image_data( img );
	return tex_id;
}

int
	SOIL_save_screenshot
	(
		const char *filename,
		int image_type,
		int x, int y,
		int width, int height
	)
{
	unsigned char *pixel_data;
	int i, j;
	int save_result;

	/*	error checks	*/
	if( (width < 1) || (height < 1) )
	{
		result_string_pointer = "Invalid screenshot dimensions";
		return 0;
	}
	if( (x < 0) || (y < 0) )
	{
		result_string_pointer = "Invalid screenshot location";
		return 0;
	}
	if( filename == NULL )
	{
		result_string_pointer = "Invalid screenshot filename";
		return 0;
	}

    /*  Get the data from OpenGL	*/
    pixel_data = (unsigned char*)malloc( (size_t)(3*width*height) );
	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glPixelStorei(GL_PACK_ROW_LENGTH, 0);
    glReadPixels (x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel_data);

    /*	invert the image	*/
    for( j = 0; j*2 < height; ++j )
	{
		int index1 = j * width * 3;
		int index2 = (height - 1 - j) * width * 3;
		for( i = width * 3; i > 0; --i )
		{
			unsigned char temp = pixel_data[index1];
			pixel_data[index1] = pixel_data[index2];
			pixel_data[index2] = temp;
			++index1;
			++index2;
		}
	}

    /*	save the image	*/
	stbi_write_tga_with_rle = 0;
    save_result = SOIL_save_image( filename, image_type, width, height, 3, pixel_data);

    /*  And free the memory	*/
    SOIL_free_image_data( pixel_data );
	return save_result;
}

unsigned char*
	SOIL_load_image
	(
		const char *filename,
		int *width, int *height, int *channels,
		int force_channels
	)
{

	FILE *f;
	unsigned char *buffer;
	size_t buffer_length, bytes_read;

	/*	error checks	*/
	if (NULL == filename) {
		result_string_pointer = "NULL filename";
		return 0;
	}
	f = fopen(filename, "rb");
	if (NULL == f) {
		/*	the file doesn't seem to exist (or be open-able)	*/
		result_string_pointer = "Can not find DDS file";
		return 0;
	}
	fseek(f, 0, SEEK_END);
	buffer_length = (size_t)ftell(f);
	fseek(f, 0, SEEK_SET);
	buffer = (unsigned char *)malloc(buffer_length);
	if (NULL == buffer) {
		result_string_pointer = "malloc failed";
		fclose(f);
		return 0;
	}
	bytes_read = fread((void*)buffer, 1, buffer_length, f);
	fclose(f);
	if (bytes_read < buffer_length) {
		/*	huh?	*/
		buffer_length = bytes_read;
	}

	unsigned char *result = SOIL_load_image_from_memory(buffer, (int)buffer_length, width, height, channels, force_channels);

	free(buffer);

	return result;
}

unsigned char*
	SOIL_load_image_from_memory
	(
		const unsigned char *const buffer,
		int buffer_length,
		int *width, int *height, int *channels,
		int force_channels
	)
{
	unsigned char* result = stbi_dds_load_from_memory((unsigned char *)(buffer), buffer_length, width, height, channels, force_channels);
	if (result)
		return result;
	 result = stbi_load_from_memory(
				buffer, buffer_length,
				width, height, channels,
				force_channels );
	if( result == NULL )
	{
		result_string_pointer = stbi_failure_reason();
	} else
	{
		result_string_pointer = "Image loaded from memory";
	}
	return result;
}

int
	SOIL_save_image
	(
		const char *filename,
		int image_type,
		int width, int height, int channels,
		const unsigned char *const data
	)
{
	int save_result;

	/*	error check	*/
	if( (width < 1) || (height < 1) ||
		(channels < 1) || (channels > 4) ||
		(data == NULL) ||
		(filename == NULL) )
	{
		return 0;
	}
	if( image_type == SOIL_SAVE_TYPE_BMP )
	{
		save_result = stbi_write_bmp( filename,
				width, height, channels, (void*)data );
	} else
	if( image_type == SOIL_SAVE_TYPE_TGA )
	{
		save_result = stbi_write_tga( filename,
				width, height, channels, (void*)data );
	} else
	if( image_type == SOIL_SAVE_TYPE_DDS )
	{
		save_result = save_image_as_DDS( filename,
				width, height, channels, (const unsigned char *const)data );
	} else
	{
		save_result = 0;
	}
	if( save_result == 0 )
	{
		result_string_pointer = "Saving the image failed";
	} else
	{
		result_string_pointer = "Image saved";
	}
	return save_result;
}

void
	SOIL_free_image_data
	(
		unsigned char *img_data
	)
{
	free( (void*)img_data );
}

const char*
	SOIL_last_result
	(
		void
	)
{
	return result_string_pointer;
}

unsigned int SOIL_direct_load_DDS_from_memory(
		const unsigned char *const buffer,
		int buffer_length,
		unsigned int reuse_texture_ID,
		int flags,
		int loading_as_cubemap,
	    int* width,
	    int* height)
{
	/*	variables	*/
	DDS_header header;
	unsigned int buffer_index = 0;
	unsigned int tex_ID = 0;
	/*	file reading variables	*/
	unsigned int S3TC_type = 0;
	unsigned char *DDS_data;
	unsigned int DDS_main_size;
	unsigned int DDS_full_size;
	int mipmaps, cubemap, uncompressed, block_size = 16;
	unsigned int flag;
	unsigned int cf_target, ogl_target_start, ogl_target_end;
	unsigned int opengl_texture_type;

	/*	1st off, does the filename even exist?	*/
	if( NULL == buffer )
	{
		/*	we can't do it!	*/
		result_string_pointer = "NULL buffer";
		return 0;
	}
	if( buffer_length < (int)sizeof( DDS_header ) )
	{
		/*	we can't do it!	*/
		result_string_pointer = "DDS file was too small to contain the DDS header";
		return 0;
	}
	/*	try reading in the header	*/
	memcpy ( (void*)(&header), (const void *)buffer, sizeof( DDS_header ) );
	buffer_index = sizeof( DDS_header );
	/*	guilty until proven innocent	*/
	result_string_pointer = "Failed to read a known DDS header";
	/*	validate the header (warning, "goto"'s ahead, shield your eyes!!)	*/
	flag = ('D'<<0)|('D'<<8)|('S'<<16)|(' '<<24);
	if( header.dwMagic != flag ) {goto quick_exit;}
	if( header.dwSize != 124 ) {goto quick_exit;}
	/*	I need all of these	*/
	flag = DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH | DDSD_PIXELFORMAT;
	if( (header.dwFlags & flag) != flag ) {goto quick_exit;}
	/*	According to the MSDN spec, the dwFlags should contain
		DDSD_LINEARSIZE if it's compressed, or DDSD_PITCH if
		uncompressed.  Some DDS writers do not conform to the
		spec, so I need to make my reader more tolerant	*/
	/*	I need one of these	*/
	flag = DDPF_FOURCC | DDPF_RGB;
	if( (header.sPixelFormat.dwFlags & flag) == 0 ) {goto quick_exit;}
	if( header.sPixelFormat.dwSize != 32 ) {goto quick_exit;}
	if( (header.sCaps.dwCaps1 & DDSCAPS_TEXTURE) == 0 ) {goto quick_exit;}
	/*	make sure it is a type we can upload	*/
	if( (header.sPixelFormat.dwFlags & DDPF_FOURCC) &&
		!(
		(header.sPixelFormat.dwFourCC == (('D'<<0)|('X'<<8)|('T'<<16)|('1'<<24))) ||
		(header.sPixelFormat.dwFourCC == (('D'<<0)|('X'<<8)|('T'<<16)|('3'<<24))) ||
		(header.sPixelFormat.dwFourCC == (('D'<<0)|('X'<<8)|('T'<<16)|('5'<<24)))
		) )
	{
		goto quick_exit;
	}
	/*	OK, validated the header, let's load the image data	*/
	result_string_pointer = "DDS header loaded and validated";
	*width = (int)header.dwWidth;
	*height = (int)header.dwHeight;
	uncompressed = 1 - (header.sPixelFormat.dwFlags & DDPF_FOURCC) / DDPF_FOURCC;
	cubemap = (header.sCaps.dwCaps2 & DDSCAPS2_CUBEMAP) / DDSCAPS2_CUBEMAP;
	if( uncompressed )
	{
		S3TC_type = GL_RGB8;
		block_size = 3;
		if( header.sPixelFormat.dwFlags & DDPF_ALPHAPIXELS )
		{
			S3TC_type = GL_RGBA8;
			block_size = 4;
		}
		DDS_main_size = (unsigned int)((*width) * (*height) * block_size);
	} else
	{
		/*	can we even handle direct uploading to OpenGL DXT compressed images?	*/
		if( query_DXT_capability() != SOIL_CAPABILITY_PRESENT )
		{
			/*	we can't do it!	*/
			result_string_pointer = "Direct upload of S3TC images not supported by the OpenGL driver";
			return 0;
		}
		/*	well, we know it is DXT1/3/5, because we checked above	*/
		switch( (header.sPixelFormat.dwFourCC >> 24) - '0' )
		{
		case 1:
			S3TC_type = SOIL_RGBA_S3TC_DXT1;
			block_size = 8;
			break;
		case 3:
			S3TC_type = SOIL_RGBA_S3TC_DXT3;
			block_size = 16;
			break;
		case 5:
			S3TC_type = SOIL_RGBA_S3TC_DXT5;
			block_size = 16;
			break;
		}
		DDS_main_size = (unsigned int)((((*width)+3)>>2)*(((*height)+3)>>2)*block_size);
	}
	if( cubemap )
	{
		/* does the user want a cubemap?	*/
		if( !loading_as_cubemap )
		{
			/*	we can't do it!	*/
			result_string_pointer = "DDS image was a cubemap";
			return 0;
		}
		/*	can we even handle cubemaps with the OpenGL driver?	*/
		if( query_cubemap_capability() != SOIL_CAPABILITY_PRESENT )
		{
			/*	we can't do it!	*/
			result_string_pointer = "Direct upload of cubemap images not supported by the OpenGL driver";
			return 0;
		}
		ogl_target_start = SOIL_TEXTURE_CUBE_MAP_POSITIVE_X;
		ogl_target_end =   SOIL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
		opengl_texture_type = SOIL_TEXTURE_CUBE_MAP;
	} else
	{
		/* does the user want a non-cubemap?	*/
		if( loading_as_cubemap )
		{
			/*	we can't do it!	*/
			result_string_pointer = "DDS image was not a cubemap";
			return 0;
		}
		ogl_target_start = GL_TEXTURE_2D;
		ogl_target_end =   GL_TEXTURE_2D;
		opengl_texture_type = GL_TEXTURE_2D;
	}
	if( (header.sCaps.dwCaps1 & DDSCAPS_MIPMAP) && (header.dwMipMapCount > 1) )
	{
		mipmaps = (int)header.dwMipMapCount - 1;
		DDS_full_size = DDS_main_size;
	} else
	{
		mipmaps = 0;
		DDS_full_size = DDS_main_size;
	}
	DDS_data = (unsigned char*)malloc( DDS_full_size );
	/*	got the image data RAM, create or use an existing OpenGL texture handle	*/
	tex_ID = reuse_texture_ID;
	if( tex_ID == 0 )
	{
		glGenTextures( 1, &tex_ID );
	}
	/*  bind an OpenGL texture ID	*/
	glBindTexture( opengl_texture_type, tex_ID );
	/*	do this for each face of the cubemap!	*/
	for( cf_target = ogl_target_start; cf_target <= ogl_target_end; ++cf_target )
	{
		if( (int)(buffer_index + DDS_full_size) <= buffer_length )
		{

			memcpy( (void*)DDS_data, (const void*)(&buffer[buffer_index]), DDS_full_size );
			buffer_index += DDS_full_size;
			/*	upload the main chunk	*/
			if( uncompressed )
			{
				/*	and remember, DXT uncompressed uses BGR(A),
					so swap to RGB(A) for ALL MIPmap levels	*/

				
				if (S3TC_type == GL_RGBA8) {
					glTexStorage2D(opengl_texture_type, mipmaps + 1, GL_COMPRESSED_RGBA_S3TC_DXT5_EXT, *width, *height);
					glTexSubImage2D(opengl_texture_type, 0, 0, 0, *width, *height, GL_BGRA, GL_UNSIGNED_BYTE, DDS_data);
				} else if (S3TC_type == GL_RGB8) {
					glTexStorage2D(opengl_texture_type, mipmaps + 1, GL_COMPRESSED_RGB_S3TC_DXT1_EXT, *width, *height);
					glTexSubImage2D(opengl_texture_type, 0, 0, 0, *width, *height, GL_BGR, GL_UNSIGNED_BYTE, DDS_data);
				}

			} else
			{
				soilGlCompressedTexImage2D(
					cf_target, mipmaps,
					S3TC_type, *width, *height, 0,
					(int)DDS_main_size,
					DDS_data );
			}
			/*	upload the mipmaps, if we have them	*/
			if(mipmaps)
				glGenerateMipmap(opengl_texture_type);
			/*	it worked!	*/
			result_string_pointer = "DDS file loaded";
		} else
		{
			glDeleteTextures( 1, & tex_ID );
			tex_ID = 0;
			cf_target = ogl_target_end + 1;
			result_string_pointer = "DDS file was too small for expected image data";
		}
	}/* end reading each face */
	SOIL_free_image_data( DDS_data );
	if( tex_ID )
	{
		/*	did I have MIPmaps?	*/
		if( mipmaps > 0 )
		{
			/*	instruct OpenGL to use the MIPmaps	*/
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
		} else
		{
			/*	instruct OpenGL _NOT_ to use the MIPmaps	*/
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
		}
		/*	does the user want clamping, or wrapping?	*/
		if( flags & SOIL_FLAG_TEXTURE_REPEATS )
		{
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_S, GL_REPEAT );
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_T, GL_REPEAT );
			glTexParameteri( opengl_texture_type, SOIL_TEXTURE_WRAP_R, GL_REPEAT );
		} else
		{
			/*	unsigned int clamp_mode = SOIL_CLAMP_TO_EDGE;	*/
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri( opengl_texture_type, GL_TEXTURE_WRAP_T, GL_CLAMP);
			glTexParameteri( opengl_texture_type, SOIL_TEXTURE_WRAP_R, GL_CLAMP);
		}
	}

quick_exit:
	/*	report success or failure	*/
	return tex_ID;
}

unsigned int SOIL_direct_load_DDS(
		const char *filename,
		unsigned int reuse_texture_ID,
		int flags,
		int loading_as_cubemap, int* width, int* height )
{
	FILE *f;
	unsigned char *buffer;
	size_t buffer_length, bytes_read;
	unsigned int tex_ID = 0;
	/*	error checks	*/
	if( NULL == filename )
	{
		result_string_pointer = "NULL filename";
		return 0;
	}
	f = fopen( filename, "rb" );
	if( NULL == f )
	{
		/*	the file doesn't seem to exist (or be open-able)	*/
		result_string_pointer = "Can not find DDS file";
		return 0;
	}
	fseek( f, 0, SEEK_END );
	buffer_length = (size_t)ftell( f );
	fseek( f, 0, SEEK_SET );
	buffer = (unsigned char *) malloc( buffer_length );
	if( NULL == buffer )
	{
		result_string_pointer = "malloc failed";
		fclose( f );
		return 0;
	}
	bytes_read = fread( (void*)buffer, 1, buffer_length, f );
	fclose( f );
	if( bytes_read < buffer_length )
	{
		/*	huh?	*/
		buffer_length = bytes_read;
	}
	/*	now try to do the loading	*/
	tex_ID = SOIL_direct_load_DDS_from_memory(
		buffer, (int)buffer_length,
		reuse_texture_ID, flags, loading_as_cubemap, width, height);
	SOIL_free_image_data( buffer );
	return tex_ID;
}

int query_NPOT_capability( void )
{
	/*	check for the capability	*/
	if( has_NPOT_capability == SOIL_CAPABILITY_UNKNOWN )
	{
		/*	we haven't yet checked for the capability, do so	*/
		if(wglewIsSupported("GL_ARB_texture_non_power_of_two"))
		{
			/*	not there, flag the failure	*/
			has_NPOT_capability = SOIL_CAPABILITY_NONE;
		} else
		{
			/*	it's there!	*/
			has_NPOT_capability = SOIL_CAPABILITY_PRESENT;
		}
	}
	/*	let the user know if we can do non-power-of-two textures or not	*/
	return has_NPOT_capability;
}

int query_tex_rectangle_capability( void )
{
	/*	check for the capability	*/
	if( has_tex_rectangle_capability == SOIL_CAPABILITY_UNKNOWN )
	{
		/*	we haven't yet checked for the capability, do so	*/
		if(wglewIsSupported("GL_ARB_texture_rectangle" )
		&& wglewIsSupported("GL_EXT_texture_rectangle" )
		&& wglewIsSupported("GL_NV_texture_rectangle" ))
		{
			/*	not there, flag the failure	*/
			has_tex_rectangle_capability = SOIL_CAPABILITY_NONE;
		} else
		{
			/*	it's there!	*/
			has_tex_rectangle_capability = SOIL_CAPABILITY_PRESENT;
		}
	}
	/*	let the user know if we can do texture rectangles or not	*/
	return has_tex_rectangle_capability;
}

int query_cubemap_capability( void )
{
	/*	check for the capability	*/
	if( has_cubemap_capability == SOIL_CAPABILITY_UNKNOWN )
	{
		/*	we haven't yet checked for the capability, do so	*/
		if(wglewIsSupported("GL_ARB_texture_cube_map" )
		&& wglewIsSupported("GL_EXT_texture_cube_map" ) )
		{
			/*	not there, flag the failure	*/
			has_cubemap_capability = SOIL_CAPABILITY_NONE;
		} else
		{
			/*	it's there!	*/
			has_cubemap_capability = SOIL_CAPABILITY_PRESENT;
		}
	}
	/*	let the user know if we can do cubemaps or not	*/
	return has_cubemap_capability;
}

void WINAPI soilGlCompressedTexImage2D_wrapper(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data);
void WINAPI soilGlCompressedTexImage2D_wrapper(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const GLvoid * data) {
	glTexStorage2D(target, level+1, internalformat, width, height);
	glCompressedTexSubImage2D(target, 0, 0, 0, width, height, internalformat, imageSize, data);
	glGenerateMipmap(target);
}

int query_DXT_capability( void )
{
	/*	check for the capability	*/
	if( has_DXT_capability == SOIL_CAPABILITY_UNKNOWN )
	{
		/*	we haven't yet checked for the capability, do so	*/
		if(wglewIsSupported("GL_EXT_texture_compression_s3tc" ) )
		{
			/*	not there, flag the failure	*/
			has_DXT_capability = SOIL_CAPABILITY_NONE;
		} else
		{
			/*	and find the address of the extension function	*/
			P_SOIL_GLCOMPRESSEDTEXIMAGE2DPROC ext_addr = NULL;
			#ifdef WIN32
				ext_addr = (P_SOIL_GLCOMPRESSEDTEXIMAGE2DPROC)
						wglGetProcAddress
						(
							"glCompressedTexImage2DARB"
						);
			#elif defined(__APPLE__) || defined(__APPLE_CC__)
				/*	I can't test this Apple stuff!	*/
				CFBundleRef bundle;
				CFURLRef bundleURL =
					CFURLCreateWithFileSystemPath(
						kCFAllocatorDefault,
						CFSTR("/System/Library/Frameworks/OpenGL.framework"),
						kCFURLPOSIXPathStyle,
						true );
				CFStringRef extensionName =
					CFStringCreateWithCString(
						kCFAllocatorDefault,
						"glCompressedTexImage2DARB",
						kCFStringEncodingASCII );
				bundle = CFBundleCreate( kCFAllocatorDefault, bundleURL );
				assert( bundle != NULL );
				ext_addr = (P_SOIL_GLCOMPRESSEDTEXIMAGE2DPROC)
						CFBundleGetFunctionPointerForName
						(
							bundle, extensionName
						);
				CFRelease( bundleURL );
				CFRelease( extensionName );
				CFRelease( bundle );
			#else
				ext_addr = (P_SOIL_GLCOMPRESSEDTEXIMAGE2DPROC)
						glXGetProcAddressARB
						(
							(const GLubyte *)"glCompressedTexImage2DARB"
						);
			#endif
			/*	Flag it so no checks needed later	*/
			if( NULL == ext_addr )
			{
				/*	hmm, not good!!  This should not happen, but does on my
					laptop's VIA chipset.  The GL_EXT_texture_compression_s3tc
					spec requires that ARB_texture_compression be present too.
					this means I can upload and have the OpenGL drive do the
					conversion, but I can't use my own routines or load DDS files
					from disk and upload them directly [8^(	*/
				has_DXT_capability = SOIL_CAPABILITY_NONE;
			} else
			{
				/*	all's well!	*/
				//soilGlCompressedTexImage2D = ext_addr;
				soilGlCompressedTexImage2D = soilGlCompressedTexImage2D_wrapper;
				has_DXT_capability = SOIL_CAPABILITY_PRESENT;
			}
		}
	}
	/*	let the user know if we can do DXT or not	*/
	return has_DXT_capability;
}

#pragma clang diagnostic pop
