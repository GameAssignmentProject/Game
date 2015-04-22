#pragma once
#include "TL-Engine.h"
#include <ctime>

using namespace tle;

class BaseWeapon 
{
private:
	int damage;
	int range;
	IModel* ballistaModel;
	bool playerOwned; // used to determine which way to move and where to spawn
	time_t lastAttack;
public:
	BaseWeapon(IMesh* theMesh, bool thePlayer);
	~BaseWeapon();
	void UpgradeBaseWeapon();
	int GetDamage();
	int GetPosition();
	int GetRange();
	time_t GetLastAttack(); // Returns the last attack by the troop
	void SetLastAttack(time_t theTime); // Set the last time a troop attacked
};