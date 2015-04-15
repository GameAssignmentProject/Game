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
	Cavalry,
	SiegeMachine
};

class Troop
{
private:
	IModel* troopModel;
	bool playerOwned; // used to determine which way to move and where to spawn
	TroopName theName; // Type of troop
	int maxHealth; // the maximum amount of health for each troop
	int currentHealth; // the current health of each unit
	int damage; // the set amount of damage a particular troop will do
	int cost; // resources cost of each troop
	int upgradeCost; // resources cost to upgrade the troop to higher stats.
	int cooldown; // amount of time between spawning of the units
	int speed; // movement speed
	int range; // distance at which the troop will begin attacking
	TroopName effectiveAgainst; // what troop does this type of troop do extra damage to
	time_t lastAttack; // time that the last attack initiated by this troop was
public:
	Troop(TroopName name); // Initial constructor, creates a troop with all the details to be a blueprint
	Troop::Troop(Troop* theBlueprint); // Takes a blueprint troop and recreates the troop to be used later
	void SpawnTroop(bool playerOwned, I3DEngine* theEngine, IMesh* troopMesh); // Takes the troop created from the blueprint and spawns it
	IModel* GetModel(); // Returns a pointer to the model the troop uses
	void Move(float x); // Moves the troop x amount in their forward direction
	float GetPosition(); // Returns the x position of the troop
	TroopName GetName(); // Returns the troop type of the troop
	void SetHealth(int health); // Set the health of the troop
	int GetMaxHealth(); // Returns the max health the troop
	int GetHealth(); // Return the current health the troop has
	bool TakeDamage(Troop* attacker); // Reduce the current helth by damage and determines whether the troop is dead
	bool UpgradeTroop(); // Upgrades the troop to the next tier
	int GetDamage(); // Returns the amount of damamge a troop does
	int GetCost(); // Returns the cost of spawning the troop
	int GetCooldown(); // Returns the amount of time needed to wait between each spawn of the troop
	int GetSpeed(); // Returns the amount a troop can move each tick
	int GetRange(); // Returns the range at which troops can attack the enemy
	TroopName GetEffective(); // Returns the troop type which the current troop is effective against
	void Fire(); //Only used by archers, instead of attacking directly, fires an arrow, which on collision does damage
	time_t GetLastAttack(); // Returns the last attack by the troop
	void SetLastAttack(time_t theTime); // Set the last time a troop attacked
};