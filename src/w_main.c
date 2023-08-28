//
// Copyright(C) 1993-1996 Id Software, Inc.
// Copyright(C) 2005-2014 Simon Howard
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// DESCRIPTION:
//     Common code to parse command line, identifying WAD files to load.
//

#include <stdlib.h>

#include "config.h"
#include "d_iwad.h"
#include "i_glob.h"
#include "i_system.h"
#include "m_argv.h"
#include "w_main.h"
#include "w_merge.h"
#include "w_wad.h"
#include "z_zone.h"

// Parse the command line, merging WAD files that are sppecified.
// Returns true if at least one file was added.
boolean W_ParseCommandLine(void)
{
    boolean modifiedgame = false;
    int p;

    //!
    // @arg <files>
    // @vanilla
    //
    // Load the specified PWAD files.
    //

    p = M_CheckParmWithArgs("-file", 1);
    if (p)
    {
        // the parms after p are wadfile/lump names,
        // until end of parms or another - preceded parm
        modifiedgame = true; // homebrew levels
        while (++p != myargc && myargv[p][0] != '-')
        {
            char *filename;

            filename = D_TryFindWADByName(myargv[p]);

            printf(" adding %s\n", filename);
            W_AddFile(filename);
            free(filename);
        }
    }

    //    W_PrintDirectory();

    return modifiedgame;
}

void W_CheckCorrectIWAD(GameMission_t mission)
{
    // A lump name that is unique to a particular game type (DOOM). 
    // POSSA1 is the first frame of a zombie trooper.
    // If we don't see this in an IWAD, it is probably the wrong
    // type.
    // i.e mindoom -iwad hexen.wad
    const char* unique_lump = "POSSA1";     

    lumpindex_t lumpnum;
    lumpnum = W_CheckNumForName(unique_lump);

    if (lumpnum == -1)
    {
        I_Error("\nYou are trying to use an incompatible IWAD file.");
    }
}
