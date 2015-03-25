// Game_Of_Way.cpp: A program using the TL-Engine
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <list>
#include <sstream>
#include <ctime>

using namespace std;
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

class Base {
private:
	time_t lastResourceGain;
	int maxHealth;
	int currentHealth;
	int resources;
	int rps; // resources gained per second
public:
	Base();
	~Base();
	void SetHealth(int health);
	int GetHealth();
	void SetMaxHealth(int health);
	int GetMaxHealth();
	void TakeDamage(int damage);
	void SpendResources(int amount);
	void GainResources(int amount);
	bool UpgradeResourceGather();
	bool UpgradeBaseHealth();
	int GetNumWeapons();
	bool PurchaseWeapon();
	int GetResources();
	time_t GetLastResourceGain();
	void SetLastResourceGain(time_t last);
	int GetRPS();
	void SetRPS(int amount);
};

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


/*Troop Blueprints*/
Troop* troopBlueprints[4];

bool CompPlayerTroopX(Troop* first, Troop* second)
{
	return (first->GetPosition() > second->GetPosition());
}

bool CompEnemyTroopX(Troop* first, Troop* second)
{
	return (first->GetPosition() <= second->GetPosition());
}

void main()
{
	srand(time(0));

	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");
	myEngine->AddMediaFolder("C:\\ProgramData\\TL-Engine\\Media");
	//myEngine->AddMediaFolder("I:\\GroupGame\\GroupGame\\Media");

	/**** Set up your scene here ****/

	IMesh* cubeMesh;
	IMesh* quadMesh;
	IMesh* skyBoxMesh;

	IModel* quad;
	IModel* ground;
	IModel* playerBaseModel;
	IModel* enemyBaseModel;
	IModel* quadScene;
	IModel* skyBox;

	cubeMesh = myEngine->LoadMesh("Cube.x");
	ground = cubeMesh->CreateModel();
	ground->SetSkin("Grass1.jpg");
	ground->ScaleX(20);
	ground->ScaleZ(10);

	playerBaseModel = cubeMesh->CreateModel(-70, 15, 0);
	playerBaseModel->SetSkin("CueTip2.jpg");
	playerBaseModel->Scale(2);

	enemyBaseModel = cubeMesh->CreateModel(70, 15, 0);
	enemyBaseModel->SetSkin("CueMetal.jpg");
	enemyBaseModel->ScaleY(2);

	skyBoxMesh = myEngine->LoadMesh("Skybox.x");
	skyBox = skyBoxMesh->CreateModel(0, -500, 0);
	// --- scene setups for environment such as castle gates ---
	quadMesh = myEngine->LoadMesh("quad.x");

	// houses 
	quadScene = quadMesh->CreateModel(20, 10, 10);
	quadScene->SetSkin("house.png");
	quadScene->RotateLocalX(180);


	quadScene = quadMesh->CreateModel(0, 10, 15);
	quadScene->SetSkin("house.png");
	quadScene->RotateLocalX(180);

	quadScene = quadMesh->CreateModel(8, 10, 25);
	quadScene->SetSkin("house.png");
	quadScene->RotateLocalX(180);

	quadScene = quadMesh->CreateModel(13, 15, 30);
	quadScene->SetSkin("house.png");
	quadScene->RotateLocalX(180);
	quadScene->Scale(2);

	// baracades
	// right baracade
	quadScene = quadMesh->CreateModel(30, 10, -5);
	quadScene->SetSkin("block.png");
	quadScene->RotateLocalX(180);

	// left baracade
	quadScene = quadMesh->CreateModel(-30, 10, -15);
	quadScene->SetSkin("block.png");
	quadScene->RotateLocalX(180);
	quadScene->RotateLocalY(180);

	quadScene = quadMesh->CreateModel(-40, 10, -13);
	quadScene->SetSkin("block.png");
	quadScene->RotateLocalX(180);
	quadScene->RotateLocalY(180);

	// grass 
	quadScene = quadMesh->CreateModel(40, 10, 10);
	quadScene->SetSkin("patch.png");
	quadScene->RotateLocalX(180);

	quadScene = quadMesh->CreateModel(-40, 10, 10);
	quadScene->SetSkin("patch.png");
	quadScene->RotateLocalX(180);

	quadScene = quadMesh->CreateModel(-45, 10, 9.999f);
	quadScene->SetSkin("patch.png");
	quadScene->RotateLocalX(180);

	//bush
	quadScene = quadMesh->CreateModel(-15, 10, -8);
	quadScene->SetSkin("bush.png");
	quadScene->RotateLocalX(180);

	const float KCubeSpeed = 40.0f; // constant cube travel speed
	const float kMapLimit = 65.0f; // map limit for the cube t travel 
	const float kCamSpeed = 50.0f; // speed in which the camera moves
	const float kStop = 0.0f;


	//timer
	float frameTime = myEngine->Timer();
	time_t lastTroopSort = time(NULL);

	// fonts 
	IFont* myFont = myEngine->LoadFont("Times New Roman", 20);
	// Load "Times New Roman" font at 20 points

	/*========
	|| base ||
	========*/

	Base* playerBase = new Base();
	Base* enemyBase = new Base();

	/*==========
	|| camera ||
	==========*/

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera();
	myCamera->SetPosition(0, 10, -50);


	/*============
	|| EKeyCode ||
	============*/
	EKeyCode escape = Key_Escape;

	//dev
	EKeyCode camFor = Key_I;
	EKeyCode camRotLeft = Key_J;
	EKeyCode camRotRight = Key_L;
	EKeyCode camBack = Key_K;
	EKeyCode statScreen = Key_Comma;

	//EKeyCode move = Key_1;
	//EKeyCode spawn = Key_2;


	// play cam controlls 
	EKeyCode camLeft = Key_A;
	EKeyCode camRight = Key_D;

	//spawning
	EKeyCode spawnPlayerTroop = Key_1;
	EKeyCode spawnPlayerTroop2 = Key_2;
	EKeyCode spawnPlayerTroop3 = Key_3;
	EKeyCode spawnPlayerTroop4 = Key_4;
	EKeyCode spawnEnemyTroop = Key_7;
	EKeyCode spawnEnemyTroop2 = Key_8;
	EKeyCode spawnEnemyTroop3 = Key_9;
	EKeyCode spawnEnemyTroop4 = Key_0;

	time_t lastSpawnedSwordsman = time(0);
	time_t lastSpawnedArcher = time(0);
	time_t lastSpawnedSpearman = time(0);
	time_t lastSpawnedCavalry = time(0);

	time_t lastSpawnedSwordsmanE = time(0);
	time_t lastSpawnedArcherE = time(0);
	time_t lastSpawnedSpearmanE = time(0);
	time_t lastSpawnedCavalryE = time(0);

	//Keep upgrades
	EKeyCode extraHealth = Key_V;

	// Troop Handler
	list<Troop*> playerTroops;
	list<Troop*> enemyTroops;

	bool viewStats = false;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		frameTime = myEngine->Timer();
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//Text Overlay
		if (viewStats)
		{
			stringstream outText;
			outText << "-Game Of Way-";
			myFont->Draw(outText.str(), 20, 20);
			outText.str(""); // Clear myStream

			outText << "Frame Time: " << frameTime;
			myFont->Draw(outText.str(), 20, 40);
			outText.str(""); // Clear myStream

			outText << "Base health: " << playerBase->GetHealth() << " / " << playerBase->GetMaxHealth();
			myFont->Draw(outText.str(), 20, 60);
			outText.str(""); // Clear myStream

			outText << "Enemy Base Health: " << enemyBase->GetHealth() << " / " << enemyBase->GetMaxHealth(); // place holder values 
			myFont->Draw(outText.str(), 20, 80);
			outText.str(""); // Clear myStream

			outText << "Resources: " << playerBase->GetResources();
			myFont->Draw(outText.str(), 20, 100);
			outText.str(""); // Clear myStream

			outText << "Resources: " << enemyBase->GetResources();
			myFont->Draw(outText.str(), 20, 120);
			outText.str(""); // Clear myStream
		}

		time_t currentTime = time(0);
		if (myEngine->KeyHit(spawnPlayerTroop) && difftime(currentTime, lastSpawnedSwordsman) > 4 && playerBase->GetResources() >= 10) // Key Press 1
		{
			playerBase->SpendResources(10);
			lastSpawnedSwordsman = currentTime;
			Troop* playerTroop = new Troop(Swordsman, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		if (myEngine->KeyHit(spawnPlayerTroop2) && difftime(currentTime, lastSpawnedArcher) > 6 && playerBase->GetResources() >= 20) // Key Press 2
		{
			playerBase->SpendResources(20);
			lastSpawnedArcher = currentTime;
			Troop* playerTroop = new Troop(Archer, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		if (myEngine->KeyHit(spawnPlayerTroop3) && difftime(currentTime, lastSpawnedSpearman) > 10 && playerBase->GetResources() >= 25) // Key Press 3
		{
			playerBase->SpendResources(25);
			lastSpawnedSpearman = currentTime;
			Troop* playerTroop = new Troop(Spearman, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		if (myEngine->KeyHit(spawnPlayerTroop4) && difftime(currentTime, lastSpawnedCavalry) > 20 && playerBase->GetResources() >= 65) // Key Press 4
		{
			playerBase->SpendResources(65);
			lastSpawnedCavalry = currentTime;
			Troop* playerTroop = new Troop(Cavalry, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}



		if (myEngine->KeyHit(spawnEnemyTroop) && difftime(currentTime, lastSpawnedSwordsmanE) > 4 && enemyBase->GetResources() >= 10) // Key Press 7
		{
			enemyBase->SpendResources(10);
			lastSpawnedSwordsmanE = currentTime;
			Troop* enemyTroop = new Troop(Swordsman, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}
		if (myEngine->KeyHit(spawnEnemyTroop2) && difftime(currentTime, lastSpawnedArcherE) > 6 && enemyBase->GetResources() >= 20) // Key Press 8
		{
			enemyBase->SpendResources(20);
			lastSpawnedArcherE = currentTime;
			Troop* enemyTroop = new Troop(Archer, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}

		if (myEngine->KeyHit(spawnEnemyTroop3) && difftime(currentTime, lastSpawnedSpearmanE) > 10 && enemyBase->GetResources() >= 25) // Key Press 9
		{
			enemyBase->SpendResources(25);
			lastSpawnedSpearmanE = currentTime;
			Troop* enemyTroop = new Troop(Spearman, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}

		if (myEngine->KeyHit(spawnEnemyTroop4) && difftime(currentTime, lastSpawnedCavalryE) > 20 && enemyBase->GetResources() >= 65) // Key Press 0
		{
			enemyBase->SpendResources(65);
			lastSpawnedCavalryE = currentTime;
			Troop* enemyTroop = new Troop(Cavalry, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}

		if (myEngine->KeyHit(statScreen))
		{
			if (viewStats) viewStats = false;
			else viewStats = true;
		}

		if (difftime(currentTime, enemyBase->GetLastResourceGain()) > 1.0)
		{
			enemyBase->GainResources(enemyBase->GetRPS());
			enemyBase->SetLastResourceGain(currentTime);
		}

		if (difftime(currentTime, playerBase->GetLastResourceGain()) > 1.0)
		{
			playerBase->GainResources(playerBase->GetRPS());
			playerBase->SetLastResourceGain(currentTime);
		}

		if (myEngine->KeyHit(extraHealth))
		{
			int currentMax = playerBase->GetMaxHealth();
			int newMax = currentMax*1.5;
			int currentHealth = playerBase->GetMaxHealth();
			int newHealth = currentHealth + (newMax - currentMax);
			playerBase->SetMaxHealth(newMax);
			playerBase->SetHealth(newHealth);
		}



		/*Sort the troops based on their x coordinates to determine who is at the front*/
		if (difftime(time(NULL), lastTroopSort)>1) {
			playerTroops.sort(CompPlayerTroopX);
			enemyTroops.sort(CompEnemyTroopX);
		}

		list<Troop*>::iterator it; //iterator to loop through troops
		//loop trough player troops
		for (it = playerTroops.begin(); it != playerTroops.end(); it++)
		{
			if (enemyTroops.empty()) // if there are no enemies
			{
				if ((*it)->GetPosition() < static_cast<float>((enemyBaseModel->GetX() - (*it)->GetRange())))
				{
					(*it)->Move((*it)->GetSpeed()*frameTime); // move the troop by an amount based on their speed
				}
				else
				{
					if (difftime(currentTime, (*it)->GetLastAttack()) > 1)
					{
						(*it)->SetLastAttack(currentTime);
						enemyBase->TakeDamage((*it)->GetDamage());
					}
				}
			}
			else
			{
				if (enemyTroops.front()->GetPosition() - (*it)->GetPosition() > (*it)->GetRange())
				{
					(*it)->Move((*it)->GetSpeed()*frameTime);
				}
				else
				{
					if (difftime(currentTime, (*it)->GetLastAttack()) > 1)
					{
						(*it)->SetLastAttack(currentTime);
						bool died = enemyTroops.front()->TakeDamage((*it)->GetDamage());
						if (died)
						{
							playerBase->GainResources(static_cast<int>(enemyTroops.front()->GetCost() / 10));
							Troop* temp = enemyTroops.front();
							temp->GetMesh()->RemoveModel(temp->GetModel());
							myEngine->RemoveMesh(temp->GetMesh());
							enemyTroops.pop_front();
							delete temp;
						}
					}
				}
			}
			//need to change range from 20 to be the troops specific range
			//need to stop the troop if at the enemy base
			//need to attack enemy troops and base
		}
		//all the same stuff as above except the roles are reversed
		for (it = enemyTroops.begin(); it != enemyTroops.end(); it++)
		{
			if (playerTroops.empty()) // There are no player troops on the field
			{
				// Troop is not in range of player base
				if ((*it)->GetPosition() > playerBaseModel->GetX() + (*it)->GetRange())
				{
					(*it)->Move(-(*it)->GetSpeed()*frameTime);
				}
				else // Troop is in range of player base
				{
					if (difftime(currentTime, (*it)->GetLastAttack()) > 1)
					{
						(*it)->SetLastAttack(currentTime);
						playerBase->TakeDamage((*it)->GetDamage()); // Base takes damage from the troop in range
					}
				}
			}
			else // There are player troops on the field
			{
				//Enemy troop is out of range of player troop
				if ((*it)->GetPosition() - playerTroops.front()->GetPosition() > (*it)->GetRange())
				{
					(*it)->Move(-(*it)->GetSpeed()*frameTime);
					//(*it)->GetHealth(); // commented because i dont know what this is for
				}
				else // Enemy troop is in range of player troop
				{
					if (difftime(currentTime, (*it)->GetLastAttack()) > 1)
					{
						(*it)->SetLastAttack(currentTime);
						bool died = playerTroops.front()->TakeDamage((*it)->GetDamage());
						if (died)
						{
							enemyBase->GainResources(static_cast<int>(playerTroops.front()->GetCost() / 10));
							Troop* temp = playerTroops.front();
							temp->GetMesh()->RemoveModel(temp->GetModel());
							myEngine->RemoveMesh(temp->GetMesh());
							playerTroops.pop_front();
							delete temp;
						}
					}
				}
			}
		}



		/////// Health////////////////////////////////////////////////////////////////////////////////
		//list<Troop*>::iterator it; //iterator to loop through troops
		//

		//Camera Controls
		if (myEngine->KeyHeld(camFor))
		{
			myCamera->MoveLocalZ(kCamSpeed*frameTime);
		}
		if (myEngine->KeyHeld(camRotLeft))
		{
			myCamera->RotateLocalY(-kCamSpeed*frameTime);
		}
		if (myEngine->KeyHeld(camRotRight))
		{
			myCamera->RotateLocalY(kCamSpeed*frameTime);
		}
		if (myEngine->KeyHeld(camBack))
		{
			myCamera->MoveLocalZ(-kCamSpeed*frameTime);
		}

		// camera controls LeftRight
		if (myEngine->KeyHeld(camLeft))
		{
			myCamera->MoveLocalX(-kCamSpeed*frameTime);
		}
		if (myEngine->KeyHeld(camRight))
		{
			myCamera->MoveLocalX(kCamSpeed*frameTime);
		}

		if (myEngine->KeyHit(escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}