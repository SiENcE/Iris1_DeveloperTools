// MAX string length
#include "include.h"
#include <string>
#include <map>
#include <vector>

namespace	nConfig
{
	// UO	
	extern int aos;
	extern std::string mulpath;
	
	// Etc
	extern bool	logTextChunk;
	extern bool logMesh;
	extern bool logObject;
	extern bool logObjPtr;
	extern bool logBoneObj;
	extern bool logBone;
	extern bool logBoneAnim;

	extern bool drawBone;
	extern bool printBoneName;

	extern bool drawBoundBox;

	extern bool useVertexArray;

	// Function
	bool	Init(std::string config_file);
};
