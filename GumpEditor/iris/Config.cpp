#include "stdafx.h"
#include "config.h"
#include "Debug.h"
#include "Exception.h"
#include "xml.h"


using namespace std;

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

	// Screen related
	int width = 640;
	int height = 480;
	int width_2d = 640;
	int height_2d = 480;
	int bpp = 16;
	int startfullscreen = 0;
	int roof_fade_time = 2000;
	int roof_fade_alpha = 1;
	int depthbuffer = 16;
	int maxzoom = 8;
	int maxangle = 45;
	int viewdistance = 4;
	int brightness = 5;
	int animationsmoothness = 25;
	int render_in_3d = 1;
	string video_driver = "windib";

	// Net related
	string server = "localhost";
	string login = "account";
	string password = "password";
	int serverport = 2593;
	int is_sphere = 0;

	// UO related
	int startx = 1500;
	int starty = 1000; 
	int startz = 0;
	int cursor = 1;
	int perspective = 0;
	string mulpath;
	string compressed_map = "";
	int aos = 0;
	string locale = "English";
	string unicode_lang = "ENU";

	// Sound related
	int music = 1;
	int sound = 1;
	int stereo = 1;
	int chunksize = 1024;
	int musicvolume = 40;
	int soundvolume = 128;
	int mp3 = 1;

	vector<font_info> fonts;
}

using namespace nConfig;
// important always write childs of section behind that Section array element
ParserData ParserInfo[] = {
	// Section Gfx
	ParserData("GFX", IS_SECTION , NULL),
	ParserData("WIDTH", IS_INTEGER , &width),
	ParserData("HEIGHT", IS_INTEGER , &height),
	ParserData("BPP", IS_INTEGER , &bpp),
	ParserData("CURSOR", IS_INTEGER , &cursor),
	ParserData("PERSPECTIVE", IS_INTEGER , &perspective),
	ParserData("FULLSCREEN", IS_INTEGER , &startfullscreen),
	ParserData("ROOF_FADE_TIME", IS_INTEGER , &roof_fade_time),
	ParserData("ROOF_FADE_ALPHA", IS_INTEGER , &roof_fade_alpha),
	ParserData("ZBUFFER_SIZE", IS_INTEGER , &depthbuffer),
	ParserData("MAXZOOM", IS_INTEGER , &maxzoom),
	ParserData("MAXANGLE", IS_INTEGER , &maxangle),
	ParserData("VIEWDISTANCE", IS_INTEGER , &viewdistance),
	ParserData("BRIGHTNESS", IS_INTEGER , &brightness),
	ParserData("ANIMATION_SMOOTHNESS", IS_INTEGER , &animationsmoothness),
	ParserData("RENDERMODE", IS_INTEGER , &render_in_3d),
	ParserData("VIDEO_DRIVER", IS_STRING, &video_driver),
	ParserData("UO", IS_SECTION , NULL),
	ParserData("STARTX", IS_INTEGER , &startx),
	ParserData("STARTY", IS_INTEGER , &starty),
	ParserData("STARTZ", IS_INTEGER , &startz),
	ParserData("MULPATH", IS_STRING , &mulpath),
	ParserData("COMPRESSED_MAP", IS_STRING , &compressed_map),
	ParserData("AOS", IS_INTEGER , &aos),
	ParserData("LOCALE", IS_STRING , &nConfig::locale),
	ParserData("UNICODE_LANG", IS_STRING , &nConfig::unicode_lang),
	ParserData("NET", IS_SECTION , NULL),
	ParserData("PORT", IS_INTEGER , &serverport),
	ParserData("SERVER", IS_STRING , &server),
	ParserData("LOGIN", IS_STRING , &login),
	ParserData("PASSWORD", IS_STRING , &password),
	ParserData("IS_SPHERE", IS_INTEGER , &is_sphere),
	ParserData("SOUND", IS_SECTION , NULL),
	ParserData("MUSIC", IS_INTEGER , &music),
	ParserData("SOUND", IS_INTEGER , &sound),
	ParserData("STEREO", IS_INTEGER , &stereo),
	ParserData("CHUNKSIZE", IS_INTEGER , &chunksize),
	ParserData("MUSICVOLUME", IS_INTEGER , &musicvolume),
	ParserData("SOUNDVOLUME", IS_INTEGER , &soundvolume),

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

		
		XML::Node* font_set = config->findNode("FONTSET");
		
		if (font_set) {
			int idx=0;
			XML::Node* font_node = NULL;			
			font_info font;
			while (font_node = font_set->findNode("FONT", idx++)) {
				if (!font_node->lookupAttribute("ID", font.id)) continue;
				if (!font_node->lookupAttribute("FILE", font.file)) continue;
				if (!font_node->lookupAttribute("NAME", font.name)) continue;
				if (!font_node->lookupAttribute("SIZE", font.size)) continue;
				if (!font_node->lookupAttribute("HUE",  font.hue)) continue;

				font.file = GfxGetFullPath(font.file.c_str());

				fonts.push_back(font);
			}
		}


		delete document ; // I don't like that I'll alter darkstorm's class xml class so this is not necessary
		if (depthbuffer <= 0)
			depthbuffer = 16;
		if (viewdistance < 3)
			viewdistance = 3;
		if (maxzoom <= 0) 
			maxzoom = 0;
		if (maxangle < 10) maxangle = 10;
		if (maxangle > 80) maxangle = 80;

		mp3 = aos;

		if (animationsmoothness < 10) animationsmoothness = 10;
		if (animationsmoothness > 100) animationsmoothness = 100;

		return true;
	}
}
