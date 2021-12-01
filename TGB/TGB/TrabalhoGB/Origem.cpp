// CODIGO ADAPTADO DA Prof Rossana Baptista Queiroz
// APP AYAGRAMM PARA FILTROS E STICKS EM FOTOS/IMAGENS
// Aluna Ayame Gamarra R da Rosa

using namespace std;

#include "SceneManager.h"


// FUNCAO QUE INICIA A APLICACAO DO APP DE FILTROS
int main()
{
	SceneManager* scene = new SceneManager;
	scene->initialize(800, 600);

	scene->run();

	scene->finish();

	return 0;
}