//
// File: include.h
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

#ifndef _INCLUDE_H_
#define _INCLUDE_H_

/*#define MAX_VIEWDISTANCE ( MAX_FOGDISTANCE * MAX_FOGDISTANCE ) */
/* = FOGDISTANCE ^ 2 */

/* #define MAX_FOGDISTANCE ( MAX_MAPRESOLUTION / 2.0 ) */
/* = MAPRESOLUTION / 2 */

/* #define MAX_BLOCKRESOLUTION 16
#define MAX_MAPRESOLUTION MAX_BLOCKRESOLUTION * 8 */

#define NODE_TOLERANCE 0.01
#define QUADS_PER_NODE 24

#define PI 3.14159f

#define QUADFLAG_BACKFACE 1
#define QUADFLAG_TRIANGLE 2
#define QUADFLAG_NEARONLY 4

#define QUADFLAG_CAN_BE_MERGED 4096

#define MODELFLAG_ISGROUND    1
#define MODELFLAG_TILEABLE    2

// Time in Milliseconds to determine a double click
#define CLICK_TIME 300

#define	MSL	512

#define WALK_BUFFER 50
#define WALK_INTERVAL 20


#define PICK_BUFSIZE 256

// Win32 related
#ifdef	WIN32
#define strcasecmp	strcmpi
#endif

extern int BLOCKRESOLUTION;
extern int MAPRESOLUTION;
extern int FOGDISTANCE;
extern int VIEWDISTANCE;

#define GAMESTATE_MENU  0
#define GAMESTATE_GAME  1

#define ROOF_NONE 1000
#define ROOF_WAIT 1001

#endif //_INCLUDE_H_
