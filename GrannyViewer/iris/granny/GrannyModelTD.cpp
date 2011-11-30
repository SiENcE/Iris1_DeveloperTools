//
// File: GrannyModelTD.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
//
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
#include "granny/GrannyModelTD.h"
#include "Debug.h"
#include <cassert>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

string ParseForPrefix (string s, string prefix)
{
  const char *p = s.c_str ();   // Find any $
  while ((*p) && (*p != '$'))
    p++;

  if (*p)
      {
        string result = "";

        for (unsigned int i = 0; i < s.size (); i++)
          if (s[i] == '$')
              {
                result += prefix;
              }
          else
            result += s[i];


        return result;

      }
  else
    return s;
}


cGrannyModelTD::cGrannyModelTD (std::string modelfile, std::string basepath, std::string defaultanim, std::string prefix)
{
	//printf("model : %s\n", modelfile.c_str());
	last_tick = 0;
	left_hand_bone = -1;
	right_hand_bone = -1;
	hand = HAND_NONE;
	this->basepath = basepath;
	m_prefix = prefix;
	m_filename = ParseForPrefix(modelfile, prefix);
	m_pathname = m_filename;
	defaultanimname = ParseForPrefix(defaultanim, prefix);
	model = NULL;
	default_animation = NULL;
	m_animset = 0;
}

cGrannyModelTD::~cGrannyModelTD ()
{
	Free ();
}

cGrannyAnimation *cGrannyModelTD::GetAnimation(int animid) 
{ 
	map <int, cGrannyAnimation*>::iterator anim_iter = animations.find(animid);

	return anim_iter == animations.end() ? NULL : anim_iter->second;
}

std::string cGrannyModelTD::GetAnimFileName(int animid, bool fullpath)
{
	cGrannyAnimation *animation = GetAnimation(animid);
	if (animation)
		return fullpath ? animation->getPathName() : animation->getFileName();

	return "";
}

float cGrannyModelTD::GetAnimLength(int animid)
{
	cGrannyAnimation *animation = GetAnimation(animid);
	return animation ? animation->getAnimationLength() : 0.0f;
}


bool cGrannyModelTD::LoadModel()
{
	if (!model) {
		model = new cGrannyFile();
		model->load(m_filename, basepath);
		//model->left_hand_bone = left_hand_bone;
		//model->right_hand_bone = right_hand_bone;
	}

	return model != NULL;
}

cGrannyAnimation *cGrannyModelTD::LoadAnimation(int animid)
{
	assert(model);

	map <int, cGrannyAnimation*>::iterator anim_iter = animations.find(animid);

	cGrannyAnimation *animation = default_animation;

	if (anim_iter == animations.end()) {
		map <int, string>::iterator name_iter = animation_names.find(animid);
		if (name_iter != animation_names.end()) {
			animation = new cGrannyAnimation();
			animation->load(name_iter->second, basepath);

			
			animation->Assign(model);
			animations.insert(make_pair(animid, animation));
		}
	} else { 
		animation = anim_iter->second;
	}

	return animation;
}

cGrannyFile *cGrannyModelTD::GetModel(int modelid)
{ 
	LoadModel();

	return model; 
}

void cGrannyModelTD::Render(int animid, int tick, float & curtime, 
							GrnMatrix * left_matrix, GrnMatrix * right_matrix)
{
	LoadModel();

	assert(model);

	cGrannyAnimation *animation = NULL;
	
	if (hand != HAND_LEFT && hand != HAND_RIGHT) // not equip
		animation = LoadAnimation(animid);

	last_tick = tick;

	if (hand != HAND_NONE) 
		glPushMatrix();

	if ((hand == HAND_LEFT) && left_matrix) 
		glMultMatrixf(left_matrix->matrix);

	if ((hand == HAND_RIGHT) && right_matrix)
		glMultMatrixf(right_matrix->matrix);

	model->Render(animation, curtime, 0, 0, 0, 0, 1.0f, 0);

	if (hand != HAND_NONE) 
		glPopMatrix();

	if (hand == HAND_OWNER) 
	{
		cGrannyFile *m = animation ? animation : model;

		if (right_matrix) 
			*right_matrix = m->getRightHandMatrix();
		if (left_matrix) 
			*left_matrix  = m->getLeftHandMatrix();
	}

}

int cGrannyModelTD::Age(int tick)
{
	return tick - last_tick;
}

void cGrannyModelTD::Free ()
{
	delete default_animation; default_animation = NULL;

	map <int, cGrannyAnimation*>::iterator iter;
	for (iter = animations.begin(); iter != animations.end(); iter++)
		delete iter->second;
	animations.clear();

	delete model;
	model = NULL;
}

void cGrannyModelTD::AddAnimation(int animid, std::string filename)
{
	animation_names.insert(make_pair(animid, ParseForPrefix(filename, m_prefix)));
}

void cGrannyModelTD::SetHandBones(int left, int right)
{
	left_hand_bone = left;
	right_hand_bone = right;
}

bool cGrannyModelTD::ExportModel(std::string filename) 
{ 
	if (!model) return false; 

	return model->export(filename);
}

bool cGrannyModelTD::ExportAnimation(std::string filename, int animid)
{
	cGrannyAnimation *animation = LoadAnimation(animid);
	if (!animation) return false;

	return animation->export(filename);
}

void cGrannyModelTD::SetColor(dword color)
{
	cGrannyModel::SetColor(color);

	if (!model) return;

	float r = (float)GetRValue(color) / 255;
	float g = (float)GetGValue(color) / 255;
	float b = (float)GetBValue(color) / 255;
	model->SetColor(r, g, b);
}

void cGrannyModelTD::ReleaseFile()
{
	if (!model) return;
	
	delete model;
	model = NULL;
}