/*
 * PointRewardSystem.h
 * < PointRewardSystem Class and PointRewardNode Struct Header File >
 *
 *  Created on: Dec 21, 2017
 *  Updated on: Feb 10, 2018
 *      Author: JohnVGC
 *
 *  Version 1.2.0
 */

#ifndef POINTREWARDSYSTEM_H_
#define POINTREWARDSYSTEM_H_

#include <string>
#include <vector>

struct PointRewardNode
{
	std::string name, tag;
	int level, maxLevel, baseCost;
	float percentInc;
	/*
	 * About maxLevel
	 * If maxLevel is 0, the reward levels up indefinitely until it is manually removed
	 * If maxLevel is 1, the reward is one time and doesn't have a level (or at least not displayed)
	 * Else, level spans from 1 to maxLevel, and once redeemed while level == maxLevel, the reward is removed automatically.
	 */
};

class PointRewardSystem
{
private:
	int energy;
	int points; // Merit points collected by the user.
	int diamonds; // Diamonds collected by the user (can instantly levelup an item if merit points are insufficient).
	std::vector<PointRewardNode> rewards;

	/*
	 * Returns the max Energy that the user can recover
	 */
	int getMaxEnergy();

	/*
	 * Returns the cost to reach the next level of a reward
	 * PARAMETER: int variable that holds the VECTOR index of the reward
	 */
	int getNextLevelCost(int);

	/*
	 * Parses a string and changes every character to its respective lowercase variant for easier string comparison
	 * Returns the string the user provided, but with all capital letters changed to lowercase
	 * PARAMETER: string variable to parse through.
	 */
	std::string lowercaseString(std::string);

public:
	/* Default Constructor */
	PointRewardSystem();

	/* Destructor */
	~PointRewardSystem();

	/*
	 * Reads the save file "rewards.csv" to initialize the reward table
	 * If "rewards.csv" is unable to be read or doesn't exist, then the user can choose to start with a blank slate: No points and empty inventory.
	 * RETURNS True if the save data could be read and was not empty.
	 * RETURNS False otherwise (for the purpose of asking the user to continue the program or not, incase a save IS suppose to be present)
	 */
	bool loadData();

	/*
	 * Writes the overall state of the Point Reward System (rewards, points earned, and the User's HP) into the save file "rewards.csv"
	 * This function is ideally used whenever a change has occurred into the reward list.
	 */
	void saveData();

	/*
	 * Adds a new reward to the reward list
	 * Queries the user about the necessary attributes about the new reward.
	 */
	void addReward();

	/*
	 * Removes a reward from the list with a given reward index
	 * Reward removal can occur manually by the user or automatically when redeeming a reward at max level
	 * If manual, Will prompt the user to confirm if they really want to delete the reward at the given index.
	 * PARAMETER: int variable that denotes which reward to remove from the list.
	 * PARAMETER: bool variable that denotes if the reward removal is a manual removal by the user or not.
	 */
	void removeReward(int, bool);

	/*
	 * Gains merit points for the user.
	 * PARAMETER: int variable that holds the number of points that the user will earn.
	 * PARAMETER: bool variable that dictates whether or not to play the sound file specific for gaining merit points
	 *            typically false when called within another command not specifically "addpoints" that have their own sound cues
	 */
	void addPoints(int, bool);

	/*
	 * Gains a diamond for the user. (Only increments one at a time on each function call as these are a very rare item).
	 */
	void addDiamond();

	/*
	 * Redeems a Reward and levels it up.
	 * The Level up will only occur if the user has accumulated enough merit points to fulfill the level up cost.
	 * PARAMETER: int variable that denotes which reward to level up from the list.
	 */
	void levelUp(int);

	/*
	 * Decreases the user's Energy.
	 * Will let the user know if their Energy is already depleted when attempting to spend Energy again.
	 * User must also have sufficient energy to spend.
	 * PARAMETER: int variable that holds the amount of HP being subtracted.
	 * PARAMETER: bool variable that dictates whether or not to play the sound file specific for taking damage
	 *            typically false when called within another command not specifically "damage()" that have their own sound cues
	 */
	 void spendEnergy(int, bool);

	/*
	 * Increases the user's Energy by a random amount
	 * Will let the user know if their Energy is already at capacity.
	 * Anytime the User recovers HP that may exceed the maximum HP allowed at their current state, excess HP is taken away to make it equal to the max.
	 */
	 void recover();

	/*
	 * Displays the available rewards in a Table format
	 * HP is shown first
	 * Then all the items are displayed in a list
	 */
	 void display();

	 /*
	  * Returns true whether the user's Energy is depleted. False otherwise
	  * Certain functions cannot be performed unless the User has Energy to Spend.
	  */
	 bool isFainted();

	/*
	 * Returns the User's current HP (for Puyo Challenge display purposes)
	 */
	int getEnergy();
};


#endif /* POINTREWARDSYSTEM_H_ */
