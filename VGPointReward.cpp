/*
 * VGPointReward.cpp
 * < Main Program implementation >
 *
 *  Created on: Dec 20, 2017
 *  Updated on: Aug 8, 2018
 *      Author: JohnVGC
 *
 *  Version 1.2.1
 */

#include "TextColors.h"
#include "PointRewardSystem.h"
#include "PuyoChallengeSystem.h"
#include <string>
#include <iostream>
#include <sstream>
#include <stdlib.h> //for rand & srand

using namespace std;

/*  Prototype  */
string lowercaseString(string);
void commandHelp();
void commandHelpPuyo();
void mainPuyo(PointRewardSystem&);
void mainRPGBattle(PointRewardSystem&);


int main()
{
	PointRewardSystem rewardSys;
	bool keepRunning = true;
	// load reward data, if a save data couldn't be loaded, the user will specify if they want to continue with a new reward list
	if(!rewardSys.loadData())
	{
		cout << "Would you like to continue with a new Reward List? (\"no\" to terminate)\n>> ";
		string startNewSaveANS;
		getline(cin, startNewSaveANS);
		startNewSaveANS = lowercaseString(startNewSaveANS);
		if(startNewSaveANS == "no")
		{
			keepRunning = false;
			cout << "Closing... " << endl;
		}
		else
		{   rewardSys.display();   }
	}
	else
	{   rewardSys.display();   }

	cout << "Type \"help\" for options." << endl;
	// Initialize Random Seed
	srand(time(NULL));

	// The primary reward system implementation (only runs when a save exists or the user wants to start a new reward save
	// keeps running until the user states "exit"
	while(keepRunning)
	{
		// Step 1: Get the line of input that the user was prompted to enter.
		string input;
		cout << ">> ";
		getline(cin, input);
		input = lowercaseString(input);
		string mainArg;
		stringstream inputStrm(input);
		// Step 2: Parse the first argument of the input, and run the appropriate command based on the user input
		inputStrm >> mainArg;
		int intArg; // for any args that need to be parsed into an integer
		if(mainArg == "display")
		{
			rewardSys.display();
		}
		else if(mainArg == "addreward")
		{
			rewardSys.addReward();
		}
		else if(mainArg == "removereward")
		{
			if(inputStrm >> intArg)
			{   rewardSys.removeReward(intArg, true);   }
			else
			{   cout << TextColor_DarkRed << "ERROR: Invalid or Insufficient Argument for <index>.\n" << TextColor_Reset;   }
		}
		else if( (mainArg == "levelup") || (mainArg == "up") || (mainArg == "redeem") )
		{
			if(inputStrm >> intArg)
			{   rewardSys.levelUp(intArg);   }
			else
			{   cout << TextColor_DarkRed << "ERROR: Invalid or Insufficient Argument for <index>.\n" << TextColor_Reset;   }
		}
		else if(mainArg == "addpoints")
		{
			if(inputStrm >> intArg)
			{   rewardSys.addPoints(intArg, true);   }
			else
			{   cout << TextColor_DarkRed << "ERROR: Invalid or Insufficient Argument for <amount>.\n" << TextColor_Reset;   }
		}
		else if(mainArg[0] == '+') // addpoints & adddiamond alternate entry
		{
			mainArg.erase(mainArg.begin()); // remove the plus sign to parse the rest of the string as an argument
			if(mainArg == "diamond")
			{
				rewardSys.addDiamond();
			}
			else // addpoints alt
			{

				if(istringstream(mainArg) >> intArg)
				{   rewardSys.addPoints(intArg, true);   }
				else
				{   cout << TextColor_DarkRed << "ERROR: Invalid or Insufficient Argument for <amount>.\n" << TextColor_Reset;   }
			}
		}
		else if(mainArg == "adddiamond")
		{
			rewardSys.addDiamond();
		}
		else if(mainArg == "spendenergy")
		{
			if(inputStrm >> intArg)
			{   rewardSys.spendEnergy(intArg, true);   }
			else
			{   cout << TextColor_DarkRed << "ERROR: Invalid or Insufficient Argument for <amount>.\n" << TextColor_Reset;   }
		}
		else if(mainArg[0] == '-') // spendenergy alternate entry
		{
			mainArg.erase(mainArg.begin()); // remove the minus sign to parse the rest of the string as an argument
			if(istringstream(mainArg) >> intArg)
			{   rewardSys.spendEnergy(intArg, true);   }
			else
			{   cout << TextColor_DarkRed << "ERROR: Invalid or Insufficient Argument for <amount>.\n" << TextColor_Reset;   }
		}
		else if( (mainArg == "recover") || (mainArg == "restore") || (mainArg == "res") )
		{
			rewardSys.recover();
		}
		/* Removing the Puyo Challenger Portion
		else if(mainArg == "playpuyo")
		{
			mainPuyo(rewardSys);
		}
		*/
		else if( (mainArg == "rpgbattle") || (mainArg == "battle") )
		{
			if(rewardSys.isFainted())
			{ cout << "Unable to battle. Not enough energy." << endl; }
			else
			{ mainRPGBattle(rewardSys); }
		}
		else if(mainArg == "help")
		{
			commandHelp();
		}
		else if(mainArg == "exit")
		{
			cout << "Closing... ";
			keepRunning = false;
		}
		else // A command could not be interpreted, let the user know as well as suggesting "help" for a list of commands
		{
			cout << TextColor_DarkRed << "INVALID COMMAND. Type \"help\" for a list of commands.\n" << TextColor_Reset;
		}
		cout << endl;
	}
	return 0;
}

//User enters the command "help" so the list of commands are printed
void commandHelp()
{
	cout << "List of available commands\n--------------------------\n"
			<< "display - shows the current list of available rewards\n"
			<< "addreward - Adds a new reward item\n"
			<< "removereward <index> - Removes a reward item from position number <index>\n"
			<< "levelup/up/redeem <index> - Redeem points for the reward at position number <index>\n"
			<< "addpoints <amount> (or +<amount>) - Adds <amount> number of Merit Points.\n"
			<< "adddiamond (or +diamond) - Adds 1 diamond to the user.\n"
			<< "spendenergy <amount> (or -<amount>) - lose <amount> units of Energy.\n"
			/* << "recover/restore/res <amount> - recovers <amount> units of Energy.\n" (Removing manual energy recover) */
			<< "recover/restore/res - recovers a random amount of Energy.\n"
			/* << "playpuyo - initiate the Puyo VS Challenger Board\n" */
			<< "rpgbattle/battle - initiates a random battle and displays the results. (costs one energy)\n" /* Replacing the Puyo System for DEMO */
			<< "help - displays the list of available commands.\n"
			<< "exit - Closes the program.\n";
}

//User enters the command "help" while in Puyo challenge mode so the list of commands are printed
void commandHelpPuyo()
{
	cout << "List of available commands for Puyo Challenger mode\n--------------------------\n"
			<< "display - shows the Puyo Challenge board and current win streak.\n"
			<< "addchallenger - Adds a new Puyo Challenger\n"
			<< "removechallenger <index> - Removes a challenger from position number <index>\n"
			<< "match - displays who your next opponents are and will give results whether you win or lose\n"
			<< "help - displays the list of available commands.\n"
			<< "exit - Exits Puyo Challenge mode and goes back to the main reward function." << endl;
}

/*
 *
 */
void mainPuyo(PointRewardSystem& rewardSys)
{
	PuyoChallengeSystem challengeSys;
	bool keepRunning = true;
	// load challege data, if a save data couldn't be loaded, the user will specify if they want to continue with a new challenge board
	if(!challengeSys.loadData())
	{
		cout << "Would you like to continue with a new Challenge Board? (\"no\" to terminate)\n>> ";
		string startNewSaveANS;
		getline(cin, startNewSaveANS);
		startNewSaveANS = lowercaseString(startNewSaveANS);
		if(startNewSaveANS == "no")
		{
			keepRunning = false;
			cout << "Exiting Puyo Mode... " << endl;
		}
		else
		{   challengeSys.display(rewardSys.getEnergy());   }
	}
	else
	{   challengeSys.display(rewardSys.getEnergy());   }
	// The "mainpuyo" command's puyo challenge system implementation (only runs when a save exists or the user wants to start a new reward save
	// keeps running until the user states "exit"
	while(keepRunning)
	{
		// Step 1: Get the line of input that the user was prompted to enter.
		string input;
		cout << ">> ";
		getline(cin, input);
		input = lowercaseString(input);
		string mainArg;
		stringstream inputStrm(input);
		// Step 2: Parse the first argument of the input, and run the appropriate command based on the user input
		inputStrm >> mainArg;
		int intArg; // for any args that need to be parsed into an integer
		if(mainArg == "display")
		{
			challengeSys.display(rewardSys.getEnergy());
		}
		else if(mainArg == "addchallenger")
		{
			challengeSys.addChallenger();
		}
		else if(mainArg == "removechallenger")
		{
			if(inputStrm >> intArg)
			{   challengeSys.removeChallenger(intArg);   }
			else
			{   cout << TextColor_DarkRed << "ERROR: Invalid or Insufficient Argument for <index>.\n" << TextColor_Reset;   }
		}
		else if(mainArg == "match")
		{
			if(!rewardSys.isFainted()) // must spend 1 unit of energy per match (only possible if isFainted() isn't true
			{
				rewardSys.spendEnergy(1, false);
				int pointsEarned;
				if(challengeSys.match(pointsEarned)) // Win Case
				{
					rewardSys.addPoints(pointsEarned, true); // at least 25 points are always earned when the user wins a match
				}
				// Nothing additional needs to happen on Lose Case
			}
			else
			{   cout << TextColor_DarkRed << "Insufficient Energy, unable to start a Puyo match. Recover Energy first.\n" << TextColor_Reset;   }
		}
		else if(mainArg == "help")
		{
			commandHelpPuyo();
		}
		else if(mainArg == "exit")
		{
			cout << "Exiting Puyo Mode... ";
			keepRunning = false;
		}
		else // A command could not be interpreted, let the user know as well as suggesting "help" for a list of commands
		{
			cout << TextColor_DarkRed << "INVALID COMMAND. Type \"help\" for a list of commands.\n" << TextColor_Reset;
		}
		cout << endl;
	}
}

/*
 * Simulates an RPG Battle with random results.
 * Generates a foe at any level between 1-100, and determines whether the user wins or loses,
 *  the merit badges earned, and whether or not the user earned a diamond based on the enemy level.
 */
void mainRPGBattle(PointRewardSystem& rewardSys)
{
	rewardSys.spendEnergy(1, false);

	int enemyLevel = rand() % 100 + 1; //RANDOM VALUE between 1-100
	cout << "You encounter a Level " << enemyLevel << " foe. ";

	bool win = ( (rand() % 100 + 1) >= enemyLevel ); // Win condition is true if a random roll between 1-100 reaches or exceeds the enemy's level.
	if(win)
	{
		cout << "After a ";
		if(enemyLevel > 75) cout << "lengthy and grueling";
		else if(enemyLevel > 50) cout << "difficult";
		else if(enemyLevel > 25) cout << "challenging";
		else cout << "swift";
		cout << " battle, you emerge victorious.\n";

		float meritPointsEarned = ( enemyLevel * (rand() % 101 + 50) / 100 ); // Earn Merit Points equal to 50-150% of Enemy's Level
		rewardSys.addPoints((int)meritPointsEarned, false);

		// The user has a chance to earn a diamond after defeating a foe
		// The chance to earn the diamond is higher if the enemy's level is higher.
		bool earnedDiamond = ( (rand() % 100 + 1) <= enemyLevel );
		if(earnedDiamond)
		{
			rewardSys.addDiamond();
		}
	}
	else
	{
		cout << "You were overwhelmed and lost the battle." << endl;
	}
}

/*
 * Parses a string and changes every character to its respective lowercase variant using "tolower()" on each character
 * Returns the string the user provided, but with all capital letters changed to lowercase
 * PARAMETER: string variable to parse through.
 */
string lowercaseString(string inString)
{
	for(unsigned int i = 0; i < inString.length(); i++)
	{	inString[i] = tolower(inString[i]);	}
	return inString;
}


