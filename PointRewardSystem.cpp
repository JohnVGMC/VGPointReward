/*
 * PointRewardSystem.cpp
 * < PointRewardSystem Class Implementation File >
 *
 *  Created on: Dec 21, 2017
 *  Updated on: Feb 10, 2018
 *      Author: JohnVGC
 *
 *  Version 1.2.0
 */

#include "PointRewardSystem.h"
#include "SoundCues.h"
#include "TextColors.h"
#include <iostream> //for cin & cout
#include <iomanip> // for setw
#include <fstream> // file input and output
#include <sstream> // string parsing
#include <stdlib.h> //for rand & srand

using namespace std;

/* Default Constructor */
PointRewardSystem::PointRewardSystem()
{
	energy = 10;
	points = 0;
	diamonds = 0;
}

/* Destructor */
PointRewardSystem::~PointRewardSystem(){
	// nothing to add
}

/*
 * Reads the save file "rewards.csv" to load in:
 * - The line containing current HP available "<energy>"
 * - The line containing the Merit Points the user has earned "<points>"
 * - The line containing the Diamonds the user has earned "<diamonds>"
 * - All remaining lines that hold the available rewards to redeem "<name>,<tag>,<level>,<maxLevel>,<baseCost>,<percentInc>"
 * If "rewards.csv" is unable to be read or doesn't exist, then the user may start with a blank slate: No points and empty inventory.
 * RETURNS True if the save data could be read and was not empty.
 * RETURNS False otherwise (for the purpose of asking the user to continue the program or not, incase a save IS suppose to be present)
 */
bool PointRewardSystem::loadData()
{
	ifstream loadIn("save/rewards.csv");
	if(loadIn.is_open())
	{
		cout << TextColor_Gray << "Loading Reward Data... ";
		string line;
		stringstream lineParse;
		/* First, the user's energy is loaded */
		getline(loadIn, line);
		lineParse.str(line);
		lineParse >> energy;
		/* Next, the user's accumulated Merit Points */
		getline(loadIn, line);
		lineParse.clear();
		lineParse.str(line);
		lineParse >> points;
		/* Next, the user's accumulated Diamonds */
		getline(loadIn, line);
		lineParse.clear();
		lineParse.str(line);
		lineParse >> diamonds;
		/* Then, read all remaining lines, loading in each reward's attributes from each line */
		while(!loadIn.eof()) // keep reading until we reach the end of the file
		{
			/* Read the line, prepare it for parsing, and ready up a new reward item node */
			getline(loadIn, line);
			lineParse.clear();
			lineParse.str(line);
			PointRewardNode newReward;
			/*
			 * parse each attribute
			 * Strings are copied directly from getlines
			 * numerical attributes are parsed in through istringstreams (the variable 'line' can hold the numerical strings as the loaded line is already copied into 'lineParse')
			 */
			getline(lineParse, newReward.name, ',');
			getline(lineParse, newReward.tag, ',');
			getline(lineParse, line, ',');
			istringstream(line) >> newReward.level;
			getline(lineParse, line, ',');
			istringstream(line) >> newReward.maxLevel;
			getline(lineParse, line, ',');
			istringstream(line) >> newReward.baseCost;
			getline(lineParse, line, ',');
			istringstream(line) >> newReward.percentInc;
			rewards.push_back(newReward);
		}
		cout << "Loaded.\n" << TextColor_Reset;
		return true;
	}
	else
	{
		cout << TextColor_DarkRed << "ERROR: Unable to load reward data.\n" << TextColor_Reset;
		return false;
	}
}

/*
 * Writes the overall state of the Point Reward System (rewards, points earned, and the User's energy) into the save file "rewards.csv" into the following format:
 * - The line containing current HP available "<energy>"
 * - The line containing the user's accumulated Merit Points "<points>"
 * - The line containing the user's accumulated Diamonds "<diamonds>"
 * - The attributes for each available reward to redeem on each line "<name>,<tag>,<level>,<maxLevel>,<baseCost>,<percentInc>"
 * This function is ideally used whenever a change has occurred into the reward list.
 */
void PointRewardSystem::saveData()
{
	ofstream saveOut("save/rewards.csv");
	if(saveOut.is_open())
	{
		cout << TextColor_Gray << "Saving Reward Data... ";
		// First the user's current energy is written, then the user's accumulated merit points
		saveOut << energy << "\n" << points << "\n" << diamonds;
		// Then save each reward attribute on each line
		for(unsigned int i = 0; i < rewards.size(); i++)
		{
			saveOut << "\n" << rewards[i].name
					<< "," << rewards[i].tag
					<< "," << rewards[i].level
					<< "," << rewards[i].maxLevel
					<< "," << rewards[i].baseCost
					<< "," << rewards[i].percentInc;
		}
		saveOut.close();
		cout << "Saved.\n" << TextColor_Reset;
	}
	else
	{	cout << TextColor_DarkRed << "ERROR: Unable to save reward data.\n" << TextColor_Reset;	}
}

/*
 * Creates a new PointRewardNode and pushes it to the end of the rewards vector (adding it to the list)
 * The user is queried about each attribute to provide for the new reward.
 */
void PointRewardSystem::addReward()
{
	PointRewardNode newReward;
	string input;
	stringstream inputStrm;
	bool validInput = false; // Prevents moving on and will re-ask the user the same question until their input is accepted
	// Reward always starts with no points and at the beginning level
	newReward.level = 1;
	// Ask for name and tag
	cout << "Please enter the NAME of the new reward:\n>> ";
	getline(cin, newReward.name);
	cout << "Please enter the TAG of the new reward:\n>> ";
	getline(cin, newReward.tag);
	do{ // baseCost query (baseCost must be greater than zero)
		cout << "Please enter the BASE COST of the new reward:\n>> ";
		getline(cin, input);
		if(istringstream(input) >> newReward.baseCost)
		{
			if(newReward.baseCost > 0)
			{   validInput = true;   }
			else
			{   cout << TextColor_DarkRed << "Base Cost must be greater than zero... Please try again.\n" << TextColor_Reset;   }
		}
		else
		{   cout << TextColor_DarkRed << "ERROR: Invalid entry... Please try again.\n" << TextColor_Reset;   }
	} while(!validInput);
	validInput = false;
	do{ // maxLevel query (maxLevel must be greater than or equal to 0)
			cout << "Please enter the MAX LEVEL of the new reward (1 for one-time, 0 for no-max-limit):\n>> ";
			getline(cin, input);
			if(istringstream(input) >> newReward.maxLevel)
			{
				if(newReward.maxLevel >= 0)
				{   validInput = true;   }
				else
				{   cout << TextColor_DarkRed << "Max Level value must be greater than or equal to 0... Please try again.\n" << TextColor_Reset;   }
			}
			else
			{   cout << TextColor_DarkRed << "ERROR: Invalid entry... Please try again.\n" << TextColor_Reset;   }
	} while(!validInput);
	validInput = false;
	// If the maxLevel is not 1, then the reward is not one time, meaning a percent increment is needed
	if(newReward.maxLevel != 1)
	{
		do{ // percent increment query (percentInc must be greater than or equal to 0)
				cout << "Please enter the PERCENT INCREMENT of the new reward (example, entering 7 for 7% increase of base cost at each level):\n>> ";
				getline(cin, input);
				if(istringstream(input) >> newReward.percentInc)
				{
					if(newReward.percentInc >= 0)
					{
						newReward.percentInc = newReward.percentInc / 100; // set as a proper percentage value
						validInput = true;
					}
					else
					{   cout << TextColor_DarkRed << "Percent Increment must be greater than or equal to -1... Please try again.\n" << TextColor_Reset;   }
				}
				else
				{   cout << TextColor_DarkRed << "ERROR: Invalid entry... Please try again.\n" << TextColor_Reset;   }
		} while(!validInput);
	}
	else // If the reward is one time, a percent increment is not needed
	{   newReward.percentInc = 0;   }
	addItem_SOUND();
	rewards.push_back(newReward);
	cout << TextColor_Cyan << "[" << newReward.tag << "] " << newReward.name << " was successfully added.\n" << TextColor_Reset;
	saveData();
}

/*
 * Removes the reward from the list with a given reward index
 * index is decremented to get the actual vector index (I.E. reward #1 is actually at index 0)
 * Will prompt the user to confirm if they really want to delete the reward at the given index.
 * If the user fully types out "yes" the reward is erased from the vector.
 * Otherwise, nothing else will occur in the function.
 * PARAMETER: int 'index' denotes which reward to remove from the list. (with a value of 1 representing the first reward)
 * PARAMETER: bool 'manualRemoval' that denotes if the reward removal is a manual removal by the user or not.
 */
void PointRewardSystem::removeReward(int index, bool manualRemoval)
{
	// The index has to be vaild for the vector (#1 representing 0)
	if( (index > 0) && ((unsigned) index <= rewards.size()) )
	{
		/*
		 * Ask if the user really wants to remove the reward from the list... (if the removal is by the user)
		 * [index] represents the reward listing number (for display and is the numbering system the user sees)
		 * [index-1] represents the reward's VECTOR index
		 */
		string userInput;
		if(manualRemoval)
		{
			cout << "Do you really want to remove Reward #" << index << " [" << rewards[index-1].tag << "] " << rewards[index-1].name << "? (type \"yes\" to confirm)\n";
			getline(cin, userInput);
			userInput = lowercaseString(userInput);
		}
		else // The removal is automatic, set the input arg to accept the removal
		{   userInput = "yes";   }
		if(userInput == "yes")
		{
			if(manualRemoval)
			{   removeItem_SOUND();   }
			cout << TextColor_DarkCyan << "[" << rewards[index-1].tag << "] " << rewards[index-1].name << " was successfully removed.\n" << TextColor_Reset;
			// Remove the reward from the vector
			rewards.erase(rewards.begin() + (index-1));
			saveData();
		}
		else
		{   cout << TextColor_DarkRed << "Reward Removal Cancelled.\n" << TextColor_Reset;   }
	}
	else
	{   cout << TextColor_Red << "ERROR: Invalid Index Value (Out of Bounds).\n" << TextColor_Reset;   }
}

/*
 * Adds Merit Points to the user's accumulated amount.
 * PARAMETER: int 'amount' is the number of Merit Points that get added to the user's point total.
 * PARAMETER: bool 'toggleSound' that dictates whether or not to play the sound file specific for gaining Merit Points
 *            typically false when called within another command not specifically "addpoints" that have their own sound cues
 */
void PointRewardSystem::addPoints(int amount, bool toggleSound)
{
	if(amount > 0) // must have Merit Points to add
	{
		if(toggleSound) /* SOUND CUE */
		{   addPoints_SOUND();   }
		points += amount;
		cout << TextColor_DarkYellow << "Earned " << amount << " Merit Points.\n" << TextColor_Reset;
		saveData();
	}
	else
	{   cout  << TextColor_DarkRed << "ERROR: Insufficient Merit Points to distribute (value must be greater than zero).\n" << TextColor_Reset;   }
}

/*
 * Increments the diamond count for the user.
 * User only gains one diamond per call (to represent how rare the item is)
 */
void PointRewardSystem::addDiamond()
{
	addDiamond_SOUND();
	diamonds++;
	cout << TextColor_DarkCyan << "Earned 1 Diamond.\n" << TextColor_Reset;
	saveData();
}

/*
 * Redeems a Reward and levels it up.
 * Will check to see if:
 * - The index value is valid
 * - If there is enough Merit Points accumulated to redeem the reward (by calculating and comparing the next cost to level up)
 * PARAMETER: int 'index' that denotes which reward to redeem from the list. (with index #1 denoting vector index 0 and so on)
 */
void PointRewardSystem::levelUp(int index)
{
	// The index has to be vaild for the vector (#1 representing 0)
	if( (index > 0) && ((unsigned) index <= rewards.size()) )
	{
		/*
		 * [index] represents the reward listing number (for display)
		 * [index-1] represents the reward's VECTOR index
		 */
		if(points >= getNextLevelCost(index-1))
		{
			/*
			 * Subtract the accumulated Merit points with the level Up cost
			 * If the current level (before level up) is equal to the max level, the reward is at its max level and cannot be leveled up further
			 * ... then the reward must be removed (removeReward() will handle the removal process on automatic mode)
			 * otherwise increment the reward level (always when maxLevel is -1)
			 */
			levelUp_SOUND();
			points -= getNextLevelCost(index-1);
			cout << TextColor_Green << "[" << rewards[index-1].tag << "] " << rewards[index-1].name << " redeemed";
			if(rewards[index-1].level == rewards[index-1].maxLevel)
			{
				cout << ".\n" << TextColor_Reset;
				removeReward(index, false);
				// removeReward will already call saveData()
			}
			else
			{
				cout << " and leveled up.\n" << TextColor_Reset;
				rewards[index-1].level++;
				saveData();
			}
		}
		else
		{
			if(diamonds > 0) // can redeem an item for a diamond instead if there's not enough Merit Points.
			{
				cout << "Insufficient Merit Points to Level Up.\nWould you like to use 1 Diamond instead?\n>> ";
				string userInput;
				getline(cin, userInput);
				userInput = lowercaseString(userInput);
				if(userInput == "yes")
				{
					/*
					 * Decrement the accumulated Diamonds by 1
					 * If the current level (before level up) is equal to the max level, the reward is at its max level and cannot be leveled up further
					 * ... then the reward must be removed (removeReward() will handle the removal process on automatic mode)
					 * otherwise increment the reward level (always when maxLevel is -1)
					 */
					levelUp_SOUND();
					diamonds--;
					cout << TextColor_Green << "[" << rewards[index-1].tag << "] " << rewards[index-1].name << " redeemed";
					if(rewards[index-1].level == rewards[index-1].maxLevel)
					{
						cout << ".\n" << TextColor_Reset;
						removeReward(index, false);
						// removeReward will already call saveData()
					}
					else
					{
						cout << " and leveled up.\n" << TextColor_Reset;
						rewards[index-1].level++;
						saveData();
					}
				}
				else
				{
					cout << TextColor_DarkRed << "Item Level Up Cancelled.\n" << TextColor_Reset;
				}
			}
			else
			{
				cout << TextColor_DarkRed << "Insufficient Merit Points & Diamonds to Level Up.\n" << TextColor_Reset;
			}
		}
	}
	else
	{   cout << TextColor_DarkRed << "ERROR: Invalid Index Value (Out of Bounds).\n" << TextColor_Reset;   }
}

/*
 * Subtracts the user's energy variable with 'amount'
 * Will let the user know if they have sufficient energy to spend.
 * PARAMETER: int 'amount' that holds the amount of Energy being subtracted.
 * PARAMETER: bool 'toggleSound' that dictates whether or not to play the sound file specific for spending energy
 *            typically false when called within another command not specifically "spendEnergy()" that have their own sound cues
 */
void PointRewardSystem::spendEnergy(int amount, bool toggleSound)
{
	if(amount > 0) // there must be a valid entry to spend
	{
		if(energy >= amount) //user have sufficient energy to spend
		{
			if(toggleSound)
			{   spendEnergy_SOUND();   }
			energy -= amount;
			cout << TextColor_Red << "Spent " << amount << " Energy.\n" << TextColor_Reset;
			saveData();
		}
		else
		{   cout << TextColor_DarkRed << "Insufficient Energy.\n" << TextColor_Reset;   }
	}
	else
	{   cout << TextColor_DarkRed << "ERROR: Invalid amount to spend (value must be greater than zero).\n" << TextColor_Reset;   }
}

/* --OLD--
 * Increases the user's Energy variable with 'amount'
 * Will let the user know if their Energy is already at capacity.
 * Anytime the User recovers Energy that may exceed the maximum allowed at their current state, excess Energy is taken away to make it equal to the max.
 * PARAMETER: int 'amount' that holds the amount of Energy to recover.
 *
 * --NEW--
 * Increases the user's Energy variable by a random amount
 * Will let the user know if their Energy is already at capacity.
 * Anytime the User recovers Energy that may exceed the maximum allowed at their current state, excess Energy is simply dropped.
 */
void PointRewardSystem::recover()
{
	int currentMax = getMaxEnergy();
	if(energy < currentMax)
	{
		// Initialize Random Seed
		srand(time(NULL));

		int amount = rand() % currentMax + 1;
		recover_SOUND();
		energy += amount;
		cout << TextColor_Blue << "Recovered " << amount << " Energy.\n" << TextColor_Reset;
		if(energy > currentMax)
		{
			energy = currentMax;
		}
		saveData();
	}
	else
	{   cout << TextColor_DarkRed << "Energy is already full. Cannot recover anymore.\n" << TextColor_Reset;   }
}

/*
 * Outputs the available rewards in a Table format
 * HP Left is shown first
 * Then all the items are displayed in a list
 */
void PointRewardSystem::display()
{
	cout << "╔══════════════════════════════════════════════════════════════════════════════\n"
			<< "║ Energy: ";
	if(energy <= 0)
	{   cout << TextColor_DarkRed;   }
	cout << energy << TextColor_Reset << " / " << getMaxEnergy() << "\n"
			<< "║ " << TextColor_Yellow << "Merit Points" << TextColor_Reset << ": ";
	if(points <= 0)
	{   cout << TextColor_DarkRed;   }
	cout << points << TextColor_Reset << "\n"
			<< "║ " << TextColor_Cyan << "Diamonds" << TextColor_Reset << ": ";
	if(diamonds <= 0)
	{   cout << TextColor_DarkRed;   }
	cout << diamonds << TextColor_Reset << "\n"
			<< "╠═════╤══════╤═════════════════════════════════════════════════════════════════\n";
	for(unsigned int i = 0; i < rewards.size(); i++)
	{
		cout << "╟ ";
		int nextCost = getNextLevelCost(i);
		if(points < nextCost) // not enough points to redeem a reward, darken its listing
		{
			if(diamonds > 0) // Darken the listing with Dark Cyan
			{
				cout << TextColor_DarkCyan;
			}
			else // Darken the listing with Dark Grey
			{
				cout << TextColor_Gray;
			}
		}
		cout << "#";
		if(i < 9)
		{   cout << " ";   }
		cout << (i+1) << " │" << right << setw(5) << nextCost << " │ ";
		if(rewards[i].maxLevel != 1) // Show current level if the reward is not one time
		{
			cout << "{Lv." << rewards[i].level;
			if(rewards[i].maxLevel != 0)  // Show max level if the reward is not endless
				{   cout << "/" << rewards[i].maxLevel;   }
			cout << "} ";
		}
		cout << "[" << rewards[i].tag << "] " << rewards[i].name << "\n" << TextColor_Reset;
	}
	cout << "╚═════╧══════╧═════════════════════════════════════════════════════════════════\n";
}

/** AUXILLARY FUNCTIONS **/

/*
 * Returns true if the user's Energy is zero.
 * Returns false otherwise
 * Certain functions cannot be performed unless the User has Hit Points.
 */
bool PointRewardSystem::isFainted()
{	return (energy <= 0);	}

/*
 * Returns the user's Energy value
 */
int PointRewardSystem::getEnergy()
{	return energy;	}

/*
 * Calculates the max Energy of the user
 * Minimum is always 10 and increases every 5 levels of each reward.
 */
int PointRewardSystem::getMaxEnergy()
{
	int maxEnergy = 10;
	for(unsigned int i = 0; i < rewards.size(); i++)
	{
		maxEnergy += ((rewards[i].level - 1) / 5);
	}
	return maxEnergy;
}

/*
 * Calculates and returns the cost needed to level up a reward
 * Level Up is calculated using the baseCost times a percentIncrement increased at each level (starts increasing after the first level)
 *     i.e. percentIncrement * (level - 1)
 * PARAMETER: int 'index' that holds then VECTOR index of the reward
 */
int PointRewardSystem::getNextLevelCost(int index)
{
	return ( rewards[index].baseCost * (1 + (rewards[index].percentInc * (rewards[index].level - 1))) );
}

/*
 * Parses a string and changes every character to its respective lowercase variant using "tolower()" on each character
 * Returns the string the user provided, but with all capital letters changed to lowercase
 * PARAMETER: string variable to parse through.
 */
string PointRewardSystem::lowercaseString(string inString)
{
	for(unsigned int i = 0; i < inString.length(); i++)
	{	inString[i] = tolower(inString[i]);	}
	return inString;
}

