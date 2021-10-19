// Hello Transforms - código inicial retirado do site LearnOpenGL
// Hello Triangle - código adaptado de prof Rossana - PG

using namespace std;

#include "SceneManager.h"


// The MAIN function, from here we start the application and run the game loop
int main()
{
	SceneManager* scene = new SceneManager;
	scene->initialize(1024, 720);

	scene->run();

	scene->finish();

	return 0;
}