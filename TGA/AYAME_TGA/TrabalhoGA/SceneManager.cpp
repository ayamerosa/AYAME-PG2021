#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;
unsigned int textzoo[3];
int top = 0;

SceneManager::SceneManager()
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::initialize(GLuint w, GLuint h)
{
	width = w;
	height = h;

	// GLFW - GLEW - OPENGL general setup -- TODO: config file
	initializeGraphics();

}

void SceneManager::initializeGraphics()
{
	//posicao inicial do zombie
	setX = 25;
	setY = 85;

	// Init GLFW
	glfwInit();

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "ZOOOOMBIE JUMP!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	//Setando a callback de redimensionamento da janela
	glfwSetWindowSizeCallback(window, resize);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Build and compile our shader program
	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	//setup the scene -- LEMBRANDO QUE A DESCRIÇÃO DE UMA CENA PODE VIR DE ARQUIVO(S) DE 
	// CONFIGURAÇÃO
	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	shader = new Shader(vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow* window, int w, int h)
{
	width = w;
	height = h;
	resized = true;

	// Define the viewport dimensions
	glViewport(0, 0, width, height);
}

void SceneManager::update()
{
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);


	//AQUI aplicaremos as transformações nos sprites
	if (keys[GLFW_KEY_LEFT]) //movimentacao para esquerda
	{
		if ((setX + 0.01) > 0.0)
		{
			setX -= 0.2f;
			setY += 0.0f;
			objects[1]->setTexture(textzoo[2]);
			objects[1]->setPosition(glm::vec3(setX, setY, 0.0));

			if (TestCollision()) {
				finish();
				keys[GLFW_KEY_LEFT] = false;
				std::cout << "VOCE PERDEU =(" << std::endl;
			}
		}
	}

	if (keys[GLFW_KEY_RIGHT]) //movimentacao para direita
	{
		if ((setX + 0.01) < width)
		{
			setX += 0.3f;
			setY += 0.0f;
			objects[1]->setTexture(textzoo[0]);
			objects[1]->setPosition(glm::vec3(setX, setY, 0.0));

			if (TestCollision()) {
				finish();
				keys[GLFW_KEY_RIGHT] = false;
				std::cout << "VOCE PERDEU =(" << std::endl;
			}
		}
	}

	if (keys[GLFW_KEY_UP]) //salto do zumbie
	{
		if ((setY + 0.01) < 85.1)
		{
			setX += 0.0f;
			setY += 0.1f;

			top = 1;

			objects[1]->setTexture(textzoo[1]);
			objects[1]->setPosition(glm::vec3(setX, setY, 0.0));
			
			if (TestCollision()) {
				finish();
				keys[GLFW_KEY_RIGHT] = false;
				std::cout << "VOCE PERDEU =(" << std::endl;
			}
		}
	}


	if (top == 1)
	{
		setX += 0.0f;
		setY += 0.6f;
		objects[1]->setTexture(textzoo[1]);
		objects[1]->setPosition(glm::vec3(setX, setY, 0.0));

		if (setY >= 235)
		{
			top = 2;
		}

		if (TestCollision()) {
			finish();
			keys[GLFW_KEY_RIGHT] = false;
			std::cout << "VOCE PERDEU =(" << std::endl;
		}
	}



	if (top == 2)
	{
		setX += 0.0f;
		setY -= 0.6f;

		if (setY <= 85)
		{
			top = 0;
			setY = 85;
		}

		objects[1]->setTexture(textzoo[1]);
		objects[1]->setPosition(glm::vec3(setX, setY, 0.0));



		if (TestCollision()) {
			finish();
			keys[GLFW_KEY_RIGHT] = false;
			std::cout << "VOCE PERDEU =(" << std::endl;
		}
	}


}

void SceneManager::render()
{
	// Clear the colorbuffer
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (resized) //se houve redimensionamento na janela, redefine a projection matrix
	{
		setupCamera2D();
		resized = false;
	}

	//atualiza e desenha os Sprites

	for (int i = 0; i < objects.size(); i++)
	{
		objects[i]->update();
		objects[i]->draw();
	}

}

void SceneManager::run()
{
	//GAME LOOP
	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		//Update method(s)
		update();

		//Render scene
		render();

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();
}


void SceneManager::setupScene()
{
	//Criação dos Sprites iniciais -- pode-se fazer métodos de criação posteriormen

	//Mínimo: posicao e escala e ponteiro para o shader
	Sprite* obj = new Sprite;
	obj->setPosition(glm::vec3(200.0f, 300.0f, 0.0));
	obj->setDimension(glm::vec3(1280.0f, 720.0f, 1.0f)); //note que depois podemos reescalar conforme tamanho da sprite
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o primeiro obj BACKGROUND

	obj = new Sprite;
	obj->setPosition(glm::vec3(setX, setY, 0.0));
	obj->setDimension(glm::vec3(64.0f, 64.0f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o segundo obj ZUMBIE

	//POSICIONA MONSTROS
	obj = new Sprite;
	obj->setPosition(glm::vec3(100.0f, 395.0f, 0.0));
	obj->setDimension(glm::vec3(30.0f, 30.0f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o terceiro obj MONSTRO

	obj = new Sprite;
	obj->setPosition(glm::vec3(425.0f, 368.0f, 0.0));
	obj->setDimension(glm::vec3(30.0f, 30.0f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o quarto obj MONSTRO

	obj = new Sprite;
	obj->setPosition(glm::vec3(780.0f, 150.0f, 0.0));
	obj->setDimension(glm::vec3(30.0f, 30.0f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o quinto obj MONSTRO


	// ADICIONA E POSICIONA AS CAIXAS NO MUNDO, OS OBSTACULOS
	obj = new Sprite;
	obj->setPosition(glm::vec3(600.0f, 85.0f, 0.0));
	obj->setDimension(glm::vec3(50.5f, 50.5f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o oitavo obj CAIXA
	
	obj = new Sprite;
	obj->setPosition(glm::vec3(430.0f, 135.0f, 0.0));
	obj->setDimension(glm::vec3(50.5f, 50.5f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o sexto obj CAIXA

	obj = new Sprite;
	obj->setPosition(glm::vec3(430.0f, 85.0f, 0.0));
	obj->setDimension(glm::vec3(50.5f, 50.5f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o sétimo obj CAIXA

	obj = new Sprite;
	obj->setPosition(glm::vec3(290.0f, 85.0f, 0.0));
	obj->setDimension(glm::vec3(50.5f, 50.5f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o oitavo obj CAIXA

	obj = new Sprite;
	obj->setPosition(glm::vec3(160.0f, 85.0f, 0.0));
	obj->setDimension(glm::vec3(50.5f, 50.5f, 1.0f));
	obj->setShader(shader);
	objects.push_back(obj); //adiciona o oitavo obj CAIXA

		//Carregamento das texturas (pode ser feito intercalado na criação)
	//Futuramente, utilizar classe de materiais e armazenar dimensoes, etc
	unsigned int texID = loadTexture("../textures/wall2.png");
	objects[0]->setTexture(texID);

	textzoo[0] = loadTexture("../textures/zoo00.png");
	textzoo[1] = loadTexture("../textures/zoo01.png");
	textzoo[2] = loadTexture("../textures/zoo02.png");

	objects[1]->setTexture(textzoo[0]);

	
	texID = loadTexture("../textures/monstro00.png");
	objects[2]->setTexture(texID);
	objects[3]->setTexture(texID);
	objects[4]->setTexture(texID);
	

	texID = loadTexture("../textures/box1.png");
	objects[5]->setTexture(texID);
	objects[6]->setTexture(texID);
	objects[7]->setTexture(texID);
	objects[8]->setTexture(texID);
	objects[9]->setTexture(texID);

	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f; //xMin
	ortho2D[1] = 800.0f; //xMax
	ortho2D[2] = 0.0f; //yMin
	ortho2D[3] = 600.0f; //yMax

	//Habilita transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void SceneManager::setupCamera2D() //TO DO: parametrizar aqui
{
	float zNear = -1.0, zFar = 1.0; //estão fixos porque não precisamos mudar

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);


	//Obtendo o identificador da matriz de projeção para enviar para o shader
	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	//Enviando a matriz de projeção para o shader
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

unsigned int SceneManager::loadTexture(string filename)
{
	unsigned int texture;

	// load and create a texture 
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
										   // set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// load image, create texture and generate mipmaps
	int width, height, nrChannels;

	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);

	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

	glActiveTexture(GL_TEXTURE0);

	return texture;
}

bool SceneManager::TestCollision() {

	if (setX <= 160 + 30 && setX >= 160 - 30)
	{
		if (setY <= 85 + 30 && setY >= 85 - 30)
		{
			return true;
		}
	}

	if (setX <= 290 + 30 && setX >= 290 - 30)
	{
		if (setY <= 85 + 30 && setY >= 85 - 30)
		{
			return true;
		}
	}

	if (setX <= 430 + 30 && setX >= 430 - 30)
	{
		if (setY <= 85 + 30 && setY >= 85 - 30)
		{
			return true;
		}
	}

	if (setX <= 430 + 30 && setX >= 430 - 30)
	{
		if (setY <= 135 + 30 && setY >= 135 - 30)
		{
			return true;
		}
	}

	if (setX <= 600 + 30 && setX >= 600 - 30)
	{
		if (setY <= 85 + 30 && setY >= 85 - 30)
		{
			return true;
		}
	}

	return false;
}