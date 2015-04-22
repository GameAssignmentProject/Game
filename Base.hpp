#pragma once

#include "TL-Engine.h"
#include <ctime>

using namespace tle;

class Base {
private:
	time_t lastResourceGain;
	int maxHealth;
	int currentHealth;
	int resources;
	int rps; // resources gained per second
	int numWeapons;
	int healthUpgradeCost;
	int rpsUpgradeCost;
public:
	Base();
	void SetHealth(int health);
	int GetHealth();
	void SetMaxHealth(int health);
	int GetMaxHealth();
	bool TakeDamage(int damage);
	void SpendResources(int amount);
	void GainResources(int amount);
	bool UpgradeResourceGather();
	bool UpgradeBaseHealth();
	int GetHealthUpgradeCost();
	int GetRPSUpgradeCost();
	int GetNumWeapons();
	void SetNumWeapons(int num);
	bool PurchaseWeapon();
	int GetResources();
	time_t GetLastResourceGain();
	void SetLastResourceGain(time_t last);
	int GetRPS();
	void SetRPS(int amount);
};