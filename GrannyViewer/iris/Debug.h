//
// File: Debug.h
// Created by:
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

#pragma once

#ifndef _DEBUG_H_
#define _DEBUG_H_

#ifdef WIN32
#include <windows.h>
#endif

#include "../multitimer.h"

//#include "SDL/SDL.h"
#include <string>
#include <cassert>

enum LOG_LEVEL 
{
	LEVEL_INFO1, LEVEL_INFO2, LEVEL_INFO3, LEVEL_INFO4,
	LEVEL_WARNING, LEVEL_ERROR, LEVEL_CRITICAL, LEVEL_NONE
};

class Debug
{
private:
  int loglevel;

public:
    Debug ();
   ~Debug ();

   void Log(int level, const char *messge, ...);
   void Log(const char *messge, ...);
   void Log(int level, const char * message, const char * filename, int line);
   void Log(std::string message) { Log(message.c_str()); }
   void Log(int level, std::string message) { Log(level, message.c_str()); }
   int GetLoglevel(void);
   void SetLoglevel(int loglevel);

protected:

};

extern	Debug	pDebug;

#define DEBUG_TIMER_START(s) TIMER_START(s)
#define DEBUG_TIMER_STOP(s)  TIMER_STOP(s); pDebug.Log("%s : %s", s, T2S(GET_TIMER(s).getTotalTime())); 

#endif //_DEBUG_H_
