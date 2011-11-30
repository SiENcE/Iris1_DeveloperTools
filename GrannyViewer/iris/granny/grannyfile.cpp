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
#include "granny/grannyfile.h"
#include "granny/GrannyTextures.h"
#include "Config.h"
#include "Debug.h"
#include <iostream>
#include <cassert>
#include <deque>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//--------------------------------------------------------------------------------------------
// Utility Function
//--------------------------------------------------------------------------------------------


extern int polycount;

void printw (float x, float y, float z, char* format, ...);

char *strlower(char *s) 
{ 
	int offset = ((int)'A')-((int)'a'); 
	for(int i=0; s[i]; i++) { 
	if( (s[i]>='A') && (s[i]<='Z') ) 
	s[i] -= offset; 
	} 
	return s; 
}


bool isMasterName(const char *str)
{
#ifdef _WINDOWS
	char *s = _strlwr(_strdup(str));
#else
    char *s = strlower(strdup(str)); 
#endif
	bool ismaster = false;

	if (strstr(s, "master") || strstr(s, "mesh"))	
		ismaster = true;

	free(s);

	return ismaster;
}

std::string right(const std::string s, int n) { return s.substr(max(0,s.size()-n),n); } 


int splitString(std::vector<std::string> &vec, const char *s)
{
	vec.clear();
	if (!s) return 0;

	std::string str;
	const char *delim = " \"\t\r\n";
	while (*s)
	{
		if (strchr(delim, *s))
		{
			// "string"
			if (*s == '\"')
			{
				while (*++s && *s != '\"')
					str += *s;
			}

			if (!str.empty())
			{
				vec.push_back(str); str = "";
			}
		}
		else
		{
			str += *s;
		}
		
		s++;
	}

	return vec.size();
}




class cNameCache
{
public:
	cNameCache()
	{
		const char *names[] = 
		{
			"__Root",			// 0 
			 "master_h_male",	// 1 
			 "Bip01",			// 2 
			 "Bip01 Pelvis",	// 3 
			 "Bip01 Spine",		// 4 
			 "Bip01 Spine1",	// 5 
			 "Bip01 Spine2",	// 6 
			 "Bip01 Neck",		// 7 
			 "Bip01 R Clavicle",// 8 
			 "Bip01 R UpperArm",// 9 
			 "Bip01 L Clavicle",// 10
			 "Bip01 L UpperArm",// 11
			 "Bip01 Head",      // 12
			 "Bip01 L Forearm", // 13
			 "Bip01 L Hand",    // 14
			 "Bip01 R Forearm", // 15
			 "Bip01 R Hand",	// 16
			 "Bip01 R Finger0", // 17
			 "Bip01 R Finger01",// 18
			 "Bip01 R Finger1", // 19
			 "Bip01 R Finger11",// 20
			 "Bip01 L Finger0", // 21
			 "Bip01 L Finger01",// 22
			 "Bip01 L Finger1", // 23
			 "Bip01 L Finger11",// 24
			 "Bip01 L Thigh",   // 25
			 "Bip01 L Calf",    // 26
			 "Bip01 L Foot",    // 27
			 "Bip01 R Thigh",   // 28
			 "Bip01 R Calf",    // 29
			 "Bip01 R Foot",    // 30
			 "CP_Grasp_Lhand",  // 31
			 "CP_Grasp_Rhand",  // 32
			 "mesh_h_male",     // 33
			 "mesh_h_female",   // 34
			 "master_h_female",	// 35
			 NULL
		};

		// register common bone names
		for (int i = 0; names[i] != NULL; i++)
			getID(names[i]);

		cGrannyFile::BONE_NAMEID[cGrannyFile::LHAND]  = getID("Bip01 L Hand");
		cGrannyFile::BONE_NAMEID[cGrannyFile::RHAND]  = getID("Bip01 R Hand");
		cGrannyFile::BONE_NAMEID[cGrannyFile::LGRASP] = getID("CP_Grasp_Lhand");
		cGrannyFile::BONE_NAMEID[cGrannyFile::RGRASP] = getID("CP_Grasp_Rhand");
	}

	dword getID(const std::string &name)
	{
		// use lowercase string to compare key
		char *s = _strlwr(_strdup(name.c_str()));
		std::map<std::string, dword>::iterator i = m_cache.find(s);
		if (i != m_cache.end()) 
		{
			free(s);
			return i->second;
		}

		m_names.push_back(name);

		dword key = m_names.size() - 1;
		m_cache.insert(std::make_pair(s,key));

		free(s);

		return key;
	}

	const std::string &getName(dword id)
	{
		if (id < m_names.size()) 
			return m_names[id];

		return "";
	}

	bool  has(const std::string &name) { return m_cache.find(name) != m_cache.end(); }
private:
	std::deque<std::string> m_names;
	std::map<std::string, dword> m_cache;
} nameCache;

dword cGrannyFile::BONE_NAMEID[NUM_BONEID];

dword cGrannyFile::getBoneNameID(const std::string &name)
{
	return nameCache.getID(name);
}

const std::string &cGrannyFile::getBoneName(dword nameID)
{
	return nameCache.getName(nameID);
}

//--------------------------------------------------------------------------------------------
// cDeformedArray
//--------------------------------------------------------------------------------------------


cGrannyFile::cGrannyFile()
{
	m_texture = 0;
	m_initialized = false;
	m_texfilename = "";

	std::fill(m_boneID, m_boneID+NUM_BONEID, -1);

	m_imported  = false;
	m_freechunk = false;
}

cGrannyFile::~cGrannyFile()
{
	// Destroy the Texture we're binding
	//delete texture;
	if (pGrannyTextureLoader)
		pGrannyTextureLoader->FreeTexture(m_texfilename);
	m_texture = NULL;
}

void cGrannyFile::calculateBoneRests( Bone *bone )
{
	assert(bone);
	if (!bone) return;

	GrnMatrix matrix;
	matrix.setTransform(bone->quaternion, bone->translate);
	glMultMatrixf(matrix.matrix);

	glGetFloatv(GL_MODELVIEW_MATRIX,matrix.matrix);

	bone->matrix=matrix;
	bone->matrix.invert();
	bone->curMatrix=matrix;
	bone->curMatrix*=bone->matrix;

	bone->curQuaternion = bone->quaternion;
	bone->curTranslate = bone->translate;

	updateHandMatrix(bone, matrix);
	
	vector<Bone *>::iterator ibone;
	for (ibone=bone->children.begin();
		ibone!=bone->children.end();ibone++)
	{
		glPushMatrix();
		calculateBoneRests(*ibone);
		glPopMatrix();
	}
}

void cGrannyFile::updateHandMatrix(Bone *bone, const GrnMatrix &matrix)
{
	if (!bone) return;

	// if model has grasp bone then overwrited hand bone matrix.
	// because, grasp bone is child of hand bone.
	if (m_boneID[LHAND]   != -1 && (int) bone->id == m_boneID[LHAND])
	{
		// if model hasn't grasp bone then calculate grasp bone pos from hand bone.
		glPushMatrix();
		glTranslatef(0.2f,0.1f,0);
		glRotatef(-135.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
		//glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX,matrix_left_hand.matrix);
		glPopMatrix();
	}
	else if (m_boneID[LGRASP]  != -1 && (int) bone->id == m_boneID[LGRASP])
		matrix_left_hand = matrix;
	else if (m_boneID[RHAND]   != -1 && (int) bone->id == m_boneID[RHAND])
	{
		glPushMatrix();
		glTranslatef(0.2f,0.1f,0);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
		glRotatef(135.0f, 0.0f, 1.0f, 0.0f);
		//glRotatef(45.0f, 0.0f, 0.0f, 1.0f);
		
		glGetFloatv(GL_MODELVIEW_MATRIX,matrix_right_hand.matrix);
		glPopMatrix();
	}
	else if (m_boneID[RGRASP]  != -1 && (int) bone->id == m_boneID[RGRASP])
		matrix_right_hand = matrix;

}

// set bone name & find master, hands bone 
//  - bone.id -> bone.ojbptr -> bone.obj -> textid -> textchunk -> bone.name
//  - bones.size() == objptr.size() == boneobj.size() <= object.size() : boneobj is subset object
void cGrannyFile::initBone()
{
	if (!m_initialized) return;

	std::vector<Bone *> &bones = getBones().bones;
	std::vector<dword> &boneObjects = getTies().boneObjects;
	std::vector<dword> &boneObjPtrs = getTies().boneObjPtrs;
	std::vector<BoneTie*> &boneties = getTies().boneties;
	vector<BoneAnim> &animBones = getAnimations().bones;

	std::map<dword,dword> m_bonePtr;

	dword lodNm = findString("__ObjectName");

	std::fill(m_boneID, m_boneID+NUM_BONEID, -1);

	for (int i = 0; i < boneties.size(); i++)
		m_boneties.push_back(boneties[i]->bone);

	std::string name;
	for (int i = 0; i < bones.size(); i++) 
	{
		Bone* bone = bones[i];
		// bonetie.boneObjPtr: connect bone.id to boneObj
		dword objptr = boneObjPtrs[bone->id];
		// bonetie.boneObjects: connect bone.id to textchunk(bone name)
		dword obj = boneObjects[objptr-1];
		// objects.getValue(obj-1,key); objects[obj]->getValue(key);
		dword textid = getValue(obj,lodNm);
		// assign bone name
		//bone->name = findID(textid);
		name = findID(textid);
		bone->nameid = getBoneNameID(name);

		m_bonePtr.insert(std::make_pair(objptr,bone->id));

		// td anim, some aos anim(id 73) has not cp_grasp_ bone, we use hane bone.
		//const char *s = bone->name.c_str();
		const char *s = name.c_str();
		if (isMasterName(s) && m_boneID[MASTER] == -1)
			m_boneID[MASTER] = bone->id;
#if 1
		else if (bone->nameid == BONE_NAMEID[LHAND])
			m_boneID[LHAND]  = bone->id;
		else if (bone->nameid == BONE_NAMEID[RHAND])
			m_boneID[RHAND]  = bone->id;
		else if (bone->nameid == BONE_NAMEID[LGRASP])
			m_boneID[LGRASP] = bone->id;
		else if (bone->nameid == BONE_NAMEID[RGRASP])
			m_boneID[RGRASP] = bone->id;
#else
		else if (_stricmp(s, "Bip01 L Hand") == 0)
			m_boneID[LHAND]  = bone->id;
		else if (_stricmp(s, "Bip01 R Hand") == 0)
			m_boneID[RHAND]  = bone->id;
		else if (_stricmp(s, "CP_Grasp_Lhand") == 0)
			m_boneID[LGRASP] = bone->id;
		else if (_stricmp(s, "CP_Grasp_Rhand") == 0)
			m_boneID[RGRASP] = bone->id;
#endif
	}

	// objptr 대신 boneid 를 직접 넣는다.
	for (int i = 0; i < animBones.size(); i++)
	{
		std::map<dword,dword>::iterator iter = m_bonePtr.find(animBones[i].id);
		assert(iter != m_bonePtr.end());
		//std::string boneStr = bones[iter->second]->name;
		//std::string boneStr = getBoneName(bones[iter->second]->nameid);
		animBones[i].id = iter->second;
	}
}

int cGrannyFile::getBoneID(const std::string &name)
{
	if (!m_initialized) return -1;

	std::vector<Bone *> &bones = getBones().bones;

	//if (isMasterName(name.c_str())) 
	//	return m_boneID[MASTER];

	dword nameid = getBoneNameID(name);

	for (int i = 0; i < bones.size(); i++) 
	{
		Bone* bone = bones[i];
		//if (_stricmp(bone->name.c_str(), name) == 0)
		if (bone->nameid == nameid)
			return bone->id;
	}

	return -1;
}


void cGrannyFile::load( std::string filename, std::string basepath )
{
	if (_stricmp(right(filename, 4).c_str(),".smd")==0)
	{
		if (!import(filename)) return;
		m_initialized = true;
		m_pathname = filename;
		m_filename = ::getFileName(filename);

		m_imported = true;

		makeVertexArray();

		print();

		return;
	}

	std::fstream * file = new fstream( filename.c_str(), ios::in|ios::binary );	

	if (!file->is_open())
	{
		delete file;
		pDebug.Log("Error: File not found: %s", filename.c_str());
		return;
	}

	m_stream = new cGrannyStream(file, filename);
	file->close();
	delete file;
	
	glPushMatrix();
	glLoadIdentity();

	m_stream->seekg( 0x40 );	// skip header (Could be FileType magic)

	dword chunk=m_stream->readDword();
	switch (chunk)
	{
	case 0xCA5E0000:
		mainChunk();
		break;
	default:
		{
			pDebug.Log(LEVEL_ERROR, "Unknown main Chunk: 0x%x", chunk);
			//assert(!"Unknown main Chunk:");
			return;
		}
	}

	loadTexture( getTextureName() );

	std::vector<Bone *> &bones = getBones().bones;
	if( bones.size() > 0 )
	{
		calculateBoneRests( bones[0] );
	}

	delete m_stream;
	m_stream = NULL;
	m_initialized = true;
	m_pathname = filename;
	m_filename = ::getFileName(filename);

	glPopMatrix();

	printRaw();

	initBone();
	makeVertexArray();

	freeChunk(); // free 5KB ?
}

bool cGrannyFile::import(std::string filename)
{
	pDebug.Log("importing file... : %s", filename.c_str());

	FILE* fp = fopen(filename.c_str(), "rt");
	if (!fp) return false;

	char buf[1024];

	std::vector<Bone *> &bones = getBones().bones;
	//BoneTies &boneTies = getTies();
	Textures &textures = getTextures();
	Animations &animations = getAnimations();

	std::list<Mesh> &meshes = getMeshes().meshes;
	meshes.push_front(Mesh());
	Mesh &mesh = *meshes.begin();
	
	std::string texture;
	std::string s;
	std::vector<std::string> tokens;
	int numFrames = 0;
	int boneCount = 0;
	int currentTimePoint = 0;

	enum { DEFAULT, NODES, SKELETON, SKELETONATIME, TRIANGLES } state = DEFAULT;

	while (fgets(buf, sizeof(buf), fp))
	{
		if (splitString(tokens, buf) == 0) continue;

		std::vector<std::string>::iterator tok = tokens.begin();
		while (tok != tokens.end())
		{
			//pDebug.Log(LEVEL_INFO1,"%s", tok->c_str());

			switch (state)
			{
			case DEFAULT:
				if (*tok == "version")
				{
					tok++;
					if (atoi(tok->c_str()) != 1)
						pDebug.Log(LEVEL_ERROR, "Warning: Unknown SMD version : %s", tok);
					else
						pDebug.Log(LEVEL_INFO1,"version %s", tok->c_str());
				}
				else if (*tok == "nodes")
				{
					state = NODES;
					//pDebug.Log(LEVEL_INFO1,"Loding skeleton...");
					pDebug.Log(LEVEL_INFO1,"nodes");
				}
				else if (*tok == "skeleton")
				{
					state = SKELETON;
					//pDebug.Log(LEVEL_INFO1,"Loading frmaes...");
					pDebug.Log(LEVEL_INFO1,"skeleton");
				}
				else if (*tok == "triangles")
				{
					state = TRIANGLES;
					//pDebug.Log(LEVEL_INFO1,"Loading skinning mesh...");
					pDebug.Log(LEVEL_INFO1,"triangles");
				}
				break;
			case NODES:
				if (*tok != "end")
				{
					// format: <index> "<name>" <parentIndex>

					int index  = atoi(tok->c_str()); tok++;
					std::string name = *tok; tok++;
					int parent = atoi(tok->c_str());
					pDebug.Log(LEVEL_INFO1,"%2d \"%s\" %2d", index, name.c_str(), parent);
					boneCount++;

					Bone *bone = new Bone();
					bone->id = index;
					//bone->name = name;
					bone->nameid = getBoneNameID(name);
					bone->parent = parent;
					
					bones.push_back(bone);

					break;
				}
				else
				{
					// If bones already in the scene, make sure they
					// match up with the bones being imported
					state = DEFAULT;
					pDebug.Log(LEVEL_INFO1,"end");

					for (int i = 0; i < bones.size(); i++)
					{
						Bone *b = bones[i];
						if (b->parent != -1)
						{
							assert(0 <= b->parent && b->parent < bones.size());
							bones[b->parent]->children.push_back(b);
						}
					}

					break;
				}
			case SKELETON:
				if (*tok == "time")
				{
					// Enter time block
					numFrames++; ++tok;
					currentTimePoint = atoi(tok->c_str());
					state = SKELETONATIME;
					pDebug.Log(LEVEL_INFO1,"time %d", currentTimePoint);
				}
				break;
			case SKELETONATIME:
				if (*tok == "time")
				{
					// Start a new time block
					numFrames++; ++tok;
					currentTimePoint = atoi(tok->c_str());
					pDebug.Log(LEVEL_INFO1,"time %d", currentTimePoint);
				}
				else if (*tok == "end")
				{
					// End of skeleton block
					state = DEFAULT;
					pDebug.Log(LEVEL_INFO1,"end");
				}
				else
				{
					// Format: <id> <xpos> <ypos> <zpos> <xang> <yang> <zang>
					// If not root, then everything is in parent space
					// All angles in radians
							
					int boneid = atoi(tok->c_str()); tok++;
					float xpos = atof(tok->c_str()); tok++;
					float ypos = atof(tok->c_str()); tok++;
					float zpos = atof(tok->c_str()); tok++;
					float xang = atof(tok->c_str()); tok++;
					float yang = atof(tok->c_str()); tok++;
					float zang = atof(tok->c_str());

					pDebug.Log(LEVEL_INFO1," %d %.6f %.6f %.6f %.6f %.6f %.6f", 
						boneid, xpos, ypos, zpos, xang, yang, zang);

					assert(0 <= boneid && boneid < bones.size());

					Point t(xpos/40, ypos/40, zpos/40);
					Point q = EulerToQuaternion(Point(xang,yang,zang));

					if (numFrames == 1)
					{
						Bone  *b = bones[boneid];
						b->translate  = t;
						b->quaternion = q;

						BoneAnim boneAnim;
						boneAnim.id = boneid;
						animations.bones.push_back(boneAnim);
					}

					assert(animations.bones.size() > boneid);
					float time = (numFrames-1) * 0.1f;
					BoneAnim &boneAnim = animations.bones[boneid];
					assert(boneAnim.id == boneid);
					
					boneAnim.translates.push_back(t);
					boneAnim.quaternions.push_back(q);
					
					boneAnim.translateTimeline.push_back(time);
					boneAnim.quaternionTimeline.push_back(time);
					
					boneAnim.length = time;
				}
				break;
			case TRIANGLES:
				if (*tok != "end")
				{
					// "<textureFilename>"
					texture = *tok; tok++;
					pDebug.Log(LEVEL_INFO1,"%s", texture.c_str());

					gPolygon poly, tpoly;

					// <bone> <xpos> <ypos> <zpos> <nx> <ny> <nz> <s> <t> * 3
					for (int i = 0; i < 3; i++)
					{
						fgets(buf, sizeof(buf), fp);
						splitString(tokens, buf);
						tok = tokens.begin();
						int boneid = atoi(tok->c_str()); tok++;

						float xpos = atof(tok->c_str()) / 40; tok++;
						float ypos = atof(tok->c_str()) / 40; tok++;
						float zpos = atof(tok->c_str()) / 40; tok++;
						float nx   = atof(tok->c_str()); tok++;
						float ny   = atof(tok->c_str()); tok++;
						float nz   = atof(tok->c_str()); tok++;
						// Textcoord
						float s    = atof(tok->c_str()); tok++;
						float t    = atof(tok->c_str());

						pDebug.Log(LEVEL_INFO1,"%2d %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f", 
							boneid, xpos, ypos, zpos,
							nx, ny, nz, s, t);

						mesh.minX = min(mesh.minX, xpos);
						mesh.maxX = max(mesh.maxX, xpos);

						mesh.minY = min(mesh.minY, ypos);
						mesh.maxY = max(mesh.maxY, ypos);

						mesh.minZ = min(mesh.minZ, zpos);
						mesh.maxZ = max(mesh.maxZ, zpos);

						BoneWeight weight;
						weight.addWeight(boneid, 1.0f);

						mesh.points.push_back(Point(xpos,ypos,zpos));
						mesh.normals.push_back(Point(nx,ny,nz));
						mesh.weights.push_back(weight);

						mesh.textureMap.push_back(Point(s,-t,0.0f));

						poly.nodes[i] = mesh.points.size() - 1;
						tpoly.nodes[i+1] = poly.nodes[i];
					}

					mesh.polygons.push_back(poly);
					textures.polygons.push_back(tpoly);
					
					break;
				}
				else
				{
					// Build an intial editable mesh from triangles
					state = DEFAULT;
					pDebug.Log(LEVEL_INFO1,"end");

					//meshes.push_back(mesh);
					break;
				}
				break;
			}

			if (tok != tokens.end()) tok++;
		}
	}

	fclose(fp); fp = NULL;

	//BoneTie tie;
	for (int i = 0; i < bones.size(); i++)
	{
		Bone *bone = bones[i];

		assert(bone->id == i);

		// 간편하게 bones 와 bonetie 가 1:1 매칭이 되게 한다.
		//BoneTie *tie = new BoneTie;
		//tie->bone = i;
		//boneTies.boneties.push_back(tie);
		m_boneties.push_back(i);

		// 마찬가지
//		m_bonePtr.insert(std::make_pair(i, i));
		
		//const char *s = bone->name.c_str();
#if 1
		const char *s = getBoneName(bone->nameid).c_str();
		if (isMasterName(s) && m_boneID[MASTER] == -1)
			m_boneID[MASTER] = bone->id;
		else if (bone->nameid == BONE_NAMEID[LHAND])
			m_boneID[LHAND]  = bone->id;
		else if (bone->nameid == BONE_NAMEID[RHAND])
			m_boneID[RHAND]  = bone->id;
		else if (bone->nameid == BONE_NAMEID[LGRASP])
			m_boneID[LGRASP] = bone->id;
		else if (bone->nameid == BONE_NAMEID[RGRASP])
			m_boneID[RGRASP] = bone->id;
#else
		const char *s = getBoneName(bone->nameid).c_str();
		if (isMasterName(s) && m_boneID[MASTER] == -1)
			m_boneID[MASTER] = bone->id;
		else if (_stricmp(s, "Bip01 L Hand") == 0)
			m_boneID[LHAND]  = bone->id;
		else if (_stricmp(s, "Bip01 R Hand") == 0)
			m_boneID[RHAND]  = bone->id;
		else if (_stricmp(s, "CP_Grasp_Lhand") == 0)
			m_boneID[LGRASP] = bone->id;
		else if (_stricmp(s, "CP_Grasp_Rhand") == 0)
			m_boneID[RGRASP] = bone->id;
#endif
	}

	if (bones.size() > 0)
		calculateBoneRests( bones[0] );

	if (!texture.empty()) 
		loadTexture(texture);

	return true;

}

void cGrannyFile::freeChunk()
{
	m_freechunk = true;
	m_object.freeUnused();
}

void cGrannyFile::mainChunk()
{
	ItemList final, copyright;

	dword children = m_stream->readDword();

	for( int i = 0; i < 6; ++i )
		m_stream->readDword(); // CRC?

	for( dword child = 0; child < children; ++child )
	{
		// Chunk Header
		dword chunk = m_stream->readDword();

		switch( chunk )
		{
			// Final Chunk (End-of-File?)
		case 0xCA5E0101:
			pDebug.Log(LEVEL_INFO1, "[loading final chunk...]");
			final.load( m_stream );
			break;

			// Copyright Chunk
		case 0xCA5E0102:
			pDebug.Log(LEVEL_INFO1, "[loading copyright chunk...]");
			copyright.load( m_stream );
			break;

			// Object Chunk
		case 0xCA5E0103:
			pDebug.Log(LEVEL_INFO1, "[loading object chunk...]");
			m_object.load( m_stream );
			break;

			//SiENcE:  Chunk is new to ML/SE Models maybe Texture: case 0xca5e0100

		default:
			{
				pDebug.Log(LEVEL_ERROR,"Unknown GRN Chunk: 0x%x", chunk);
				//assert(!"Unknown GRN Chunk");
				return;
			}
		}
	}
}

bool cGrannyFile::loadTexture( const std::string &texfilename)
{
	if (m_texture && pGrannyTextureLoader)
		pGrannyTextureLoader->FreeTexture(m_texfilename);

	m_texture = NULL;
	m_texfilename = texfilename;

	if (pGrannyTextureLoader && !m_texfilename.empty()) 
	{
		//pDebug.Log("texture load: %s\n", m_texfilename.c_str());
		m_texture = pGrannyTextureLoader->LoadTexture(m_texfilename);
	}

	return m_texture != NULL;
}

void cGrannyFile::registVertexArray()
{
	if (!initialized()) return;


}

void cGrannyFile::makeVertexArray()
{
	if (!initialized()) return;

	Meshes &meshes = getMeshes();
	std::list<Mesh>::iterator imesh = meshes.meshes.begin();

	if (imesh == meshes.meshes.end()) return;

	std::vector<Point> &points = imesh->points;
	std::vector<gPolygon>& polygons = imesh->polygons;
//	std::vector<Weights
		
	Textures &textures = getTextures();

	//float *vertex = NULL;
	dword poly = 0;

	/* 
	lizards_alligator_brown_lod2.grn
		vertexarray poly:258, index:774, texcoord:1548, normal:2322
		mesh[0] points:131, normals: 131, polygons:258, weights:131, texturemap:388
		0 deformed size:131
	lizards_alligator_brown_lod2.smd
		vertexarray poly:258, index:774, texcoord:1548, normal:2322
		mesh[0] points:774, normals: 774, polygons:258, weights:774, texturemap:774
		0 deformed size:774
	*/

	dword polySize = polygons.size();
	dword verSize = polySize * 3;//points.size();
	
	m_verColor.resize(verSize*3, 1.0f);
	m_verNormal.resize(verSize*3);
	m_verTexcoord.resize(verSize*2);
	m_verIndex.resize(polySize*3);

	//Mesh newMesh;
	//newMesh.points.resize(polySize*3);
	//newMesh.weights.resize(polySize*3);

	dword index = 0;
	//polycount = 0;
	for ( int poly = 0; poly < polySize; poly++)
	{
		for ( int i = 0; i < 3; ++i )
		{
			index = poly*3+i;
			//index = polygons[poly].nodes[i];
			//m_verIndex[poly*3+i] = index;
			m_verIndex[index] = index;
			//newMesh.points[poly]  = points[index];
			//newMesh.weights[poly] = 
#if 1
			if (imesh->normals.size() > 0 && 
				imesh->normals.size() > polygons[poly].nodes[i])
			{
				//assert(imesh->normals.size() > polygon->nodes[i]); // crash id:834
				Point &p = imesh->normals[ polygons[poly].nodes[ i ] ];
				m_verNormal[index*3+0] = p.points[0];
				m_verNormal[index*3+1] = p.points[1];
				m_verNormal[index*3+2] = p.points[2];
				//glNormal3fv( p.points);
			}
			else
			{
				//assert(0 && "normal error");
				m_verNormal[index*3+0] = 1.0f;
				m_verNormal[index*3+1] = 1.0f;
				m_verNormal[index*3+2] = 1.0f;
			}

#endif	
			// Do we have texture-map information?
			if (imesh->textureMap.size() > 0 &&
				textures.polygons.size() > poly &&
				imesh->textureMap.size() > textures.polygons[ poly ].nodes[ i + 1 ])
			{
				//assert(textures.polygons[ poly ].nodes.size() > i + 1);	
				Point &p = imesh->textureMap[ textures.polygons[ poly ].nodes[ i + 1 ] ];

				m_verTexcoord[index*2+0] = p.points[0];
				m_verTexcoord[index*2+1] = p.points[1];
				//m_verTexcoord.push_back(p.points[0]);
				//m_verTexcoord.push_back(p.points[1]);
				//glTexCoord2fv(p.points);				
			}
			else
			{
				m_verTexcoord[index*2+0] = 0.0f;
				m_verTexcoord[index*2+1] = 0.0f;
	
				//assert(0 && "texcoord error");
				//m_verTexcoord.push_back(1.0f);
				//m_verTexcoord.push_back(1.0f);
			}
		

			
			//m_verIndex.push_back(polygon->nodes[i]);
			//vertex = data + (polygon->nodes[i] * 3);
			//glVertex3fv(vertex);
		}
		//m_verIndex.push_back(polygon->nodes[0]);	
	}
	pDebug.Log(LEVEL_INFO2, "vertexarray poly:%d, index:%d, texcoord:%d, normal:%d", 
		polySize, m_verIndex.size(), m_verTexcoord.size(), m_verNormal.size());
}

cDeformedArray * cGrannyFile::createDeformed (cGrannyFile * animation, float time, list<Mesh>::iterator imesh)
{
	cDeformedArray * deformed = NULL;

	Meshes &meshes = getMeshes();
	Bones &bones = getBones();
	Textures &textures = getTextures();
	// boneid = boneTies[boneWeight.bones[i]].bone 
	//  - boneWeight.bones[i] 에 저장된 색인과 연결된 boneid
	//  - bone 은 30 개이지만, boneWeight 와 연결된 본은 20개 인경우, bonetie 갯수는 20개
	//BoneTies &boneTies = getTies();

	assert(bones.bones.size()>0);

	glPushMatrix();
	glLoadIdentity();
	if( animation )
		animation->getSkeleton( bones.bones[0], time ); 
	else
		calculateBoneRests(bones.bones[0]);
	glPopMatrix();

	dword pnt = 0;
	vector<BoneWeight>::iterator iwt;

	// imesh->weights.size() == imesh->points.size()
	if (imesh->weights.size() > 0) 
	{
		deformed = new cDeformedArray (imesh->weights.size());
		float * data = deformed->data();

		for( iwt = imesh->weights.begin(); iwt != imesh->weights.end(); ++iwt, ++pnt )
		{
			for(unsigned int wt = 0; wt < iwt->numWeights; ++wt )
			{
				Point post;
				float weight=iwt->weights[wt];

				assert(10 > wt);
				//assert(boneTies.boneties.size() > iwt->bones[wt]);
				assert(m_boneties.size() > iwt->bones[wt]);

				//dword bone=boneTies.boneties[iwt->bones[wt]]->bone;
				dword bone=m_boneties[iwt->bones[wt]];

				assert(bones.bones.size() > bone);
				assert(imesh->points.size() > pnt);

				post=bones.bones[bone]->curMatrix*imesh->points[pnt];
				data[0]+=(post.points[0]*weight);
				data[1]+=(post.points[1]*weight);
				data[2]+=(post.points[2]*weight); 
			}

			data += 3;
		}
	} 
	else if (imesh->points.size() > 0) 
	{
		deformed = new cDeformedArray (imesh->points.size());
		float * data = deformed->data();

		for (unsigned int i = 0; i < imesh->points.size(); i++) 
		{
#if 0
			Point post;
			float weight=1.0f;
			post=bones.bones[1]->curMatrix*imesh->points[i];
			//post=imesh->points[i];
			*data = post.points[0]*weight; data++;
			*data = post.points[1]*weight; data++;
			*data = post.points[2]*weight; data++;
#else
			data[0] = imesh->points[i].points[0];
			data[1] = imesh->points[i].points[1];
			data[2] = imesh->points[i].points[2];
			data += 3;
#endif

		}
	}

#if 1
	if (!m_imported)
	{
		std::vector<gPolygon>& polygons = imesh->polygons;
		dword polySize = polygons.size();

		dword index = 0;
		cDeformedArray *newDeformed = new cDeformedArray(polySize*3);
		float *data1 = newDeformed->data(), *data2 = deformed->data();
		for ( int poly = 0; poly < polySize; poly++)
		{
			for ( int i = 0; i < 3; ++i )
			{
				index = polygons[poly].nodes[i];
				memcpy(data1+poly*9+i*3, data2+index*3, sizeof(float)*3);
			}
		}

		delete deformed;
		deformed = newDeformed;
	}
#endif
	
	if (animation) 
	{
		deformed->matrix_left_hand  = animation->matrix_left_hand;
		deformed->matrix_right_hand = animation->matrix_right_hand;
	}

	return deformed;
}

int cGrannyFile::getFrame (cGrannyFile * animation, float & curTime)
{
	if (animation) 
	{
		if (curTime >= animation->length())
			curTime = 0.0f;
		if (animation->length() > 0.0f)
			return (int) (curTime / animation->length() * 25.0f);
	}
	return 0;
}

void cGrannyFile::Render (cGrannyFile * animation, float &curTime,
						  cCharacterLight * character_light, float r, float g,
						  float b, float alpha, bool is_corpse)
{
	if (!initialized())
		return;

	cDeformedArray * deformed = NULL;

	int frame = is_corpse ? 24 : getFrame (animation, curTime);
	float time = 0.0f;

	if (animation) 
	{
		time = ((float) frame) / 25.0f * animation->length();
		deformed = animation->getDeformed(frame);
	}

	Meshes &meshes = getMeshes();
	Bones &bones = getBones();
	Textures &textures = getTextures();
	list<Mesh>::iterator imesh;

	//if (meshes.meshes.size() != 1) {
	//	//pDebug.Log(LEVEL_ERROR, "Warning: More than one Granny Mesh is not supported!\n");
	//	return;
	//}

	imesh = meshes.meshes.begin();

	if (!deformed) 
	{
		deformed = createDeformed(animation, time, imesh);
		if (animation)
			animation->addDeformed(deformed, frame);

		//pDebug.Log("%d deformed size:%d", frame, deformed->size());
	} 
	else 
	{										   
		if (animation) 
		{
			animation->matrix_left_hand  = deformed->matrix_left_hand;
			animation->matrix_right_hand = deformed->matrix_right_hand;
		}
	}

	if (nConfig::drawBoundBox || nConfig::drawBone)
	{
		GLboolean useTexture = GL_FALSE;
		glGetBooleanv(GL_TEXTURE_2D, &useTexture);

		if (useTexture) glDisable(GL_TEXTURE_2D);

		if (nConfig::drawBoundBox && imesh != meshes.meshes.end())
    		drawBoundBox(*imesh);

		if (nConfig::drawBone)
		{
			glColor3f(1.0f,1.0f,0);
			glPushMatrix();

			if (animation) 
				animation->drawSkeleton(bones.bones[0],time);
			else 
				drawSkeleton(getBones().bones[0],0);
			
			glPopMatrix();
			glColor3f(1.0f,1.0f,1.0f);
		}

		if (useTexture) glEnable(GL_TEXTURE_2D);
	}

	// we would need to recalculate normals here.. screw that dude.
	// I doubt OSI does (although since I have yet to actually run UO:TD,
	// I don't know for sure).

	if (deformed) 
	{
		float * data = deformed->data();

		glBindTexture( GL_TEXTURE_2D, getTexture() );

		if (nConfig::useVertexArray)
		{
			glVertexPointer(3, GL_FLOAT, 0, data);
			glTexCoordPointer(2, GL_FLOAT, 0, &m_verTexcoord[0]);
			glNormalPointer(GL_FLOAT, 0, &m_verNormal[0]);
			glColorPointer(3, GL_FLOAT, 0, &m_verColor[0]);

			//assert(m_verIndex.size() % 3 == 0);
			glDrawElements(GL_TRIANGLES, m_verIndex.size(), GL_UNSIGNED_INT, &m_verIndex[0]);

			polycount = deformed->size();
		}
		else
		{
			glBegin( GL_TRIANGLES );

			glColor3fv(&m_verColor[0]);

			for (int i = 0; i < m_verIndex.size(); i++)
			{
				glTexCoord2fv(&m_verTexcoord[i*2]);
				glVertex3fv(data + m_verIndex[i] * 3);
			}

			glEnd();
		}

		glBindTexture( GL_TEXTURE_2D, 0 );

		if (!animation)
			delete deformed;
	}
}

void cGrannyFile::getSkeleton( Bone *bone, float & curTime )
{
}

void cGrannyFile::SetColor(float r, float g, float b)
{
	for (int i = 0; i < m_verColor.size() / 3; i++)
	{
		m_verColor[i*3+0] = r;
		m_verColor[i*3+1] = g;
		m_verColor[i*3+2] = b;
	}
}

//--------------------------------------------------------------------------------------------
// cGrannyAnimation
//--------------------------------------------------------------------------------------------

cGrannyAnimation::cGrannyAnimation()
{
//	cGrannyFile::cGrannyFile();
//	m_animBones = NULL;
	m_length = 0.0f;
	m_assignModel = NULL;
}

cGrannyAnimation::~cGrannyAnimation()
{
	std::map <int, cDeformedArray *>::iterator iter;
	for (iter = m_cache.begin(); iter != m_cache.end(); iter++)
		delete iter->second;
	m_cache.clear();

	//delete [] m_animBones;
}


// connect animation.bone to model.bone by bone.name
void cGrannyAnimation::Assign (cGrannyFile * model)
{
	if (!initialized() || !model || !model->initialized())
		return;

	m_assignModel = model;

	m_boneID[LHAND]  = model->getBoneID(LHAND);
	m_boneID[RHAND]  = model->getBoneID(RHAND);
	m_boneID[LGRASP] = model->getBoneID(LGRASP);
	m_boneID[RGRASP] = model->getBoneID(RGRASP);

	//BoneTies &boneTies = getTies();
	//BoneTies &boneLodTies = model->getTies();
	Animations &anims = getAnimations();
	vector<BoneAnim> &animBones = anims.bones;
	vector<Bone*> &bones = getBones().bones;

	//dword anmNm = findString("__ObjectName");
	//dword lodNm = model->findString("__ObjectName");

	m_animBones.assign(model->getBones().bones.size(), (dword)-1);

	int boneId = 0, aboneId = 0;
	// for each animation bone
	for (int i = 0; i < animBones.size(); i++)
	{
		aboneId = animBones[i].id;	

		std::string boneStr = getBoneName(bones[aboneId]->nameid);
		if (aboneId == m_boneID[MASTER])
			boneId = model->getBoneID(MASTER);
		else
			boneId = model->getBoneID(boneStr);

		// model 의 bone.id 와 연결된 BoneAnim 색인을 저장
		if (boneId != -1)
			m_animBones[ boneId ] = i;

		pDebug.Log(LEVEL_INFO2, "Assign BoneAnim[%2d] to Model Bone :%2d, \"%s\"", i, boneId, boneStr.c_str());
	}

	for (int i = 0; i < m_animBones.size(); i++)
		pDebug.Log(LEVEL_INFO2, "Model Bone ID %2d : BoneAnim[%2d]", i, m_animBones[i]);

	m_length = m_object.getAnimLength();
}

void cGrannyAnimation::getSkeleton( Bone *bone, float & curTime )
{
	float X,Y,Z;
	dword rid=0,mid=0;
	GrnMatrix matrix;

	//if (!m_animBones || !bone)
	if (m_animBones.empty() || !bone)
		return;

	if( (m_animBones[ bone->id ] == (dword)-1))
	{
		//pDebug.Log("can't found id %d anim bone", bone->id);
		return;
	}
	else
	{
		BoneAnim &boneAnim = getBoneAnim( m_animBones[ bone->id ] );
		vector<float>::iterator movi, roti;

		//pDebug.Log("getSkeleton bone id : %d, boneAnim id : %d", bone->id, boneAnim.id);

		// Problem here, i suppose that some animations are looped back/forward
		if( curTime > boneAnim.length )
		{
			curTime = 0.0f;
		}

		for( roti = boneAnim.quaternionTimeline.begin();
			roti != boneAnim.quaternionTimeline.end() && (*roti) < curTime;
			roti++, rid++ );

		for( movi = boneAnim.translateTimeline.begin();
			movi != boneAnim.translateTimeline.end() && (*movi)  < curTime;
			movi++, mid++ );

		Point t = boneAnim.translates[ mid ],q=boneAnim.quaternions[ rid ];

		X = t.points[ 0 ];
		Y = t.points[ 1 ];
		Z = t.points[ 2 ];

		// Interpolate when between Keyframes
		if( curTime != (*movi) )
		{
			float F1 = *movi;
			movi++;
			if (movi!=boneAnim.translateTimeline.end())
			{
				float F2 = *movi;
				float x2 = boneAnim.translates[mid+1].points[0];
				float y2 = boneAnim.translates[mid+1].points[1];
				float z2 = boneAnim.translates[mid+1].points[2];
				float tm = ( curTime - F1 ) / ( F2 - F1 );
				t.points[0] += tm * ( x2 - X );
				t.points[1] += tm * ( y2 - Y );
				t.points[2] += tm * ( z2 - Z );
			}
		}

		matrix.setTransform(q, t);

		bone->curQuaternion = q;
		bone->curTranslate = t;

		glMultMatrixf( matrix.matrix );
		glGetFloatv( GL_MODELVIEW_MATRIX, matrix.matrix );
		bone->curMatrix = matrix;
		bone->curMatrix *= bone->matrix;

		updateHandMatrix(bone, matrix);
	}

	vector<Bone *>::iterator ibone;
	for( ibone = bone->children.begin(); ibone != bone->children.end(); ++ibone )
	{
		glPushMatrix();
		getSkeleton( *ibone, curTime);
		glPopMatrix();
	}
}

//--------------------------------------------------------------------------------------------
// cDeformedArray
//--------------------------------------------------------------------------------------------

cDeformedArray * cGrannyAnimation::getDeformed (int index)
{
	std::map <int, cDeformedArray *>::iterator iter = m_cache.find(index);
	if (iter != m_cache.end()) 
		return iter->second;
	return NULL;
}

void cGrannyAnimation::addDeformed (cDeformedArray * deformed, int index)
{
	std::map <int, cDeformedArray *>::iterator iter = m_cache.find(index);
	if (iter != m_cache.end()) 
	{
		delete iter->second;
		m_cache.erase(index);
		//pDebug.Log("cGrannyAnimation::addDeformed - id:%d mist!", index);
	}

	m_cache.insert(make_pair(index, deformed));
}

cDeformedArray::cDeformedArray(int size) : m_size(size)
{
	assert(size > 0);
	m_data = new float[size * 3];
	memset(m_data, 0, sizeof(float)*size*3);
}


cDeformedArray::~cDeformedArray()
{
	delete [] m_data;
}


float * cDeformedArray::data()
{
	return m_data;
}



