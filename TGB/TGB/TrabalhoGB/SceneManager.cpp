#include "SceneManager.h"

//static controllers for mouse and keyboard
static bool keys[1024];
static bool resized;
static GLuint width, height;

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
	// Init GLFW
	glfwInit();

	mouse_condition = 0;

	for (int i = 0; i <= 7; i++)
	{
		xPosFil[i] = 50.0 + (100 * i);
		xPosSt[i] = 50.0 + (100 * i);
	}
	
	yPosFilt = 40.0;
	xSizeFilt = 80.0;
	ySizeFilt = 60.0;

	yPosSt = 560.0;
	xSizeSt = 60.0;
	ySizeSt = 60.0;

	// Create a GLFWwindow object that we can use for GLFW's functions
	window = glfwCreateWindow(width, height, "AYAGRAMM FILTERS (:", nullptr, nullptr);
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
	// Shader program que configuram filtros existentes no app
	addShader("../shaders/vert.vs", "../shaders/frag.fr");
	addShader("../shaders/filter01.vs", "../shaders/filter01.fr");
	addShader("../shaders/filter02.vs", "../shaders/filter02.fr");
	addShader("../shaders/filter03.vs", "../shaders/filter03.fr");
	addShader("../shaders/filter04.vs", "../shaders/filter04.fr");
	addShader("../shaders/filter05.vs", "../shaders/filter05.fr");
	addShader("../shaders/filter06.vs", "../shaders/filter06.fr");
	addShader("../shaders/filter07.vs", "../shaders/filter07.fr");


	setupScene();

	resized = true; //para entrar no setup da câmera na 1a vez

}

void SceneManager::addShader(string vFilename, string fFilename)
{
	Shader* shader = new Shader(vFilename.c_str(), fFilename.c_str());
	shaders.push_back(shader);
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
	
	glfwGetCursorPos(window, &xpos, &ypos);

	//Onde aplicamos as transformações de sprites
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		for (int filter = 0; filter <= 7; filter += 1)
		{
			if (TestPositionFilter(filter)){
				objects[1]->setShader(shaders[filter]);
			}
		}

		for (int sticker = 0; sticker <= 7; sticker += 1)
		{
			if (sticker == 0)
			{
				if (TestPositionSt(sticker)) {
					int position = 1;
					for (int st = 11; st <= 17; st += 1)
					{
						objects[st]->setPosition(glm::vec3(xPosSt[position], yPosSt, 0.0));
						objects[st]->setDimension(glm::vec3(xSizeSt, ySizeSt, 1.0f));
						position += 1;
					}
				}
			}
			else
			{
				if (TestPositionSt(sticker) && mouse_condition == 0) {
					mouse_condition = sticker + 10;
				}
			}
			if (mouse_condition != 0) {
				objects[mouse_condition]->setPosition(glm::vec3(xpos, height - ypos, 0.0));
				objects[mouse_condition]->setDimension(glm::vec3(xSizeSt + 40, ySizeSt + 30, 1.0f));
			}
		}
	}
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
	{
		mouse_condition = 0;
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
	//CONSOLE
	//O APP INICIA SOLICITANDO OS DADOS DA IMAGEM QUE O USUARIO QUER APLICAR OS FILTROS
	//O NOME DEVE SER INSERIDO COM A EXTENSAO DA FOTO (JPG, PNG) + LARGURA + ALTURA
	cout << "***** BEM-VINDX AO AYAGRAMM *****" <<endl;
	cout << endl;
	cout << endl;
	cout << "-> Inclua a extensao do arquivo, ex.: 'imagem.png' <-"<<endl;
	cout << "Digite o nome da sua imagem: ";
	cin >> imagem;
	cout << "Digite a largura da imagem: ";
	cin >> largura;
	cout << "Digite a altura da imagem: ";
	cin >> altura;

	 largura = (400 * largura) / altura;
	 altura = 400;

	//Mínimo: posicao e escala e ponteiro para o shader
	Sprite* obj = new Sprite;
	obj->setPosition(glm::vec3(400.0f, 300.0f, 0.0));
	obj->setDimension(glm::vec3(800.0f, 600.0f, 1.0f)); //note que depois podemos reescalar conforme tamanho da sprite
	obj->setShader(shaders[0]);
	objects.push_back(obj); //ADICIONA O FUNDO DO APP

	obj = new Sprite;
	obj->setPosition(glm::vec3(400.0f, 300.0f, 0.0));
	obj->setDimension(glm::vec3(largura, altura, 1.0f)); //note que depois podemos reescalar conforme tamanho da sprite
	obj->setShader(shaders[0]);
	objects.push_back(obj); //ADICIONA A FOTO ESCOLHIDA ORIGINALMENTE

	for (int ft = 0; ft <= 7; ft += 1)
	{
		obj = new Sprite;
		obj->setPosition(glm::vec3(xPosFil[ft], yPosFilt, 0.0));
		obj->setDimension(glm::vec3(xSizeFilt, ySizeFilt, 1.0f));
		obj->setShader(shaders[ft]);
		objects.push_back(obj); //ADICIONA OS FILTROS
	}

	for (int st = 0; st <= 7; st += 1)
	{
		obj = new Sprite;
		obj->setPosition(glm::vec3(xPosSt[st], yPosSt, 0.0));
		if (st == 0) {
			obj->setDimension(glm::vec3(xSizeSt - 10, ySizeSt - 10, 1.0f));
		}
		else
		{
			obj->setDimension(glm::vec3(xSizeSt, ySizeSt, 1.0f));
		}
		obj->setShader(shaders[0]);
		objects.push_back(obj); //ADICIONA ALGUNS EMOJIS 
	}

	//Carregamento das texturas (pode ser feito intercalado na criação)
	//Futuramente, utilizar classe de materiais e armazenar dimensoes, etc

	unsigned int texID = loadTexture("../textures/fundo.png");
	objects[0]->setTexture(texID);

	texID = loadTexture("../textures/" + imagem);
	for (int i = 1; i <= 9; i += 1)
	{
		objects[i]->setTexture(texID);
	}

	texID = loadTexture("../textures/limpa.png");
	objects[10]->setTexture(texID);
	texID = loadTexture("../textures/icon01.png");
	objects[11]->setTexture(texID);
	texID = loadTexture("../textures/icon02.png");
	objects[12]->setTexture(texID);
	texID = loadTexture("../textures/icon03.png");
	objects[13]->setTexture(texID);
	texID = loadTexture("../textures/icon04.png");
	objects[14]->setTexture(texID);
	texID = loadTexture("../textures/icon05.png");
	objects[15]->setTexture(texID);
	texID = loadTexture("../textures/icon06.png");
	objects[16]->setTexture(texID);
	texID = loadTexture("../textures/icon07.png");
	objects[17]->setTexture(texID);

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
	shaders[0]->Use();
	GLint projLoc = glGetUniformLocation(shaders[0]->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	for (int i = 0; i <= 7; i += 1)
	{
		shaders[i]->Use();
		projLoc = glGetUniformLocation(shaders[i]->ID, "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	}
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

bool SceneManager::TestPositionFilter(int filter_n) 
{
	glfwGetCursorPos(window, &xpos, &ypos);

	if (xpos < (xPosFil[filter_n] + xSizeFilt / 2) && xpos > (xPosFil[filter_n] - xSizeFilt / 2))
	{
		if ((height - ypos) < (yPosFilt + ySizeFilt / 2) && ((height - ypos) > yPosFilt - ySizeFilt / 2))
		{
			return true;
		}
	}
	return false;
}

bool SceneManager::TestPositionSt(int emoji_n)
{
	glfwGetCursorPos(window, &xpos, &ypos);

	if (xpos < (xPosSt[emoji_n] + xSizeSt / 2) && xpos > (xPosSt[emoji_n] - xSizeSt / 2))
	{
		if ((height - ypos) < (yPosSt + ySizeSt / 2) && ((height - ypos) > yPosSt - ySizeSt / 2))
		{
			return true;
		}
	}
	return false;
}