// MAX string length
#include "include.h"
#include <string>
#include <map>

namespace	nConfig
{
	struct font_info {
		int id;
		std::string file;
		std::string name;
		int size;
		int hue;
	};

	extern std::string version;
	// GFX
	extern int width, height, bpp, startfullscreen;
	extern int cursor, perspective, depthbuffer, maxzoom, maxangle;
	extern int viewdistance, brightness, animationsmoothness, render_in_3d;
	extern int width_2d, height_2d;
	extern std::string video_driver;

	// UO	
	extern int startx, starty, startz;
	extern int aos;
	extern std::string mulpath;
	extern std::string compressed_map;
	extern std::string locale;
	extern std::string unicode_lang;

	// NET
	extern std::string server, login, password;
	extern int serverport;
	extern int roof_fade_time, roof_fade_alpha;
	extern int is_sphere;

	// SOUND
	extern int music, sound, stereo, chunksize, musicvolume, mp3;

	// FONT
	extern std::vector<font_info> fonts;

	// Function
	bool	Init(std::string config_file);
};
