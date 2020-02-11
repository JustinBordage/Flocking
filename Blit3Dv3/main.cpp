/*
	Simple example of loading/rotating/displaying sprites in Blit3D
*/

//memory leak detection
#define CRTDBG_MAP_ALLOC
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>

#include "Blit3D.h"
#include "Flock.h"
#include "Boid.h"
#include "Random.h"

//Blit3D Objects
Sprite* BoidSprite = NULL;
AngelcodeFont* debugFont;
Blit3D *blit3D = NULL;

//Flocking Related Objects
Flock* boidManager = NULL;
Random* rng;

//Input variables
bool LMB = false;
bool paused = false;
glm::vec2 cursorPos = glm::vec2(0.f, 0.f);

//Displays debug info
int numberOfBoids = 0;
string boidsText = "";

void Init()
{
	//Loads the boid sprite
	BoidSprite = blit3D->MakeSprite(1, 1, 89, 59, "Media\\Red Boid.png");

	//Loads the debug font
	debugFont = blit3D->MakeAngelcodeFontFromBinary32("Media\\Oswald_72.bin");

	//Random Number Generator
	rng = new Random();

	//Creates a flock of 1250 boids
	boidManager = new Flock(1250);

	//Processes the data to display the number of boids
	numberOfBoids = boidManager->boidList.size();
	boidsText = "# of Boids: " + std::to_string(numberOfBoids);
}

void DeInit(void)
{
	//Destroys the Flocking Objects
	if (rng != NULL) delete rng;
	if (boidManager != NULL) delete boidManager;

	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
}

void Update(double seconds)
{
	if (LMB)
	{
		//Adds a boid on the screen
		boidManager->addBoid(cursorPos.x, cursorPos.y);

		//Processes the data to display the number of boids
		numberOfBoids = boidManager->boidList.size();
		boidsText = "# of Boids: " + std::to_string(numberOfBoids);
	}

	if (!paused)
	{
		//Updates the world if not paused
		boidManager->UpdateWorld(seconds);
	}

	///FPS Counter
	//static float elapsedTime = 0.f;
	//static int FPS = 0;
	//
	//elapsedTime += seconds;
	//FPS++;
	//if (elapsedTime >= 1.f)
	//{
	//	elapsedTime -= 1.f;
	//	std::cout << "FPS: " << FPS << "\n";
	//	FPS = 0;
	//}
}

void Draw(void)
{
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);	//clear colour: r,g,b,a
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Draws all the boids in the world
	boidManager->DrawWorld();

	//Displays the number of boids present in the world
	debugFont->BlitText(20.f, 90.f, boidsText);
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence

	if (key == GLFW_KEY_P && action == GLFW_PRESS)
		paused = !paused;
}

void DoMouseButton(int key, int action, int scancode)
{
	if (key == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			LMB = true;
		}
		else if (action == GLFW_RELEASE)
		{
			LMB = false;
		}
	}
}

void DoCursor(double x, double y)
{
	cursorPos = glm::vec2(x, 1080 - y);
}

int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	blit3D = new Blit3D(Blit3DWindowModel::BORDERLESSFULLSCREEN_1080P, 1920, 1080);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);

	blit3D->SetDoInput(DoInput);
	blit3D->SetDoCursor(DoCursor);
	blit3D->SetDoMouseButton(DoMouseButton);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}