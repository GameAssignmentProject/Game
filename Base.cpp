#include "Base.hpp"

Base::Base()
{
	lastResourceGain = time(0);
	maxHealth = 5000;
	currentHealth = maxHealth;
	resources = 50000;
	rps = 10;
	numWeapons = 0;
	healthUpgradeCost = 500;
	rpsUpgradeCost = 500;
}

void Base::SetHealth(int health)
{
	if (health < maxHealth)
	{
		currentHealth = health;
	}
	else
	{
		currentHealth = maxHealth;
	}
}

int Base::GetHealth()
{
	return currentHealth;
}

void Base::SetMaxHealth(int health)
{
	maxHealth = health;
}

int Base::GetMaxHealth()
{
	return maxHealth;
}

bool Base::TakeDamage(int damage)
{
	currentHealth -= damage;
	if (currentHealth <= 0)
	{
		return true;
	}
	return false;
}

void Base::SpendResources(int amount)
{
	resources -= amount;
}

void Base::GainResources(int amount)
{
	resources += amount;
}

bool Base::UpgradeResourceGather()
{
	rps += 10;
	rpsUpgradeCost *= 2;
	return true;
}

bool Base::UpgradeBaseHealth()
{
	int increaseAmount = static_cast<int>(maxHealth * 0.5);
	maxHealth += increaseAmount;
	currentHealth += increaseAmount;
	healthUpgradeCost *= 2;
	return true;
}

int Base::GetHealthUpgradeCost()
{
	return healthUpgradeCost;
}

int Base::GetRPSUpgradeCost()
{
	return rpsUpgradeCost;
}

int Base::GetNumWeapons()
{
	return numWeapons;
}

void Base::SetNumWeapons(int num)
{
	numWeapons = num;
}

bool Base::PurchaseWeapon()
{
	return false;
}

int Base::GetResources()
{
	return resources;
}

time_t Base::GetLastResourceGain()
{
	return lastResourceGain;
}

void Base::SetLastResourceGain(time_t last)
{
	lastResourceGain = last;
}

int Base::GetRPS()
{
	return rps;
}

void Base::SetRPS(int amount)
{
	rps = amount;
}
