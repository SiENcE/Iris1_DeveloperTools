//
// File: GumpLoader.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//
#include "stdafx.h"

#include "GumpLoader.h"
#include "VerdataLoader.h"
#include "Debug.h"
#include "uotype.h"
#include <string.h>
#include <iostream>

using namespace std;

cGumpLoader * pGumpLoader = NULL;

cGumpLoader::cGumpLoader ()
{
	gumpfile=NULL;
	gump_index=NULL;
}

cGumpLoader::~cGumpLoader ()
{
	delete gumpfile;
	delete[] gump_index;
}

bool cGumpLoader::load(std::string filename, std::string indexname)
{
	delete gumpfile;
	delete[] gump_index;

	string errstr;

	gumpfile = new ifstream(filename.c_str(), ios::in | ios::binary);
	std::ifstream* gumpindexfile = new ifstream(indexname.c_str(), ios::in | ios::binary);

	if (!gumpfile->is_open() || !gumpindexfile->is_open()) {
		delete gumpfile;
		delete gumpindexfile;
		gumpfile = NULL;
		gumpindexfile = NULL;
		return false;
	}

	gumpindexfile->seekg(0, ios::end);
	unsigned long idxE = gumpindexfile->tellg();
	gump_count = idxE / sizeof(stIndexRecord);
	
	gump_index = new stIndexRecord[gump_count];
	gumpindexfile->seekg(0, ios::beg);
	gumpindexfile->read((char*)gump_index,idxE);

	delete gumpindexfile;

	return true;
}

inline unsigned int color15to32b( unsigned short color )
{
	if( !color )
		return color;

	unsigned char r = ( ( color >> 10 ) & 0x1F );
	unsigned char g = ( ( color >> 5 ) & 0x1F );
	unsigned char b = ( color & 0x1F );

	//unsigned int color32 = ( r * 8 ) | ( ( g * 8 ) << 8 ) | ( ( b * 8 ) << 16 ) | 0xFF000000;
	unsigned int color32 = (( r * 8 ) << 16) | ( ( g * 8 ) << 8 ) | ( ( b * 8 )) | 0xFF000000;

	return color32;
}


Uint32* cGumpLoader::LoadGumpRaw(int index, int &width, int &height)
{
	width = 0;
	height = 0;
	if((index < 0) || ((unsigned int)index >= gump_count))
		return NULL;

	sPatchResult patch;
	patch.file = NULL;

	if (pVerdataLoader) 
		patch = pVerdataLoader->FindPatch(VERDATAPATCH_GUMP, index);

	stIndexRecord idx;

	if (patch.file) {
		idx = patch.index;
	} else {
		patch.file = gumpfile;
		idx = gump_index[index];
	}

	if(idx.offset == 0xffffffff)
		return NULL;

	int w = idx.width;  
	int h = idx.height;

	if(w > 1024 || h > 1024)
		return NULL;
	
	Uint32 * data = (Uint32 *) malloc (w * h * 4); // dword alignment
	memset(data, 0xff, w * h * 4);
	Uint32 * heightTable = new Uint32[h];

	patch.file->seekg(idx.offset, ios::beg);
	patch.file->read((char* )heightTable, h * 4);

	unsigned int rle = 0;
	
	for (unsigned short y = 0, bmp_y = h - 1; y < h; y++, bmp_y--) {
		gumpfile->seekg(heightTable[y] * 4 + idx.offset, ios::beg);

		unsigned short x = 0;
		Uint32 *linedata = (Uint32 *) data  + bmp_y * w;

		while (x < w) {
			patch.file->read((char *) &rle, 4);	// Read a RLE key

			unsigned short length = (rle >> 16) & 0xFFFF;	// First two byte
			unsigned short color = rle & 0xFFFF;	// Second two byte

			Uint32 col32 = color15to32b(color);

			if (!(col32 & 0xff000000)) col32=0x00ffffff;

			for (unsigned int i = 0; i < length; i++,x++) {
				//memcpy(linedata++, &col32, 4);
				*linedata++=col32;
				//linedata++;
				//x++;
			}
		}
	}

	delete heightTable;

	width = w;
	height = h;
	
	return data;
}

CGumpPtr cGumpLoader::LoadGump(int iGumpID)
{
	int w=0,h=0;

	UINT32* data = LoadGumpRaw(iGumpID, w, h);
	if (!data)  return NULL;

	return new CDibGump(iGumpID, CSize(w,h), (LPBYTE)data);
}

bool cGumpLoader::CheckGump(int index)
{
	if((index < 0) || ((unsigned int)index >= gump_count))
		return false;


	sPatchResult patch;
	patch.file = NULL;

	if (pVerdataLoader) 
		patch = pVerdataLoader->FindPatch(VERDATAPATCH_GUMP, index);

	stIndexRecord idx;

	if (patch.file) {
		idx = patch.index;
	} else {
		patch.file = gumpfile;
		idx = gump_index[index];
	}

	if(idx.offset == 0xffffffff)
		return false;
	return true;
}

bool cGumpLoader::GetGumpSize(int index, int &width, int &height)
{
	width = 0;
	height = 0;

	if((index < 0) || ((unsigned int)index >= gump_count))
		return false;

	sPatchResult patch;
	patch.file = NULL;

	if (pVerdataLoader) 
		patch = pVerdataLoader->FindPatch(VERDATAPATCH_GUMP, index);

	stIndexRecord idx;

	if (patch.file) {
		idx = patch.index;
	} else {
		patch.file = gumpfile;
		idx = gump_index[index];
	}

	if(idx.offset == 0xffffffff)
		return false;

	width = idx.width;   
	height = idx.height;
	return true;

}

int cGumpLoader::GetGumpWidth(int index)
{
	int width, height;
	GetGumpSize(index, width, height);
	return width;
}

int cGumpLoader::GetGumpHeight(int index)
{
	int width, height;
	GetGumpSize(index, width, height);
	return height;
}

