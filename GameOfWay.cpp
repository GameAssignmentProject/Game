// Game_Of_Way.cpp: A program using the TL-Engine
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <deque>
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
	void TakeDamage(int damage);
	void SpendResources(int amount);
	void GainResources(int amount);
	bool UpgradeResourceGather();
	bool UpgradeBaseHealth();
	int GetNumWeapons();
	bool PurchaseWeapon();
};

Base::Base() {

}

class Troop {
private:
	IMesh* troopMesh;//mesh and model may not be needed when using sprites
	IModel* troopModel;
	bool playerOwned; // used to determine which way to move and where to spawn
	TroopName name; // Type of troop
	int maxHealth;
	int currentHealth;
	int damage;
	int cost; // resources cost of each troop
	int cooldown; // amount of time between spawning of the units
	int speed; // movement speed
	int range; // distance at which the troop will begin attacking
	TroopName effectiveAgainst;
public:
	Troop(TroopName name, bool playerOwned, I3DEngine* theEngine);
	~Troop();
	void Move(float x); // move by x amount
	float GetPosition(); // returns the x position of the troop
	TroopName GetName();
	void SetHealth(int health);
	int GetMaxHealth();
	int GetHealth();
	void TakeDamage(int amount);
	bool UpgradeTroop();
	int GetDamage();
	int GetCost();
	int GetCooldown();
	int GetSpeed();
	int GetRange();
	TroopName GetEffective();
	void Attack(int idToAttack);
	void Fire();
};

Troop::Troop(TroopName name, bool playerMade, I3DEngine* theEngine) 
{
	troopMesh = theEngine->LoadMesh("cube.x");
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
	case Archer:
		playerOwned = playerMade;
		troopModel->SetSkin("Archer.jpg");
		maxHealth = 20;
		currentHealth = maxHealth;
		damage = 5;
		speed = 5;
		break;
	case Swordsman:
		playerOwned = playerMade;
		troopModel->SetSkin("Swordsman.jpg");
		maxHealth = 40;
		currentHealth = maxHealth;
		damage = 8;
		speed = 3;
		break;
	case Spearman:
		playerOwned = playerMade;
		troopModel->SetSkin("Spearman.jpg");
		maxHealth = 25;
		currentHealth = maxHealth;
		damage = 12;
		speed = 2;
		break;
	}
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

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");

	/**** Set up your scene here ****/

	IMesh* cubeMesh;
	IModel* cube;
	IModel* cube1;
	IModel* cubeP;

	cubeMesh = myEngine->LoadMesh("Cube.x");
	cube = cubeMesh->CreateModel();
	cube->SetSkin("Grass1.jpg");
	cube->ScaleX(15);

	cube1 = cubeMesh->CreateModel(-70, 15, 0);
	cube1->SetSkin("CueTip.jpg");
	cube1->ScaleY(2);

	cube1 = cubeMesh->CreateModel(70, 15, 0);
	cube1->SetSkin("CueMetal.jpg");
	cube1->ScaleY(2);
	
	const float KCubeSpeed = 40.0f; // constant cube travel speed
	const float kMapLimit = 65.0f; // map limit for the cube t travel 
	const float kCamSpeed = 50.0f;
	

	//timer
	float frameTime = myEngine->Timer();

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

	EKeyCode spawnPlayerTroop = Key_1;
	EKeyCode spawnPlayerTroop2 = Key_2;
	EKeyCode spawnEnemyTroop = Key_8;
	EKeyCode spawnEnemyTroop2 = Key_9;


	// Troop Handler
	deque<Troop*> playerTroops;
	deque<Troop*> enemyTroops;


	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		frameTime = myEngine->Timer();
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		if (myEngine->KeyHeld(spawnPlayerTroop)) // Key Press 1
		{
			Troop* playerTroop = new Troop(Swordsman, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		if (myEngine->KeyHeld(spawnPlayerTroop2)) // Key Press 2
		{
			Troop* playerTroop = new Troop(Archer, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		if (myEngine->KeyHeld(spawnEnemyTroop)) // Key Press 8
		{
			Troop* enemyTroop = new Troop(Swordsman, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}
		if (myEngine->KeyHeld(spawnEnemyTroop2)) // Key Press 9
		{
			Troop* enemyTroop = new Troop(Archer, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}

		deque<Troop*>::iterator it; //iterator to loop through troops
		//loop trough player troops
		for (it = playerTroops.begin(); it != playerTroops.end(); it++) 
		{
			if (enemyTroops.empty()) // if there are no enemies
			{
				(*it)->Move((*it)->GetSpeed()*frameTime); // move the troop by an amount based on their speed
				//(troop)->Move((troop)->GetSpeed()*frameTime);
			}
			else if (enemyTroops.front()->GetPosition() - (*it)->GetPosition() > 20)
			{
				//if the distance between the first enemy and the troop above the troops range
				(*it)->Move((*it)->GetSpeed()*frameTime); //move
			}
			//need to change range from 20 to be the troops specific range
			//need to stop the troop if at the enemy base
			//need to attack enemy troops and base
		}
		//all the same stuff as above except the roles are reversed
		for (it = enemyTroops.begin(); it != enemyTroops.end(); it++) {
			if (playerTroops.empty())
			{
				(*it)->Move(-(*it)->GetSpeed()*frameTime);
			}
			else if ((*it)->GetPosition() - playerTroops.front()->GetPosition() > 20)
			{
				(*it)->Move(-(*it)->GetSpeed()*frameTime);
			}
		}

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