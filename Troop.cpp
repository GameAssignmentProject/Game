#include "Troop.hpp"

Troop::Troop(TroopName name)
{
	switch (name) // from the type, determine the stats of the troop
	{
	case Swordsman:
		theName = Swordsman;
		maxHealth = 150;
		damage = 10;
		cost = 100;
		cooldown = 2;
		speed = 5;
		range = 10;
		effectiveAgainst = Archer;
		break;
	case Archer:
		theName = Archer;
		maxHealth = 100;
		damage = 8;
		cost = 150;
		cooldown = 3;
		speed = 7;
		range = 30;
		effectiveAgainst = Spearman;
		break;
	case Spearman:
		theName = Spearman;
		maxHealth = 150;
		damage = 15;
		cost = 250;
		cooldown = 5;
		speed = 4;
		range = 15;
		effectiveAgainst = Cavalry;
		break;
	case Cavalry:
		theName = Cavalry;
		maxHealth = 500;
		damage = 15;
		cost = 500;
		cooldown = 15;
		speed = 10;
		range = 12;
		effectiveAgainst = Swordsman;
		break;
	case SiegeMachine:
		theName = SiegeMachine;
		maxHealth = 1000;
		damage = 100;
		cost = 1000;
		cooldown = 30;
		speed = 2;
		range = 5;
	}
	currentHealth = maxHealth;
}

Troop::Troop(Troop* theBlueprint)
{
	theName = theBlueprint->GetName();
	maxHealth = theBlueprint->GetMaxHealth();
	currentHealth = theBlueprint->GetHealth();
	damage = theBlueprint->GetDamage();
	cost = theBlueprint->GetCost();
	cooldown = theBlueprint->GetCooldown();
	speed = theBlueprint->GetSpeed();
	range = theBlueprint->GetRange();
	effectiveAgainst = theBlueprint->GetEffective();
	lastAttack = 0;
}

void Troop::SpawnTroop(bool playerMade, I3DEngine* theEngine, IMesh* troopMesh)
{
	playerOwned = playerMade;
	if (playerMade) // if the player owns the new troop
	{
		troopModel = troopMesh->CreateModel(-120, 10, 0);
	}
	else // if the enemy owns the new troop
	{
		troopModel = troopMesh->CreateModel(120, 10, 0);
		troopModel->RotateLocalY(180);
	}
	switch (theName)
	{
	case Swordsman:
		troopModel->SetSkin("Troops\\Swordsman\\Swordsman.png");
		break;
	case Archer:
		troopModel->SetSkin("Troops\\Archer\\Archer.png");
		break;
	case Spearman:
		troopModel->SetSkin("Troops\\Spearman\\Spearman.png");
		break;
	case Cavalry:
		troopModel->SetSkin("Troops\\Cavalry\\Cavalry.png");
		break;
	case SiegeMachine:
		troopModel->SetSkin("Troops\\SiegeMachine\\SiegeMachine.png");
		break;
	}
	troopModel->RotateLocalX(180);
}

IModel* Troop::GetModel()
{
	return troopModel;
}

/*Move the troop by the amount along the x (along the map)*/
void Troop::Move(float x)
{
	troopModel->MoveLocalX(x);
}
/*Returns the value of the troops position on the x*/
float Troop::GetPosition()
{
	return troopModel->GetX();
}

/*Returns the troop type of the troop, used to determine if it is weak against certain unit*/
TroopName Troop::GetName()
{
	return theName;
}

/*Sets the current health of the troop, if the value passes is more than max health then the health is set to max health*/
void Troop::SetHealth(int health)
{
	if (health > maxHealth)
	{
		currentHealth = maxHealth;
	}
	else
	{
		currentHealth = health;
	}
}

/*Gets the max health of the troop*/
int Troop::GetMaxHealth()
{
	return maxHealth;
}

/* returns the health of each unit*/
int Troop::GetHealth()
{
	return currentHealth;
}

/*Calculates the new health of the troop after recieving damage amount
  if the troop has 0 or less health after being damaged, returns true
  indicating death of the troop else returns false*/
bool Troop::TakeDamage(Troop* attacker)
{
	if (attacker->GetEffective() == theName)
	{
		currentHealth -= attacker->GetDamage() * 3;
	}
	else if (effectiveAgainst == attacker->GetName())
	{
		currentHealth -= attacker->GetDamage() / 3;
	}
	else
	{
		currentHealth -= attacker->GetDamage();
	}
	if (currentHealth <= 0)
	{
		currentHealth = 0;
		return true;
	}
	return false;
}

/*Only to be used for the blueprint troops
  Upgrades the troop type to have better stats*/
bool Troop::UpgradeTroop()
{
	return false;
}

/*Returns the amount of damage that a troop will do against the enemy*/
int Troop::GetDamage()
{
	return damage;
}

/*Returns the cost of spawning the unit*/
int Troop::GetCost()
{
	return cost;
}

/*Returns the amount of time in seconds needed to wait between each spawn of the type of troop*/
int Troop::GetCooldown()
{
	return cooldown;
}

/*Returns the speed of the troop, used when moving the unit*/
int Troop::GetSpeed()
{
	return speed;
}

/*Returns the range of each type of unit*/
int Troop::GetRange()
{
	return range;
}

/*Returns the troop type which the current troop is effective against*/
TroopName Troop::GetEffective()
{
	return effectiveAgainst;
}

/*Only used for archers, instead of damaging units directly, the archer fires an arrow which does damage on collision*/
void Troop::Fire()
{

}

/*Returns the time when the troop attacked last*/
time_t Troop::GetLastAttack()
{
	return lastAttack;
}

/*Sets the time the troop last attacked*/
void Troop::SetLastAttack(time_t theTime)
{
	lastAttack = theTime;
}