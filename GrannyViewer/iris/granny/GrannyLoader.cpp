//
// File: GrannyLoader.cpp
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
#include "granny/GrannyLoader.h"
#include "granny/GrannyModelTD.h"
#include "granny/GrannyModelAOS.h"
#include "granny/grannyfile.h"
#include "granny/GrannyTextures.h"

#include "Config.h"
#include "Debug.h"

#include "xml.h"

#include <cassert>

#include "../../FileString.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

cGrannyLoader * pGrannyLoader = NULL;

static void toLower(char &c) { c = tolower(c); }

std::string to_lower(std::string s)
{
	for_each(s.begin(), s.end(), toLower);
	return s;
}

struct AOSBodyData{
	AOSBodyData ( const std::string& NewName, int NewID) : name(NewName) , id(NewID) {}
	std::string name;
	int id;

	static int GetAosBodyType(const std::string& bodyName);
	static std::string GetAosBodyName(int bodyType);
};

TypeData AnimTypeInfo[] = 
{
	TypeData ("walkunarmed", 0),
		TypeData ("walkarmed", 1),
		TypeData ("rununarmed", 2),
		TypeData ("runarmed", 3),
		TypeData ("stand", 4),
		TypeData ("standforonehandedattack", 5),
		TypeData ("walkinattackpositiononehanded", 6),
		TypeData ("standfortwohandedattack", 7),
		TypeData ("walkinattackpositiontwohanded", 8),
		TypeData ("attackonehanded", 9),
		TypeData ("takehit", 10),
		TypeData ("fidget.3", 11),
		TypeData ("attack.unarmed.1", 12),
		TypeData ("attack.unarmed.2", 13),
		TypeData ("attack.twohanded.down", 14),
		TypeData ("attack.twohanded.wide", 15),
		TypeData ("attack.twohanded.jab", 16),
		TypeData ("walkinattackposition", 17),
		TypeData ("cast.directed", 18),
		TypeData ("cast.area/summon", 19),
		TypeData ("attack.bowandarrow", 20),
		TypeData ("attack.crossbow", 21),
		TypeData ("die.1", 22),
		TypeData ("die.2", 23),
		TypeData ("horseback.stand", 24),
		TypeData ("horseback.rideslow", 25),
		TypeData ("horseback.ridefast", 26),
		TypeData ("horseback.attack", 27),
		TypeData ("horseback.attack.bowandarrow", 28),
		TypeData ("horseback.attack.crossbow", 29),
		TypeData ("horseback.attack.2", 30),
		TypeData ("bow", 31),
		TypeData ("salute", 32),
		TypeData ("eat", 33),
		TypeData ("llama.stand", 34),
		TypeData ("llama.rideslow", 35),
		TypeData ("llama.ridefast", 36),
		TypeData ("llama.attack", 37),
		TypeData ("llama.attack.bowandarrow", 38),
		TypeData ("llama.attack.crossbow", 39),
		TypeData ("llama.attack.2", 40),
		TypeData ("ostard.stand", 41),
		TypeData ("ostard.rideslow", 42),
		TypeData ("ostard.ridefast", 43),
		TypeData ("ostard.attack", 44),
		TypeData ("ostard.attack.bowandarrow", 45),
		TypeData ("ostard.attack.crossbow", 46),
		TypeData ("ostard.attack.2", 47),
		TypeData ("kirin.stand", 48),
		TypeData ("kirin.rideslow", 49),
		TypeData ("kirin.ridefast", 50),
		TypeData ("kirin.attack", 51),
		TypeData ("kirin.attack.bowandarrow", 52),
		TypeData ("kirin.attack.crossbow", 53),
		TypeData ("kirin.attack.2", 54),
		TypeData ("giant_beetle.stand", 55),
		TypeData ("giant_beetle.rideslow", 56),
		TypeData ("giant_beetle.ridefast", 57),
		TypeData ("giant_beetle.attack", 58),
		TypeData ("giant_beetle.attack.bowandarrow", 59),
		TypeData ("giant_beetle.attack.crossbow", 60),
		TypeData ("giant_beetle.attack.2", 61),
		TypeData ("ridgeback.stand", 62),
		TypeData ("ridgeback.rideslow", 63),
		TypeData ("ridgeback.ridefast", 64),
		TypeData ("ridgeback.attack", 65),
		TypeData ("ridgeback.attack.bowandarrow", 66),
		TypeData ("ridgeback.attack.crossbow", 67),
		TypeData ("ridgeback.attack.2", 68),
		TypeData ("seahorse.stand", 69),
		TypeData ("seahorse.rideslow", 70),
		TypeData ("seahorse.ridefast", 71),
		TypeData ("seahorse.attack", 72),
		TypeData ("seahorse.attack.bowandarrow", 73),
		TypeData ("seahorse.attack.crossbow", 74),
		TypeData ("seahorse.attack.2", 75),
		TypeData ("sit", 76),
		TypeData ("applaud", 78),
		TypeData ("argue.1", 79),
		TypeData ("argue.2", 80),
		TypeData ("argue.3", 81),
		TypeData ("kiss", 82),
		TypeData ("bow.2", 83),
		TypeData ("bow.3", 84),
		TypeData ("coverears", 85),
		TypeData ("courtesy.1", 86),
		TypeData ("dance.folk.1", 87),
		TypeData ("dance.folk.2", 88),
		TypeData ("dance.3", 89),
		TypeData ("dance.4", 90),
		TypeData ("foldarms", 91),
		TypeData ("impatient", 92),
		TypeData ("lecture", 93),
		TypeData ("nod_yes", 94),
		TypeData ("point", 95),
		TypeData ("salute.1", 96),
		TypeData ("salute.2", 97),
		TypeData ("salute.3", 98),
		TypeData ("shake_no", 99),
		TypeData ("victory.1", 101),
		TypeData ("victory.2", 102),
		TypeData ("wave.1", 103),
		TypeData ("wave.2", 104),
		TypeData ("wave.long", 105),
		TypeData ("what", 106),
		TypeData ("fidget.sexy", 107),
		TypeData ("stand.sexy", 108),
		TypeData ("fidget.sinister", 109),
		TypeData ("stand.sinister", 110),
		TypeData ("fidget.drunk", 111),
		TypeData ("stand.drunk", 112),
		TypeData ("fidget.athletic", 113),
		TypeData ("stand.athletic", 114),
		TypeData ("fidget.aged", 115),
		TypeData ("stand.aged", 116),
		TypeData ("fidget.jammin", 117),
		TypeData ("stand.jammin", 118),
		TypeData ("default.animation", 200),
		TypeData ("", -1),
};

class cAnimTypeMap {
public:
	std::map<std::string, int> cacheName;
	std::map<int, std::string> cacheId;
	cAnimTypeMap()
	{
		for (int i = 0; AnimTypeInfo[i].blockid != -1; i++) {
			cacheName.insert(make_pair(AnimTypeInfo[i].name,AnimTypeInfo[i].blockid));
			cacheId.insert(make_pair(AnimTypeInfo[i].blockid, AnimTypeInfo[i].name));
		}
	}

	int findId(const std::string &name)
	{
		std::map<std::string, int>::iterator i = cacheName.find(name);
		return i != cacheName.end() ? i->second : -1;
	}

	std::string findName(int id)
	{
		std::map<int, std::string>::iterator i = cacheId.find(id);
		return i != cacheId.end() ? i->second : "";
	}

} mapAnimType;


int TypeData::GetAnimType(const std::string &animName)
{
	return mapAnimType.findId(animName);
}

string TypeData::GetAnimTypeName(int animType)
{
	return mapAnimType.findName(animType);
}


AOSBodyData AOSBodyInfo[] = {
	AOSBodyData("EARS",    1),
		AOSBodyData("FACE",    2),
		AOSBodyData("FEET",    3),
		AOSBodyData("HANDS",   4),
		AOSBodyData("HEAD",    5),
		AOSBodyData("LLEGS",   6),
		AOSBodyData("NECK",    7),
		AOSBodyData("TORSOUP", 8),
		AOSBodyData("TORSO",   9),
		AOSBodyData("UARMS",  10),
		AOSBodyData("ULEGS",  11),
		AOSBodyData("FARMS",  12),
		AOSBodyData("PELVIS", 13),
		AOSBodyData("", 0),
};

int AOSBodyData::GetAosBodyType(const std::string& bodyName)
{
	for (int i = 0; AOSBodyInfo[i].id != 0; i++) 
		if (AOSBodyInfo[i].name == bodyName)
			return AOSBodyInfo[i].id;

	return -1;
}

string AOSBodyData::GetAosBodyName(int bodyType)
{
	for (int i = 0; AOSBodyInfo[i].id != 0; i++) 
		if (AOSBodyInfo[i].id == bodyType)
			return AOSBodyInfo[i].name;

	return "";
}

cGrannyLoader::cGrannyLoader()
{
}


cGrannyLoader::~cGrannyLoader ()
{
	std::for_each(m_animsets.begin(), m_animsets.end(), my_delete2<std::pair<int,cAnimSet*> >); 
	m_animsets.clear();

	std::for_each(models.begin(), models.end(), my_delete2<std::pair<int,cGrannyModel*> >); 
	models.clear();

	assert(pGrannyTextureLoader);
	delete pGrannyTextureLoader;
	pGrannyTextureLoader = NULL;
}

void cGrannyLoader::Clear ()
{
}

std::map<int, std::string> cGrannyLoader::GetAnimDataType()
{
	return mapAnimType.cacheId;
}

std::string cGrannyLoader::GetAosBodyName(int bodyType)
{
	return AOSBodyData::GetAosBodyName(bodyType);
}

std::string cGrannyLoader::GetAnimName(int aid)
{
	return mapAnimType.findName(aid);
}

inline
void cGrannyLoader::InsertAnimset(TiXmlElement *set_node)
{
	if (!set_node) return;

	XML::Node value;
	cAnimSet * animset = new cAnimSet;
	value = set_node->FirstChildElement("ID");
	animset->id = value.asInteger();

	if (value = set_node->FirstChildElement("DEFAULTANIM")) 
	{
		animset->defaultanim = mulpath + to_lower(value.asString());

		// animtype 을 anim_typeid 로 변경
		int anim_typeid = TypeData::GetAnimType("default.animation");

		if (anim_typeid != -1) 
			animset->anim_names.insert(make_pair(anim_typeid, animset->defaultanim));
	}


	TiXmlElement *anim_node = set_node->FirstChildElement("ANIMATION");
	while (anim_node) 
	{
		std::string animtype = "", filename = "";
		if (value = anim_node->FirstChildElement("TYPE"))
			animtype = value.asString();
		if ((value = anim_node->FirstChildElement("FILE")))
			filename = mulpath + to_lower(value.asString());

		// animtype 을 anim_typeid 로 변경
		int anim_typeid = TypeData::GetAnimType(animtype);

		if (anim_typeid != -1) 
			animset->anim_names.insert(make_pair(anim_typeid, filename));

		anim_node = anim_node->NextSiblingElement("ANIMATION");
	}

	m_animsets.insert(make_pair(animset->id, animset));
}

inline
void cGrannyLoader::InsertCharacter(TiXmlElement *char_node)
{
	if (!char_node) return;

	XML::Node value;
	std::string filename = "", default_anim = "", prefix = "", desc;
	int left_hand_bone = -1;
	int right_hand_bone = -1;
	int animset = -1;
	int hand = HAND_NONE;

	value = char_node->FirstChildElement("ID");
	Uint32 id = (value != NULL) ? value.asInteger() : 0;

	if (value = char_node->FirstChildElement("FILE")) 
		filename = mulpath + to_lower(value.asString());

	if (value = char_node->FirstChildElement("ANIMSET"))
		animset = value.asInteger();
	if (value = char_node->FirstChildElement("PREFIX"))
		prefix = to_lower(value.asString());

	if (value = char_node->FirstChildElement("LEFT_HAND_BONE")) 
	{
		left_hand_bone = value.asInteger();
		hand = HAND_OWNER;
	}
	if (value = char_node->FirstChildElement("RIGHT_HAND_BONE")) 
	{
		right_hand_bone = value.asInteger();
		hand = HAND_OWNER;
	}

	if (value = char_node->FirstChildElement("HAND")) 
	{
		if (value.asString () == "left")
			hand = HAND_LEFT;
		if (value.asString () == "right")
			hand = HAND_RIGHT;
	}

	if (value = char_node->FirstChildElement("DESCRIPTION"))
		desc = value.asString();

	Uint16 assign = 0;
	if (value = char_node->FirstChildElement("ASSIGN"))
		assign = value.asInteger();
	id |= assign << 16;

	// humonoid, monster can hold weapons.
	if (assign == 0 && (animset <= 1 || animset == 3)) 
		hand = HAND_OWNER; 

	map <int, cAnimSet *>::iterator iter;
	map <int, string>::iterator name_iter;

	iter = m_animsets.find(animset);

	if (id && (iter != m_animsets.end())) 
	{
		assert(iter->second);
		cGrannyModel * model = new cGrannyModelTD (filename, tex_basepath, iter->second->defaultanim, prefix);
		model->SetHandBones(left_hand_bone, right_hand_bone);
		model->SetHand(hand);
		model->desc = desc;

		model->SetAnimset (animset);

		for (name_iter=iter->second->anim_names.begin(); name_iter!=iter->second->anim_names.end();name_iter++)
		{
			model->AddAnimation(name_iter->first, name_iter->second);
		}

		//models.erase(id);
		if (models.find(id)!=models.end()) 
		{
			pDebug.Log("Warning: duplicated model id : %d", id);
			delete models.find(id)->second;
			models.erase(id);
		}
		models.insert(make_pair(id, model));
	} 
	else 
	{
		pDebug.Log ("Warning: Invalid Granny Definition");
	}
}


inline
void cGrannyLoader::InsertCharacterAOS(TiXmlElement *char_node)
{
	if (!char_node) return;

	XML::Node value;

	std::string filename, default_anim, desc;
	int animset = -1;

	int left_hand_bone = -1;
	int right_hand_bone = -1;
	int hand = HAND_NONE;

	value = char_node->FirstChildElement("ID");
	Uint32 id = value.asInteger();
	if (value = char_node->FirstChildElement("ANIMSET"))
		animset = value.asInteger();

	if (value = char_node->FirstChildElement ("LEFT_HAND_BONE"))
	{
		left_hand_bone = value.asInteger ();
		hand = HAND_OWNER;
	}
	if (value = char_node->FirstChildElement ("RIGHT_HAND_BONE"))
	{
		right_hand_bone = value.asInteger ();
		hand = HAND_OWNER;
	}

	if (value = char_node->FirstChildElement("DESCRIPTION"))
		desc = value.asString();

	Uint16 assign = 0;
	if (value = char_node->FirstChildElement("ASSIGN"))
		assign = value.asInteger();
	id |= assign << 16;

	map <int, cAnimSet *>::iterator iter;
	map <int, string>::iterator name_iter;


	iter = m_animsets.find(animset);

	if (id && (iter != m_animsets.end())) 
	{
		assert(iter->second);
		cGrannyModelAOS * model = new cGrannyModelAOS (tex_basepath, iter->second->defaultanim);
		model->SetHandBones (left_hand_bone, right_hand_bone);
		model->SetHand (hand);
		model->SetAnimset (animset);
		model->desc = desc;

		// HARKON : bodypart is freed by loader
		model->setFreeBodyParts(false);

		for(int i = 0; AOSBodyInfo[i].id != 0; i++) 
		{
			const AOSBodyData& Data = AOSBodyInfo[i];
			TiXmlElement *submodel = char_node->FirstChildElement(Data.name);
			if (submodel) 
			{
				cGrannyModel *m = model->AddModel(Data.id, mulpath + to_lower(submodel->GetText()));
				UINT id2 = 0xC000 | id << 4 | Data.id;
				models.insert(make_pair(id2, m));
			}
		}

		for (name_iter=iter->second->anim_names.begin(); name_iter!=iter->second->anim_names.end();name_iter++)
		{ 
			model->AddAnimation(name_iter->first, name_iter->second);
		}
		//models.erase(id);
		if (models.find(id)!=models.end()) 
		{
			pDebug.Log("Warning: duplicated model id : %d\n", id);
			delete models.find(id)->second;
			models.erase(id);
		}
		models.insert(make_pair(id, model));

	} 
	else 
	{
		pDebug.Log ("Warning: Invalid Granny AOS Definition");
	}
}


bool cGrannyLoader::Load(std::string filename, std::string mulpath)
{
	tick = 0;

	XML::Parser parser;
	XML::Node *granny=NULL, *document=NULL;

	try
	{
		parser.loadData(filename);
		document = parser.parseDocument();

		granny = document->findNode( "GRANNY" );

		if( !granny )
			throw "Couldn't find granny node.";
	}
	catch( ... )
	{
		pDebug.Log ("Couldn't load Granny Definitions");
		return false;
	}

	XML::Node value;

	TiXmlNode *tgranny  = granny->getTinyNode();


	bool ext = false;
	std::string texpath, smdpath;

	if (value = tgranny->FirstChildElement("TEXTUREPATH"))
		texpath = value.asString();

	if (value = tgranny->FirstChildElement("SMDPATH"))
		smdpath = value.asString();

	if (smdpath==".") 
	{
		CString strPath = CFileString::GetModuleDir()+"/";
		strPath.Replace('\\','/');
		mulpath = strPath;
	}
	else if (!smdpath.empty())
	{
		mulpath = smdpath;
	}

	this->mulpath = mulpath;

	if (texpath.find(":/") != std::string::npos || texpath.find(":\\") != std::string::npos)
		tex_basepath = texpath;
	else
		tex_basepath = mulpath + texpath;

	assert(!pGrannyTextureLoader);
	pGrannyTextureLoader = new cGrannyTextureLoader(tex_basepath);

	TiXmlHandle hgranny(granny->getTinyNode());

#if 0
	TiXmlElement *set_node = hgranny.FirstChild("ANIMSET_LIST").FirstChild("ANIMSET").Element();

	while (set_node) 
	{
		InsertAnimset(set_node);

		set_node = set_node->NextSiblingElement(set_node->Value());
	}

	TiXmlElement *char_node = hgranny.FirstChild("CHARACTER_AOS_LIST").FirstChild("CHARACTER_AOS").Element();
	while (char_node) 
	{
		InsertCharacterAOS(char_node);
		
		char_node = char_node->NextSiblingElement(char_node->Value());
	}

	char_node = hgranny.FirstChild("CHARACTER_LIST").FirstChild("CHARACTER").Element();
	while (char_node) 
	{
		InsertCharacter(char_node);
		
		char_node = char_node->NextSiblingElement(char_node->Value());
	}
#else
	TiXmlElement *anim_node = tgranny->FirstChildElement("ANIMSET");
	while (anim_node) 
	{
		InsertAnimset(anim_node);
		anim_node = anim_node->NextSiblingElement("ANIMSET");
	}
	
	TiXmlElement * char_node = tgranny->FirstChildElement();
	while (char_node) 
	{
		if (char_node->ValueStr()=="CHARACTER")
			InsertCharacter(char_node);
		else if (char_node->ValueStr()=="CHARACTER_AOS")
			InsertCharacterAOS(char_node);
		
		char_node = char_node->NextSiblingElement();
	}
#endif
	

	delete document;

	return true;
}





void cGrannyLoader::Render(Uint32 id, Uint32 type, float & curtime, 
						   GrnMatrix * left_matrix, GrnMatrix * right_matrix)
{
	GrannyModelMapIter iter = models.find(id);

	tick ++;

	if (iter != models.end()) {
		assert(iter->second);
		iter->second->Render(type, tick, curtime, left_matrix, right_matrix);
	}

}

cGrannyModel* cGrannyLoader::GetModel(Uint32 id)
{
	GrannyModelMapIter iter = models.find(id);
	if (iter == models.end()) return NULL;

	return iter->second;
}


void cGrannyLoader::Render( Uint32 id, Uint32 type, float &curtime, 
						   GrnMatrix * left_matrix, GrnMatrix * right_matrix,
						   std::vector < int >bodyparts)
{
	std::map < Uint32, cGrannyModel * >::iterator iter = models.find (id);

	tick++;

	if (iter != models.end ())
	{
		assert (iter->second);
		if ((nConfig::aos) && (id == 400 || id == 401))
		{
			cGrannyModelAOS *model = dynamic_cast<cGrannyModelAOS *>(iter->second);
			model->setBodyParts(bodyparts);
			model->Render(type, tick, curtime, left_matrix, right_matrix);
		}
	}

}

int cGrannyLoader::GetAnimset (Uint32 id)
{
	cGrannyModel* model=GetModel(id);
	return model ? model->animset() : -1;
}

bool cGrannyLoader::isFound (Uint32 id)
{
	return GetModel(id) != NULL;
}


bool cGrannyLoader::Release(Uint32 id)
{
	cGrannyModel *pModel = GetModel(id);
	if (!pModel) return false;

	pModel->ReleaseFile();

	return true;
}

float cGrannyLoader::GetAnimLength(Uint32 id, Uint32 type)
{
	cGrannyModel *pModel = GetModel(id);
	return pModel ? pModel->GetAnimLength(type) : 0.0f;
}

bool cGrannyLoader::IsShow(Uint32 id)
{
	cGrannyModel *pModel = GetModel(id);
	return pModel ? pModel->show : false;
}