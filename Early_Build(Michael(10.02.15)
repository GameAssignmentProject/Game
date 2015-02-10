// Game_Of_Way.cpp: A program using the TL-Engine
// Michael Lorimer(10/02/2015)

#include <TL-Engine.h>	// TL-Engine include file and namespace
using namespace tle;

void main()
{
	// Create a 3D engine (using TLX engine here) and open a window for it
	I3DEngine* myEngine = New3DEngine( kTLX );
	myEngine->StartWindowed();

	// Add default folder for meshes and other media
	myEngine->AddMediaFolder( "C:\\Users\\nibzomatic\\Desktop\\Game_Of_Way\\media" );

	/**** Set up your scene here ****/

	IMesh* cubeMesh;
	IModel* cube;
	IModel* cube1;
	IModel* cubeP;

	cubeMesh = myEngine->LoadMesh( "Cube.x" );
	cube = cubeMesh->CreateModel();
	cube->SetSkin("Grass1.jpg");
	cube->ScaleX(15);

	cube1 = cubeMesh->CreateModel(-70, 15, 0);
	cube1->SetSkin("CueTip.jpg");
	cube1->ScaleY(2);

	cube1 = cubeMesh->CreateModel(70, 15, 0);
	cube1->SetSkin("CueMetal.jpg");
	cube1->ScaleY(2);

	cubeP = cubeMesh->CreateModel(-60, 10, 0);
	cubeP->SetSkin("tiles1.jpg");


	//variables
	float cubeX = cubeP->GetX();
	bool cubeGo = false;

	const float KCubeSpeed = 40.0f; // constant cube travel speed
	const float kMapLimit = 65.0f; // map limit for the cube t travel 
	const float kCamSpeed = 50.0f;

	//timer
	float frameTime = myEngine->Timer();

	/*==========
	|| camera ||
	==========*/

	ICamera* myCamera;
	myCamera = myEngine->CreateCamera(kFPS);
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

	EKeyCode move = Key_1;
	EKeyCode spawn = Key_2;


	// play cam controlls 
	EKeyCode camLeft = Key_A;
	EKeyCode camRight = Key_D;




	// The main game loop, repeat until engine is stopped
	while (myEngine->IsRunning())
	{
		frameTime = myEngine->Timer();
		// Draw the scene
		myEngine->DrawScene();

		/**** Update your scene each frame here ****/
		
		
		// number controles 
		if (myEngine->KeyHit(move))
		{
			cubeGo = true; // bool to show a cube is moving
		}

		// -- movement of a cube (unit)
		if (cubeX=cubeP->GetX() >= kMapLimit)
		{
			cubeGo = false;
		}

		if (cubeGo)
		{
			cubeP->MoveLocalX(KCubeSpeed*frameTime);
		}
		if (!cubeGo)
		{
			cubeP->MoveLocalX(0.0f);
			if (myEngine->KeyHit(spawn))//-------------------edit later -------------------
			{
				//CreateCube(cubeMesh, cubeP);
				cubeP = cubeMesh->CreateModel(-kMapLimit, 10, 0);
				cubeP->SetSkin("tiles1.jpg");
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
			myCamera->MoveLocalX(kCamSpeed*frameTime);
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
