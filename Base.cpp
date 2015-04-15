#include "Base.hpp"

Base::Base()
{
	lastResourceGain = time(0);
	maxHealth = 5000;
	currentHealth = maxHealth;
	resources = 500;
	rps = 10;
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
	return false;
}

bool Base::UpgradeBaseHealth()
{
	return false;
}

int Base::GetNumWeapons()
{
	return 0;
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
