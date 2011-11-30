/*****
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 *****/

#include "stdafx.h"
#include "granny/grntype.h"
#include "granny/grntextureinfos.h"

#include <stdlib.h>
#include <iostream>

#include "Debug.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

TextureInfo::TextureInfo()
{
}
TextureInfo::~TextureInfo()
{
}

void TextureInfo::load( cGrannyStream * file, dword tiOffset, dword baseOffset, dword peers )
{
	dword oldPos;
	for (unsigned int i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		switch (chunk)
		{
			case 0xCA5E0303:	//Geometry
				oldPos=file->tellg();
				file->seekg(offset+baseOffset);
				width=file->readDword();
				height=file->readDword();
				depth=file->readDword();
				file->seekg(oldPos);
				pDebug.Log(LEVEL_INFO1,"[texture geometry] width %2d, height %2d, depth %2d", width, height, depth);
				break;
			case 0xCA5E0305:	//Geometry Container
				load(file,offset,baseOffset,children);
				break;
			case 0xCA5E0f04:	//textureid
				oldPos=file->tellg();
				file->seekg(offset+baseOffset);
				id=file->readDword();
				file->seekg(oldPos);
				pDebug.Log(LEVEL_INFO1,"[texture] id %d", id);
				break;
			default:
				{
					pDebug.Log(LEVEL_ERROR, "Unknown Textureinfo Chunk: 0x%x", chunk);
					//assert(!"Unknown Textureinfo Chunk");
					return;
				}
		}
		i+=children+1;
	}
}
dword TextureInfo::getID()
{
	return id;
}

TextureInfos::TextureInfos()
{
}

TextureInfos::~TextureInfos()
{
}

void TextureInfos::load(cGrannyStream * file,dword texOffset,
		dword baseOffset,dword peers)
{
	//dword oldPos;
	TextureInfo texture;
	for (unsigned int i=0;i<peers;)
	{
		dword chunk=file->readDword();
		dword offset=file->readDword();
		dword children=file->readDword();
		switch (chunk)
		{
			case 0xCA5E0301:	//Texture
				
				texture.load(file,offset,baseOffset,children);
				textures.push_back(texture);
				break;
			default:
				{
					pDebug.Log(LEVEL_ERROR, "Unknown Textureinfos Chunk: 0x%x", chunk);
					//assert(!"Unknown Textureinfos Chunk");
					return;
				}
		}
		i+=children+1;
	}
}

dword TextureInfos::getTextureID()
{
	if (!textures.size())
		return 0;
	return textures[0].getID();
}
