/*
 * TextColors.h
 * < cout Text Color Codes header file >
 *
 *  Created on: Dec 22, 2017
 *      Author: JohnVGC
 *
 *  Version 1.0.0
 *
 *  base version
 */

#ifndef TEXTCOLORS_H_
#define TEXTCOLORS_H_

#include <string>

using namespace std;

const string TextColor_Red("\033[1;31m"); // for DAMAGE TAKEN in method damage()
const string TextColor_DarkRed("\033[0;31m"); // for ERRORS, for LOW HP in the following methods [display() puyo/display() puyo/match()]
const string TextColor_Green("\033[1;32m"); // for ITEM LEVELED UP in levelUp(), for ITEM READY TO LEVEL UP in display()
const string TextColor_Yellow("\033[1;33m");
const string TextColor_DarkYellow("\033[0;33m"); // for POINTS ADDED in addPoints()
const string TextColor_Blue("\033[1;34m"); // for HP RECOVERED in recover()
const string TextColor_Magenta("\033[1;35m"); // for CHALLENGER DEFEATED in puyo/match()
const string TextColor_DarkMagenta("\033[0;35m"); // for MATCH LOST in puyo/match()
const string TextColor_Cyan("\033[1;36m"); // for ITEM or CHALLENGER ADDED in methods [addReward() puyo/addChallenger()]
const string TextColor_DarkCyan("\033[0;36m"); // for ITEM or CHALLENGER REMOVED in methods [removeReward() puyo/removeChallenger()]
const string TextColor_Gray("\033[1;30m"); // for general LOAD and SAVE DATA outputs

const string TextColor_Reset("\033[0m");

#endif /* TEXTCOLORS_H_ */
