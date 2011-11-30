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

#ifndef __GRN_H__
#define __GRN_H__

#include <map>
#include <string>
#include "grnitemlist.h"
#include "grnmatrix.h"
#include "renderer/Texture.h"

class cCharacterLight;

class cDeformedArray
{
public:
	cDeformedArray(int size);
	virtual ~cDeformedArray();
	float * data();
	int size() { return m_size; }

	GrnMatrix matrix_right_hand;
	GrnMatrix matrix_left_hand;
private:
	int m_size;
	float * m_data;
};

class cGrannyFile;

#if 0
class cIrisModelFile
{
public:
	cIrisModelFile() {}
	virtual ~cIrisModelFile() {}
	virtual void load( std::string filename, std::string basepath ) = 0;
	//virtual Animations &getAnimations();

	virtual void Render(cIrisModelFile *animation,  float & curTime);

	virtual float length () { return 0.0f; }
	virtual bool export (std::string filename) { return false; }

	virtual GrnMatrix &getLeftHandMatrix()  = 0;
	virtual GrnMatrix &getRightHandMatrix() = 0;

	virtual void print() = 0;
	
	virtual std::string getPathName() = 0; //{ return m_pathname; }
	virtual std::string getFileName() = 0; //{ return m_filename; }

	virtual Point getModelOrg(float *w=NULL, float *h=NULL, float *d=NULL) = 0;
};
#endif

class cGrannyFile //: public cIrisModelFile
{
public:
	cGrannyFile();
	virtual ~cGrannyFile();

	void load( std::string filename, std::string basepath );

	virtual void Render(cGrannyFile *animation,  float & curTime, cCharacterLight * character_light, float r, float g, float b, float alpha, bool is_corpse);
	//virtual void Render(cGrannyFile *animation,  float & curTime);

	virtual float length () { return 0.0f; }
	virtual bool export (std::string filename);
	virtual bool import (std::string filename);

	GrnMatrix &getLeftHandMatrix()			{ return matrix_left_hand;              }
	GrnMatrix &getRightHandMatrix()			{ return matrix_right_hand;             }

	virtual float getAnimationLength()		{ return getAnimations().length();      }
	
	void print();
	
	std::string getPathName()				{ return m_pathname;                    }
	std::string getFileName()				{ return m_filename;                    }

	Point getModelOrg(float *w=NULL, float *h=NULL, float *d=NULL);

	void SetColor(float r, float g, float b);
protected:
	GrnMatrix matrix_right_hand;
	GrnMatrix matrix_left_hand;

	void calculateBoneRests( Bone *bone );
	void updateHandMatrix(Bone *bone, const GrnMatrix &matrix);
	bool isImportBone(int boneid);

	Animations &getAnimations()				{ return m_object.getAnimations();      }
	std::string getTextureName()			{ return m_object.getTextureName();     }

	Meshes &getMeshes()						{ return m_object.getMeshes();          }
	Bones &getBones()						{ return m_object.getBones();           }
	Textures &getTextures()					{ return m_object.getTextures();		}
	BoneAnim &getBoneAnim(dword id)			{ return m_object.getBoneAnim( id );    }
	BoneTies &getTies()						{ return m_object.getTies();			}
	TextChunk &getTextChunk()				{ return m_object.getTextChunk();       }
	Objects &getObjects()					{ return m_object.getObjects();         }

	dword findString( std::string str )		{ return m_object.findString(str);      }
	dword getValue(dword obj,dword key)		{ return m_object.getValue(obj,key);    }
	std::string findID(dword id)			{ return m_object.findID(id);           }
	dword findValue(dword key,dword value)  { return m_object.findValue(key,value); }

	virtual void getSkeleton( Bone *bone, float & curTime );
	virtual void drawSkeleton(Bone* bone, float curTime);

	virtual cDeformedArray * getDeformed (int index)                { return NULL;     }
	virtual void addDeformed (cDeformedArray * deformed, int index) { delete deformed; }
	cDeformedArray * createDeformed (cGrannyFile * animation, float time, std::list<Mesh>::iterator imesh);

	int getFrame (cGrannyFile * animation, float & curTime);

	bool initialized ()                     { return m_initialized;	    }
	bool loadTexture( const std::string &texfilename );

	// Returns the GL Texture Identifier
	GLuint getTexture() const               { return m_texture ? m_texture->GetGLTex() : 0 ; }

	enum BONEID { MASTER = 0, LHAND, RHAND, LGRASP,  RGRASP, NUM_BONEID };

	void setBoneID(BONEID bone, int boneid) { m_boneID[bone] = boneid;  }
	int  getBoneID(BONEID bone)				{ return m_boneID[bone];    }
	int  getBoneID(const std::string &name);

	std::string getTextureFileName()		{ return m_texfilename;	    }
	void drawBoundBox(const Mesh &mesh);
	
	void mainChunk();
	void freeChunk();	// free unused items in ItemList

	void initBone();
	void printRaw();	// print not modified data

	ItemList m_object;
	cGrannyStream *m_stream;
	Texture *m_texture; // Our main Texture
	bool m_initialized;
	std::string m_texfilename;
	std::string m_filename, m_pathname;

	int m_boneID[NUM_BONEID];

	std::vector<dword> m_boneties;

	std::vector<dword> m_verIndex;
	std::vector<float> m_verNormal;
	std::vector<float> m_verTexcoord;
	std::vector<float> m_verColor;

	virtual void makeVertexArray();
	void registVertexArray();
	
	bool m_imported, m_freechunk;

	static dword getBoneNameID(const std::string &name);
	static const std::string &getBoneName(dword nameID);

	static dword BONE_NAMEID[NUM_BONEID];

	friend class cGrannyAnimation;
	friend class cNameCache;
};

class cGrannyAnimation : public cGrannyFile
{
public:
	cGrannyAnimation();
	virtual ~cGrannyAnimation();
	void Assign (cGrannyFile * model);
	dword * GetAnimBones(void);
	virtual void getSkeleton( Bone *bone, float & curTime);
	virtual float length () { return m_length;	}
	virtual cDeformedArray * getDeformed (int index);
	virtual void addDeformed (cDeformedArray * deformed, int index);

	virtual void drawSkeleton(Bone* bone, float curTime);
	virtual bool export (std::string filename);

protected:
	std::vector<dword> m_animBones;
	float m_length;
	std::map <int, cDeformedArray *> m_cache;

	virtual void makeVertexArray() {}
	
	cGrannyFile *m_assignModel;
};

#endif
