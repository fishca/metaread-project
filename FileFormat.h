//---------------------------------------------------------------------------

#ifndef FileFormatH
#define FileFormatH

#include "Common.h"
#include "Parse_tree.h"
#include "MessageRegistration.h"

//---------------------------------------------------------------------------
// Форматы файлов

enum file_format
{
	ff_min = 0,

	ff_gif = 1, // GIF
	ff_pcx = 2, // PCX
	ff_bmp = 3, // BMP
	ff_jpg = 4, // JPG
	ff_png = 5, // PNG
	ff_tiff = 6, // TIFF
	ff_ico = 7, // ICO
	ff_wmf = 8, // WMF
	ff_emf = 9, // EMF
	ff_zip = 10, // ZIP

	ff_max
};

file_format get_file_format(TTempStream* stream);

//---------------------------------------------------------------------------

#endif
