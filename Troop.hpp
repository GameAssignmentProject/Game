#pragma once
#include <TL-Engine.h>
#include <ctime>

using namespace tle;

/* Used to identify the type of each troop
Needed to assign stats and also set what
type of troops to be effective against*/
enum TroopName
{
	Swordsman,
	Archer,
	Spearman,
	Cavalry
};

class Troop
{
private:
	IMesh* troopMesh;//mesh and model may not be needed when using sprites
	IModel* troopModel;
	bool playerOwned; // used to determine which way to move and where to spawn
	TroopName name; // Type of troop
	int maxHealth; // the maximum amount of health for each troop 
	int currentHealth; // the current health of each unit
	int damage; // the set amount of damage a particular troop will do
	int cost; // resources cost of each troop
	int cooldown; // amount of time between spawning of the units
	int speed; // movement speed
	int range; // distance at which the troop will begin attacking
	TroopName effectiveAgainst;
	time_t lastAttack;
public:
	Troop(TroopName name, bool playerOwned, I3DEngine* theEngine);
	IMesh* GetMesh();
	IModel* GetModel();
	void Move(float x); // move by x amount
	float GetPosition(); // returns the x position of the troop
	TroopName GetName();
	void SetHealth(int health);
	int GetMaxHealth();
	int GetHealth();
	bool TakeDamage(int amount);
	bool UpgradeTroop();
	int GetDamage();
	int GetCost();
	int GetCooldown();
	int GetSpeed();
	int GetRange();
	TroopName GetEffective();
	void Fire();
	time_t GetLastAttack();
	void SetLastAttack(time_t theTime);
};