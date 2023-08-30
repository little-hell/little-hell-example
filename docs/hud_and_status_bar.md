# The DOOM HUD 

As far as the code suggests, the "HUD" is just onscreen messages shown to the player, including the chat and notifications about item pickups and cheats. In reality, the HUD refers to both those messages _and_ the [status bar](https://doomwiki.org/wiki/Status_bar).

## On-Screen Messages

_Currently unwritten_

## Status Bar

![](status_bar_populated.png)

_Credit: [doomwiki.org](dhttps://doomwiki.org/wiki/File:StatusBar.png)_

### Reponsibilities of the Status Bar
As one might expect, it is reasonable (and correct) to assume the status bar is responsible for the following:
- Drawing and updating an integer number on-screen that represents the ammo count for the currently selected weapon (as a regular number)
- Drawing and updating a "percentage" (just an integer with a percent-sign at the end) for the players health and armor
- Displaying DOOM guys face, and changing its animation based on a number of in-game events and conditions. 
- Drawing and updating the contents of 3 slots that hold keys or skulls collected by the player.
- Drawing and updating four "fractions" (two numbers spaced apart to line up with the "/" character baked into the background status bar texture) that represent the current and maximum ammo for pistol ammo ("BULL"), shotgun ammo ("SHELL"), rocket ammo ("RCKT") and energy cells ("CELL")

The status bar (or at least its code) also performs a number of important, non-obvious tasks:
- Scanning keypresses to detect if cheat codes have been entered. 
- Performing palette special effects when the player is injured or is given a status effect from picking up an item.

The status bar is also responsible for two additional things, depending on whether the player is in a deathmatch game.
- If in a deathmatch game, the background status bar texture (`STBAR`) is applied as is. Note that it contains a box for the frag count. 

    ![](status_bar.png)
_The background status bar texture, called `STBAR` in the WAD._
- If the player is not in a deathmatch game, the `STARMS` texture is applied over top of the background texture: 

    ![](status_bar_arms.png)