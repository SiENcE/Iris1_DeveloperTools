//
// File: Debug.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
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
#include "config.h"
#include "Debug.h"
#include "Exception.h"
#include "xml.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define IS_SECTION 1
#define IS_INTEGER 2
#define IS_STRING  3
#define IS_BOOL    4
#define IS_END     5

struct ParserData{
	ParserData ( const string & NewName,int NewType,void * NewDataPtr) : name(NewName) , type(NewType) , dataptr(NewDataPtr){};
	string name;
	int type;
	void * dataptr; // datatofill -> yes void pointers are bad style.
};

namespace nConfig {

	string version = "0.6.0";

	// UO related
	string mulpath;
	int aos = 0;

	bool logTextChunk = false;
	bool logMesh = false;
	bool logObject = false;
	bool logObjPtr = false;
	bool logBoneObj = false;
	bool logBone = false;
	bool logBoneAnim = false;

	bool drawBoundBox = false;

	bool drawBone = false;
	bool printBoneName = false;

	bool useVertexArray = true;
}

using namespace nConfig;

// important always write childs of section behind that Section array element
ParserData ParserInfo[] = {
	ParserData("UO", IS_SECTION , NULL),
	ParserData("MULPATH", IS_STRING , &mulpath),
	ParserData("AOS", IS_INTEGER , &aos),
	ParserData("", IS_END , NULL)
};

namespace nConfig{

	// Functions
	bool Init(std::string config_file) {
		XML::Parser parser;
		XML::Node *config, *document;

		try
		{
			parser.loadData( config_file );
			document = parser.parseDocument();

			config = document->findNode( "CONFIG" );

			if( !config )
				THROWEXCEPTION ("Couldn't find configuration node.");
		}
		catch( ... )
		{
			//pDebug(Debug::Critical) << "Couldn't parse the configuration file. Please reinstall Ultima Iris.\n";
			return false;
		}
		XML::Node * section = NULL, * value = NULL;
		
		for(int i = 0;ParserInfo[i].type != IS_END;i++){
			const ParserData & Data = ParserInfo[i];
			if(Data.type == IS_SECTION){
				section = config->findNode(Data.name);
			}
			else{
				value = section != NULL ? section->findNode( Data.name ) : config->findNode( Data.name ); // If no section is loaded get from <config>
				if(value == NULL) ;//pDebug(Debug::Warning) << "Couldn't load " << Data.name << "\n";
				else{
					if(Data.type == IS_BOOL) *reinterpret_cast<bool*>(Data.dataptr) = value->asBool();
					else if(Data.type == IS_INTEGER) *reinterpret_cast<int*>(Data.dataptr) = value->asInteger();
					else if(Data.type == IS_STRING) *reinterpret_cast<string*>(Data.dataptr) = value->asString();
				}
			}		  
		}
 
		delete document ; // I don't like that I'll alter darkstorm's class xml class so this is not necessary

		return true;
	}
}
