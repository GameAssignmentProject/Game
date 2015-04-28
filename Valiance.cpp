// Game_Of_Way.cpp: A program using the TL-Engine
#include <TL-Engine.h>	// TL-Engine include file and namespace
#include <list>
#include <sstream>
#include <ctime>
#include <cstdlib>
#include <vector>
#include "Troop.hpp"
#include "Base.hpp"
#include "BaseWeapon.hpp"
#include "Projectile.hpp"

using namespace std;
using namespace tle;

const float CAM_SPEED = 50.0f; // speed in which the camera moves

float Random()
{
	float choice = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	return choice;
}

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

TroopName AIDecideTroop(TroopName theTroopName, int resources, int numEnemies)
{
	float swordsman;
	float archer;
	float spearman;
	float cavalry;
	if (resources >= 4000 && theTroopName == None)
	{
		swordsman = 0.0f;
		archer = 0.0f;
		spearman = 0.0f;
		cavalry = 0.0f;
	}
	else if (resources >= 4000 && numEnemies >= 10)
	{
		swordsman = 0.0f;
		archer = 0.0f;
		spearman = 0.0f;
		cavalry = 0.0f;
	}
	if (theTroopName == Swordsman)
	{
		swordsman = 0.2f;
		archer = 0.0f;
		spearman = 0.3f;
		cavalry = 0.5f;
	}
	else if (theTroopName == Archer)
	{
		swordsman = 0.6f;
		archer = 0.2f;
		spearman = 0.0f;
		cavalry = 0.2f;
	}
	else if (theTroopName == Spearman)
	{
		swordsman = 0.2f;
		archer = 0.6f;
		spearman = 0.2f;
		cavalry = 0.0f;
	}
	else if (theTroopName == Cavalry)
	{
		swordsman = 0.0f;
		archer = 0.2f;
		spearman = 0.7f;
		cavalry = 0.1f;
	}
	else if (theTroopName == SiegeMachine)
	{
		swordsman = 0.3f;
		archer = 0.4f;
		spearman = 0.3f;
		cavalry = 0.2f;
	}
	else if (theTroopName == None)
	{
		swordsman = 0.3f;
		archer = 0.3f;
		spearman = 0.2f;
		cavalry = 0.2f;
	}

	float choice = Random();

	if (choice <= swordsman)
	{
		return Swordsman;
	}
	else if (choice <= swordsman + archer)
	{
		return Archer;
	}
	else if (choice <= swordsman + archer + spearman)
	{
		return Spearman;
	}
	else if (choice <= swordsman + archer + spearman + cavalry)
	{
		return Cavalry;
	}
	else
	{
		return SiegeMachine;
	}
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

	int forceTroopSpawn = 0;
	int aiDecisionState = 0;

	float GameSpeed = 5.0f;

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

	// sprites
	ISprite* Title = myEngine->CreateSprite("Title.jpg");
	Title->SetZ(0.9);
	ISprite* OptionScreen;
	ISprite* ExitBox;

	// menu states 
	bool start = false; // bool to enter the game after the start menu and make the menu dissapear
	bool menu = true; // for when the options menu or any other menu but the title scfeen is open
	bool options = false; // options menu toggle

	//Models
	list<IModel*> sceneryModels; // models contained within are never interacted with
	IModel* playerBaseModel;
	IModel* enemyBaseModel;
	IModel* tempModel; // used to create each model then add to the sceneryModels list

	IModel* bloodModel; // used to spawn a blood particle
	IModel* brickModel;

	//vector <IModel*> BloodVec;
	//vector <IModel*> BaseVec;
	//int bloodspot = 0;
	//int limit = 10;

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
	time_t startGameTime = time(NULL);

	// fonts 
	IFont* myFont = myEngine->LoadFont("Times New Roman", 20);
	IFont* myFont2 = myEngine->LoadFont("Monotype Corsiva", 40);
	// Load "Times New Roman" font at size 20

	// Bases
	Base* playerBase = new Base();
	Base* enemyBase = new Base();

	BaseWeapon* playerBaseWeapon = nullptr;
	BaseWeapon* enemyBaseWeapon = nullptr;

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

	/* AI */
	TroopName nextSpawningEnemy = None; // if none decide a unit
	time_t changeDecisionTimer = 0; // if unit decided but been 10 seconds, redecide, may still choose unit already picked

	
	ISprite* swordsmanSquare = NULL;
	ISprite* archerSquare = NULL;
	ISprite* spearmanSquare = NULL;
	ISprite* cavalrySquare = NULL;
	ISprite* batteringramSquare = NULL;
	ISprite* heartSquare = NULL;
	ISprite* rpsSquare = NULL;
	ISprite* ballistaSquare = NULL;
	

	////////////////////////////////////////////////////////////////////////
	////
	////     GAME LOOP
	////
	////////////////////////////////////////////////////////////////////////

	while (myEngine->IsRunning() && !playerBaseDead && !enemyBaseDead)
	{
		// Draw the scene
		myEngine->DrawScene();

		if (menu) // to prevent the game closing when exiting options or other menu 
		{
			if (myEngine->KeyHit(escape))
			{
				myEngine->Stop();
			}
			if (myEngine->KeyHit(Key_O) && !options) // options bool activation
			{
				OptionScreen = myEngine->CreateSprite("OptionScreen.jpg");
				myEngine->RemoveSprite(Title);
				menu = false;
				options = true;
			}
		}
		if (options) // options menu 
		{
			if (myEngine->KeyHit(escape)) // exot options menu 
			{
				myEngine->RemoveSprite(OptionScreen);
				Title = myEngine->CreateSprite("Title.jpg");
				options = false;
				menu = false;
			}
		}
		if (!start) // title screen 
		{
			if (myEngine->KeyHit(Key_Return))
			{
				myEngine->RemoveSprite(Title);
				start = true;
				menu = false;
			}


		}
		if (!menu && start && !options)
		{

			if (swordsmanSquare == NULL) {
				swordsmanSquare = myEngine->CreateSprite("UIModels\\NewSwordsmanButton.png");
				archerSquare = myEngine->CreateSprite("UIModels\\NewArcherButton.png");
				spearmanSquare = myEngine->CreateSprite("UIModels\\NewSpearmanButton.png");
				cavalrySquare = myEngine->CreateSprite("UIModels\\NewCavalryButton.png");
				batteringramSquare = myEngine->CreateSprite("UIModels\\NewBatteringRamButton.png");
				heartSquare = myEngine->CreateSprite("UIModels\\heart.png");
				rpsSquare = myEngine->CreateSprite("UIModels\\resource.png");
				ballistaSquare = myEngine->CreateSprite("UIModels\\Ballista.png");




				swordsmanSquare->SetPosition(120, 550);
				archerSquare->SetPosition(230, 550);
				spearmanSquare->SetPosition(340, 550);
				cavalrySquare->SetPosition(450, 550);
				batteringramSquare->SetPosition(560, 550);
				heartSquare->SetPosition(890, 550);
				rpsSquare->SetPosition(1000, 550);
				ballistaSquare->SetPosition(1110, 550);
				
			}

			//Square = myEngine->CreateSprite();

			// UI
			stringstream outText;

			outText << "- Valiance -";
			myFont2->Draw(outText.str(), 600, 10);
			outText.str(""); // Clear myStream

			outText << "Resources: " << playerBase->GetResources();
			myFont->Draw(outText.str(), 20, 20);
			outText.str(""); // Clear myStream

			outText << "Base health: " << playerBase->GetHealth() << " / " << playerBase->GetMaxHealth();
			myFont->Draw(outText.str(), 20, 40);
			outText.str(""); // Clear myStream

			outText << "Enemy Base Health: " << enemyBase->GetHealth() << " / " << enemyBase->GetMaxHealth(); // place holder values 
			myFont->Draw(outText.str(), 990, 20);
			outText.str(""); // Clear myStream

			//Swordsman

			outText << "Cost:             " << playerBlueprints[Swordsman]->GetCost();
			myFont->Draw(outText.str(), 30, 500);
			outText.str(""); // Clear myStream


			outText << "Key:               1";
			myFont->Draw(outText.str(), 30, 520);
			outText.str(""); // Clear myStream

			outText << "Name:      Swordsman";
			myFont->Draw(outText.str(), 30, 645);
			outText.str(""); // Clear myStream


			outText << "Upgrade\nCost: ";
			myFont->Draw(outText.str(), 30, 670);
			outText.str(""); // Clear myStream


			outText << "Q: " << playerBlueprints[Swordsman]->GetUpgradeCost();
			myFont->Draw(outText.str(), 130, 670);
			outText.str(""); // Clear myStream

			


			// Archer

			outText << playerBlueprints[Archer]->GetCost();
			myFont->Draw(outText.str(), 260, 500);
			outText.str(""); // Clear myStream

			outText << "Archer";
			myFont->Draw(outText.str(), 250, 645);
			outText.str(""); // Clear myStream

			outText << "2";
			myFont->Draw(outText.str(), 270, 520);
			outText.str(""); // Clear myStream

			outText << "W: " << playerBlueprints[Archer]->GetUpgradeCost();
			myFont->Draw(outText.str(), 240, 670);
			outText.str(""); // Clear myStream

			// Spearman

			outText <<   playerBlueprints[Spearman]->GetCost();
			myFont->Draw(outText.str(), 370, 500);
			outText.str(""); // Clear myStream

			outText << "Spearman";
			myFont->Draw(outText.str(), 350, 645);
			outText.str(""); // Clear myStream

			outText << "3";
			myFont->Draw(outText.str(), 380, 520);
			outText.str(""); // Clear myStream

			outText << "E: " << playerBlueprints[Spearman]->GetUpgradeCost();
			myFont->Draw(outText.str(), 350, 670);
			outText.str(""); // Clear myStream

			// Cavalry

			outText <<  playerBlueprints[Cavalry]->GetCost();
			myFont->Draw(outText.str(), 480, 500);
			outText.str(""); // Clear myStream

			outText << "Cavalry";
			myFont->Draw(outText.str(), 465, 645);
			outText.str(""); // Clear myStream

			outText << "4";
			myFont->Draw(outText.str(), 490, 520);
			outText.str(""); // Clear myStream

			outText << "R: " << playerBlueprints[Cavalry]->GetUpgradeCost();
			myFont->Draw(outText.str(), 460, 670);
			outText.str(""); // Clear myStream

			// BatteringRam

			outText <<  playerBlueprints[SiegeMachine]->GetCost();
			myFont->Draw(outText.str(), 585, 500);
			outText.str(""); // Clear myStream

			outText << "Ram";
			myFont->Draw(outText.str(), 590, 645);
			outText.str(""); // Clear myStream

			outText << "5";
			myFont->Draw(outText.str(), 600, 520);
			outText.str(""); // Clear myStream

			outText << "T: " << playerBlueprints[SiegeMachine]->GetUpgradeCost();
			myFont->Draw(outText.str(), 570, 670);
			outText.str(""); // Clear myStream


			//Upgrades for keep


			// Health 
			outText << "Cost:         " << playerBase->GetHealthUpgradeCost();
			myFont->Draw(outText.str(), 820, 500);
			outText.str(""); // Clear myStream

			outText << "Key:           Z";
			myFont->Draw(outText.str(), 820, 520);
			outText.str(""); // Clear myStream

			outText << "  Upgrade\nBase Health ";
			myFont->Draw(outText.str(), 885, 650);
			outText.str(""); // Clear myStream

			
			//rps

			outText << playerBase->GetRPSUpgradeCost();
			myFont->Draw(outText.str(), 1030, 500);
			outText.str(""); // Clear myStream

			outText << "X";
			myFont->Draw(outText.str(), 1040, 520);
			outText.str(""); // Clear myStream

			outText << " Upgrade\nBase RPS ";
			myFont->Draw(outText.str(), 1005, 650);
			outText.str(""); // Clear myStream



			//ballista

			if (playerBase->GetNumWeapons() == 0)
			{
				outText << playerBaseWeapon->GetCost();
			}
			else
			{
				outText << playerBaseWeapon->GetUpgradeCost();
			}

			myFont->Draw(outText.str(), 1140, 500);
			outText.str(""); // Clear myStream

			outText << "C";
			myFont->Draw(outText.str(), 1150, 520);
			outText.str(""); // Clear myStream

			if (playerBase->GetNumWeapons() == 0)
			{
				outText << "Purchase\n Ballista";
			}
			else
			{
				
				outText << "Upgrade\nBallista";
			}

			myFont->Draw(outText.str(), 1120, 650);
			outText.str(""); // Clear myStream

			





			if (myEngine->KeyHit(escape))
			{
				menu = true;
				start = false;
				Title = myEngine->CreateSprite("Title.jpg");
			}
			if (myEngine->KeyHit(Key_O) && !options) // options bool activation
			{
				OptionScreen = myEngine->CreateSprite("OptionScreen.jpg");
				menu = false;
				options = true;
			}
			//returns time since last call of Timer()
			frameTime = myEngine->Timer();
			frameTime *= GameSpeed;

			/**** Update your scene each frame here ****/

			if (playerBase->GetHealth() <= 0)
				playerBaseDead = !playerBaseDead;
			if (enemyBase->GetHealth() <= 0)
				enemyBaseDead = !enemyBaseDead;

			//Text Overlay
			if (viewStats)
			{
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
			if (myEngine->KeyHit(spawnSwordsman) && difftime(currentTime, lastSpawnedSwordsman) > playerBlueprints[Swordsman]->GetCooldown() / GameSpeed && playerBase->GetResources() >= playerBlueprints[Swordsman]->GetCost()) // Key Press 1
			{
				playerBase->SpendResources(playerBlueprints[Swordsman]->GetCost()); // Base loses resources
				lastSpawnedSwordsman = currentTime; // reset the cooldown timer for player swordsman
				Troop* tempTroop = new Troop(playerBlueprints[Swordsman]); // Create the base stats for the troop from the blueprint of a swordsman
				tempTroop->SpawnTroop(true, myEngine, quadMesh); // Spawn the troop created above
				playerTroops.push_back(tempTroop); // add the troop to the players troops
			}
			//Spawn Player Archer
			if (myEngine->KeyHit(spawnArcher) && difftime(currentTime, lastSpawnedArcher) > playerBlueprints[Archer]->GetCooldown() / GameSpeed && playerBase->GetResources() >= playerBlueprints[Archer]->GetCost()) // Key Press 2
			{
				playerBase->SpendResources(playerBlueprints[Archer]->GetCost());
				lastSpawnedArcher = currentTime;
				Troop* tempTroop = new Troop(playerBlueprints[Archer]);
				tempTroop->SpawnTroop(true, myEngine, quadMesh);
				playerTroops.push_back(tempTroop); // add the troop to the players troops
			}
			//Spawn Player Spearman
			if (myEngine->KeyHit(spawnSpearman) && difftime(currentTime, lastSpawnedSpearman) > playerBlueprints[Spearman]->GetCooldown() / GameSpeed && playerBase->GetResources() >= playerBlueprints[Spearman]->GetCost()) // Key Press 3
			{
				playerBase->SpendResources(playerBlueprints[Spearman]->GetCost());
				lastSpawnedSpearman = currentTime;
				Troop* tempTroop = new Troop(playerBlueprints[Spearman]);
				tempTroop->SpawnTroop(true, myEngine, quadMesh);
				playerTroops.push_back(tempTroop); // add the troop to the players troops
			}
			//Spawn Player Cavalry
			if (myEngine->KeyHit(spawnCavalry) && difftime(currentTime, lastSpawnedCavalry) > playerBlueprints[Cavalry]->GetCooldown() / GameSpeed && playerBase->GetResources() >= playerBlueprints[Cavalry]->GetCost()) // Key Press 4
			{
				playerBase->SpendResources(playerBlueprints[Cavalry]->GetCost());
				lastSpawnedCavalry = currentTime;
				Troop* tempTroop = new Troop(playerBlueprints[Cavalry]);
				tempTroop->SpawnTroop(true, myEngine, quadMesh);
				playerTroops.push_back(tempTroop); // add the troop to the players troops
			}
			//Spawn Player Siege Machine
			if (myEngine->KeyHit(spawnSiegeMachine) && difftime(currentTime, lastSpawnedSiegeMachine) > playerBlueprints[SiegeMachine]->GetCooldown() / GameSpeed && playerBase->GetResources() >= playerBlueprints[SiegeMachine]->GetCost()) // Key Press 5
			{
				playerBase->SpendResources(playerBlueprints[SiegeMachine]->GetCost());
				lastSpawnedSiegeMachine = currentTime;
				Troop* tempTroop = new Troop(playerBlueprints[SiegeMachine]);
				tempTroop->SpawnTroop(true, myEngine, quadMesh);
				playerTroops.push_back(tempTroop); // add the troop to the players troops
			}


			//Upgrade if key pressed & base has enough resources
			//Upgrade Swordsman
			if (playerBlueprints[Swordsman]->GetStage() < 3 && myEngine->KeyHit(upgradeSwordsman) && playerBase->GetResources() >= playerBlueprints[Swordsman]->GetUpgradeCost())
			{
				playerBase->SpendResources(playerBlueprints[Swordsman]->GetUpgradeCost());
				playerBlueprints[Swordsman]->UpgradeTroop();
			}
			//Upgrade Archer
			if (playerBlueprints[Archer]->GetStage() < 3 && myEngine->KeyHit(upgradeArcher) && playerBase->GetResources() >= playerBlueprints[Archer]->GetUpgradeCost())
			{
				playerBase->SpendResources(playerBlueprints[Archer]->GetUpgradeCost());
				playerBlueprints[Archer]->UpgradeTroop();
			}
			//Upgrade Swordsman
			if (playerBlueprints[Spearman]->GetStage() < 3 && myEngine->KeyHit(upgradeSpearman) && playerBase->GetResources() >= playerBlueprints[Spearman]->GetUpgradeCost())
			{
				playerBase->SpendResources(playerBlueprints[Spearman]->GetUpgradeCost());
				playerBlueprints[Spearman]->UpgradeTroop();
			}
			//Upgrade Swordsman
			if (playerBlueprints[Cavalry]->GetStage() < 3 && myEngine->KeyHit(upgradeCavalry) && playerBase->GetResources() >= playerBlueprints[Cavalry]->GetUpgradeCost())
			{
				playerBase->SpendResources(playerBlueprints[Cavalry]->GetUpgradeCost());
				playerBlueprints[Cavalry]->UpgradeTroop();
			}
			//Upgrade Swordsman
			if (playerBlueprints[SiegeMachine]->GetStage() < 3 && myEngine->KeyHit(upgradeSiegeMachine) && playerBase->GetResources() >= playerBlueprints[SiegeMachine]->GetUpgradeCost())
			{
				playerBase->SpendResources(playerBlueprints[SiegeMachine]->GetUpgradeCost());
				playerBlueprints[SiegeMachine]->UpgradeTroop();
			}

			//Upgrade Base Health
			if (playerBase->GetMaxHealth() < 16875 && myEngine->KeyHit(upgradeBaseHealth) && playerBase->GetResources() >= playerBase->GetHealthUpgradeCost())
			{
				playerBase->SpendResources(playerBase->GetHealthUpgradeCost());
				playerBase->UpgradeBaseHealth();
			}
			//Upgrade Base RPS
			if (playerBase->GetRPS() < 40 && myEngine->KeyHit(upgradeBaseRPS) && playerBase->GetResources() >= playerBase->GetRPSUpgradeCost())
			{
				playerBase->SpendResources(playerBase->GetRPSUpgradeCost());
				playerBase->UpgradeResourceGather();
			}

			//Purchase Base Weapon
			if (playerBase->GetNumWeapons() == 0 && myEngine->KeyHit(purchaseBaseWeapon) && playerBase->GetResources() >= playerBaseWeapon->GetCost())
			{
				playerBase->SpendResources(playerBaseWeapon->GetCost());
				playerBase->SetNumWeapons(1);
				playerBaseWeapon = new BaseWeapon(quadMesh, true);
			}
			else if (playerBase->GetNumWeapons() == 1 && myEngine->KeyHit(purchaseBaseWeapon) && playerBase->GetResources() >= playerBaseWeapon->GetUpgradeCost() && playerBaseWeapon->GetDamage() < 40)
			{
				playerBase->SpendResources(playerBaseWeapon->GetUpgradeCost());
				playerBaseWeapon->UpgradeBaseWeapon();
			}

			//Toggle Stat Screen
			if (myEngine->KeyHit(statScreen))
			{
				viewStats = !viewStats;
			}
			// Player Base Resource Gain
			if (difftime(currentTime, playerBase->GetLastResourceGain()) > 1.0 / GameSpeed)
			{
				playerBase->GainResources(playerBase->GetRPS());
				playerBase->SetLastResourceGain(currentTime);
			}

			// Enemy Base Resource Gain
			if (difftime(currentTime, enemyBase->GetLastResourceGain()) > 1.0 / GameSpeed)
			{
				enemyBase->GainResources(enemyBase->GetRPS());
				enemyBase->SetLastResourceGain(currentTime);
			}

			// Sort the troops based on their x coordinates to determine who is at the front
			if (difftime(currentTime, lastTroopSort) > 1.0 / GameSpeed) {
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
						if (difftime(currentTime, theTroop->GetLastAttack()) > 1.0 / GameSpeed)
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
						if (difftime(currentTime, theTroop->GetLastAttack()) > 1.0 / GameSpeed)
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
						if (difftime(currentTime, theTroop->GetLastAttack()) > 1.0 / GameSpeed)
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
						if (difftime(currentTime, theTroop->GetLastAttack()) > 1.0 / GameSpeed)
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

			if (playerBase->GetNumWeapons() > 0 && !enemyTroops.empty())
			{
				if (enemyTroops.front()->GetPosition() - playerBaseWeapon->GetPosition() < playerBaseWeapon->GetRange())
				{
					if (difftime(currentTime, playerBaseWeapon->GetLastAttack()) > 2.0 / GameSpeed)
					{
						playerBaseWeapon->SetLastAttack(currentTime);
						bool died = enemyTroops.front()->TakeDamage(playerBaseWeapon->GetDamage()); // Base takes damage from the troop in range
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

			if (enemyBase->GetNumWeapons() > 0 && !playerTroops.empty())
			{
				if (enemyBaseWeapon->GetPosition() - playerTroops.front()->GetPosition() < enemyBaseWeapon->GetRange())
				{
					if (difftime(currentTime, enemyBaseWeapon->GetLastAttack()) > 2.0 / GameSpeed)
					{
						enemyBaseWeapon->SetLastAttack(currentTime);
						bool died = playerTroops.front()->TakeDamage(enemyBaseWeapon->GetDamage()); // Base takes damage from the troop in range
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

			/* AI HANDLER */
			// If not decided which troop to spawn or been 10 seconds since last decision
			if (nextSpawningEnemy == None || currentTime - changeDecisionTimer > 10)
			{
				if (playerTroops.empty())
				{
					nextSpawningEnemy = AIDecideTroop(None, enemyBase->GetResources(), playerTroops.size());
					changeDecisionTimer = currentTime;
				}
				else
				{
					nextSpawningEnemy = AIDecideTroop(playerTroops.front()->GetName(), enemyBase->GetResources(), playerTroops.size());
					changeDecisionTimer = currentTime;
				}
			}
			// AI Actions
			// If a troop has been decided which to send out
			if (nextSpawningEnemy != None)
			{
				//Purchase Base Weapon
				if (enemyBase->GetNumWeapons() == 0 && enemyBase->GetResources() >= 500)
				{
					enemyBase->SpendResources(500);
					enemyBase->SetNumWeapons(1);
					enemyBaseWeapon = new BaseWeapon(quadMesh, false);
				}

				if ((playerTroops.empty() || playerTroops.front()->GetPosition() < 0) && forceTroopSpawn == 0 && aiDecisionState < 21)
				{
					if (aiDecisionState == 0 || aiDecisionState == 2 || aiDecisionState == 8)
					{
						if (enemyBase->GetResources() >= enemyBase->GetRPSUpgradeCost())
						{
							enemyBase->SpendResources(enemyBase->GetRPSUpgradeCost());
							enemyBase->UpgradeResourceGather();
							aiDecisionState++;
							forceTroopSpawn = aiDecisionState;
						}
					}
					else if (aiDecisionState == 1 || aiDecisionState == 10 || aiDecisionState == 13)
					{
						if (enemyBase->GetResources() >= enemyBlueprints[Swordsman]->GetUpgradeCost())
						{
							enemyBase->SpendResources(enemyBlueprints[Swordsman]->GetUpgradeCost());
							enemyBlueprints[Swordsman]->UpgradeTroop();
							aiDecisionState++;
							forceTroopSpawn = aiDecisionState;
						}
					}
					else if (aiDecisionState == 3)
					{
						if (enemyBase->GetResources() >= enemyBaseWeapon->GetUpgradeCost())
						{
							enemyBase->SpendResources(enemyBaseWeapon->GetUpgradeCost());
							enemyBaseWeapon->UpgradeBaseWeapon();
							aiDecisionState++;
							forceTroopSpawn = aiDecisionState;
						}
					}
					else if (aiDecisionState == 4 || aiDecisionState == 7 || aiDecisionState == 9)
					{
						if (enemyBase->GetResources() >= enemyBase->GetHealthUpgradeCost())
						{
							enemyBase->SpendResources(enemyBase->GetHealthUpgradeCost());
							enemyBase->UpgradeBaseHealth();
							aiDecisionState++;
							forceTroopSpawn = aiDecisionState;
						}
					}
					else if (aiDecisionState == 5 || aiDecisionState == 11 || aiDecisionState == 15)
					{
						if (enemyBase->GetResources() >= enemyBlueprints[Archer]->GetUpgradeCost())
						{
							enemyBase->SpendResources(enemyBlueprints[Archer]->GetUpgradeCost());
							enemyBlueprints[Archer]->UpgradeTroop();
							aiDecisionState++;
							forceTroopSpawn = aiDecisionState;
						}
					}
					else if (aiDecisionState == 6 || aiDecisionState == 14 || aiDecisionState == 16)
					{
						if (enemyBase->GetResources() >= enemyBlueprints[Spearman]->GetUpgradeCost())
						{
							enemyBase->SpendResources(enemyBlueprints[Spearman]->GetUpgradeCost());
							enemyBlueprints[Spearman]->UpgradeTroop();
							aiDecisionState++;
							forceTroopSpawn = aiDecisionState;
						}
					}
					else if (aiDecisionState == 12 || aiDecisionState == 17 || aiDecisionState == 19)
					{
						if (enemyBase->GetResources() >= enemyBlueprints[Cavalry]->GetUpgradeCost())
						{
							enemyBase->SpendResources(enemyBlueprints[Cavalry]->GetUpgradeCost());
							enemyBlueprints[Cavalry]->UpgradeTroop();
							aiDecisionState++;
							forceTroopSpawn = aiDecisionState;
						}
					}
					else if (aiDecisionState == 18 || aiDecisionState == 20 || aiDecisionState == 21)
					{
						if (enemyBase->GetResources() >= enemyBlueprints[SiegeMachine]->GetUpgradeCost())
						{
							enemyBase->SpendResources(enemyBlueprints[SiegeMachine]->GetUpgradeCost());
							enemyBlueprints[SiegeMachine]->UpgradeTroop();
							aiDecisionState++;
							forceTroopSpawn = aiDecisionState;
						}
					}
				}
				else
				{
					if (enemyBase->GetResources() >= enemyBlueprints[nextSpawningEnemy]->GetCost())
					{
						switch (nextSpawningEnemy)
						{
						case Swordsman:
							if (currentTime - lastSpawnedSwordsmanE > enemyBlueprints[nextSpawningEnemy]->GetCooldown())
							{
								enemyBase->SpendResources(enemyBlueprints[Swordsman]->GetCost());
								lastSpawnedSwordsmanE = currentTime;
								Troop* tempTroop = new Troop(enemyBlueprints[Swordsman]);
								tempTroop->SpawnTroop(false, myEngine, quadMesh);
								enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
								if (forceTroopSpawn > 0)
									forceTroopSpawn--;
							}
							break;
						case Archer:
							if (currentTime - lastSpawnedArcherE > enemyBlueprints[nextSpawningEnemy]->GetCooldown())
							{
								enemyBase->SpendResources(enemyBlueprints[Archer]->GetCost());
								lastSpawnedArcherE = currentTime;
								Troop* tempTroop = new Troop(enemyBlueprints[Archer]);
								tempTroop->SpawnTroop(false, myEngine, quadMesh);
								enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
								if (forceTroopSpawn > 0)
									forceTroopSpawn--;
							}
							break;
						case Spearman:
							if (currentTime - lastSpawnedSpearmanE > enemyBlueprints[nextSpawningEnemy]->GetCooldown())
							{
								enemyBase->SpendResources(enemyBlueprints[Spearman]->GetCost());
								lastSpawnedSpearmanE = currentTime;
								Troop* tempTroop = new Troop(enemyBlueprints[Spearman]);
								tempTroop->SpawnTroop(false, myEngine, quadMesh);
								enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
								if (forceTroopSpawn > 0)
									forceTroopSpawn--;
							}
							break;
						case Cavalry:
							if (currentTime - lastSpawnedCavalryE > enemyBlueprints[nextSpawningEnemy]->GetCooldown())
							{
								enemyBase->SpendResources(enemyBlueprints[Cavalry]->GetCost());
								lastSpawnedCavalryE = currentTime;
								Troop* tempTroop = new Troop(enemyBlueprints[Cavalry]);
								tempTroop->SpawnTroop(false, myEngine, quadMesh);
								enemyTroops.push_back(tempTroop); // add the troop to the enemy troops
								if (forceTroopSpawn > 0)
									forceTroopSpawn--;
							}
							break;
						}
						nextSpawningEnemy = None;
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
			if (myEngine->KeyHeld(camLeft) && myCamera->GetX() >= -100)
			{
				myCamera->MoveLocalX(-CAM_SPEED*frameTime);
			}
			if (myEngine->KeyHeld(camRight) && myCamera->GetX() <= 100)
			{
				myCamera->MoveLocalX(CAM_SPEED*frameTime);
			}
		}
	}
	myEngine->Delete(); // Delete the 3D engine now we are finished with it
}