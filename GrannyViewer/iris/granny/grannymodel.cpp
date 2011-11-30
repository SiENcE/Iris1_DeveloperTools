#include "stdafx.h"
#include "granny/grntype.h"
#include "granny/grannymodel.h"
#include "granny/grnbones.h"
#include <iostream>

using namespace std;

// Utility Function
void calculateBoneRests( Bone *bone );

cGrannyFile::cGrannyFile()
{
	texture = 0;
}

cGrannyFile::~cGrannyFile()
{
	// Destroy the Texture we're binding
	delete texture;
}

void cGrannyFile::load( std::string filename, std::string basepath )
{
	//printf("%s\n",filename.c_str());

	file.open( filename.c_str(), ios::in|ios::binary );	
	if( !file.is_open() )
		return;

	file.seekg( 0x40 );	// skip header (Could be FileType magic)

	dword chunk=readDword(file);
	switch (chunk)
	{
	case 0xCA5E0000:
		mainChunk();
		break;
	default:
		hex( cerr );
		cerr << "Unknown main chunk: " << chunk << endl;
		exit( 1 );
	}
	file.close();

	if( getTextureName() != "" )
		loadTexture( basepath.c_str() );

	Bones &bones = getBones();
	if( bones.bones.size() > 0 )
	{
		calculateBoneRests( bones.bones[0] );
	}
}

void cGrannyFile::addTime( float t )
{
}

string cGrannyFile::getTextureName()
{
	return object.getTextureName();
}

Meshes &cGrannyFile::getMeshes()
{
	return object.getMeshes();
}

Bones &cGrannyFile::getBones()
{
	return object.getBones();
}

BoneTies &cGrannyFile::getTies()
{
	return object.getTies();
}

Textures &cGrannyFile::getTextures()
{
	return object.getTextures();
}

BoneAnim &cGrannyFile::getBoneAnim( dword id )
{
	return object.getBoneAnim( id );
}

Animations &cGrannyFile::getAnimations()
{
	return object.getAnimations();
}

dword cGrannyFile::findString(string str)
{
	return object.findString(str);
}

dword cGrannyFile::getValue(dword obj,dword key)
{
	return object.getValue(obj,key);
}
dword cGrannyFile::findValue(dword key,dword value)
{
	return object.findValue(key,value);
}

std::string cGrannyFile::findID(dword id)
{
	return object.findID(id);
}

void cGrannyFile::mainChunk()
{
	dword children = readDword( file );

	for( int i = 0; i < 6; ++i )
		readDword( file ); // CRC?

	for( dword child = 0; child < children; ++child )
	{
		// Chunk Header
		dword chunk = readDword( file );

		switch( chunk )
		{
			// Final Chunk (End-of-File?)
		case 0xCA5E0101:
			final.load( file );
			break;

			// Copyright Chunk
		case 0xCA5E0102:
			copyright.load( file );
			break;

			// Object Chunk
		case 0xCA5E0103:
			object.load( file );
			break;
		default:
			hex(cerr);
			cerr<<"Unknown GRN Chunk: "<<chunk<<endl;
			exit(1);
		}
	}
}

bool cGrannyFile::loadTexture( const char *basepath )
{
	std::string filename;
	filename = basepath;
	filename += getTextureName();
	filename += ".tga";

	// Try to load our Texture
	texture = new Texture;
	if( !texture->LoadFromFile( filename.c_str() ) )
	{
		delete texture;
		texture = 0;
		return false;
	}

	return true;
}

void calculateBoneRests( Bone *bone )
{
	float X,Y,Z,x,y,z,w;
	X=bone->translate.points[0];
	Y=bone->translate.points[1];
	Z=bone->translate.points[2];

	x=bone->quaternion.points[0];
	y=bone->quaternion.points[1];
	z=bone->quaternion.points[2];
	w=bone->quaternion.points[3];

	GrnMatrix matrix;
	float x2=x+x; float y2=y+y; float z2=z+z;
	float xx=x*x2; float xy=x2*y; float xz=x2*z; float xw=x2*w;
	float yy=y*y2; float yz=y2*z; float yw=y2*w;
	float zz=z*z2; float zw=z2*w;

	matrix[0]= 1-yy-zz;
	matrix[1]=   xy+zw;
	matrix[2]=   xz-yw;
	matrix[3]= 0;
	matrix[4]=   xy-zw;
	matrix[5]= 1-xx-zz;
	matrix[6]=   yz+xw;
	matrix[7]= 0;
	matrix[8]=   xz+yw;
	matrix[9]=   yz-xw;
	matrix[10]=1-xx-yy;
	matrix[11]=0;
	matrix[12]=X;
	matrix[13]=Y;
	matrix[14]=Z;
	matrix[15]=1;

	glMultMatrixf(matrix.matrix);
	glGetFloatv(GL_MODELVIEW_MATRIX,matrix.matrix);

	bone->matrix=matrix;
	bone->matrix.invert();
	bone->curMatrix=matrix;
	bone->curMatrix*=bone->matrix;

	vector<Bone *>::iterator ibone;
	for (ibone=bone->children.begin();
		ibone!=bone->children.end();ibone++)
	{
		glPushMatrix();
		calculateBoneRests(*ibone);
		glPopMatrix();
	}
}
