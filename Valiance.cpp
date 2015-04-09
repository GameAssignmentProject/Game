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
Troop* troopBlueprints[4];

bool CompPlayerTroopX(Troop* first, Troop* second)
{
	return (first->GetPosition() > second->GetPosition());
}

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
	cubeMesh = myEngine->LoadMesh("Cube.x");
	skyBoxMesh = myEngine->LoadMesh("Skybox.x");
	quadMesh = myEngine->LoadMesh("quad.x");

	//Create ground
	tempModel = cubeMesh->CreateModel(0.0f, 0.0f, 0.0f);
	tempModel->SetSkin("Grass1.jpg");
	tempModel->ScaleX(20);
	tempModel->ScaleZ(10);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	//Create the player base
	playerBaseModel = cubeMesh->CreateModel(-70, 15, 0);
	playerBaseModel->SetSkin("CueTip2.jpg");
	playerBaseModel->Scale(2);

	//Create the enemy base
	enemyBaseModel = cubeMesh->CreateModel(70, 15, 0);
	enemyBaseModel->SetSkin("CueMetal.jpg");
	enemyBaseModel->ScaleY(2);

	//Create the skybox
	tempModel = skyBoxMesh->CreateModel(0, -500, 0);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	//Create houses
	tempModel = quadMesh->CreateModel(20, 10, 10);
	tempModel->SetSkin("house.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(0, 10, 15);
	tempModel->SetSkin("house.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(8, 10, 25);
	tempModel->SetSkin("house.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(13, 15, 30);
	tempModel->SetSkin("house.png");
	tempModel->RotateLocalX(180);
	tempModel->Scale(2);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	// baracades
	tempModel = quadMesh->CreateModel(30, 10, -5);
	tempModel->SetSkin("block.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(-30, 10, -15);
	tempModel->SetSkin("block.png");
	tempModel->RotateLocalX(180);
	tempModel->RotateLocalY(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(-40, 10, -13);
	tempModel->SetSkin("block.png");
	tempModel->RotateLocalX(180);
	tempModel->RotateLocalY(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	// grass 
	tempModel = quadMesh->CreateModel(40, 10, 10);
	tempModel->SetSkin("patch.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(-40, 10, 10);
	tempModel->SetSkin("patch.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	tempModel = quadMesh->CreateModel(-45, 10, 9.99f);
	tempModel->SetSkin("patch.png");
	tempModel->RotateLocalX(180);
	sceneryModels.push_back(tempModel); // add to scenery models list to keep track

	//bush
	tempModel = quadMesh->CreateModel(-15, 10, -8);
	tempModel->SetSkin("bush.png");
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
	myCamera->SetPosition(0, 10, -50);

	//Key Codes
	EKeyCode escape = Key_Escape;

	//dev
	EKeyCode camFor = Key_I;
	EKeyCode camRotLeft = Key_J;
	EKeyCode camRotRight = Key_L;
	EKeyCode camBack = Key_K;
	EKeyCode statScreen = Key_Comma;
	EKeyCode spawnEnemyTroop = Key_7;
	EKeyCode spawnEnemyTroop2 = Key_8;
	EKeyCode spawnEnemyTroop3 = Key_9;
	EKeyCode spawnEnemyTroop4 = Key_0;
	EKeyCode extraHealth = Key_V;

	// play cam controlls 
	EKeyCode camLeft = Key_A;
	EKeyCode camRight = Key_D;

	//spawning troops
	EKeyCode spawnPlayerTroop = Key_1;
	EKeyCode spawnPlayerTroop2 = Key_2;
	EKeyCode spawnPlayerTroop3 = Key_3;
	EKeyCode spawnPlayerTroop4 = Key_4;

	// Tracks the last time a type of unit was produced
	time_t lastSpawnedSwordsman = 0;
	time_t lastSpawnedArcher = 0;
	time_t lastSpawnedSpearman = 0;
	time_t lastSpawnedCavalry = 0;

	time_t lastSpawnedSwordsmanE = 0;
	time_t lastSpawnedArcherE = 0;
	time_t lastSpawnedSpearmanE = 0;
	time_t lastSpawnedCavalryE = 0;

	// Troop Handler
	list<Troop*> playerTroops;
	list<Troop*> enemyTroops;

	//Togglable view of stat screen
	bool viewStats = false;

	
	////////////////////////////////////////////////////////////////////////
	////
	////     GAME LOOP
	////
	////////////////////////////////////////////////////////////////////////

	while (myEngine->IsRunning())
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

		time_t currentTime = time(0);

		//Spawn Player Swordsman
		if (myEngine->KeyHit(spawnPlayerTroop) && difftime(currentTime, lastSpawnedSwordsman) > 4 && playerBase->GetResources() >= 10) // Key Press 1
		{
			playerBase->SpendResources(10);
			lastSpawnedSwordsman = currentTime;
			Troop* playerTroop = new Troop(Swordsman, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		//Spawn Player Archer
		if (myEngine->KeyHit(spawnPlayerTroop2) && difftime(currentTime, lastSpawnedArcher) > 6 && playerBase->GetResources() >= 20) // Key Press 2
		{
			playerBase->SpendResources(20);
			lastSpawnedArcher = currentTime;
			Troop* playerTroop = new Troop(Archer, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		//Spawn Player Spearman
		if (myEngine->KeyHit(spawnPlayerTroop3) && difftime(currentTime, lastSpawnedSpearman) > 10 && playerBase->GetResources() >= 25) // Key Press 3
		{
			playerBase->SpendResources(25);
			lastSpawnedSpearman = currentTime;
			Troop* playerTroop = new Troop(Spearman, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}
		//Spawn Player Cavalry
		if (myEngine->KeyHit(spawnPlayerTroop4) && difftime(currentTime, lastSpawnedCavalry) > 20 && playerBase->GetResources() >= 65) // Key Press 4
		{
			playerBase->SpendResources(65);
			lastSpawnedCavalry = currentTime;
			Troop* playerTroop = new Troop(Cavalry, true, myEngine); // create the player troop
			playerTroops.push_back(playerTroop); // add the troop to the players troops
		}

		//Spawn Enemy Swordsman
		if (myEngine->KeyHit(spawnEnemyTroop) && difftime(currentTime, lastSpawnedSwordsmanE) > 4 && enemyBase->GetResources() >= 10) // Key Press 7
		{
			enemyBase->SpendResources(10);
			lastSpawnedSwordsmanE = currentTime;
			Troop* enemyTroop = new Troop(Swordsman, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}
		//Spawn Enemy Archer
		if (myEngine->KeyHit(spawnEnemyTroop2) && difftime(currentTime, lastSpawnedArcherE) > 6 && enemyBase->GetResources() >= 20) // Key Press 8
		{
			enemyBase->SpendResources(20);
			lastSpawnedArcherE = currentTime;
			Troop* enemyTroop = new Troop(Archer, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}
		//Spawn Enemy Spearman
		if (myEngine->KeyHit(spawnEnemyTroop3) && difftime(currentTime, lastSpawnedSpearmanE) > 10 && enemyBase->GetResources() >= 25) // Key Press 9
		{
			enemyBase->SpendResources(25);
			lastSpawnedSpearmanE = currentTime;
			Troop* enemyTroop = new Troop(Spearman, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}
		//Spawn Enemy Cavalry
		if (myEngine->KeyHit(spawnEnemyTroop4) && difftime(currentTime, lastSpawnedCavalryE) > 20 && enemyBase->GetResources() >= 65) // Key Press 0
		{
			enemyBase->SpendResources(65);
			lastSpawnedCavalryE = currentTime;
			Troop* enemyTroop = new Troop(Cavalry, false, myEngine); // create the enemy troop
			enemyTroops.push_back(enemyTroop); // add the troop to the enemy troops
		}

		//Toggle Stat Screen
		if (myEngine->KeyHit(statScreen))
		{
			if (viewStats) viewStats = false;
			else viewStats = true;
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

		// Increase the health of the player base - Will be removed after further development
		if (myEngine->KeyHit(extraHealth))
		{
			int currentMax = playerBase->GetMaxHealth();
			int newMax = currentMax*1.5;
			int currentHealth = playerBase->GetMaxHealth();
			int newHealth = currentHealth + (newMax - currentMax);
			playerBase->SetMaxHealth(newMax);
			playerBase->SetHealth(newHealth);
		}

		// Sort the troops based on their x coordinates to determine who is at the front
		if (difftime(time(NULL), lastTroopSort)>1) {
			playerTroops.sort(CompPlayerTroopX);
			enemyTroops.sort(CompEnemyTroopX);
		}

		//loop trough player troops
		for (auto theTroop : playerTroops)
		{
			if (enemyTroops.empty()) // if there are no enemies
			{
				if (theTroop->GetPosition() < static_cast<float>((enemyBaseModel->GetX() - theTroop->GetRange())))
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
						bool died = enemyTroops.front()->TakeDamage(theTroop->GetDamage());
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
		}

		//all the same stuff as above except the roles are reversed
		for (auto theTroop : enemyTroops)
		{
			if (playerTroops.empty()) // There are no player troops on the field
			{
				// Troop is not in range of player base
				if (theTroop->GetPosition() > playerBaseModel->GetX() + theTroop->GetRange())
				{
					theTroop->Move(-theTroop->GetSpeed()*frameTime);
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
					theTroop->Move(-theTroop->GetSpeed()*frameTime);
					//(*it)->GetHealth(); // commented because i dont know what this is for
				}
				else // Enemy troop is in range of player troop
				{
					if (difftime(currentTime, theTroop->GetLastAttack()) > 1)
					{
						theTroop->SetLastAttack(currentTime);
						bool died = playerTroops.front()->TakeDamage(theTroop->GetDamage());
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