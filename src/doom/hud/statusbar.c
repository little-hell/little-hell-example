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
#include "../../engine/graphics/widget.h"
#include "../../engine/graphics/drawable.h"

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

pixel_t *st_backing_screen; 


StatusBar *status_bar;

// main player in game
static player_t *player;

// ST_Start() has just been called
static boolean st_firsttime;

// lump number for PLAYPAL
static int lu_palette;

// used for timing
static unsigned int st_clock;

// used for making messages go away
static int st_msgcounter = 0;

// TODO: unused 
// used when in chat
//static st_chatstateenum_t st_chatstate;

// TODO: unused 
// whether in automap or first-person
//static st_stateenum_t st_gamestate;

// whether left-side main status bar is active
static boolean status_bar_enabled;

// whether status bar chat is active
static boolean st_chat;

// value of st_chat before message popped up
static boolean st_oldchat;

// whether chat window has the cursor on
static boolean st_cursoron;

// !deathmatch
static boolean st_notdeathmatch;

// !deathmatch && status_bar_enabled
static boolean st_armson;

// ----------------
// Graphics patches
// ----------------

// TOOD: We really need to mantain an array (or something) of all the 
// patches we're dealing with. Ideally a hash map with the patch name
// and it's address. There are a whopping 12 patch_t variables declared 
// at the top of this file, it's ugly.

// main bar left
static patch_t *sbar;

// main bar right, for doom 1.0
static patch_t *sbarr;

// 0-9, tall numbers
static patch_t *tallnum[10];

// tall % sign
static patch_t *percent_symbol_large;

// tall % sign
static patch_t *minus_symbol;

// 0-9, short, yellow (,different!) numbers
static patch_t *shortnum[10];


// ---------
// Drawables
// ---------

// This the large ammo counter for the currently selected weapon
static Drawable *large_ammo_counter;

// Show the number of frags (kills). Used only in deathmatch 
static Drawable *frag_counter;

// Show the players current health
static Drawable *health_counter;

// Show the players current armor 
static Drawable *armor_counter;

// Smaller ammo counters for each ammo type, used on the 
// right-hand side of the status bar.
static Drawable *widget_ammo_counter_bullets;
static Drawable *widget_ammo_counter_shells;
static Drawable *widget_ammo_counter_rockets;
static Drawable *widget_ammo_counter_cells;

// -------------------------------------
// TODO: reduce the scope of these into 
// drawable that handles drawing the 
// arms (weapon inventory).
// -------------------------------------

// arms background widget
static st_binicon_t w_armsbg;
// arms background patch/texture
static patch_t *armsbg;
// weapon ownership patches
static patch_t *arms[6][2];

// weapon ownership widgets
static st_multicon_t w_arms[6];

// ------------------------------------



// ----------------------------------------
// TODO: reduce the scope of these into the 
// drawable that handles drawing DOOM guy.
// ----------------------------------------

// used to use appopriately pained face
static int st_oldhealth = -1;

// face status widget
static st_multicon_t w_faces;

// face background
static patch_t *faceback;

// count until face changes
static int st_facecount = 0;

// current face index, used by w_faces
static int st_faceindex = 0;

// for determining if DOOM guy grins when he gets
// all weapons?
static boolean oldweaponsowned[NUMWEAPONS];

// a random number per tick
static int st_randomnumber;

// face status patches
static patch_t *faces[ST_NUMFACES];
// ------------------------------------


// ---------------------------------------
// TODO: reduce the scope of this into the 
// drawable that handles the keyboxes

// Patches/textures for 3 key-cards, 3 skulls
static patch_t *keys[NUMCARDS];

// keycard widgets
static st_multicon_t w_keyboxes[3];

// holds key-type for each key box on bar
static int keyboxes[3];
// ------------------------------------


// ------------------------------------
// TODO: reduce the scope of this into
// the drawable for the frag counter.

// Frags pos.
#define ST_FRAGSX     138
#define ST_FRAGSY     171
#define ST_FRAGSWIDTH 2

// number of frags so far in deathmatch
static int st_fragscount;

// !deathmatch
static boolean st_fragson;

// ------------------------------------

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

StatusBar *status_bar;

StatusBar *status_bar_create(boolean *enabled)
{
    log_debug("status_bar_create(): Creating status bar");

    status_bar = Z_Malloc(sizeof(StatusBar), PU_STATIC, 0);

    status_bar->drawable = drawable_create(drawable);

    return status_bar;
}

void status_bar_draw_background(StatusBar *status_bar, pixel_t *screen)
{
    if (status_bar->drawable == NULL || status_bar->drawable->options == NULL || &status_bar->drawable->options->x == NULL) {
        log_fatal("status_bar_draw_background(): StatusBar has not been properly initialized! Can not draw.");
        system_exit();
    }

    int x = status_bar->drawable->options->x;
    int y = status_bar->drawable->options->y;
    //int width = status_bar->width;
    //int height = status_bar->height;

    // TODO: return early
    if (status_bar_enabled)
    {
        
        //V_UseBuffer(status_bar->drawable->screen);

        V_DrawPatch(x, y, sbar);

        // TOOD: remove (we don't care about DOOM 1.0)
        // draw right side of bar if needed (Doom 1.0)
        if (sbarr)
        {
            V_DrawPatch(ST_ARMSBGX, y, sbarr);
        }

        // TODO: Make the frag counter it's own widget and or function
        if (netgame)
        {
            const int frag_widget_x = 143;
            const int frag_widget_y = 169;
            V_DrawPatch(frag_widget_x, frag_widget_y, faceback);
        }

        //V_RestoreBuffer();

        //V_CopyRect(x, 0, status_bar->drawable->screen, width, height, x, y);
    }
}


// Respond to keyboard input events,
//  intercept cheats.
boolean ST_Responder(event_t *ev)
{
    int i;

    // Filter automap on/off.
    // TODO: stop fiddling bits
    if (ev->type == ev_keyup && ((ev->data1 & 0xffff0000) == AM_MSGHEADER))
    {
        switch (ev->data1)
        {
            case AM_MSGENTERED:
                log_debug("Entering AutoMap");
                // Unused?
                //st_gamestate = AutomapState;
                st_firsttime = true;
                break;

            case AM_MSGEXITED:
                log_debug("Exiting AutoMap");
                // Unused? 
                //st_gamestate = FirstPersonState;
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
                player->cheats ^= CF_GODMODE;
                if (player->cheats & CF_GODMODE)
                {
                    if (player->mo)
                    {
                        player->mo->health = GOD_MODE_HEALTH;
                    }
                    player->health = GOD_MODE_HEALTH;
                    player->message = STSTR_DQDON;
                }
                else
                {
                    player->message = STSTR_DQDOFF;
                }
            }
            // 'fa' cheat for killer fucking arsenal
            else if (cht_CheckCheat(&cheat_ammonokey, ev->data2))
            {
                player->armorpoints = IDFA_ARMOR;
                player->armortype = IDFA_ARMOR_CLASS;

                for (i = 0; i < NUMWEAPONS; i++)
                {
                    player->weaponowned[i] = true;
                }

                for (i = 0; i < NUMAMMO; i++)
                {
                    player->ammo[i] = player->maxammo[i];
                }

                player->message = STSTR_FAADDED;
            }
            // 'kfa' cheat for key full ammo
            else if (cht_CheckCheat(&cheat_ammo, ev->data2))
            {
                player->armorpoints = IDKFA_ARMOR;
                player->armortype = IDKFA_ARMOR_CLASS;

                for (i = 0; i < NUMWEAPONS; i++)
                {
                    player->weaponowned[i] = true;
                }

                for (i = 0; i < NUMAMMO; i++)
                {
                    player->ammo[i] = player->maxammo[i];
                }

                for (i = 0; i < NUMCARDS; i++)
                {
                    player->cards[i] = true;
                }

                player->message = STSTR_KFAADDED;
            }
            // 'mus' cheat for changing music
            else if (cht_CheckCheat(&cheat_mus, ev->data2))
            {

                char buf[3];
                int musnum;

                player->message = STSTR_MUS;
                cht_GetParam(&cheat_mus, buf);

                musnum = mus_e1m1 + (buf[0] - '1') * 9 + (buf[1] - '1');

                if (((buf[0] - '1') * 9 + buf[1] - '1') > 31)
                {
                    player->message = STSTR_NOMUS;
                }
                else
                {
                    S_ChangeMusic(musnum, 1);
                }
            }
            else if (cht_CheckCheat(&cheat_noclip, ev->data2))
            {
                // Noclip cheat.

                player->cheats ^= CF_NOCLIP;

                if (player->cheats & CF_NOCLIP)
                {
                    player->message = STSTR_NCON;
                }
                else
                {
                    player->message = STSTR_NCOFF;
                }
            }
            // 'behold?' power-up cheats
            for (i = 0; i < 6; i++)
            {
                if (cht_CheckCheat(&cheat_powerup[i], ev->data2))
                {
                    if (!player->powers[i])
                    {
                        P_GivePower(player, i);
                    }
                    else if (i != pw_strength)
                    {
                        player->powers[i] = 1;
                    }
                    else
                    {
                        player->powers[i] = 0;
                    }

                    player->message = STSTR_BEHOLDX;
                }
            }

            // 'behold' power-up menu
            if (cht_CheckCheat(&cheat_powerup[6], ev->data2))
            {
                player->message = STSTR_BEHOLD;
            }
            // 'choppers' invulnerability & chainsaw
            else if (cht_CheckCheat(&cheat_choppers, ev->data2))
            {
                player->weaponowned[wp_chainsaw] = true;
                player->powers[pw_invulnerability] = true;
                player->message = STSTR_CHOPPERS;
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
                player->message = buf;
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
            player->message = STSTR_CLEV;
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

    health = player->health > 100 ? 100 : player->health;

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
        if (!player->health)
        {
            priority = 9;
            st_faceindex = ST_DEADFACE;
            st_facecount = 1;
        }
    }

    if (priority < 9)
    {
        if (player->bonuscount)
        {
            // picking up bonus
            doevilgrin = false;

            for (i = 0; i < NUMWEAPONS; i++)
            {
                if (oldweaponsowned[i] != player->weaponowned[i])
                {
                    doevilgrin = true;
                    oldweaponsowned[i] = player->weaponowned[i];
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
        if (player->damagecount && player->attacker && player->attacker != player->mo)
        {
            // being attacked
            priority = 7;

            if (player->health - st_oldhealth > ST_MUCHPAIN)
            {
                st_facecount = ST_TURNCOUNT;
                st_faceindex = ST_calcPainOffset() + ST_OUCHOFFSET;
            }
            else
            {
                badguyangle =
                    R_PointToAngle2(player->mo->x, player->mo->y, player->attacker->x, player->attacker->y);

                if (badguyangle > player->mo->angle)
                {
                    // whether right or left
                    diffang = badguyangle - player->mo->angle;
                    i = diffang > ANG180;
                }
                else
                {
                    // whether left or right
                    diffang = player->mo->angle - badguyangle;
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
        if (player->damagecount)
        {
            if (player->health - st_oldhealth > ST_MUCHPAIN)
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
        if (player->attackdown)
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
        if ((player->cheats & CF_GODMODE) || player->powers[pw_invulnerability])
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

static void update_large_ammo_counter(player_t *player, Drawable *ammo_counter) 
{
    // If the player has switched to a weapon that doesn't use ammo
    // (like the fists or chainsaw, set it to a special value so that
    // it doesn't display.
    
    int ammo_type = weaponinfo[player->readyweapon].ammo;
    // TODO: This is a weird legacy thing. For some reason this is how we  
    // tell the ammo counter not to display any ammo. We should just use the
    // 'enabled' boolean instead?

    int *new_value;
    
    // The player is a weapon that doesn't show ammo (fists or a chainsaw)
    if (ammo_type == am_noammo)
    {
        ammo_counter->options->enabled = false; 
    }
    else
    {
        new_value = &player->ammo[weaponinfo[player->readyweapon].ammo];
    }

    drawable_update_number_value(ammo_counter, new_value);
}

void ST_updateWidgets(void)
{
    
    update_large_ammo_counter(player, large_ammo_counter);

    // update keycard multiple widgets
    for (int i = 0; i < 3; i++)
    {
        keyboxes[i] = player->cards[i] ? i : -1;

        if (player->cards[i + 3])
        {
            keyboxes[i] = i + 3;
        }
    }

    // refresh everything if this is him coming back to life
    ST_updateFaceWidget();

    // used by the w_armsbg widget
    st_notdeathmatch = !deathmatch;

    // used by w_arms[] widgets
    st_armson = status_bar_enabled && !deathmatch;

    // TODO: factor out into a separate frag counter update function
    // used by frag_counter widget
    st_fragson = deathmatch && status_bar_enabled;
    st_fragscount = 0;

    for (int i = 0; i < MAXPLAYERS; i++)
    {
        if (i != consoleplayer)
        {
            st_fragscount += player->frags[i];
        }
        else
        {
            st_fragscount -= player->frags[i];
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
    st_oldhealth = player->health;
}

static int st_palette = 0;

void ST_doPaletteStuff(void)
{

    int palette;
    byte *pal;
    int cnt;
    int bzc;

    cnt = player->damagecount;

    if (player->powers[pw_strength])
    {
        // slowly fade the berzerk out
        bzc = 12 - (player->powers[pw_strength] >> 6);

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

    else if (player->bonuscount)
    {
        palette = (player->bonuscount + 7) >> 3;

        if (palette >= NUMBONUSPALS)
        {
            palette = NUMBONUSPALS - 1;
        }

        palette += STARTBONUSPALS;
    }

    else if (player->powers[pw_ironfeet] > 4 * 32 || player->powers[pw_ironfeet] & 8)
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
    // TODO: Why are we passing refresh all the way down to every function
    // in the widget library? There's like one use of it. Can't we just
    // return early if refresh = false?

    // used by w_arms[] widgets
    st_armson = status_bar_enabled && !deathmatch;

    // used by frag_counter widget
    st_fragson = deathmatch && status_bar_enabled;

    drawable_draw(large_ammo_counter, refresh);

    // Draw the ammo counters
    // TODO: Move to separate Draw function, 
    // and have a list of all drawables in memory to iterate over instead of
    // making an explicit call for each one
    drawable_draw(widget_ammo_counter_bullets, refresh);
    drawable_draw(widget_ammo_counter_shells, refresh);
    drawable_draw(widget_ammo_counter_rockets, refresh);
    drawable_draw(widget_ammo_counter_cells, refresh);

    drawable_draw(health_counter, refresh);
    drawable_draw(armor_counter, refresh);
    //widget_draw_number_widget(frag_counter, st_backing_screen, refresh);

    STlib_updateBinIcon(&w_armsbg, refresh);

    for (int i = 0; i < 6; i++)
    {
        STlib_updateMultIcon(&w_arms[i], refresh);
    }

    STlib_updateMultIcon(&w_faces, refresh);

    for (int i = 0; i < 3; i++)
    {
        STlib_updateMultIcon(&w_keyboxes[i], refresh);
    }

}
//TODO:
void status_bar_draw(StatusBar *status_bar, pixel_t *screen)
{
    status_bar_draw_background(status_bar, screen);
    //status_bar_DrawWidgets();
}


void ST_doRefresh(void)
{

    st_firsttime = false;

    // draw status bar background to off-screen buff
    status_bar_draw(status_bar, st_backing_screen);

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

    status_bar_enabled = (!fullscreen) || automapactive;
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

// A callback function that can be used for loading (or unloading) graphics
// patches depending on the 
typedef void (*load_or_unload_callback_t)(const char *lumpname, patch_t **variable);

// Iterates through all graphics to be loaded or unloaded, along with
// the variable they use, invoking the specified callback function.
// The implementation of the function passed determines whether a load
// or unload operation takes place.
//
// status_bar_cache_patches(load_patch);
static void status_bar_cache_patches(load_or_unload_callback_t load_unload_function)
{

    char namebuf[9];

    // Load the key card textures
    for (int i = 0; i < NUMCARDS; i++)
    {
        M_snprintf(namebuf, 9, "STKEYS%d", i);
        load_unload_function(namebuf, &keys[i]);
    }

    // arms background
    load_unload_function("STARMS", &armsbg);

    // arms ownership widgets
    for (int i = 0; i < 6; i++)
    {
        M_snprintf(namebuf, 9, "STGNUM%d", i + 2);

        // gray #
        load_unload_function(namebuf, &arms[i][0]);

        // yellow #
        arms[i][1] = shortnum[i + 2];
    }

    // face backgrounds for different color players
    M_snprintf(namebuf, 9, "STFB%d", consoleplayer);
    load_unload_function(namebuf, &faceback);

    // TODO: Make a separate function
    // face states
    int facenum = 0;
    for (int i = 0; i < ST_NUMPAINFACES; i++)
    {
        for (int j = 0; j < ST_NUMSTRAIGHTFACES; j++)
        {
            M_snprintf(namebuf, 9, "STFST%d%d", i, j);
            load_unload_function(namebuf, &faces[facenum]);
            ++facenum;
        }
        M_snprintf(namebuf, 9, "STFTR%d0", i); // turn right
        load_unload_function(namebuf, &faces[facenum]);
        ++facenum;
        M_snprintf(namebuf, 9, "STFTL%d0", i); // turn left
        load_unload_function(namebuf, &faces[facenum]);
        ++facenum;
        M_snprintf(namebuf, 9, "STFOUCH%d", i); // ouch!
        load_unload_function(namebuf, &faces[facenum]);
        ++facenum;
        M_snprintf(namebuf, 9, "STFEVL%d", i); // evil grin ;)
        load_unload_function(namebuf, &faces[facenum]);
        ++facenum;
        M_snprintf(namebuf, 9, "STFKILL%d", i); // pissed off
        load_unload_function(namebuf, &faces[facenum]);
        ++facenum;
    }

    load_unload_function("STFGOD0", &faces[facenum]);
    ++facenum;
    load_unload_function("STFDEAD0", &faces[facenum]);
    ++facenum;
}

// Cache a lump name containing a particular patch (texture) that
// is required for the status bar.
const static void load_patch(const char *lumpname, patch_t **variable)
{
    *variable = W_CacheLumpName(lumpname, PU_STATIC);
}

void ST_initData(void)
{
    drawable_init();


    int i;

    st_firsttime = true;
    player = &players[consoleplayer];

    st_clock = 0;
    // TODO: Unused?
    //st_chatstate = StartChatState;
    // TODO: Unused?
    //st_gamestate = FirstPersonState;

    status_bar_enabled = true;
    st_oldchat = st_chat = false;
    st_cursoron = false;

    st_faceindex = 0;
    st_palette = -1;

    st_oldhealth = -1;

    for (i = 0; i < NUMWEAPONS; i++)
    {
        oldweaponsowned[i] = player->weaponowned[i];
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
static void create_large_ammo_counter(player_t *player)
{
    log_trace("create_large_ammo_counter(): Creating ammo counter");
    
    int *value = &player->ammo[weaponinfo[player->readyweapon].ammo];
    
    DrawableOptions *options = Z_Malloc(sizeof(DrawableOptions), PU_STATIC, 0);

    *options = (DrawableOptions) {
        .x = 44, 
        .y = 171, 
        .digits = 3, 
        .use_large_font = true, 
        .show_percent_sign = false
    };

    large_ammo_counter = drawable_create_number(value, options, &status_bar_enabled);
}

static void create_ammo_counters(player_t *player)
{
    // Create the large ammo counter that shows the ammunition of the current
    // weapon.
    create_large_ammo_counter(player);
    
    // Create the four smaller ammo counters that are specific to the ammunition
    // type.
    //const int X_POS = 288;

    /**

    // The ammo counter for "BULL"
    widget_ammo_counter_bullets = widget_create_fraction_widget(
        X_POS, 173, &player->ammo[0], &player->maxammo[0], &status_bar_enabled, shortnum);

    // The ammo counter for "SHELL"
    widget_ammo_counter_shells = widget_create_fraction_widget(
        X_POS, 179, &player->ammo[1], &player->maxammo[1], &status_bar_enabled, shortnum);

    // The ammo counter for "RCKT"
    widget_ammo_counter_rockets = widget_create_fraction_widget(
        X_POS, 191, &player->ammo[2], &player->maxammo[2], &status_bar_enabled, shortnum);

    // The ammo counter for "CELL"
    widget_ammo_counter_cells = widget_create_fraction_widget(
        X_POS, 185, &player->ammo[3], &player->maxammo[3], &status_bar_enabled, shortnum);
    **/
}

/**
 * Create the statusbar widget for displaying health 
 */
static void create_health_counter(player_t *player)
{
    log_trace("create_health_counter(): Creating health percentage");
    
    DrawableOptions *options = Z_Malloc(sizeof(DrawableOptions), PU_STATIC, 0);

    *options = (DrawableOptions) {
        .x = 90, 
        .y = 171, 
        .digits = 3, 
        .use_large_font = true, 
        .show_percent_sign = false
    };

    health_counter = drawable_create_number(
        &player->health, options, &status_bar_enabled);
}
/**
 * Create the statusbar widget for displaying armor
 * Original comment: "should be colored later"
 */
void create_armor_counter(player_t *player)
{
    log_trace("create_armor_counter(): Creating armor percentage");
    
    int *value = &player->armorpoints;
    
    DrawableOptions *options = Z_Malloc(sizeof(DrawableOptions), PU_STATIC, 0);

    *options = (DrawableOptions) {
        .digits = 3, 
        .x = 221, 
        .y = 171, 
        .use_large_font = true, 
        .show_percent_sign = false
    };

    armor_counter = drawable_create_number(
        value, options, &status_bar_enabled);
}

/**
 * Create the statusbar widget for showing frag counts. 
 */
static void create_frag_counter_widget()
{
    log_trace("create_frag_counter_widget(): Creating frag counter widget");
    
    DrawableOptions *options = Z_Malloc(sizeof(DrawableOptions), PU_STATIC, 0);

    *options = (DrawableOptions) {
        .digits = 2, 
        .x = 138, 
        .y = 171, 
        .use_large_font = true, 
        .show_percent_sign = false
    };


    frag_counter = drawable_create_number(&st_fragscount, options, &st_fragson);
}

void ST_createWidgets(player_t *player)
{
   
    create_health_counter(player);
    create_armor_counter(player);
    create_ammo_counters(player);

    // arms background
    STlib_initBinIcon(
        &w_armsbg, ST_ARMSBGX, ST_ARMSBGY, armsbg, &st_notdeathmatch, &status_bar_enabled);

    // weapons owned
    for (int i = 0; i < 6; i++)
    {
        STlib_initMultIcon(
            &w_arms[i],
            ST_ARMSX + (i % 3) * ST_ARMSXSPACE,
            ST_ARMSY + (i / 3) * ST_ARMSYSPACE,
            arms[i],
            &player->weaponowned[i + 1],
            &st_armson);
    }

    // faces
    STlib_initMultIcon(&w_faces, ST_FACESX, ST_FACESY, faces, &st_faceindex, &status_bar_enabled);

    // keyboxes 0-2
    STlib_initMultIcon(&w_keyboxes[0], ST_KEY0X, ST_KEY0Y, keys, &keyboxes[0], &status_bar_enabled);

    STlib_initMultIcon(&w_keyboxes[1], ST_KEY1X, ST_KEY1Y, keys, &keyboxes[1], &status_bar_enabled);

    STlib_initMultIcon(&w_keyboxes[2], ST_KEY2X, ST_KEY2Y, keys, &keyboxes[2], &status_bar_enabled);
}

static boolean st_stopped = true;


void ST_Start(void)
{
    
    if (!st_stopped)
    {
        ST_Stop();
    }
    st_backing_screen = Z_Malloc(SCREENWIDTH * SCREENHEIGHT * sizeof(*st_backing_screen), PU_STATIC, 0);
    ST_initData();
    status_bar = status_bar_create(&status_bar_enabled);
    ST_createWidgets(player);
    st_stopped = false;
}

void ST_Stop(void)
{
    if (st_stopped)
    {
        return;
    }
    I_SetPalette(W_CacheLumpNum(lu_palette, PU_CACHE));

    st_stopped = true;
}

void ST_Init(void)
{
    lu_palette = W_GetNumForName("PLAYPAL");

    // Run through the list of patches, loading each one with the "load_patch" function.
    // Note: It's implied here that if there's a "load" function, there is an "unload"
    // function. There isn't. But I guess the flexibility is there to have one.
    status_bar_cache_patches(load_patch);
}
