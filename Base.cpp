#include "Base.hpp"

Base::Base()
{
	lastResourceGain = time(0);
	maxHealth = 100;
	currentHealth = maxHealth;
	resources = 100;
	rps = 1;
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

void Base::TakeDamage(int damage)
{
	currentHealth -= damage;
	if (currentHealth <= 0)
	{
		currentHealth = 0;
	}
}

int Base::GetResources()
{
	return resources;
}

void Base::SpendResources(int amount)
{
	resources -= amount;
}

void Base::GainResources(int amount)
{
	resources += amount;
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
