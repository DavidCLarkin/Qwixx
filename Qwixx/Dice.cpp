// Source file for the Dice class
// Author: David Larkin, 20070186

#include "stdafx.h"
#include "dice.h"
#include <cstdlib>
#include <ctime>
#include <vector>

using namespace std;

Dice::Dice(string type, int roll, int sides) : m_Sides(sides), m_Type(type)
{
}

void Dice::roll(int numbOfRolls)
{
	vector<int> rolls;

	for (int i = 0; i < numbOfRolls; i++)
	{
		int roll = (rand() % m_Sides) + 1;
		rolls.push_back(roll);
	}

	cout << "Your rolls are : " << endl;
	for (vector<int>::iterator i = rolls.begin(); i != rolls.end(); ++i)
	{
		cout << "\t" << *i << endl;
	}

}

const int Dice::roll()
{
	int roll = (rand() % m_Sides) + 1;
	return roll;
}

void Dice::setSides(int sides)
{
	m_Sides = sides;
}

int Dice::getSides()
{
	int sides;
	cout << "How many sides do you want?" << endl;
	cin >> sides;
	return sides;
}

string* Dice::getType()
{
	return &m_Type;
}

int* Dice::getRoll()
{
	return &m_Roll;
}

void Dice::setRoll(int roll)
{
	m_Roll = roll;
}

Dice::~Dice()
{
	//cout << "Destroying dice.\n";
}

ostream& operator <<(ostream &stream, Dice &dice)
{
	return stream << "Dice(" << "Type: " << *(&dice.m_Type) << ", " << "Roll: " << *(&dice.m_Roll) << ")"; //dereferencing so it shows values, not addresses
}