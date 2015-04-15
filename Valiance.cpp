// Game_Of_Way.cpp: A program using the TL-Engine
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <list>
#include <sstream>
#include <ctime>
#include "Troop.hpp"
#include "Base.hpp"
#include "BaseWeapon.hpp"
#include "Projectile.hpp"

using namespace std;
using namespace tle;

const float CAM_SPEED = 50.0f; // speed in which the camera moves

/*Troop Blueprints*/
Troop* playerBlueprints[5];
Troop* enemyBlueprints[5];

/*Check which troop is in front of each other*/
bool CompPlayerTroopX(Troop* first, Troop* second)
{
	return (first->GetPosition() > second->GetPosition());
}

/*Check which troop is in front of each other - reversed to player troops*/
bool CompEnemyTroopX(Troop* first, Troop* second)
{
	return (first->GetPosition() < second->GetPosition());
}

void main()
{
	////////////////////////////////////////////////////////////////////////
	////
	////     INITIALISATION
	////
	////////////////////////////////////////////////////////////////////////

	//Bases status
	bool playerBaseDead = false;
	bool enemyBaseDead = false;
	
	//set the seed for any time rand() is called
	srand(time(0));

	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine(kTLX);
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder("Media");

	/**** Set up your scene here ****/
	//Meshes
	IMesh* cubeMesh;
	IMesh* quadMesh;
	IMesh* skyBoxMesh;

	//Models
	list<IModel*> sceneryModels; // models contained within are never interacted with
	IModel* playerBaseModel;
	IModel* enemyBaseModel;
	IModel* tempModel; // used to create each model then add to the sceneryModels list

	//Load Meshes
	cubeMesh = myEngine->LoadMesh("Meshes\\Cube.x");
	skyBoxMesh = myEngine->LoadMesh("Meshes\\Skybox.x");
	quadMesh = myEngine->LoadMesh("Meshes\\quad.x");

	//Create ground
	tempModel = cubeMesh->CreateModel(0.0f, 0.0f, 0.0f);
	tempModel->SetSkin("Scenery\\Grass1.jpg");
	tempModel->ScaleX(30);
	tempModel->ScaleZ(10);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	//Create the player base
	playerBaseModel = cubeMesh->CreateModel(-120, 15, 0);
	playerBaseModel->SetSkin("Base\\PlayerBase.jpg");
	playerBaseModel->ScaleX(3);
	playerBaseModel->ScaleY(2);
	playerBaseModel->ScaleZ(3);

	//Create the enemy base
	enemyBaseModel = cubeMesh->CreateModel(120, 15, 0);
	enemyBaseModel->SetSkin("Base\\EnemyBase.jpg");
	enemyBaseModel->ScaleX(3);
	enemyBaseModel->ScaleY(2);
	enemyBaseModel->ScaleZ(3);

	//Create the skybox
	tempModel = skyBoxMesh->CreateModel(0, -500, 0);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	//Create houses
	tempModel = quadMesh->CreateModel(20, 10, 10);
	tempModel->SetSkin("Scenery\\house.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(0, 10, 15);
	tempModel->SetSkin("Scenery\\house.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(8, 10, 25);
	tempModel->SetSkin("Scenery\\house.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(13, 15, 30);
	tempModel->SetSkin("Scenery\\house.png");
	tempModel->RotateLocalX(180);
	tempModel->Scale(2);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	// baracades
	tempModel = quadMesh->CreateModel(30, 10, -5);
	tempModel->SetSkin("Scenery\\block.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(-30, 10, -15);
	tempModel->SetSkin("Scenery\\block.png");
	tempModel->RotateLocalX(180);
	tempModel->RotateLocalY(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(-40, 10, -13);
	tempModel->SetSkin("Scenery\\block.png");
	tempModel->RotateLocalX(180);
	tempModel->RotateLocalY(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	// grass 
	tempModel = quadMesh->CreateModel(40, 10, 10);
	tempModel->SetSkin("Scenery\\patch.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(-40, 10, 10);
	tempModel->SetSkin("Scenery\\patch.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(-45, 10, 9.99f);
	tempModel->SetSkin("Scenery\\patch.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	//bush
	tempModel = quadMesh->CreateModel(-15, 10, -8);
	tempModel->SetSkin("Scenery\\bush.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	//timer
	float frameTime = myEngine->Timer();
	time_t lastTroopSort = time(NULL);

	// fonts 
	IFont* myFont = myEngine->LoadFont("Times New Roman", 20);
	// Load "Times New Roman" font at size 20

	// Bases
	Base* playerBase = new Base();
	Base* enemyBase = new Base();

	// Camera
	ICamera* myCamera;
	myCamera = myEngine->CreateCamera();
	myCamera->SetPosition(0, 20, -49);
	myCamera->RotateLocalX(10);

	//Key Codes
	EKeyCode escape = Key_Escape;

	//dev
	EKeyCode camFor = Key_I;
	EKeyCode camRotLeft = Key_J;
	EKeyCode camRotRight = Key_L;
	EKeyCode camBack = Key_K;
	EKeyCode statScreen = Key_Comma;
	EKeyCode spawnEnemyTroop = Key_6;
	EKeyCode spawnEnemyTroop2 = Key_7;
	EKeyCode spawnEnemyTroop3 = Key_8;
	EKeyCode spawnEnemyTroop4 = Key_9;
	EKeyCode spawnEnemyTroop5 = Key_0;

	// play cam controls
	EKeyCode camLeft = Key_A;
	EKeyCode camRight = Key_D;

	// troop controls
	EKeyCode spawnSwordsman = Key_1;
	EKeyCode spawnArcher = Key_2;
	EKeyCode spawnSpearman = Key_3;
	EKeyCode spawnCavalry = Key_4;
	EKeyCode spawnSiegeMachine = Key_5;
	EKeyCode upgradeSwordsman = Key_Q;
	EKeyCode upgradeArcher = Key_W;
	EKeyCode upgradeSpearman = Key_E;
	EKeyCode upgradeCavalry = Key_R;
	EKeyCode upgradeSiegeMachine = Key_T;

	// Base Controls
	EKeyCode upgradeBaseHealth = Key_Z;
	EKeyCode upgradeBaseRPS = Key_X;

	// Base Weapon Controls
	EKeyCode purchaseBaseWeapon = Key_C;
	EKeyCode upgradeBaseWeapon = Key_V;

	// Tracks the last time a type of unit was produced
	time_t lastSpawnedSwordsman = 0;
	time_t lastSpawnedArcher = 0;
	time_t lastSpawnedSpearman = 0;
	time_t lastSpawnedCavalry = 0;
	time_t lastSpawnedSiegeMachine = 0;

	time_t lastSpawnedSwordsmanE = 0;
	time_t lastSpawnedArcherE = 0;
	time_t lastSpawnedSpearmanE = 0;
	time_t lastSpawnedCavalryE = 0;
	time_t lastSpawnedSiegeMachineE = 0;

	// Troop Handler
	list<Troop*> playerTroops;
	list<Troop*> enemyTroops;

	// Setup Blueprints
	Troop* blueprint;
	for (int i = Swordsman; i <= SiegeMachine; i++)
	{
		blueprint = new Troop(TroopName(i));
		playerBlueprints[i] = blueprint;
		blueprint = new Troop(TroopName(i));
		enemyBlueprints[i] = blueprint;
	}

	//Togglable view of stat screen
	bool viewStats = false;
	
	////////////////////////////////////////////////////////////////////////
	////
	////     GAME LOOP
	////
	////////////////////////////////////////////////////////////////////////

	while (myEngine->IsRunning() || playerBaseDead || enemyBaseDead )
	{
		//returns time since last call of Timer()
		frameTime = myEngine->Timer();

		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/

		//Text Overlay
		if (viewStats)
		{
			stringstream outText;
			outText << "- Valiance -";
			myFont->Draw(outText.str(), 20, 20);
			outText.str(""); // Clear myStream

			outText << "Frame Time: " << frameTime;
			myFont->Draw(outText.str(), 20, 40);
			outText.str(""); // Clear myStream

			outText << "Player Base health: " << playerBase->GetHealth() << " / " << playerBase->GetMaxHealth();
			myFont->Draw(outText.str(), 20, 60);
			outText.str(""); // Clear myStream

			outText << "Player Resources: " << playerBase->GetResources();
			myFont->Draw(outText.str(), 20, 80);
			outText.str(""); // Clear myStream

			outText << "Enemy Base Health: " << enemyBase->GetHealth() << " / " << enemyBase->GetMaxHealth(); // place holder values 
			myFont->Draw(outText.str(), 20, 120);
			outText.str(""); // Clear myStream

			outText << "Enemy Resources: " << enemyBase->GetResources();
			myFont->Draw(outText.str(), 20, 140);
			outText.str(""); // Clear myStream
		}

		/*Current time of this iteration of the game loop*/
		time_t currentTime = time(0);

		//if requested to spawn troop, cooldown timer is over, base has enough resources
		//Spawn Player Swordsman
		if (myEngine->KeyHit(spawnSwordsman) && difftime(currentTime, lastSpawnedSwordsman) > playerBlueprints[Swordsman]->GetCooldown() && playerBase->GetResources() >=playerBlueprints[Swordsman]->GetCost()) // Key Press 1
		{
			playerBase->SpendResources(playerBlueprints[Swordsman]->GetCost()); // Base loses resources
			lastSpawnedSwordsman = currentTime; // reset the cooldown timer for player swordsman
			Troop* tempTroop = new Troop(playerBlueprints[Swordsman]); // Create the base stats for the troop from the blueprint of a swordsman
			tempTroop->SpawnTroop(true, myEngine, quadMesh); // Spawn the troop created above
			playerTroops.push_back(tempTroop); // add the troop to the players troops
		}
		//Spawn Player Archer
		if (myEngine->KeyHit(spawnArcher) && difftime(currentTime, lastSpawnedArcher) > playerBlueprints[Archer]->GetCooldown() && playerBase->GetResources() >= playerBlueprints[Archer]->GetCost()) // Key Press 2
		{
			playerBase->SpendResources(playerBlueprints[Archer]->GetCost());
			lastSpawnedArcher = currentTime;
			Troop* tempTroop = new Troop(playerBlueprints[Archer]);
			tempTroop->SpawnTroop(true, myEngine, quadMesh);
			playerTroops.push_back(tempTroop); // add the troop to the players troops
		}
		//Spawn Player Spearman
		if (myEngine->KeyHit(spawnSpearman) && difftime(currentTime, lastSpawnedSpearman) > playerBlueprints[Spearman]->GetCooldown() && playerBase->GetResources() >= playerBlueprints[Spearman]->GetCost()) // Key Press 3
		{
			playerBase->SpendResources(playerBlueprints[Spearman]->GetCost());
			lastSpawnedSpearman = currentTime;
			Troop* tempTroop = new Troop(playerBlueprints[Spearman]);
			tempTroop->SpawnTroop(true, myEngine, quadMesh);
			playerTroops.push_back(tempTroop); // add the troop to the players troops
		}
		//Spawn Player Cavalry
		if (myEngine->KeyHit(spawnCavalry) && difftime(currentTime, lastSpawnedCavalry) > playerBlueprints[Cavalry]->GetCooldown() && playerBase->GetResources() >= playerBlueprints[Cavalry]->GetCost()) // Key Press 4
		{
			playerBase->SpendResources(playerBlueprints[Cavalry]->GetCost());
			lastSpawnedCavalry = currentTime;
			Troop* tempTroop = new Troop(playerBlueprints[Cavalry]);
			tempTroop->SpawnTroop(true, myEngine, quadMesh);
			playerTroops.push_back(tempTroop); // add the troop to the players troops
		}
		//Spawn Player Siege Machine
		if (myEngine->KeyHit(spawnSiegeMachine) && difftime(currentTime, lastSpawnedSiegeMachine) > playerBlueprints[SiegeMachine]->GetCooldown() && playerBase->GetResources() >= playerBlueprints[SiegeMachine]->GetCost()) // Key Press 5
		{
			playerBase->SpendResources(playerBlueprints[SiegeMachine]->GetCost());
			lastSpawnedSiegeMachine = currentTime;
			Troop* tempTroop = new Troop(playerBlueprints[SiegeMachine]);
			tempTroop->SpawnTroop(true, myEngine, quadMesh);
			playerTroops.push_back(tempTroop); // add the troop to the players troops
		}

		//Spawn Enemy Swordsman
		if (myEngine->KeyHit(spawnEnemyTroop) && difftime(currentTime, lastSpawnedSwordsmanE) > enemyBlueprints[Swordsman]->GetCooldown() && enemyBase->GetResources() >= enemyBlueprints[Swordsman]->GetCost()) // Key Press 6
		{
			enemyBase->SpendResources(enemyBlueprints[Swordsman]->GetCost());
			lastSpawnedSwordsmanE = currentTime;
			Troop* tempTroop = new Troop(enemyBlueprints[Swordsman]);
			tempTroop->SpawnTroop(false, myEngine, quadMesh);
			enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
		}
		//Spawn Enemy Archer
		if (myEngine->KeyHit(spawnEnemyTroop2) && difftime(currentTime, lastSpawnedArcherE) > enemyBlueprints[Archer]->GetCooldown() && enemyBase->GetResources() >= enemyBlueprints[Archer]->GetCost()) // Key Press 7
		{
			enemyBase->SpendResources(enemyBlueprints[Archer]->GetCost());
			lastSpawnedArcherE = currentTime;
			Troop* tempTroop = new Troop(enemyBlueprints[Archer]);
			tempTroop->SpawnTroop(false, myEngine, quadMesh);
			enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
		}
		//Spawn Enemy Spearman
		if (myEngine->KeyHit(spawnEnemyTroop3) && difftime(currentTime, lastSpawnedSpearmanE) > enemyBlueprints[Spearman]->GetCooldown() && enemyBase->GetResources() >= enemyBlueprints[Spearman]->GetCost()) // Key Press 8
		{
			enemyBase->SpendResources(enemyBlueprints[Spearman]->GetCost());
			lastSpawnedSpearmanE = currentTime;
			Troop* tempTroop = new Troop(enemyBlueprints[Spearman]);
			tempTroop->SpawnTroop(false, myEngine, quadMesh);
			enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
		}
		//Spawn Enemy Cavalry
		if (myEngine->KeyHit(spawnEnemyTroop4) && difftime(currentTime, lastSpawnedCavalryE) > enemyBlueprints[Cavalry]->GetCooldown() && enemyBase->GetResources() >= enemyBlueprints[Cavalry]->GetCost()) // Key Press 9
		{
			enemyBase->SpendResources(enemyBlueprints[Cavalry]->GetCost());
			lastSpawnedCavalryE = currentTime;
			Troop* tempTroop = new Troop(enemyBlueprints[Cavalry]);
			tempTroop->SpawnTroop(false, myEngine, quadMesh);
			enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
		}
		//Spawn Enemy Siege Machine
		if (myEngine->KeyHit(spawnEnemyTroop5) && difftime(currentTime, lastSpawnedSiegeMachineE) > enemyBlueprints[SiegeMachine]->GetCooldown() && enemyBase->GetResources() >= enemyBlueprints[SiegeMachine]->GetCost()) //Key Press 0
		{
			enemyBase->SpendResources(enemyBlueprints[SiegeMachine]->GetCost());
			lastSpawnedCavalryE = currentTime;
			Troop* tempTroop = new Troop(enemyBlueprints[SiegeMachine]);
			tempTroop->SpawnTroop(false, myEngine, quadMesh);
			enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
		}

		//Upgrade if key pressed & base has enough resources
		//Upgrade Swordsman
		if (myEngine->KeyHit(upgradeSwordsman))
		{
			playerBlueprints[Swordsman]->UpgradeTroop();
		}
		//Upgrade Archer
		if (myEngine->KeyHit(upgradeArcher))
		{
			playerBlueprints[Archer]->UpgradeTroop();
		}
		//Upgrade Swordsman
		if (myEngine->KeyHit(upgradeSpearman))
		{
			playerBlueprints[Spearman]->UpgradeTroop();
		}
		//Upgrade Swordsman
		if (myEngine->KeyHit(upgradeCavalry))
		{
			playerBlueprints[Cavalry]->UpgradeTroop();
		}
		//Upgrade Swordsman
		if (myEngine->KeyHit(upgradeSiegeMachine))
		{
			playerBlueprints[SiegeMachine]->UpgradeTroop();
		}

		//Upgrade Base Health
		if (myEngine->KeyHit(upgradeBaseHealth))
		{
			playerBase->SetMaxHealth(static_cast<int>(playerBase->GetMaxHealth()*1.5));
		}
		//Upgrade Base RPS
		if (myEngine->KeyHit(upgradeBaseRPS))
		{
			playerBase->SetRPS(playerBase->GetRPS() + 10);
		}

		//Purchase Base Weapon
		if (myEngine->KeyHit(purchaseBaseWeapon))
		{
			//Purchse Base Weapon
		}
		//Upgrade Base Weapon
		if (myEngine->KeyHit(upgradeBaseWeapon))
		{
			//Upgrade Base Weapon
		}
		
		//Toggle Stat Screen
		if (myEngine->KeyHit(statScreen))
		{
			viewStats = !viewStats;
		}
		// Player Base Resource Gain
		if (difftime(currentTime, playerBase->GetLastResourceGain()) > 1.0)
		{
			playerBase->GainResources(playerBase->GetRPS());
			playerBase->SetLastResourceGain(currentTime);
		}

		// Enemy Base Resource Gain
		if (difftime(currentTime, enemyBase->GetLastResourceGain()) > 1.0)
		{
			enemyBase->GainResources(enemyBase->GetRPS());
			enemyBase->SetLastResourceGain(currentTime);
		}

		// Sort the troops based on their x coordinates to determine who is at the front
		if (difftime(time(NULL), lastTroopSort)>1) {
			playerTroops.sort(CompPlayerTroopX);
			enemyTroops.sort(CompEnemyTroopX);
		}

		//loop trough player troops
		for (auto theTroop : playerTroops)
		{
			if (enemyTroops.empty() || theTroop->GetPosition() > (enemyBaseModel->GetX() - 8) - theTroop->GetRange()) // if there are no enemies
			{
				if (theTroop->GetPosition() < static_cast<float>(((enemyBaseModel->GetX() - 8) - theTroop->GetRange())))
				{
					theTroop->Move(theTroop->GetSpeed()*frameTime); // move the troop by an amount based on their speed
				}
				else
				{
					if (difftime(currentTime, theTroop->GetLastAttack()) > 1)
					{
						theTroop->SetLastAttack(currentTime);
						enemyBase->TakeDamage(theTroop->GetDamage());
					}
				}
			}
			else
			{
				if (enemyTroops.front()->GetPosition() - theTroop->GetPosition() > theTroop->GetRange())
				{
					theTroop->Move(theTroop->GetSpeed()*frameTime);
				}
				else
				{
					if (difftime(currentTime, theTroop->GetLastAttack()) > 1)
					{
						theTroop->SetLastAttack(currentTime);
						bool died = enemyTroops.front()->TakeDamage(theTroop);
						if (died)
						{
							playerBase->GainResources(static_cast<int>(enemyTroops.front()->GetCost() / 10));
							Troop* temp = enemyTroops.front();
							quadMesh->RemoveModel(temp->GetModel());
							enemyTroops.pop_front();
							delete temp;
						}
					}
				}
			}
		}

		//all the same stuff as above except the roles are reversed
		for (auto theTroop : enemyTroops)
		{
			if (playerTroops.empty() || theTroop->GetPosition() < (playerBaseModel->GetX() + 8) + theTroop->GetRange()) // There are no player troops on the field || can attack the base
			{
				// Troop is not in range of player base
				if (theTroop->GetPosition() > (playerBaseModel->GetX() + 8) + theTroop->GetRange())
				{
					theTroop->Move(theTroop->GetSpeed()*frameTime);
				}
				else // Troop is in range of player base
				{
					if (difftime(currentTime, theTroop->GetLastAttack()) > 1)
					{
						theTroop->SetLastAttack(currentTime);
						playerBase->TakeDamage(theTroop->GetDamage()); // Base takes damage from the troop in range
					}
				}
			}
			else // There are player troops on the field
			{
				//Enemy troop is out of range of player troop
				if (theTroop->GetPosition() - playerTroops.front()->GetPosition() > theTroop->GetRange())
				{
					theTroop->Move(theTroop->GetSpeed()*frameTime);
				}
				else // Enemy troop is in range of player troop
				{
					if (difftime(currentTime, theTroop->GetLastAttack()) > 1)
					{
						theTroop->SetLastAttack(currentTime);
						bool died = playerTroops.front()->TakeDamage(theTroop);
						if (died)
						{
							enemyBase->GainResources(static_cast<int>(playerTroops.front()->GetCost() / 10));
							Troop* temp = playerTroops.front();
							quadMesh->RemoveModel(temp->GetModel());
							playerTroops.pop_front();
							delete temp;
						}
					}
				}
			}
		}

		//Camera Controls
		if (myEngine->KeyHeld(camFor))
		{
			myCamera->MoveLocalZ(CAM_SPEED*frameTime);
		}
		if (myEngine->KeyHeld(camRotLeft))
		{
			myCamera->RotateLocalY(-CAM_SPEED*frameTime);
		}
		if (myEngine->KeyHeld(camRotRight))
		{
			myCamera->RotateLocalY(CAM_SPEED*frameTime);
		}
		if (myEngine->KeyHeld(camBack))
		{
			myCamera->MoveLocalZ(-CAM_SPEED*frameTime);
		}

		// camera controls LeftRight
		if (myEngine->KeyHeld(camLeft))
		{
			myCamera->MoveLocalX(-CAM_SPEED*frameTime);
		}
		if (myEngine->KeyHeld(camRight))
		{
			myCamera->MoveLocalX(CAM_SPEED*frameTime);
		}

		if (myEngine->KeyHit(escape))
		{
			myEngine->Stop();
		}
	}

	// Delete the 3D engine now we are finished with it
	myEngine->Delete();
}