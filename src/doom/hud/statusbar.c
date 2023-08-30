/*
 * Copyright(C) 1993-1996 Id Software, Inc.
 * Copyright(C) 2005-2014 Simon Howard
 * Copyright(C) 2023 Joshua Murphy 
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * DESCRIPTION:
 *	Status bar code.
 *	Does the face/direction indicator animatin.
 *	Does palette indicators as well (red pain/berserk, bright pickup)
 *
 * This file was previously named "st_stuff.c"
 */


#include <stdio.h>
#include <ctype.h>

#include "../../i_system.h"
#include "../../i_video.h"
#include "../../z_zone.h"
#include "../../m_misc.h"
#include "../m_random.h"
#include "../../w_wad.h"
#include "../../log.h"

#include "../doomdef.h"
#include "../../doomkeys.h"

#include "../g_game.h"

#include "statusbar.h"
#include "widget.h"

#include "../r_local.h"

#include "../p_local.h"
#include "../p_inter.h"

#include "../am_map.h"
#include "../../m_cheat.h"

#include "../s_sound.h"

// Needs access to LFB.
#include "../../v_video.h"

// State.
#include "../doomstat.h"

// Data.
#include "../dstrings.h"
#include "../sounds.h"

//
// STATUS BAR DATA
//


// Palette indices.
// For damage/bonus red-/gold-shifts
#define STARTREDPALS   1
#define STARTBONUSPALS 9
#define NUMREDPALS     8
#define NUMBONUSPALS   4
// Radiation suit, green shift.
#define RADIATIONPAL 13

// N/256*100% probability
//  that the normal face state will change
#define ST_FACEPROBABILITY 96

// For Responder
#define ST_TOGGLECHAT KEY_ENTER

// Should be set to patch width
//  for tall numbers later on
#define ST_TALLNUMWIDTH (tallnum[0]->width)

// Number of status faces.
#define ST_NUMPAINFACES     5
#define ST_NUMSTRAIGHTFACES 3
#define ST_NUMTURNFACES     2
#define ST_NUMSPECIALFACES  3

#define ST_FACESTRIDE (ST_NUMSTRAIGHTFACES + ST_NUMTURNFACES + ST_NUMSPECIALFACES)

#define ST_NUMEXTRAFACES 2

#define ST_NUMFACES (ST_FACESTRIDE * ST_NUMPAINFACES + ST_NUMEXTRAFACES)

#define ST_TURNOFFSET     (ST_NUMSTRAIGHTFACES)
#define ST_OUCHOFFSET     (ST_TURNOFFSET + ST_NUMTURNFACES)
#define ST_EVILGRINOFFSET (ST_OUCHOFFSET + 1)
#define ST_RAMPAGEOFFSET  (ST_EVILGRINOFFSET + 1)
#define ST_GODFACE        (ST_NUMPAINFACES * ST_FACESTRIDE)
#define ST_DEADFACE       (ST_GODFACE + 1)

#define ST_FACESX 143
#define ST_FACESY 168

#define ST_EVILGRINCOUNT     (2 * TICRATE)
#define ST_STRAIGHTFACECOUNT (TICRATE / 2)
#define ST_TURNCOUNT         (1 * TICRATE)
#define ST_OUCHCOUNT         (1 * TICRATE)
#define ST_RAMPAGEDELAY      (2 * TICRATE)

#define ST_MUCHPAIN 20


// Location and size of statistics,
//  justified according to widget type.
// Problem is, within which space? STbar? Screen?
// Note: this could be read in by a lump.
//       Problem is, is the stuff rendered
//       into a buffer,
//       or into the frame buffer?

// Weapon pos.
#define ST_ARMSX      111
#define ST_ARMSY      172
#define ST_ARMSBGX    104
#define ST_ARMSBGY    168
#define ST_ARMSXSPACE 12
#define ST_ARMSYSPACE 10

// Frags pos.
#define ST_FRAGSX     138
#define ST_FRAGSY     171
#define ST_FRAGSWIDTH 2

// Key icon positions.
#define ST_KEY0WIDTH  8
#define ST_KEY0HEIGHT 5
#define ST_KEY0X      239
#define ST_KEY0Y      171
#define ST_KEY1WIDTH  ST_KEY0WIDTH
#define ST_KEY1X      239
#define ST_KEY1Y      181
#define ST_KEY2WIDTH  ST_KEY0WIDTH
#define ST_KEY2X      239
#define ST_KEY2Y      191

// pistol
#define ST_WEAPON0X 110
#define ST_WEAPON0Y 172

// shotgun
#define ST_WEAPON1X 122
#define ST_WEAPON1Y 172

// chain gun
#define ST_WEAPON2X 134
#define ST_WEAPON2Y 172

// missile launcher
#define ST_WEAPON3X 110
#define ST_WEAPON3Y 181

// plasma gun
#define ST_WEAPON4X 122
#define ST_WEAPON4Y 181

// bfg
#define ST_WEAPON5X 134
#define ST_WEAPON5Y 181

// WPNS title
#define ST_WPNSX 109
#define ST_WPNSY 191

// DETH title
#define ST_DETHX 109
#define ST_DETHY 191

#define ST_MSGTEXTX 0
#define ST_MSGTEXTY 0
// Dimensions given in characters.
#define ST_MSGWIDTH 52
// Or shall I say, in lines?
#define ST_MSGHEIGHT 1

#define ST_OUTTEXTX 0
#define ST_OUTTEXTY 6

// Width, in characters again.
#define ST_OUTWIDTH 52
// Height, in lines.
#define ST_OUTHEIGHT 1

#define ST_MAPTITLEX (SCREENWIDTH - ST_MAPWIDTH * ST_CHATFONTWIDTH)

#define ST_MAPTITLEY 0
#define ST_MAPHEIGHT 1

// graphics are drawn to a backing screen and blitted to the real screen
pixel_t *st_backing_screen;

status_bar_t *status_bar;

// main player in game
static player_t *plyr;

// ST_Start() has just been called
static boolean st_firsttime;

// lump number for PLAYPAL
static int lu_palette;

// used for timing
static unsigned int st_clock;

// used for making messages go away
static int st_msgcounter = 0;

// used when in chat
static st_chatstateenum_t st_chatstate;

// whether in automap or first-person
static st_stateenum_t st_gamestate;

// whether left-side main status bar is active
static boolean st_statusbaron;

// whether status bar chat is active
static boolean st_chat;

// value of st_chat before message popped up
static boolean st_oldchat;

// whether chat window has the cursor on
static boolean st_cursoron;

// !deathmatch
static boolean st_notdeathmatch;

// !deathmatch && st_statusbaron
static boolean st_armson;

// !deathmatch
static boolean st_fragson;

// main bar left
static patch_t *sbar;

// main bar right, for doom 1.0
static patch_t *sbarr;

// 0-9, tall numbers
static patch_t *tallnum[10];

// tall % sign
static patch_t *tallpercent;

// 0-9, short, yellow (,different!) numbers
static patch_t *shortnum[10];

// 3 key-cards, 3 skulls
static patch_t *keys[NUMCARDS];

// face status patches
static patch_t *faces[ST_NUMFACES];

// face background
static patch_t *faceback;

// main bar right
static patch_t *armsbg;

// weapon ownership patches
static patch_t *arms[6][2];

/* This widget is the large ammo counter 
 * on the bottom left of the screen that
 * displays the ammo count of the currently
 * selected weapons.
 */
static widget_number_t *widget_ammo_current_counter;

// in deathmatch only, summary of frags stats
static widget_number_t w_frags;

// health widget
static widget_number_t *widget_health;

static widget_fraction_t *widget_ammo_bullet_counter;
static widget_fraction_t *widget_ammo_shell_counter;
static widget_fraction_t *widget_ammo_rocket_counter;
static widget_fraction_t *widget_ammo_cell_counter;

// arms background
static st_binicon_t w_armsbg;


// weapon ownership widgets
static st_multicon_t w_arms[6];

// face status widget
static st_multicon_t w_faces;

// keycard widgets
static st_multicon_t w_keyboxes[3];

// armor widget
static widget_number_t *widget_armor;

// ammo widgets
static widget_number_t w_ammo[4];

// max ammo widgets
static widget_number_t w_maxammo[4];


// number of frags so far in deathmatch
static int st_fragscount;

// used to use appopriately pained face
static int st_oldhealth = -1;

// used for evil grin
static boolean oldweaponsowned[NUMWEAPONS];

// count until face changes
static int st_facecount = 0;

// current face index, used by w_faces
static int st_faceindex = 0;

// holds key-type for each key box on bar
static int keyboxes[3];

// a random number per tick
static int st_randomnumber;

cheatseq_t cheat_mus = CHEAT("idmus", 2);
cheatseq_t cheat_god = CHEAT("iddqd", 0);
cheatseq_t cheat_ammo = CHEAT("idkfa", 0);
cheatseq_t cheat_ammonokey = CHEAT("idfa", 0);
cheatseq_t cheat_noclip = CHEAT("idspispopd", 0);

cheatseq_t cheat_powerup[7] = {
    CHEAT("idbeholdv", 0),
    CHEAT("idbeholds", 0),
    CHEAT("idbeholdi", 0),
    CHEAT("idbeholdr", 0),
    CHEAT("idbeholda", 0),
    CHEAT("idbeholdl", 0),
    CHEAT("idbehold", 0),
};

cheatseq_t cheat_choppers = CHEAT("idchoppers", 0);
cheatseq_t cheat_clev = CHEAT("idclev", 2);
cheatseq_t cheat_mypos = CHEAT("idmypos", 0);


//
// STATUS BAR CODE
//
void ST_Stop(void);

// TODO: return st_backing_screen
void *StatusBar_CreateBackground()
{
    log_debug("StatusBar_CreateBackground(): Creating status bar background");
    // TODO: reduce the scope of st_backing_screen.
    st_backing_screen =
        (pixel_t *) Z_Malloc(ST_WIDTH * ST_HEIGHT * sizeof(*st_backing_screen), PU_STATIC, 0);
}


status_bar_t *StatusBar_CreateStatusBar(void)
{
    log_debug("StatusBar_CreateStatusBar(): Creating status bar");
    status_bar_t *status_bar = Z_Malloc(sizeof(status_bar_t), PU_STATIC, 0);
    status_bar->height = 32;
    status_bar->width = SCREENWIDTH;
    status_bar->x = 0;
    status_bar->y = SCREENHEIGHT - status_bar->height;

    // TODO:
    // status_bar->backing_screen = StatusBar_CreateBackground()
    StatusBar_CreateBackground();

    return status_bar;
}

void StatusBar_DrawBackground(status_bar_t *status_bar)
{
    int x = status_bar->x;
    int y = status_bar->y;
    int width = status_bar->width;
    int height = status_bar->height;

    // TODO: return early
    if (st_statusbaron)
    {
        V_UseBuffer(st_backing_screen);

        V_DrawPatch(x, 0, sbar);

        // TOOD: remove (we don't care about DOOM 1.0)
        // draw right side of bar if needed (Doom 1.0)
        if (sbarr)
        {
            V_DrawPatch(ST_ARMSBGX, 0, sbarr);
        }

        // TODO: Make the frag counter it's own widget and or function
        if (netgame)
        {
            const int frag_widget_x = 143;
            const int frag_widget_y = 169;
            V_DrawPatch(frag_widget_x, frag_widget_y, faceback);
        }

        V_RestoreBuffer();

        V_CopyRect(x, 0, st_backing_screen, width, height, x, y);
    }
}


// Respond to keyboard input events,
//  intercept cheats.
boolean ST_Responder(event_t *ev)
{
    int i;

    // Filter automap on/off.
    if (ev->type == ev_keyup && ((ev->data1 & 0xffff0000) == AM_MSGHEADER))
    {
        switch (ev->data1)
        {
            case AM_MSGENTERED:
                st_gamestate = AutomapState;
                st_firsttime = true;
                break;

            case AM_MSGEXITED:
                //	fprintf(stderr, "AM exited\n");
                st_gamestate = FirstPersonState;
                break;
        }
    }

    // if a user keypress...
    else if (ev->type == ev_keydown)
    {
        if (!netgame && gameskill != sk_nightmare)
        {
            // 'dqd' cheat for toggleable god mode
            if (cht_CheckCheat(&cheat_god, ev->data2))
            {
                plyr->cheats ^= CF_GODMODE;
                if (plyr->cheats & CF_GODMODE)
                {
                    if (plyr->mo)
                    {
                        plyr->mo->health = GOD_MODE_HEALTH;
                    }
                    plyr->health = GOD_MODE_HEALTH;
                    plyr->message = STSTR_DQDON;
                }
                else
                {
                    plyr->message = STSTR_DQDOFF;
                }
            }
            // 'fa' cheat for killer fucking arsenal
            else if (cht_CheckCheat(&cheat_ammonokey, ev->data2))
            {
                plyr->armorpoints = IDFA_ARMOR;
                plyr->armortype = IDFA_ARMOR_CLASS;

                for (i = 0; i < NUMWEAPONS; i++)
                {
                    plyr->weaponowned[i] = true;
                }

                for (i = 0; i < NUMAMMO; i++)
                {
                    plyr->ammo[i] = plyr->maxammo[i];
                }

                plyr->message = STSTR_FAADDED;
            }
            // 'kfa' cheat for key full ammo
            else if (cht_CheckCheat(&cheat_ammo, ev->data2))
            {
                plyr->armorpoints = IDKFA_ARMOR;
                plyr->armortype = IDKFA_ARMOR_CLASS;

                for (i = 0; i < NUMWEAPONS; i++)
                {
                    plyr->weaponowned[i] = true;
                }

                for (i = 0; i < NUMAMMO; i++)
                {
                    plyr->ammo[i] = plyr->maxammo[i];
                }

                for (i = 0; i < NUMCARDS; i++)
                {
                    plyr->cards[i] = true;
                }

                plyr->message = STSTR_KFAADDED;
            }
            // 'mus' cheat for changing music
            else if (cht_CheckCheat(&cheat_mus, ev->data2))
            {

                char buf[3];
                int musnum;

                plyr->message = STSTR_MUS;
                cht_GetParam(&cheat_mus, buf);

                musnum = mus_e1m1 + (buf[0] - '1') * 9 + (buf[1] - '1');

                if (((buf[0] - '1') * 9 + buf[1] - '1') > 31)
                {
                    plyr->message = STSTR_NOMUS;
                }
                else
                {
                    S_ChangeMusic(musnum, 1);
                }
            }
            else if (cht_CheckCheat(&cheat_noclip, ev->data2))
            {
                // Noclip cheat.

                plyr->cheats ^= CF_NOCLIP;

                if (plyr->cheats & CF_NOCLIP)
                {
                    plyr->message = STSTR_NCON;
                }
                else
                {
                    plyr->message = STSTR_NCOFF;
                }
            }
            // 'behold?' power-up cheats
            for (i = 0; i < 6; i++)
            {
                if (cht_CheckCheat(&cheat_powerup[i], ev->data2))
                {
                    if (!plyr->powers[i])
                    {
                        P_GivePower(plyr, i);
                    }
                    else if (i != pw_strength)
                    {
                        plyr->powers[i] = 1;
                    }
                    else
                    {
                        plyr->powers[i] = 0;
                    }

                    plyr->message = STSTR_BEHOLDX;
                }
            }

            // 'behold' power-up menu
            if (cht_CheckCheat(&cheat_powerup[6], ev->data2))
            {
                plyr->message = STSTR_BEHOLD;
            }
            // 'choppers' invulnerability & chainsaw
            else if (cht_CheckCheat(&cheat_choppers, ev->data2))
            {
                plyr->weaponowned[wp_chainsaw] = true;
                plyr->powers[pw_invulnerability] = true;
                plyr->message = STSTR_CHOPPERS;
            }
            // 'mypos' for player position
            else if (cht_CheckCheat(&cheat_mypos, ev->data2))
            {
                static char buf[ST_MSGWIDTH];
                M_snprintf(
                    buf,
                    sizeof(buf),
                    "ang=0x%x;x,y=(0x%x,0x%x)",
                    players[consoleplayer].mo->angle,
                    players[consoleplayer].mo->x,
                    players[consoleplayer].mo->y);
                plyr->message = buf;
            }
        }

        // 'clev' change-level cheat
        if (!netgame && cht_CheckCheat(&cheat_clev, ev->data2))
        {
            char buf[3];
            int epsd;
            int map;

            cht_GetParam(&cheat_clev, buf);

            epsd = buf[0] - '0';
            map = buf[1] - '0';

            // Catch invalid maps.
            if (epsd < 1)
            {
                return false;
            }
            if (epsd >= 4)
            {
                return false;
            }
            if (map < 1)
            {
                return false;
            }
            if (map > 9)
            {
                return false;
            }

            // So be it.
            plyr->message = STSTR_CLEV;
            G_DeferedInitNew(gameskill, epsd, map);
        }
    }
    return false;
}


int ST_calcPainOffset(void)
{
    int health;
    static int lastcalc;
    static int oldhealth = -1;

    health = plyr->health > 100 ? 100 : plyr->health;

    if (health != oldhealth)
    {
        lastcalc = ST_FACESTRIDE * (((100 - health) * ST_NUMPAINFACES) / 101);
        oldhealth = health;
    }
    return lastcalc;
}


//
// This is a not-very-pretty routine which handles
//  the face states and their timing.
// the precedence of expressions is:
//  dead > evil grin > turned head > straight ahead
//
void ST_updateFaceWidget(void)
{
    int i;
    angle_t badguyangle;
    angle_t diffang;
    static int lastattackdown = -1;
    static int priority = 0;
    boolean doevilgrin;

    if (priority < 10)
    {
        // dead
        if (!plyr->health)
        {
            priority = 9;
            st_faceindex = ST_DEADFACE;
            st_facecount = 1;
        }
    }

    if (priority < 9)
    {
        if (plyr->bonuscount)
        {
            // picking up bonus
            doevilgrin = false;

            for (i = 0; i < NUMWEAPONS; i++)
            {
                if (oldweaponsowned[i] != plyr->weaponowned[i])
                {
                    doevilgrin = true;
                    oldweaponsowned[i] = plyr->weaponowned[i];
                }
            }
            if (doevilgrin)
            {
                // evil grin if just picked up weapon
                priority = 8;
                st_facecount = ST_EVILGRINCOUNT;
                st_faceindex = ST_calcPainOffset() + ST_EVILGRINOFFSET;
            }
        }
    }

    if (priority < 8)
    {
        if (plyr->damagecount && plyr->attacker && plyr->attacker != plyr->mo)
        {
            // being attacked
            priority = 7;

            if (plyr->health - st_oldhealth > ST_MUCHPAIN)
            {
                st_facecount = ST_TURNCOUNT;
                st_faceindex = ST_calcPainOffset() + ST_OUCHOFFSET;
            }
            else
            {
                badguyangle =
                    R_PointToAngle2(plyr->mo->x, plyr->mo->y, plyr->attacker->x, plyr->attacker->y);

                if (badguyangle > plyr->mo->angle)
                {
                    // whether right or left
                    diffang = badguyangle - plyr->mo->angle;
                    i = diffang > ANG180;
                }
                else
                {
                    // whether left or right
                    diffang = plyr->mo->angle - badguyangle;
                    i = diffang <= ANG180;
                } // confusing, aint it?


                st_facecount = ST_TURNCOUNT;
                st_faceindex = ST_calcPainOffset();

                if (diffang < ANG45)
                {
                    // head-on
                    st_faceindex += ST_RAMPAGEOFFSET;
                }
                else if (i)
                {
                    // turn face right
                    st_faceindex += ST_TURNOFFSET;
                }
                else
                {
                    // turn face left
                    st_faceindex += ST_TURNOFFSET + 1;
                }
            }
        }
    }

    if (priority < 7)
    {
        // getting hurt because of your own damn stupidity
        if (plyr->damagecount)
        {
            if (plyr->health - st_oldhealth > ST_MUCHPAIN)
            {
                priority = 7;
                st_facecount = ST_TURNCOUNT;
                st_faceindex = ST_calcPainOffset() + ST_OUCHOFFSET;
            }
            else
            {
                priority = 6;
                st_facecount = ST_TURNCOUNT;
                st_faceindex = ST_calcPainOffset() + ST_RAMPAGEOFFSET;
            }
        }
    }

    if (priority < 6)
    {
        // rapid firing
        if (plyr->attackdown)
        {
            if (lastattackdown == -1)
            {
                lastattackdown = ST_RAMPAGEDELAY;
            }
            else if (!--lastattackdown)
            {
                priority = 5;
                st_faceindex = ST_calcPainOffset() + ST_RAMPAGEOFFSET;
                st_facecount = 1;
                lastattackdown = 1;
            }
        }
        else
        {
            lastattackdown = -1;
        }
    }

    if (priority < 5)
    {
        // invulnerability
        if ((plyr->cheats & CF_GODMODE) || plyr->powers[pw_invulnerability])
        {
            priority = 4;

            st_faceindex = ST_GODFACE;
            st_facecount = 1;
        }
    }

    // look left or look right if the facecount has timed out
    if (!st_facecount)
    {
        st_faceindex = ST_calcPainOffset() + (st_randomnumber % 3);
        st_facecount = ST_STRAIGHTFACECOUNT;
        priority = 0;
    }

    st_facecount--;
}

void ST_updateWidgets(void)
{
    static int largeammo = 1994; // means "n/a"
    // must redirect the pointer if the ready weapon has changed.
    if (weaponinfo[plyr->readyweapon].ammo == am_noammo)
    {
        widget_ammo_current_counter->value = &largeammo;
    }
    else
    {
        widget_ammo_current_counter->value = &plyr->ammo[weaponinfo[plyr->readyweapon].ammo];
    }
    widget_ammo_current_counter->data = plyr->readyweapon;

    // update keycard multiple widgets
    for (int i = 0; i < 3; i++)
    {
        keyboxes[i] = plyr->cards[i] ? i : -1;

        if (plyr->cards[i + 3])
        {
            keyboxes[i] = i + 3;
        }
    }

    // refresh everything if this is him coming back to life
    ST_updateFaceWidget();

    // used by the w_armsbg widget
    st_notdeathmatch = !deathmatch;

    // used by w_arms[] widgets
    st_armson = st_statusbaron && !deathmatch;

    // used by w_frags widget
    st_fragson = deathmatch && st_statusbaron;
    st_fragscount = 0;

    for (int i = 0; i < MAXPLAYERS; i++)
    {
        if (i != consoleplayer)
        {
            st_fragscount += plyr->frags[i];
        }
        else
        {
            st_fragscount -= plyr->frags[i];
        }
    }

    // get rid of chat window if up because of message
    if (!--st_msgcounter)
    {
        st_chat = st_oldchat;
    }
}

void ST_Ticker(void)
{

    st_clock++;
    st_randomnumber = M_Random();
    ST_updateWidgets();
    st_oldhealth = plyr->health;
}

static int st_palette = 0;

void ST_doPaletteStuff(void)
{

    int palette;
    byte *pal;
    int cnt;
    int bzc;

    cnt = plyr->damagecount;

    if (plyr->powers[pw_strength])
    {
        // slowly fade the berzerk out
        bzc = 12 - (plyr->powers[pw_strength] >> 6);

        if (bzc > cnt)
        {
            cnt = bzc;
        }
    }

    if (cnt)
    {
        palette = (cnt + 7) >> 3;

        if (palette >= NUMREDPALS)
        {
            palette = NUMREDPALS - 1;
        }

        palette += STARTREDPALS;
    }

    else if (plyr->bonuscount)
    {
        palette = (plyr->bonuscount + 7) >> 3;

        if (palette >= NUMBONUSPALS)
        {
            palette = NUMBONUSPALS - 1;
        }

        palette += STARTBONUSPALS;
    }

    else if (plyr->powers[pw_ironfeet] > 4 * 32 || plyr->powers[pw_ironfeet] & 8)
    {
        palette = RADIATIONPAL;
    }
    else
    {
        palette = 0;
    }

    if (palette != st_palette)
    {
        st_palette = palette;
        pal = (byte *) W_CacheLumpNum(lu_palette, PU_CACHE) + palette * 768;
        I_SetPalette(pal);
    }
}

void ST_drawWidgets(boolean refresh)
{
    int i;

    // TODO: Why are we passing refresh all the way down to every function
    // in the STWidget library? There's like one use of it. Can't we just
    // return early if refresh = false?

    // used by w_arms[] widgets
    st_armson = st_statusbaron && !deathmatch;

    // used by w_frags widget
    st_fragson = deathmatch && st_statusbaron;

    STWidget_DrawNumberWidget(widget_ammo_current_counter, refresh);

    // Draw the ammo counters
    // TODO: Move to separate Draw function
    STWidget_DrawFractionWidget(widget_ammo_bullet_counter, refresh);
    STWidget_DrawFractionWidget(widget_ammo_shell_counter, refresh);
    STWidget_DrawFractionWidget(widget_ammo_rocket_counter, refresh);
    STWidget_DrawFractionWidget(widget_ammo_cell_counter, refresh);

    STWidget_DrawNumberWidget(widget_health, refresh);
    STWidget_DrawNumberWidget(widget_armor, refresh);

    STlib_updateBinIcon(&w_armsbg, refresh);

    for (i = 0; i < 6; i++)
    {
        STlib_updateMultIcon(&w_arms[i], refresh);
    }

    STlib_updateMultIcon(&w_faces, refresh);

    for (i = 0; i < 3; i++)
    {
        STlib_updateMultIcon(&w_keyboxes[i], refresh);
    }

    STlib_updateNum(&w_frags, refresh);
}
//TODO:
void StatusBar_DrawStatusBar(status_bar_t *status_bar)
{
    StatusBar_DrawBackground(status_bar);
    //StatusBar_DrawWidgets();
}


void ST_doRefresh(void)
{

    st_firsttime = false;

    // draw status bar background to off-screen buff
    StatusBar_DrawStatusBar(status_bar);

    // and refresh all widgets
    ST_drawWidgets(true);
}

void ST_diffDraw(void)
{
    // update all widgets
    ST_drawWidgets(false);
}

void ST_Drawer(boolean fullscreen, boolean refresh)
{

    st_statusbaron = (!fullscreen) || automapactive;
    st_firsttime = st_firsttime || refresh;

    // Do red-/gold-shifts from damage/items
    ST_doPaletteStuff();

    // If just after ST_Start(), refresh all
    if (st_firsttime)
    {
        ST_doRefresh();
        // Otherwise, update as little as possible
    }
    else
    {
        ST_diffDraw();
    }
}

typedef void (*load_callback_t)(const char *lumpname, patch_t **variable);

// Iterates through all graphics to be loaded or unloaded, along with
// the variable they use, invoking the specified callback function.

static void ST_loadUnloadGraphics(load_callback_t callback)
{

    int i;
    int j;
    int facenum;

    char namebuf[9];

    // Load the numbers, tall and short
    for (i = 0; i < 10; i++)
    {
        M_snprintf(namebuf, 9, "STTNUM%d", i);
        callback(namebuf, &tallnum[i]);

        M_snprintf(namebuf, 9, "STYSNUM%d", i);
        callback(namebuf, &shortnum[i]);
    }

    // Load percent key.
    //Note: why not load STMINUS here, too?

    callback("STTPRCNT", &tallpercent);

    // key cards
    for (i = 0; i < NUMCARDS; i++)
    {
        M_snprintf(namebuf, 9, "STKEYS%d", i);
        callback(namebuf, &keys[i]);
    }

    // arms background
    callback("STARMS", &armsbg);

    // arms ownership widgets
    for (i = 0; i < 6; i++)
    {
        M_snprintf(namebuf, 9, "STGNUM%d", i + 2);

        // gray #
        callback(namebuf, &arms[i][0]);

        // yellow #
        arms[i][1] = shortnum[i + 2];
    }

    // face backgrounds for different color players
    M_snprintf(namebuf, 9, "STFB%d", consoleplayer);
    callback(namebuf, &faceback);

    // status bar background bits
    if (W_CheckNumForName("STBAR") >= 0)
    {
        callback("STBAR", &sbar);
        sbarr = NULL;
    }
    else
    {
        callback("STMBARL", &sbar);
        callback("STMBARR", &sbarr);
    }

    // face states
    facenum = 0;
    for (i = 0; i < ST_NUMPAINFACES; i++)
    {
        for (j = 0; j < ST_NUMSTRAIGHTFACES; j++)
        {
            M_snprintf(namebuf, 9, "STFST%d%d", i, j);
            callback(namebuf, &faces[facenum]);
            ++facenum;
        }
        M_snprintf(namebuf, 9, "STFTR%d0", i); // turn right
        callback(namebuf, &faces[facenum]);
        ++facenum;
        M_snprintf(namebuf, 9, "STFTL%d0", i); // turn left
        callback(namebuf, &faces[facenum]);
        ++facenum;
        M_snprintf(namebuf, 9, "STFOUCH%d", i); // ouch!
        callback(namebuf, &faces[facenum]);
        ++facenum;
        M_snprintf(namebuf, 9, "STFEVL%d", i); // evil grin ;)
        callback(namebuf, &faces[facenum]);
        ++facenum;
        M_snprintf(namebuf, 9, "STFKILL%d", i); // pissed off
        callback(namebuf, &faces[facenum]);
        ++facenum;
    }

    callback("STFGOD0", &faces[facenum]);
    ++facenum;
    callback("STFDEAD0", &faces[facenum]);
    ++facenum;
}

static void ST_loadCallback(const char *lumpname, patch_t **variable)
{
    *variable = W_CacheLumpName(lumpname, PU_STATIC);
}

void ST_initData(void)
{

    int i;

    st_firsttime = true;
    plyr = &players[consoleplayer];

    st_clock = 0;
    st_chatstate = StartChatState;
    st_gamestate = FirstPersonState;

    st_statusbaron = true;
    st_oldchat = st_chat = false;
    st_cursoron = false;

    st_faceindex = 0;
    st_palette = -1;

    st_oldhealth = -1;

    for (i = 0; i < NUMWEAPONS; i++)
    {
        oldweaponsowned[i] = plyr->weaponowned[i];
    }

    for (i = 0; i < 3; i++)
    {
        keyboxes[i] = -1;
    }

    STlib_init();
}

/**
 * Create the statusbar widget for displaying the ammo count
 * for the currently selected weapon.
 */
void StatusBar_CreateCurrentAmmoCountWidget()
{
    log_trace("StatusBar_CreateCurrentAmmoCountWidget(): Creating ammo counter");

    const int x = 44;
    const int y = 171;
    const int num_digits = 3;

    int *value = &plyr->ammo[weaponinfo[plyr->readyweapon].ammo];

    widget_ammo_current_counter =
        STWidget_CreateNumberWidget(x, y, num_digits, value, &st_statusbaron, tallnum, NULL);

    // the last weapon type
    widget_ammo_current_counter->data = plyr->readyweapon;
}

void StatusBar_CreateAmmoCounterWidgets()
{
    const int x = 288;

    // The ammo counter for "BULL"
    widget_ammo_bullet_counter = STWidget_CreateFractionWidget(
        x, 173, &plyr->ammo[0], &plyr->maxammo[0], &st_statusbaron, shortnum);

    // The ammo counter for "SHELL"
    widget_ammo_shell_counter = STWidget_CreateFractionWidget(
        x, 179, &plyr->ammo[1], &plyr->maxammo[1], &st_statusbaron, shortnum);

    // The ammo counter for "RCKT"
    widget_ammo_rocket_counter = STWidget_CreateFractionWidget(
        x, 191, &plyr->ammo[2], &plyr->maxammo[2], &st_statusbaron, shortnum);

    // The ammo counter for "CELL"
    widget_ammo_cell_counter = STWidget_CreateFractionWidget(
        x, 185, &plyr->ammo[3], &plyr->maxammo[3], &st_statusbaron, shortnum);
}

/**
 * Create the statusbar widget for displaying health 
 */
void StatusBar_CreateHealthWidget()
{
    log_trace("StatusBar_CreateHealthWidget(): Creating health percentage");

    const int num_digits = 3;
    const int x = 90;
    const int y = 171;

    widget_health = STWidget_CreateNumberWidget(
        x, y, num_digits, &plyr->health, &st_statusbaron, tallnum, tallpercent);
}
/**
 * Create the statusbar widget for displaying armor
 * Original comment: "should be colored later"
 */
void StatusBar_CreateArmorWidget()
{
    log_trace("StatusBar_CreateArmorWidget(): Creating armor percentage");

    const int num_digits = 3;
    const int x = 221;
    const int y = 171;

    widget_armor = STWidget_CreateNumberWidget(
        x, y, num_digits, &plyr->armorpoints, &st_statusbaron, tallnum, tallpercent);
}

void ST_createWidgets(void)
{

    StatusBar_CreateCurrentAmmoCountWidget();
    StatusBar_CreateHealthWidget();
    StatusBar_CreateArmorWidget();
    StatusBar_CreateAmmoCounterWidgets();


    // arms background
    STlib_initBinIcon(
        &w_armsbg, ST_ARMSBGX, ST_ARMSBGY, armsbg, &st_notdeathmatch, &st_statusbaron);

    // weapons owned
    for (int i = 0; i < 6; i++)
    {
        STlib_initMultIcon(
            &w_arms[i],
            ST_ARMSX + (i % 3) * ST_ARMSXSPACE,
            ST_ARMSY + (i / 3) * ST_ARMSYSPACE,
            arms[i],
            &plyr->weaponowned[i + 1],
            &st_armson);
    }

    // frags sum
    STlib_initNum(
        &w_frags, ST_FRAGSX, ST_FRAGSY, tallnum, &st_fragscount, &st_fragson, ST_FRAGSWIDTH);

    // faces
    STlib_initMultIcon(&w_faces, ST_FACESX, ST_FACESY, faces, &st_faceindex, &st_statusbaron);

    // keyboxes 0-2
    STlib_initMultIcon(&w_keyboxes[0], ST_KEY0X, ST_KEY0Y, keys, &keyboxes[0], &st_statusbaron);

    STlib_initMultIcon(&w_keyboxes[1], ST_KEY1X, ST_KEY1Y, keys, &keyboxes[1], &st_statusbaron);

    STlib_initMultIcon(&w_keyboxes[2], ST_KEY2X, ST_KEY2Y, keys, &keyboxes[2], &st_statusbaron);
}

static boolean st_stopped = true;


void ST_Start(void)
{

    if (!st_stopped)
    {
        ST_Stop();
    }

    ST_initData();
    status_bar = StatusBar_CreateStatusBar();
    ST_createWidgets();
    st_stopped = false;
}

void ST_Stop(void)
{
    if (st_stopped)
    {
        return;
    }
    // TOOD: When we replace this function, it should free() all widgets
    // and perhaps by called via atexit
    I_SetPalette(W_CacheLumpNum(lu_palette, PU_CACHE));

    st_stopped = true;
}

void ST_Init(void)
{
    lu_palette = W_GetNumForName("PLAYPAL");
    ST_loadUnloadGraphics(ST_loadCallback);
}
