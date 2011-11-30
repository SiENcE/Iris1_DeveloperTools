//
// File: Debug.cpp
// Created by: Alexander Oster - tensor@ultima-iris.de
// Modified by: DarkStorm (4.3.2003)
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <cassert>
#include "Debug.h"

//#define _DEBUG

Debug pDebug;

using namespace std;

Debug::Debug()
{
	loglevel = LEVEL_WARNING;
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
	va_list args;
	va_start(args, message);

	TCHAR buffer[512];

	int nbuf = _vsntprintf(buffer, sizeof(buffer), message, args);

	// was there an error? was the expanded string too long?
	assert(nbuf >= 0);

	va_end(args);

	GfxLog("%s\n",buffer);

	// Dump the message out to a file
	if(logToFile) {
		FILE *fp = fopen("debug.txt", "at");

		if(fp) {
			fprintf(fp, "%s\n", buffer);
			fclose(fp);
		}
	} else {
		fprintf(stderr, "%s\n", buffer);
	}
}

void Debug::Log(const char *message, const char *filename, int line, int level)
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

	char lineStr[32];

	sprintf(lineStr, "%d", line);

	errorMsg += "(" + string(filename) + "," + string(lineStr) + "): " + string(message) + "\n";

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

