#pragma once

#include "Shader.h"
#include "Sprite.h" 

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

using namespace std;

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	//GLFW callbacks
	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void resize(GLFWwindow* window, int width, int height);

	//Métodos pricipais
	void initialize(GLuint width, GLuint height);
	void run();
	void finish();

	//Métodos chamados no run (ciclo)
	void update();
	void render();

	//Métodos para configuração e carregamento dos elementos da cena
	void initializeGraphics();
	void addShader(string vFilename, string fFilename);
	void setupScene(); //antigo setupGeometry
	void setupCamera2D();
	unsigned int loadTexture(string filename);
	bool TestPositionFilter(int filter_n);
	bool TestPositionSt(int emoji_n);

private:

	//Janela GLFW
	GLFWwindow* window;

	//Programa de shader
	vector <Shader*> shaders;

	//Câmera 2D - Matriz de projeção (ortográfica) com os limites em x,y
	glm::vec4 ortho2D; //xmin, xmax, ymin, ymax
	glm::mat4 projection;

	//Nossos objetos (sprites) da cena
	vector <Sprite*> objects;

	float xPosFil[8], yPosFilt, xSizeFilt, ySizeFilt, xPosSt[8], yPosSt, xSizeSt, ySizeSt, largura, altura;
	string imagem;
	double xpos, ypos;
	int mouse_condition;
};