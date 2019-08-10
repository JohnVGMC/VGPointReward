/*
 * PuyoChallengeSystem.h
 * < PuyoChallengeSystem Class and PuyoChallengerNode Struct Header File >
 *
 *  Created on: Dec 25, 2017
 *      Author: JohnVGC
 *
 *  Version 1.0.0
 *
 *  base version
 */

#ifndef PUYOCHALLENGESYSTEM_H_
#define PUYOCHALLENGESYSTEM_H_

#include <string>
#include <vector>

	const std::string pptCharList[24] =
	{
			"Ringo",
			"Risukuma",
			"Schezo",
			"O",
			"Tee",
			"Maguro",
			"Amitie",
			"Feli",
			"Ai",
			"Ess",
			"Klug",
			"Sig",
			"Draco Centauros",
			"Zed",
			"Jay & Elle",
			"Arle & Carbuncle",
			"Suketoudara",
			"Dark Prince",
			"Rulue",
			"Raffine",
			"Witch",
			"Ecolo",
			"Ex",
			"Lemres"
	}; // Always size 24

	const std::string pptModeList[5] =
	{
			"Versus",
			"Swap",
			"Fusion",
			"Party",
			"Big Bang"
	}; //always size 5

struct PuyoChallengerNode
{
	std::string name;
	int HP, pptChar;
	bool prefersTetris;
};

class PuyoChallengeSystem
{
private:
	int winStreak;
	std::vector<PuyoChallengerNode> challengers;


	/*
	 * Randomly selects opponents for the opponent array
	 * Runs through the challenge board for challengers using the same dropset as the chosen character, and adds them to the guests vector on the corresponding index (otherwise puts -1 to denote empty)
	 * PARAMETER: int array that is the opponents array (holds Puyo dropsets)
	 * PARAMETER: int array that is the guests array (holds the challenger number corresponding to the opponent array, or -1 to use the Puyo character instead)
	 * PARAMETER: int variable that holds the number of opponents to select (and is also the size of the two arrays
	 */
	void determineOpponents(int[], int[], int);

	/*
	 * Calculates points earned by the player when winning a Puyo match
	 * Requires the following variables for the points earned equation: winStreak and guestCount
	 * winStreak is already a class wide variable
	 * PARAMETER: int variable that represents the guestCount, the variable that counted how many challengers from the challenge board participated
	 */
	int calculatePoints(int);

	/*
	 * Parses a string and changes every character to its respective lowercase variant for easier string comparison
	 * Returns the string the user provided, but with all capital letters changed to lowercase
	 * PARAMETER: string variable to parse through.
	 */
	std::string lowercaseString(std::string);

public:
	/* Default Constructor */
	PuyoChallengeSystem();

	/* Destructor */
	~PuyoChallengeSystem();

	/*
	 * Reads the save file "challengers.csv" to initialize the challenger board
	 * If "challengers.csv" is unable to be read or doesn't exist, then the user can choose to start with a blank slate: No win streak and empty challenger board.
	 * RETURNS True if the save data could be read and was not empty.
	 * RETURNS False otherwise (for the purpose of asking the user to continue the program or not, incase a save IS suppose to be present)
	 */
	bool loadData();

	/*
	 * Writes the overall state of the Puyo Challenge Board (challengers and their HP & the user's win streak) into the save file "challengers.csv"
	 * This function is ideally used whenever a change has occurred into the challenge board.
	 */
	void saveData();

	/*
	 * Adds a new challenger to the puyo challenge board
	 * Queries the user about the necessary attributes about the new challenger.
	 */
	void addChallenger();

	/*
	 * Removes a challenger from the board with a given challenger index
	 * Will prompt the user to confirm if they really want to remove the challenger at the given index.
	 * PARAMETER: int variable that denotes which reward to remove from the list.
	 */
	void removeChallenger(int);

	/*
	 * Begins a puyo match with a random selection of opponents to Puyo battle against.
	 * The user is asked whether they won or lost the match. (and to state who won the match)
	 *
	 * If the user wins, the user is rewarded with points for the PointRewardSystem and their win streak is increased.
	 * Challengers lose 1 HP for each of their participation.
	 * Otherwise, the user's win streak is reset.
	 *
	 * Returns TRUE if the player wins the match... otherwise returns FALSE if the player loses.
	 * PARAMETER: int reference variable that holds the points that the player wins (only utilized when the function returns TRUE).
	 */
	bool match(int&);

	/*
	 * Displays the available challengers in a Table format
	 * HP (from the Point Reward System) and win streak are shown in the header
	 * Then all the challengers are displayed in a list
	 * PARAMETER: int variable that hold's the User's Energy
	 */
	 void display(int);

};

#endif /* PUYOCHALLENGESYSTEM_H_ */
