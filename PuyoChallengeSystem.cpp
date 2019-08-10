/*
 * PuyoChallengeSystem.cpp
 * < PuyoChallengeSystem Class Implementation File >
 *
 *  Created on: Dec 25, 2017
 *  Updated on: Jan 13, 2018
 *      Author: JohnVGC
 *
 *  Version 1.1.0
 *
 *  * Points changed to Merit Point
 *    - due to the Merit Points system being on an overall smaller scale, the Merit Points earned from a match are decreased.
 */

#include "PuyoChallengeSystem.h"
#include "SoundCues.h"
#include "TextColors.h"
#include <iostream> //for cin & cout
#include <fstream> // file input and output
#include <sstream> // string parsing
#include <stdlib.h> //for rand & srand

using namespace std;

/* Default Constructor */
PuyoChallengeSystem::PuyoChallengeSystem()
{
	winStreak = 0;
}

/* Destructor */
PuyoChallengeSystem::~PuyoChallengeSystem(){
	// nothing to add
}

/*
 * Reads the save file "challengers.csv" to load in:
 * - The line containing the user's current win streak "<winStreak>"
 * - All remaining lines that hold the available challengers's atrributes "<name>,<HP>,<pptChar>,<prefersTetris>"
 * If "challengers.csv" is unable to be read or doesn't exist, then the user can choose to start with a blank slate: No win streak and empty challenger board.
 * RETURNS True if the save data could be read and was not empty.
 * RETURNS False otherwise (for the purpose of asking the user to continue the program or not, incase a save IS suppose to be present)
 */
bool PuyoChallengeSystem::loadData()
{
	ifstream loadIn("save/challengers.csv");
	if(loadIn.is_open())
	{
		cout << TextColor_Gray << "Loading Challenger Data... ";
		string line;
		stringstream lineParse;
		/* First, the user's win streak is loaded */
		getline(loadIn, line);
		lineParse.str(line);
		lineParse >> winStreak;
		/* Then, read all remaining lines, loading in each challenger's attributes from each line */
		while(!loadIn.eof())
		{
			/* Read the line, prepare it for parsing, and ready up a new challenger node */
			getline(loadIn, line);
			lineParse.clear();
			lineParse.str(line);
			PuyoChallengerNode newChallenger;
			/*
			 * parse each attribute
			 * Strings are copied directly from getlines
			 * numerical attributes are parsed in through istringstreams (the variable 'line' can hold the numerical strings as the loaded line is already copied into 'lineParse')
			 */
			getline(lineParse, newChallenger.name, ',');
			getline(lineParse, line, ',');
			istringstream(line) >> newChallenger.HP;
			getline(lineParse, line, ',');
			istringstream(line) >> newChallenger.pptChar;
			getline(lineParse, line, ',');
			istringstream(line) >> newChallenger.prefersTetris;
			challengers.push_back(newChallenger);
		}
		cout << "Loaded.\n" << TextColor_Reset;
		return true;
	}
	else
	{
		cout << TextColor_DarkRed << "ERROR: Unable to load challenger data.\n" << TextColor_Reset;
		return false;
	}
}

/*
 * Writes the overall state of the Puyo Challenge Board (challengers and their HP & the user's win streak) into the save file "challengers.csv" into the following format:
 * - The line containing the user's current win streak "<winStreak>"
 * - The attributes for each available reward to redeem on each line "<name>,<HP>,<pptChar>,<prefersTetris>"
 * This function is ideally used whenever a change has occurred into the challenge board.
 */
void PuyoChallengeSystem::saveData()
{
	ofstream saveOut("save/challengers.csv");
	if(saveOut.is_open())
	{
		cout << TextColor_Gray << "Saving Challenger Data... ";
		// First the user's current win streak is written
		saveOut << winStreak;
		// Then save each challenger attribute on each line
		for(unsigned int i = 0; i < challengers.size(); i++)
		{
			saveOut << "\n" << challengers[i].name
					<< "," << challengers[i].HP
					<< "," << challengers[i].pptChar
					<< "," << challengers[i].prefersTetris;
		}
		saveOut.close();
		cout << "Saved.\n" << TextColor_Reset;
	}
	else
	{   cout << TextColor_DarkRed << "ERROR: Unable to save challenger data.\n" << TextColor_Reset;	}
}

/*
 * Creates a new PuyoChallengerNode and pushes it to the end of the challengers vector (adding it to the challenge board)
 * The user is queried about each attribute to provide for the new challenger.
 */
void PuyoChallengeSystem::addChallenger()
{
	PuyoChallengerNode newChallenger;
	string input;
	stringstream inputStrm;
	bool validInput = false; // Prevents moving on and will re-ask the user the same question until their input is accepted
	// Ask for name
	cout << "Please enter the NAME of the new challenger:\n>> ";
	getline(cin, newChallenger.name);
	do{ // challenger HP query (HP must be greater than zero)
		cout << "Please enter the HP of the new challenger:\n>> ";
		getline(cin, input);
		if(istringstream(input) >> newChallenger.HP)
		{
			if(newChallenger.HP > 0)
			{   validInput = true;   }
			else
			{   cout << TextColor_DarkRed << "HP must be greater than zero... Please try again.\n" << TextColor_Reset;   }
		}
		else
		{   cout << TextColor_DarkRed << "ERROR: Invalid entry... Please try again.\n" << TextColor_Reset;   }
	} while(!validInput);
	validInput = false;
	do{ // challenger's PPT Character query (pptChar must be between 0-23 to correspond to a Puyo Puyo Tetris character)
		cout << "Please enter which of the following PPT Characters the challenger will utilize:\n";
		cout << "\t0-" << pptCharList[0] << "     1-" << pptCharList[1] << "     2-" << pptCharList[2];
		cout << "     3-" << pptCharList[3] << "     4-" << pptCharList[4] << "     5-" << pptCharList[5];
		cout << "\n\t6-" << pptCharList[6] << "     7-" << pptCharList[7] << "     8-" << pptCharList[8];
		cout << "     9-" << pptCharList[9] << "     10-" << pptCharList[10] << "     11-" << pptCharList[11];
		cout << "\n\t12-" << pptCharList[12] << "     13-" << pptCharList[13] << "     14-" << pptCharList[14];
		cout << "     15-" << pptCharList[15] << "     16-" << pptCharList[16] << "     17-" << pptCharList[17];
		cout << "\n\t18-" << pptCharList[18] << "     19-" << pptCharList[19] << "     20-" << pptCharList[20];
		cout << "     21-" << pptCharList[21] << "     22-" << pptCharList[22] << "     23-" << pptCharList[23];
		cout << "\n>> ";
		getline(cin, input);
		if(istringstream(input) >> newChallenger.pptChar)
		{
			if( (newChallenger.pptChar >= 0) && (newChallenger.pptChar <= 23) )
			{   validInput = true;   }
			else
			{   cout << TextColor_DarkRed << "Dropset value must be between 0-23... Please try again.\n" << TextColor_Reset;   }
		}
		else
		{   cout << TextColor_DarkRed << "ERROR: Invalid entry... Please try again.\n" << TextColor_Reset;   }
	} while(!validInput);
	validInput = false;
	do
	{
		cout << "PUYO or TETRIS:\n>> ";
		getline(cin, input);
		input = lowercaseString(input);
		if(input == "puyo")
		{
			newChallenger.prefersTetris = false;
			validInput = true;
		}
		else if(input == "tetris")
		{
			newChallenger.prefersTetris = true;
			validInput = true;
		}
		else
		{   cout << TextColor_DarkRed << "ERROR: Invalid entry... Please try again.\n" << TextColor_Reset;   }
	} while(!validInput);
	addItem_SOUND();
	challengers.push_back(newChallenger);
	cout << TextColor_Cyan << newChallenger.name << " was successfully added.\n" << TextColor_Reset;
	saveData();
}

/*
 * Removes the challenger from the board with a given challenger index
 * index is decremented to get the actual vector index when applicable (I.E. challenger #1 is actually at index 0)
 * Will prompt the user to confirm if they really want to remove the challenger at the given index.
 * If the user fully types out "yes" the challenger is erased from the vector.
 * Otherwise, nothing else will occur in the function.
 * PARAMETER: int 'index' denotes which reward to remove from the list. (with a value of 1 representing the first reward)
 */
void PuyoChallengeSystem::removeChallenger(int index)
{
	// The index has to be vaild for the vector (#1 representing 0)
	if( (index > 0) && ((unsigned) index <= challengers.size()) )
	{
		/*
		 * Ask if the user really wants to remove the challenger from the challenge board...
		 * [index] represents the challenger listing number (for display and is the numbering system the user sees)
		 * [index-1] represents the challenger's VECTOR index
		 */
		cout << "Do you really want to remove Challenger #" << index << " " << challengers[index-1].name << "? (type \"yes\" to confirm)\n";
		string userInput;
		getline(cin, userInput);
		userInput = lowercaseString(userInput);
		if(userInput == "yes")
		{
			removeItem_SOUND();
			cout << TextColor_DarkCyan << challengers[index-1].name << " was successfully removed.\n" << TextColor_Reset;
			// Remove the challenger from the vector
			challengers.erase(challengers.begin() + (index-1));
			saveData();
		}
		else
		{   cout << TextColor_DarkRed << "Challenger Removal Cancelled.\n" << TextColor_Reset;   }
	}
	else
	{   cout << TextColor_Red << "ERROR: Invalid Index Value (Out of Bounds).\n" << TextColor_Reset;   }
}

/*
 * Begins a puyo match with a random selection of opponents to Puyo battle against. (Based on win streak)
 *    1 if win streak is 0-9
 *    2 if win streak is 10-24
 *    3 if win streak is 25-49
 *    4 if win streak is 50 or higher
 * Opponents are determined with chooseOpponents()
 * The user is asked whether they won or lost the match. (and to state who won the match)
 *
 * If the user wins, the user is rewarded with points from the calculatePoints() method for the PointRewardSystem and their win streak is increased.
 * Otherwise, the user loses 1 HP for losing and their win streak is reset. (and is asked which CPU won the match)
 *
 * Any guest challengers that lose suffer a loss of 1 HP as well.
 *
 * Returns TRUE if the player wins the match... otherwise returns FALSE if the player loses.
 * PARAMETER: int reference variable that holds the points that the player wins (only utilized when the function returns TRUE).
 */
bool PuyoChallengeSystem::match(int& pointsEarned)
{
	beginMatch_SOUND();
	// choose game mode
	srand(time(NULL)); // Initialize Random Seed
	int gameMode = rand() % 5;
	cout << "Game Mode: " << pptModeList[gameMode];
	if((gameMode != 1) && (gameMode != 2))
	{
		bool playerTetris = rand() % 2;
		if(playerTetris)
		{   cout << " - Tetris";   }
		else
		{   cout << " - Puyo";   }
	}
	cout << "\n";
	// determine Opponents
	int opponentCount;
	if(winStreak >= 25) opponentCount = 3;
	else if(winStreak >= 10) opponentCount = 2;
	else opponentCount = 1;
	int opponents[opponentCount]; // PPT Character values (0-23)
	int guests[opponentCount];
	determineOpponents(opponents, guests, opponentCount);
	// display opponents and the user will play the match from there
	cout << "Challengers:";
	for(int i = 0; i < opponentCount; i++)
	{
		cout << "\n\t";
		if(guests[i] != -1)
		{
			cout << challengers[guests[i]].name << " (" << pptCharList[challengers[guests[i]].pptChar];
			if((gameMode != 1) && (gameMode != 2))
			{
				if(challengers[guests[i]].prefersTetris)
				{   cout << "/Tetris";   }
				else
				{   cout << "/Puyo";   }
			}
			cout << ")";
		}
		else
		{
			cout << pptCharList[opponents[i]];
			if((gameMode != 1) && (gameMode != 2))
			{
				if(challengers[guests[i]].prefersTetris)
				{
					bool TetrisMode = rand() % 2;
					if(TetrisMode)
					{   cout << " (Tetris)";   }
					else
					{   cout << " (Puyo)";   }
				}
			}
		}
	}
	cout << "\n";
	// query the user whether they won or lost the match... will not move on until the user enters either "win" or "lose"
	bool keepRunning = true;
	bool victory;
	do{
		cout << "Enter \"win\" or \"lose\" for result\n>> ";
		string input;
		getline(cin, input);
		input = lowercaseString(input);
		if(input == "win")
		{
			keepRunning = false;
			bool defeatedAChallenger = false;
			// winstreak is increased
			winStreak++;
			cout << "Win Streak is now: " << winStreak << " wins.\n";
			// look through the guests and any challengers found will have their HP decreased (-1 is empty and indicates a Puyo character was an opponent instead
			int guestsBattled = 0;
			for (int i = 0; i < opponentCount; i++)
			{
				if(guests[i] != -1)
				{
					challengers[guests[i]].HP--;
					cout << challengers[guests[i]].name << " is now at ";
					if(challengers[guests[i]].HP <= 5)
					{   cout << TextColor_DarkRed;   }
					cout << challengers[guests[i]].HP << " HP.\n";
					guestsBattled++;
					// Check if the challenger has reached 0 HP to set the appropriate sound Cue
					if(challengers[guests[i]].HP == 0)
					{   defeatedAChallenger = true;   }
				}
			}
			// SOUND CUE
			if(defeatedAChallenger)
			{   challengerDefeated_SOUND();   }
			else
			{   win_SOUND();   }
			/*
			 * Run through the challenge board in reverse order to remove any challengers with zero (or less) HP, signifying they are defeated
			 * Done in reverse order so that removal does not affect parsing through the vector elements
			 */
			for(int i = (challengers.size() - 1); i >= 0; i--)
			{
				if(challengers[i].HP == 0)
				{
					cout << TextColor_Magenta << challengers[i].name << " has been defeated.\n" << TextColor_Reset;
					challengers.erase(challengers.begin() + i);
				}
			}
			// calculate points earned
			pointsEarned = calculatePoints(guestsBattled);
			victory = true;
		}
		else if(input == "lose")
		{
			keepRunning = false;
			// No points earned (and no challenger loses HP)
			// winStreak is also reset
			lose_SOUND();
			cout << TextColor_DarkMagenta << "Better luck next time.\n" << TextColor_Reset;
			winStreak = 0;
			pointsEarned = 0;
			victory = false;
		}
		else
		{   cout << TextColor_DarkRed << "Invalid Input... please try again.\n" << TextColor_Reset;   }
	}while(keepRunning);
	saveData();
	return victory;
}

/*
 * Outputs the available challengers in a Table format
 * Energy (from the Point Reward System) and win streak are shown in the header
 * Then all the challengers are displayed in a list
 * PARAMETER: int 'userEnergy' that hold's the User's Energy to display
 */
void PuyoChallengeSystem::display(int userEnergy)
{
	cout << "╔══════════════════════════════════════════════════════════\n"
			<< "║ Energy: ";
	if(userEnergy <= 0)
		{   cout << TextColor_DarkRed;   }
	cout << userEnergy << TextColor_Reset << "\n";
	cout << "║ Win Streak: " << winStreak << "\n"
			<< "╠═════╤════════════════════════════════════════════════════\n";
	for(unsigned int i = 0; i < challengers.size(); i++)
	{
		cout << "╟ #";
		if(i < 9)
		{   cout << " ";   }
		cout << (i+1) << " │ ";
		if(challengers[i].HP <= 5)
		{   cout << TextColor_DarkRed;   }
		cout << "[" << challengers[i].HP << "HP]" << TextColor_Reset << " " << challengers[i].name << " (" << pptCharList[challengers[i].pptChar] << "/";
		if(challengers[i].prefersTetris)
		{   cout << "Tetris";   }
		else
		{   cout << "Puyo";   }
		cout << ")\n";
	}
	cout << "╚═════╧════════════════════════════════════════════════════\n";
}

/*
 * Randomly selects opponents for the opponent array
 * Runs through the challenge board for challengers using the same dropset as the chosen character, and adds them to the guests vector on the corresponding index (otherwise puts -1 to denote empty)
 * PARAMETER: int array that is the opponents array (holds Puyo dropsets)
 * PARAMETER: int array that is the guests array (holds the challenger number corresponding to the opponent array, or -1 to use the Puyo character instead)
 * PARAMETER: int variable that holds the number of opponents to select (and is also the size of the two arrays
 */
void PuyoChallengeSystem::determineOpponents(int opponents[], int guests[], int opponentCount)
{
	// Initialize Random Seed
	srand(time(NULL));
	for(int i = 0; i < opponentCount; i++)
	{
		opponents[i] = rand() % 24;
		unsigned int i2 = 0;
		guests[i] = -1;
		while( (guests[i] == -1) && (i2 < challengers.size()))
		{
			if(challengers[i2].pptChar == opponents[i])
			{
				guests[i] = i2;
			}
			i2++;
		}
	}
}

/*
 * Calculates points earned by the player when winning a Puyo match
 * Requires the following variables for the points earned equation: winStreak and guestCount
 * winStreak is already a class wide variable
 * PARAMETER: int variable that represents the guestCount, the variable that counted how many challengers from the challenge board participated
 */
int PuyoChallengeSystem::calculatePoints(int guestCount)
{
	int baseCredits;
	int earnedPoints = 0;
	bool valid = false;
	do
	{
		cout << "Credits Earned:\n>> ";
		string input;
		getline(cin, input);
		if(istringstream(input) >> baseCredits)
		{
			// Initialize Random Seed
			srand(time(NULL));
			earnedPoints = baseCredits + ( baseCredits * (rand() % winStreak) / 100 );
			valid = true;
		}
		else
		{   cout << TextColor_DarkRed << "ERROR: Invalid entry... Please try again.\n" << TextColor_Reset;   }
	} while(!valid);
	return earnedPoints;
}

/*
 * Parses a string and changes every character to its respective lowercase variant using "tolower()" on each character
 * Returns the string the user provided, but with all capital letters changed to lowercase
 * PARAMETER: string variable to parse through.
 */
string PuyoChallengeSystem::lowercaseString(string inString)
{
	for(unsigned int i = 0; i < inString.length(); i++)
	{	inString[i] = tolower(inString[i]);	}
	return inString;
}
