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

/*
* only need for grannyviwer
*/

#include "stdafx.h"
#include "granny/grntype.h"
#include "granny/grannyfile.h"
#include "granny/GrannyTextures.h"
#include "Config.h"
#include "Debug.h"
#include <iostream>
#include <cassert>

void printw (GLvoid *font, float x, float y, float z, char* format, ...)
{
	//  Just a pointer to a font style..
	//  Fonts supported by GLUT are: GLUT_BITMAP_8_BY_13, 
	//  GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10, 
	//  GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10,
	//  GLUT_BITMAP_HELVETICA_12, and GLUT_BITMAP_HELVETICA_18.
	GLvoid *font_style = font;

    va_list arg_list;
    char str[256];
	int i;
    
    va_start(arg_list, format);
    vsprintf(str, format, arg_list);
    va_end(arg_list);
    
    glRasterPos3f (x, y, z);

    for (i = 0; str[i] != '\0'; i++)
        glutBitmapCharacter(font_style, str[i]);
}


void printw (float x, float y, float z, char* format, ...)
{
	//  Just a pointer to a font style..
	//  Fonts supported by GLUT are: GLUT_BITMAP_8_BY_13, 
	//  GLUT_BITMAP_9_BY_15, GLUT_BITMAP_TIMES_ROMAN_10, 
	//  GLUT_BITMAP_TIMES_ROMAN_24, GLUT_BITMAP_HELVETICA_10,
	//  GLUT_BITMAP_HELVETICA_12, and GLUT_BITMAP_HELVETICA_18.
	GLvoid *font_style = GLUT_BITMAP_HELVETICA_10;

    va_list arg_list;
    char str[256];
	int i;
    
    va_start(arg_list, format);
    vsprintf(str, format, arg_list);
    va_end(arg_list);
    
    glRasterPos3f (x, y, z);

    for (i = 0; str[i] != '\0'; i++)
        glutBitmapCharacter(font_style, str[i]);
}

//--------------------------------------------------------------------------------------------
// cGrannyFile
//--------------------------------------------------------------------------------------------

bool cGrannyFile::isImportBone( int boneid)
{
	if (boneid < 0) return false;

	int *end = m_boneID+NUM_BONEID;
	return std::find(m_boneID, end, boneid) != end;
}

void cGrannyFile::print()
{
	std::vector<Bone *> &bones = getBones().bones;
	std::list<Mesh> &meshes = getMeshes().meshes;
	Animations &anims = getAnimations();
	std::vector<BoneAnim> &boneAnims = anims.bones;

	pDebug.Log(LEVEL_INFO3, "granny file begin---------------------------------------------------");
	pDebug.Log(LEVEL_INFO3, "granny file loading...: %s", m_filename.c_str());
	pDebug.Log(LEVEL_INFO3, "texture file: %s", m_texfilename.c_str());
	pDebug.Log(LEVEL_INFO3, "mesh count: %d", meshes.size());
	pDebug.Log(LEVEL_INFO3, "bone count: %d", bones.size());
	pDebug.Log(LEVEL_INFO3, "bone ties count: %d",  m_boneties.size());
	pDebug.Log(LEVEL_INFO3, "bone anim count: %d length: %f", anims.size(), anims.length());

	if (nConfig::logBoneAnim) 
	{
		pDebug.Log(LEVEL_INFO2, "boneAnim   ---------------------------------------------------");
		for (int i = 0; i < boneAnims.size(); i++)
		{
			dword id = boneAnims[i].id;
			Bone* bone = bones[id];
			pDebug.Log(LEVEL_INFO2,
				"BoneAnim[%2d] : Bone[%2d] T: %2d Q: %2d U:%2d Len:%.3f \"%s\"", 
				i, id, 
				boneAnims[i].translates.size(),   // boneAnims[i].translateTimeline.size(), 
				boneAnims[i].quaternions.size(),  // boneAnims[i].quaternionTimeline.size(), 
				boneAnims[i].numUnknowns, boneAnims[i].length,
				getBoneName(bone->nameid).c_str());
		}
	}

	if (nConfig::logBone) 
	{
		pDebug.Log(LEVEL_INFO2, "Bone         ---------------------------------------------------");
		for (int i = 0; i < bones.size(); i++) 
		{
			Bone* bone = bones[i];
			Point& p = bone->translate, q=bone->quaternion;

			pDebug.Log(LEVEL_INFO2,"Bone[%2d] ID:%2d P:%2d "
				"T(%+.3f, %+.3f, %+.3f) Q(%+.3f, %+.3f, %+.3f, %+.3f) \"%s\"", 
				i, bone->id, bone->parent, 
				p.points[0],p.points[1],p.points[2],
				q.points[0],q.points[1],q.points[2], q.points[3],
				//bone->name.c_str());
				getBoneName(bone->nameid).c_str());
		}
	}
	if (nConfig::logMesh) 
	{
		pDebug.Log(LEVEL_INFO2,"Mesh      ---------------------------------------------------");
		int i = 0;
		for (std::list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); it++, i++) 
		{
			Mesh& m = *it;
			pDebug.Log(LEVEL_INFO2,"mesh[%d] points:%d, normals: %d, polygons:%d, weights:%d, texturemap:%d", 
				i, m.points.size(), m.normals.size(), m.polygons.size(), m.weights.size(), m.textureMap.size());
			pDebug.Log(LEVEL_INFO2,"        bound: x(%+.3f~%+.3f) y(%+.3f~%+.3f) z(%+.3f~%+.3f)",
				m.minX, m.maxX, m.minY, m.maxY, m.minZ, m.maxZ);
		}
	}
	pDebug.Log(LEVEL_INFO3, "granny file end  ---------------------------------------------------");

}

void cGrannyFile::printRaw()
{
	if (!m_initialized) return;

	//-------------------------------
	// print granny file info
	//------------------------------

	std::vector<Bone *> &bones = getBones().bones;
	std::list<Mesh> &meshes = getMeshes().meshes;
	std::vector<BoneTie *> &boneties = getTies().boneties;
	std::vector<dword> &boneObjects = getTies().boneObjects;
	std::vector<dword> &boneObjPtrs = getTies().boneObjPtrs;
	TextChunk &tchunk = getTextChunk();
	std::vector<Object*> &objects = getObjects().objects;
	
	Animations &anims = getAnimations();
	std::vector<BoneAnim> &boneAnims = anims.bones;

	pDebug.Log(LEVEL_INFO3, "granny file begin---------------------------------------------------");
	pDebug.Log(LEVEL_INFO3, "granny file loading...: %s", m_filename.c_str());
	pDebug.Log(LEVEL_INFO3, "texture file: %s", m_texfilename.c_str());
	pDebug.Log(LEVEL_INFO3, "mesh count: %d", meshes.size());
	pDebug.Log(LEVEL_INFO3, "bone count: %d", bones.size());
	pDebug.Log(LEVEL_INFO3, "bone ties count: %d",  boneties.size());
	pDebug.Log(LEVEL_INFO3, "bone object count: %d", boneObjects.size());
	pDebug.Log(LEVEL_INFO3, "bone object ptr count: %d", boneObjPtrs.size());
	pDebug.Log(LEVEL_INFO3, "bone anim count: %d length: %f", anims.size(), anims.length());


	if (nConfig::logMesh) 
	{
		pDebug.Log(LEVEL_INFO2,"Mesh      ---------------------------------------------------");
		int i = 0;
		for (std::list<Mesh>::iterator it = meshes.begin(); it != meshes.end(); it++, i++) 
		{
			Mesh& m = *it;
			pDebug.Log(LEVEL_INFO2,"mesh[%d] points:%d, normals: %d, polygons:%d, weights:%d, texturemap:%d", 
				i, m.points.size(), m.normals.size(), m.polygons.size(), m.weights.size(), m.textureMap.size());
			pDebug.Log(LEVEL_INFO2,"        bound: x(%+.3f~%+.3f) y(%+.3f~%+.3f) z(%+.3f~%+.3f)",
				m.minX, m.maxX, m.minY, m.maxY, m.minZ, m.maxZ);
		}
	}

	if (m_imported) return;

	if (nConfig::logTextChunk)
	{
		pDebug.Log(LEVEL_INFO2,"textChunk    ---------------------------------------------------");
		for (int i=0; i < tchunk.size(); i++)
			pDebug.Log(LEVEL_INFO2, "text[%2d] \"%s\"", i, tchunk.findID(i).c_str());
	}

	dword lodNm = findString("__ObjectName");

	if (nConfig::logObject) 
	{
		pDebug.Log(LEVEL_INFO2,"textKey     ---------------------------------------------------");

		pDebug.Log(LEVEL_INFO2, "__Standard    key: %2d", findString("__Standard"));
		pDebug.Log(LEVEL_INFO2, "__FileName    key: %2d", findString("__FileName"));
		pDebug.Log(LEVEL_INFO2, "__ObjectName  key: %2d", findString("__ObjectName"));
		pDebug.Log(LEVEL_INFO2, "__Description key: %2d", findString("__Description"));
		pDebug.Log(LEVEL_INFO2, "__Root        key: %2d", findString("__Root"));

		pDebug.Log(LEVEL_INFO2,"object     ---------------------------------------------------");

		pDebug.Log(LEVEL_INFO2, "object[id, key] value");
		for (int i = 0; i < objects.size(); i++) 
		{
			Object *o = objects[i];
			for (int k = 0; k < o->numKeys; k++)
				pDebug.Log(LEVEL_INFO2, "object[%2d,%2d] %2d", i, o->keys[k], o->values[k]);
		}
	}

#if 0
	// not used
	pDebug.Log(LEVEL_INFO2, "bontiesBone---------------------------------------------------");
	for (int i = 0; i < boneties.size(); i++)
	{
		BoneTie* tie= boneties[i];
		pDebug.Log(LEVEL_INFO2, "boneties[%2d] bone:%2d", i, tie->bone);
	}
#endif

	if (nConfig::logObjPtr) 
	{
		pDebug.Log(LEVEL_INFO2, "boneObjPtr ---------------------------------------------------");
		for (int i = 0 ; i < boneObjPtrs.size(); i++) {
			int ptr = boneObjPtrs[i]-1;
			//int obj = boneObjects[ptr]-1;
			//int txt = getValue(obj,lodNm);
			pDebug.Log(LEVEL_INFO2, "boneobjptr[%2d] boneoject[%2d]", 
				i, ptr);
		}
	}

	if (nConfig::logBoneObj) 
	{
		pDebug.Log(LEVEL_INFO2, "boneObject ---------------------------------------------------");
		for (int i = 0 ; i < boneObjects.size(); i++) {
			pDebug.Log(LEVEL_INFO2, "boneobject[%2d] object[%2d,%2d]", i, boneObjects[i]-1, lodNm);
		}
	}

	if (nConfig::logBoneAnim) 
	{
		pDebug.Log(LEVEL_INFO2, "boneAnim   ---------------------------------------------------");
		for (int i = 0; i < boneAnims.size(); i++)
		{
			dword id = boneAnims[i].id;
			dword obj = boneObjects[id-1];
			dword textid = getValue(obj,lodNm);
			pDebug.Log(LEVEL_INFO2,
				"BoneAnim[%2d] : ObjPtr:%2d T: %2d Q: %2d U:%2d Len:%.3f \"%s\"", 
				i, id-1, 
				boneAnims[i].translates.size(),   // boneAnims[i].translateTimeline.size(), 
				boneAnims[i].quaternions.size(),  // boneAnims[i].quaternionTimeline.size(), 
				boneAnims[i].numUnknowns, boneAnims[i].length,
				findID(textid).c_str());
		}
	}

	if (nConfig::logBone) 
	{
		pDebug.Log(LEVEL_INFO2, "Bone       ---------------------------------------------------");
		for (int i = 0; i < bones.size(); i++) 
		{
			Bone* bone = bones[i];
			Point& p = bone->translate, q=bone->quaternion;

			// bonetie.boneObjPtr: bone.id 와 boneObj 를 연결
			dword objptr = boneObjPtrs[bone->id];
			// bonetie.boneObjects: bone.id 와 textchunk(bone name) 를 연결
			dword obj = boneObjects[objptr-1];
			// objects.getValue(obj-1,key); objects[obj]->getValue(key);
			dword textid = getValue(obj,lodNm);

			pDebug.Log(LEVEL_INFO2,"Bone[%2d] ID:%2d P:%2d ObjPtr:%2d Obj:%2d "
				"T(%+.3f, %+.3f, %+.3f) Q(%+.3f, %+.3f, %+.3f, %+.3f) \"%s\"", 
				i, bone->id, bone->parent, objptr-1, obj-1,
				p.points[0],p.points[1],p.points[2],
				q.points[0],q.points[1],q.points[2], q.points[3],
				//bone->name.c_str());
				findID(textid).c_str());
		}
	}
	pDebug.Log(LEVEL_INFO3, "granny file end  ---------------------------------------------------");
}

bool cGrannyFile::export(std::string filename)
{
	if (!m_initialized || m_imported) return false;

	pDebug.Log("exporting file... : %s.grn -> %s.smd", 
		::getFileName(m_filename).c_str(), ::getFileName(filename, '\\').c_str());

	FILE* fp = fopen(filename.c_str(), "wt");
	if (!fp) return false;

	std::vector<Bone *> &bones = getBones().bones;
	//BoneTies &boneTies = getTies();
	Textures &textures = getTextures();
	std::list<Mesh> &meshes = getMeshes().meshes;
	std::list<Mesh>::iterator imesh = meshes.begin();

	fprintf(fp, "version 1\n");
	fprintf(fp, "nodes\n");
	for (int i = 0; i < bones.size(); i++) 
	{
		Bone* bone = bones[i];
		//fprintf(fp, "%2d \"%s\" %2d\n", bone->id, bone->name.c_str(), i==0?-1:bone->parent);
		fprintf(fp, "%2d \"%s\" %2d\n", bone->id, getBoneName(bone->nameid).c_str(), i==0?-1:bone->parent);
	}
	fprintf(fp, "end\n");
	fprintf(fp, "skeleton\n");
	fprintf(fp, "time 0\n");

	for (int i = 0; i < bones.size(); i++) 
	{
		Bone* bone = bones[i];
		Point p = QuaternionToEuler(bone->quaternion);
		float *t = bone->translate.points, *q= p.points;

		fprintf(fp, "%2d %.6f %.6f %.6f %.6f %.6f %.6f\n", 
			bone->id,  t[0]*40, t[1]*40, t[2]*40, q[0], q[1], q[2]);
	}

	fprintf(fp, "end\n");
	
	if (imesh != meshes.end()) 
	{
		fprintf(fp, "triangles\n");
		dword poly = 0;
		std::vector<gPolygon>::iterator polygon = imesh->polygons.begin();
		for (; polygon != imesh->polygons.end(); polygon++, poly++) 
		{
			fprintf(fp, "%s\n", m_texfilename.c_str());
			for (int i = 0; i < 3; i++) 
			{
				int node = polygon->nodes[i];
				dword bone = 1;

				if (imesh->weights.size() > 0) 
				{
					assert(imesh->weights.size() > node);
					BoneWeight &weight =  imesh->weights[node];
					int maxBoneIdx = 0;
					float maxBoneWeight = 0.0f;
					for (int w = 0; w < weight.numWeights; w++) 
					{
						if (maxBoneWeight < weight.weights[w]) 
						{
							maxBoneWeight = weight.weights[w];
							maxBoneIdx = w;
						}
					}
					//bone = boneTies.boneties[weight.bones[maxBoneIdx]]->bone;
					bone = m_boneties[weight.bones[maxBoneIdx]];
				}

				float *p = imesh->points[node].points;
				float *n = imesh->normals[node].points;
				float *t = imesh->textureMap[textures.polygons[poly].nodes[i+1]].points;

				fprintf(fp, "%2d %.6f %.6f %.6f %.6f %.6f %.6f %.6f %.6f\n",
					bone, p[0]*40, p[1]*40, p[2]*40, n[0], n[1], n[2], t[0], -t[1]);
			}
		}

		fprintf(fp, "end\n");
	}

	fclose(fp); fp = NULL;

	return true;
}

void cGrannyFile::drawBoundBox(const Mesh &mesh)
{
	// bounding box
	Point p[24];

	p[0].points[0] = mesh.minX;  p[0].points[1]  = mesh.maxY; p[0].points[2]  = mesh.minZ; 
	p[1].points[0] = mesh.maxX;  p[1].points[1]  = mesh.maxY; p[1].points[2]  = mesh.minZ; 
	p[2] = p[1];
	p[3].points[0] = mesh.maxX;  p[3].points[1]  = mesh.minY; p[3].points[2]  = mesh.minZ;
	p[4] = p[3];
	p[5].points[0] = mesh.minX;  p[5].points[1]  = mesh.minY; p[5].points[2]  = mesh.minZ; 
	p[6] = p[5];
	p[7] = p[0];

	p[8].points[0]  = mesh.minX; p[8].points[1]  = mesh.maxY; p[8].points[2]  = mesh.maxZ; 
	p[9].points[0]  = mesh.maxX; p[9].points[1]  = mesh.maxY; p[9].points[2]  = mesh.maxZ; 
	p[10] = p[9];
	p[11].points[0] = mesh.maxX; p[11].points[1] = mesh.minY; p[11].points[2] = mesh.maxZ;
	p[12] = p[11];
	p[13].points[0] = mesh.minX; p[13].points[1] = mesh.minY; p[13].points[2] = mesh.maxZ; 
	p[14] = p[13];
	p[15] = p[8];

	p[16] = p[0];
	p[17] = p[8];
	p[18] = p[1];
	p[19] = p[9];
	p[20] = p[3];
	p[21] = p[11];
	p[22] = p[5];
	p[23] = p[13];

	glColor3f(1,1,0);

	glEnable(GL_LINE_STIPPLE);
	glLineStipple(1.0f, 0x0f0f);
	glBegin(GL_LINES);
	for (int i = 0; i < 24; i++)
		glVertex3fv(p[i].points);	
	glEnd();
	glDisable(GL_LINE_STIPPLE);

	glColor3f(0.7f,0.7f,0.7f);
	int pt[] = { 0, 1, 3, 5, 8, 9, 11, 13 };
	for (int i = 0; i < 8; i++)
	{
		Point &t = p[pt[i]];
		printw(GLUT_BITMAP_HELVETICA_10, t.points[0], t.points[1],t.points[2], 
			"(%.2f,%.2f,%.2f)",t.points[0], t.points[1],t.points[2]);
	}
	glColor3f(1.0f,1.0f,1.0f);
}

void cGrannyFile::drawSkeleton(Bone* bone, float curTime)
{
	if (!bone) return;

	glPushMatrix();

	GrnMatrix m;
	m.setTransform(bone->quaternion, bone->translate);
	
	Point p1(0,0,0),p2;
	p2 = m*p1;

	// bone line
	glColor3f(1,1,0);
	glBegin(GL_LINES);
	glVertex3fv(p1.points);
	glVertex3fv(p2.points);
	glEnd();

	glMultMatrixf(m.matrix);

	// bone point
	glPointSize(isImportBone(bone->id) ? 7 : 5);
	glColor3f(1,0,0);
	glBegin(GL_POINTS);
	glVertex3f(0,0,0);
	glEnd();
	glPointSize(1);

	// bone name
	if (nConfig::printBoneName) 
	{
		if (isImportBone(bone->id))
			glColor3f(1.0f,0.1f,0.1f);
		else
			glColor3f(0.7f,0.7f,0.7f);
		//printw(0,0,0,"(%d) %s", bone->id, bone->name.c_str());
		printw(0,0,0,"(%d) %s", bone->id, getBoneName(bone->nameid).c_str());
	}
	
	std::vector<Bone *>::iterator ibone;
	for( ibone = bone->children.begin(); ibone != bone->children.end(); ++ibone )
		drawSkeleton(*ibone, 0);	
	
	glPopMatrix();
}

Point cGrannyFile::getModelOrg(float *w, float *h, float *d)
{
	Meshes &meshes = getMeshes();
	std::list<Mesh>::iterator imesh = meshes.meshes.begin();

	if (imesh == meshes.meshes.end()) return Point();

	if (w) *w = abs(imesh->maxX-imesh->minX);
	if (h) *h = abs(imesh->maxY-imesh->minY);
	if (d) *d = abs(imesh->maxZ-imesh->minZ);

	return Point(-(imesh->maxX+imesh->minX)/2, -(imesh->maxZ+imesh->minZ)/2, -(imesh->maxZ+imesh->minZ)/2);
}

//--------------------------------------------------------------------------------------------
// cGrannyAnimation
//--------------------------------------------------------------------------------------------

void cGrannyAnimation::drawSkeleton(Bone* bone, float curTime)
{
	float X,Y,Z;
	dword rid=0,mid=0;
	GrnMatrix matrix;

	//if (!m_animBones)
	//	return;


	if( (m_animBones.size() <= bone->id || m_animBones[ bone->id ] == (dword)-1))
	{
		//pDebug.Log("can't found id %d anim bone", bone->id);
		return;
	}
	else
	{
		BoneAnim &boneAnim = getBoneAnim( m_animBones[ bone->id ] );
		std::vector<float>::iterator movi, roti;

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
			movi != boneAnim.translateTimeline.end() && (*movi) < curTime;
			movi++, mid++ );

		Point t=boneAnim.translates[ mid ],q=boneAnim.quaternions[ rid ];

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

		Point p1(0,0,0),p2(0,0,0);
		p2=matrix*p1;

		// bone line
		glColor3f(1,1,0);
		glBegin(GL_LINES);
		glVertex3fv(p1.points);
		glVertex3fv(p2.points);
		glEnd();

		glMultMatrixf( matrix.matrix );

		// bone point
		glPointSize(isImportBone(bone->id) ? 7 : 5);
		glColor3f(1,0,0);
		glBegin(GL_POINTS);
		glVertex3f(0,0,0);
		glEnd();
		glPointSize(1);

		// bone name
		if (nConfig::printBoneName) 
		{
			if (isImportBone(bone->id))
				glColor3f(1.0f,0.1f,0.1f);
			else
				glColor3f(0.7f,0.7f,0.7f);
			//printw(0,0,0,"(%d) %s", bone->id, bone->name.c_str());
			printw(0,0,0,"(%d) %s", bone->id, getBoneName(bone->nameid).c_str());
		}

	}

	for(std::vector<Bone *>::iterator ibone = bone->children.begin(); ibone != bone->children.end(); ++ibone )
	{
		glPushMatrix();
		drawSkeleton( *ibone, curTime);
		glPopMatrix();
	}
}

bool cGrannyAnimation::export(std::string filename)
{
	if (!m_initialized || !m_assignModel || m_imported) return false;

	pDebug.Log("exporting file... : %s.grn -> %s.smd", 
		::getFileName(m_filename).c_str(), ::getFileName(filename, '\\').c_str());

	FILE* fp = fopen(filename.c_str(), "wt");
	if (!fp) return false;

	std::vector<Bone *> &bones = getBones().bones;
	//Bones &assignBones = assignModel->getBones();

	fprintf(fp,"version 1\n");
	fprintf(fp,"nodes\n");

	for (int i = 0; i < bones.size(); i++) 
	{
		Bone* bone = bones[i];
//		fprintf(fp,"%2d \"%s\" %2d\n", bone->id, bone->name.c_str(), i==0?-1:bone->parent);
		fprintf(fp,"%2d \"%s\" %2d\n", bone->id, getBoneName(bone->nameid).c_str(), i==0?-1:bone->parent);
	}
	fprintf(fp,"end\n");
	fprintf(fp,"skeleton\n");

	std::vector<dword> tmp(m_animBones);
	m_animBones.clear();
	for (int i = 0; i < bones.size(); i++)
		m_animBones.push_back(bones[i]->id);

	for (int time = 0; time * 0.1 <= length(); time++)
	{
		fprintf(fp,"time %d\n", time);

		float curTime = time * 0.1;

		glPushMatrix();
		glLoadIdentity();
		calculateBoneRests(bones[0]);
		getSkeleton(bones[0], curTime);
		glPopMatrix();

		for (int i = 0; i < bones.size(); i++) 
		{
			Bone* bone = bones[i];
			
			Point p = QuaternionToEuler(bone->curQuaternion);

			float *t = bone->curTranslate.points, *q= p.points;

			fprintf(fp,"%2d %.6f %.6f %.6f %.6f %.6f %.6f\n", 
				bone->id,  t[0]*40, t[1]*40, t[2]*40, q[0], q[1], q[2]);
		}
	}

	m_animBones.swap(tmp);

	fprintf(fp,"end\n");
	fclose(fp); fp = NULL;

	return true;
}