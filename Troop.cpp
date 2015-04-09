#include "Troop.hpp"


Troop::Troop(TroopName name, bool playerMade, I3DEngine* theEngine)
{
	troopMesh = theEngine->LoadMesh("quad.x");
	if (playerMade)
	{
		troopModel = troopMesh->CreateModel(-70, 10, 0); // -60, 10, 0, spawning location for player base
	}
	else
	{
		troopModel = troopMesh->CreateModel(70, 10, 0); // 60, 10, 0, spawning location for enemy base
		troopModel->RotateLocalY(180);
	}
	troopModel->RotateLocalX(180);
	playerOwned = playerMade;
	lastAttack = time(0);
	switch (name) // from the type, determine the stats of the troop
	{
	case Swordsman:
		playerOwned = playerMade;
		troopModel->SetSkin("Swordsman.png");
		troopModel->SetLocalZ(0.1);
		maxHealth = 40;
		currentHealth = maxHealth;
		damage = 8;
		speed = 3;
		range = 10;
		cost = 10;
		break;
	case Archer:
		playerOwned = playerMade;
		troopModel->SetSkin("Archer.png");
		troopModel->SetLocalZ(0.2);
		maxHealth = 20;
		currentHealth = maxHealth;
		damage = 5;
		speed = 5;
		range = 20;
		cost = 20;
		break;
	case Spearman:
		playerOwned = playerMade;
		troopModel->SetSkin("Spearman.png");
		troopModel->SetLocalZ(0.3);
		maxHealth = 25;
		currentHealth = maxHealth;
		damage = 12;
		speed = 2;
		range = 15;
		cost = 25;
		break;
	case Cavalry:
		playerOwned = playerMade;
		troopModel->SetSkin("Cavalry.jpg");
		troopModel->SetLocalZ(0.4);
		maxHealth = 60;
		currentHealth = maxHealth;
		damage = 15;
		speed = 8;
		range = 12;
		cost = 65;
		break;
	}
}

IMesh* Troop::GetMesh()
{
	return troopMesh;
}

IModel* Troop::GetModel()
{
	return troopModel;
}

/*Move the troop by the amount along the x (along the map)*/
void Troop::Move(float x)
{
	troopModel->MoveX(x);
}
/*Returns the value of the troops position on the x*/
float Troop::GetPosition()
{
	return troopModel->GetX();
}
/*Returns the speed of the troop, used when moving the unit*/
int Troop::GetSpeed()
{
	return speed;
}
/* returns the health of each unit*/
int Troop::GetHealth()
{
	return currentHealth;
}

bool Troop::TakeDamage(int damage)
{
	currentHealth -= damage;
	if (currentHealth <= 0)
	{
		currentHealth = 0;
		return true;
	}
	return false;
}

int Troop::GetDamage()
{
	return damage;
}

/* returns the range of each type of unit*/
int Troop::GetRange()
{
	return range;
}

time_t Troop::GetLastAttack()
{
	return lastAttack;
}

void Troop::SetLastAttack(time_t theTime)
{
	lastAttack = theTime;
}

int Troop::GetCost()
{
	return cost;
}