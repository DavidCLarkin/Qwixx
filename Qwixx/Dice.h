// Header file for Dice
// Author: David Larkin, 20070186
#pragma once
#ifndef DICE_H
#define DICE_H

//Die Thrower
//Simulates throwing a dice/die

#include<iostream>;
#include <string>;

using namespace std;

class Dice
{
public:
	Dice(string type, int roll, int sides = 6);
	~Dice();
	void setSides(int sides);
	void roll(int numbOfRolls);
	const int roll();
	int getSides();
	string* getType();
	void setRoll(int roll);
	int* getRoll();
private:
	friend ostream& operator <<(ostream &stream, Dice &dice);
	int m_Sides;
	string m_Type;
	int m_Roll;

};
#endif // !dice_hpp
