//
// File: Debug.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
// Modified by: DarkStorm (4.3.2003)
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
#include <iostream>
#include <string>
#include <cassert>
#include "Debug.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

Debug pDebug;

using namespace std;

Debug::Debug()
{
	loglevel = LEVEL_INFO3;
}

Debug::~Debug()
{
	// TODO: Place Destructor Here
}

// Only print to stderr in Debug mode, otherwise redirect to file (debug.txt)
#ifndef WIN32
#define _DEBUG
#endif

#ifndef _DEBUG
bool logToFile = true;
#else
bool logToFile = false;
#endif

void Debug::Log(const char *message, ...)
{
	int level = LEVEL_INFO4;

	if(level < loglevel)
		return;

	va_list args;
	va_start(args, message);

	char buffer[2048];

	int nbuf = _vsntprintf(buffer, sizeof(buffer), message, args);

	// was there an error? was the expanded string too long?
	assert(nbuf >= 0);

	va_end(args);

	Log(level, buffer, NULL, 0);
}

void Debug::Log(int level, const char *message, ...)
{
	if(level < loglevel)
		return;

	va_list args;
	va_start(args, message);

	char buffer[2048];

	int nbuf = _vsntprintf(buffer, sizeof(buffer), message, args);

	// was there an error? was the expanded string too long?
	assert(nbuf >= 0);

	va_end(args);

	Log(level, buffer, NULL, 0);
}


void Debug::Log(int level, const char *message, const char *filename, int line)
{
	// Ignore messages below our loglevel
	if(level < loglevel)
		return;

	std::string errorMsg;

	switch (level) {
	case LEVEL_WARNING:
		errorMsg = "Warning ";
		break;
	case LEVEL_ERROR:
		errorMsg = "Error ";
		break;
	case LEVEL_CRITICAL:
		errorMsg = "CRITICAL Error ";
		break;
	}

	if (filename != NULL) {
		char lineStr[32];

		sprintf(lineStr, "%d", line);
		errorMsg += "(" + string(filename) + "," + string(lineStr) + "): ";
	}

	errorMsg += string(message) + "\n";

	GfxLog(errorMsg.c_str());

	// Dump the message out to a file
	if(logToFile) {
		FILE *fp = fopen("debug.txt", "at");

		if(fp) {
			fprintf(fp, errorMsg.c_str());
			fclose(fp);
		}
	} else {
		fprintf(stderr, errorMsg.c_str());
	}
}


int Debug::GetLoglevel(void)
{
	return loglevel;
}

void Debug::SetLoglevel(int loglevel)
{
	this->loglevel = loglevel;
}
