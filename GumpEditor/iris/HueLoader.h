#pragma once

#include <vector>
#include <string>

struct stHue
{
	unsigned int colors[32];
	unsigned int tableStart;
	unsigned int tableEnd;
};

class cHueLoader
{
private:
	std::vector<stHue> hues;
public:
	cHueLoader();
	virtual ~cHueLoader();

	const stHue *getHue( unsigned short id );
	unsigned int getColor(unsigned short id);
	bool load(std::string filename);

	int getSize() { return hues.size(); }
};
