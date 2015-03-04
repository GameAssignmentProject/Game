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
};

Base::Base() 
{
	maxHealth = 100;
	currentHealth = maxHealth;
	resources = 0;
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
public:
	Troop(TroopName name, bool playerOwned, I3DEngine* theEngine);
	~Troop();
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
	void Attack();
	void Fire();
};

Troop::Troop(TroopName name, bool playerMade, I3DEngine* theEngine)
{
	troopMesh = theEngine->LoadMesh("quad.x");
	if (playerMade)
	{
		troopModel = troopMesh->CreateModel(-60, 10, 0); // -60, 10, 0, spawning location for player base
	}
	else
	{
		troopModel = troopMesh->CreateModel(60, 10, 0); // 60, 10, 0, spawning location for enemy base
	}
	switch (name) // from the type, determine the stats of the troop
	{
	case Swordsman:
		playerOwned = playerMade;
		troopModel->SetSkin("Swordsman.png");
		troopModel->RotateLocalX(180);
		troopModel->SetLocalZ(0.1);
		maxHealth = 40;
		currentHealth = maxHealth;
		damage = 8;
		speed = 3;
		range = 10;
		break;
	case Archer:
		playerOwned = playerMade;
		troopModel->SetSkin("Archer.png");
		troopModel->RotateLocalX(180);
		troopModel->SetLocalZ(0.2);
		maxHealth = 20;
		currentHealth = maxHealth;
		damage = 5;
		speed = 5;
		range = 20;
		break;
	case Spearman:
		playerOwned = playerMade;
		troopModel->SetSkin("Spearman.png");
		troopModel->SetLocalZ(0.3);
		troopModel->RotateLocalX(180);
		maxHealth = 25;
		currentHealth = maxHealth;
		damage = 12;
		speed = 2;
		range = 15;
		break;
	case Cavalry:
		playerOwned = playerMade;
		troopModel->SetSkin("Cavalry.jpg");
		troopModel->SetLocalZ(0.4);
		maxHealth = 60;
		currentHealth = maxHealth;
		damage = 15;
		speed = 8;
		range = 10;
		break;
	}
}

Troop::~Troop()
{
	//troopModel->~IModel();
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

	IModel* quad;
	IModel* ground;
	IModel* playerBaseModel;
	IModel* enemyBaseModel;

	cubeMesh = myEngine->LoadMesh("Cube.x");
	ground = cubeMesh->CreateModel();
	ground->SetSkin("Grass1.jpg");
	ground->ScaleX(15);

	playerBaseModel = cubeMesh->CreateModel(-70, 15, 0);
	playerBaseModel->SetSkin("CueTip.jpg");
	playerBaseModel->ScaleY(2);

	enemyBaseModel = cubeMesh->CreateModel(70, 15, 0);
	enemyBaseModel->SetSkin("CueMetal.jpg");
	enemyBaseModel->ScaleY(2);

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

	//dev cam
	EKeyCode camFor = Key_I;
	EKeyCode camRotLeft = Key_J;
	EKeyCode camRotRight = Key_L;
	EKeyCode camBack = Key_K;

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

	//Keep upgrades
	EKeyCode extraHealth = Key_V;


	// Troop Handler
	list<Troop*> playerTroops;
	list<Troop*> enemyTroops;

	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		frameTime = myEngine->Timer();
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//fonts
		stringstream outText;
		outText << "-Game Of Way-";
		myFont->Draw(outText.str(), 20, 20);
		outText.str(""); // Clear myStream

		outText << "Frame Time: "<< frameTime;
		myFont->Draw(outText.str(), 20, 40);
		outText.str(""); // Clear myStream

		
		outText << "Base health: " << playerBase->GetHealth() << " / " << playerBase->GetMaxHealth();
		myFont->Draw(outText.str(), 20, 60);
		outText.str(""); // Clear myStream

		outText << "Enemy Base Health: 100 "; // place holder values 
		myFont->Draw(outText.str(), 20, 80);
		outText.str(""); // Clear myStream

		

		if (myEngine->KeyHit(spawnPlayerTroop)) // Key Press 1
		{
			Troop* playerTroop = new Troop(Swordsman, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		if (myEngine->KeyHit(spawnPlayerTroop2)) // Key Press 2
		{
			Troop* playerTroop = new Troop(Archer, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}

		if (myEngine->KeyHit(spawnPlayerTroop3)) // Key Press 3
		{
			Troop* playerTroop = new Troop(Spearman, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}

		if (myEngine->KeyHit(spawnPlayerTroop4)) // Key Press 4
		{
			Troop* playerTroop = new Troop(Cavalry, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}



		if (myEngine->KeyHit(spawnEnemyTroop)) // Key Press 7
		{
			Troop* enemyTroop = new Troop(Swordsman, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}
		if (myEngine->KeyHit(spawnEnemyTroop2)) // Key Press 8
		{
			Troop* enemyTroop = new Troop(Archer, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}

		if (myEngine->KeyHit(spawnEnemyTroop3)) // Key Press 9
		{
			Troop* enemyTroop = new Troop(Spearman, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}

		if (myEngine->KeyHit(spawnEnemyTroop4)) // Key Press 0
		{
			Troop* enemyTroop = new Troop(Cavalry, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
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
					(*it)->Move((*it)->GetSpeed()*frameTime); // move the troop by an amount based on their speed
					(*it)->GetHealth();
					
					//(troop)->Move((troop)->GetSpeed()*frameTime);
			}
			else if (enemyTroops.front()->GetPosition() - (*it)->GetPosition() > (*it)->GetRange())
			{

				//if the distance between the first enemy and the troop above the troops range
				(*it)->Move((*it)->GetSpeed()*frameTime); //move
				(*it)->GetHealth();
			}
			//need to change range from 20 to be the troops specific range
			//need to stop the troop if at the enemy base
			//need to attack enemy troops and base
		}
		//all the same stuff as above except the roles are reversed
		for (it = enemyTroops.begin(); it != enemyTroops.end(); it++) {
			if (playerTroops.empty()) // There are no player troops on the field
			{ 
				// Troop is not in range of player base
				if ((*it)->GetPosition() > playerBaseModel->GetX() + (*it)->GetRange()) 
				{
					(*it)->Move(-(*it)->GetSpeed()*frameTime);
					//(*it)->GetHealth(); // commented because i dont know what this is for
				}
				else // Troop is in range of player base
				{
					playerBase->TakeDamage((*it)->GetDamage()); // Base takes damage from the troop in range
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
					bool died = playerTroops.front()->TakeDamage((*it)->GetDamage());
					if (died)
					{
						Troop* temp = playerTroops.front();
						temp->GetMesh()->RemoveModel(temp->GetModel());
						myEngine->RemoveMesh(temp->GetMesh());
						playerTroops.pop_front();
						delete temp;
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
