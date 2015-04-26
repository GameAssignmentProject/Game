#include "BaseWeapon.hpp"

BaseWeapon::BaseWeapon(IMesh* theMesh, bool thePlayer)
{
	damage = 20;
	range = 30;
	playerOwned = thePlayer;
	if (playerOwned)
	{
		ballistaModel = theMesh->CreateModel(-110, 29, -2.5f);
	}
	else
	{
		ballistaModel = theMesh->CreateModel(110, 29, 2.5f);
		ballistaModel->RotateLocalY(180);
	}
	ballistaModel->SetSkin("Troops\\Ballista\\Ballista.png");
	ballistaModel->RotateLocalX(180);
	lastAttack = 0;
}

void BaseWeapon::UpgradeBaseWeapon()
{
	damage += 20;
	range += 20;
}

int BaseWeapon::GetDamage()
{
	return damage;
}

int BaseWeapon::GetPosition()
{
	return ballistaModel->GetX();
}

int BaseWeapon::GetRange()
{
	return range;
}

/*Returns the time when the troop attacked last*/
time_t BaseWeapon::GetLastAttack()
{
	return lastAttack;
}

/*Sets the time the troop last attacked*/
void BaseWeapon::SetLastAttack(time_t theTime)
{
	lastAttack = theTime;
}
