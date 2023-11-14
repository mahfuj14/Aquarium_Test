#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>

#include "Lights.h"
#include "Shader.h"
#include "Camera.h"
#include "Floor.h"
#include "Renderable.h"
#include "stb_image.h"
#include "Skybox.h"
#include <Windows.h>

// settings
const unsigned int SCR_WIDTH = 1540;
const unsigned int SCR_HEIGHT = 1060;
const int FRAMES_PER_SECOND = 120;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
glm::vec3 CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


unsigned int drawControlPoints();
long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L);

Camera camera(glm::vec3(0.0f, 3.0f, 8.0f));
Shader* lightPosShader, * dShader, * shaderCube, *shaderSkybox;
Floor* podloga_w;
Renderable* object;
std::vector<Renderable*> table;
DirLight* lightDir;
SpotLight* lightSpot;
PointLight* lightPoint;
Skybox* sky;

bool lightingOn = false;
float ambientOn = 1.0;
float diffuseOn = 1.0;
float specularOn = 1.0;
bool dark = false;

float directionalLightOn = 0.0;
float pointLightOn = 0.0;
float spotLightOn = 0.0;



int fishNum = 10;
int rockNum = 5;

int objNum = fishNum + rockNum;


bool enableF = 1, enableD = 1, enableS = 1, enableP=1;
float KEY_PRESS = 0.0;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

class Vector2D {
public:
	double X, Y;


	Vector2D(double, double);
};
Vector2D::Vector2D(double xx, double  yy) {
	X = xx;
	Y = yy;
}

//static void PythagorasTree(Vector2D* center, Vector2D* up, const unsigned long depth);
//static void DrawRectangle(Vector2D* center, Vector2D* up, const unsigned long depth);
//static void PythagorasTreeByLen(Vector2D* center, Vector2D* up, const unsigned long depth, const double magn, const unsigned long screenw, const unsigned long screenh);

std::vector <float> cntrlPoints = {
-0.2200, 0.5650, 5.1000,
-0.2150, 0.7300, 5.1000,
-0.2350, 0.9200, 5.1000,
-0.2350, 0.9650, 5.1000,
-0.2300, 1.0500, 5.1000,
-0.2100, 1.1300, 5.1000,
-0.2250, 1.2050, 5.1000,
-0.2400, 1.2800, 5.1000,
-0.2400, 1.3900, 5.1000,
-0.2400, 1.4700, 5.1000,
-0.2400, 1.6000, 5.1000,
-0.2600, 1.6600, 5.1000,
-0.3200, 1.6800, 5.1000,
-0.3900, 1.7050, 5.1000,
-0.5050, 1.7400, 5.1000,
-0.5650, 1.7600, 5.1000,
-0.6400, 1.7850, 5.1000,
-0.7400, 1.8250, 5.1000,
-0.8700, 1.8950, 5.1000,
-0.9000, 1.9800, 5.1000,
-0.8750, 2.0750, 5.1000,
-0.8550, 2.1250, 5.1000,
-0.7950, 2.1950, 5.1000,
-0.7250, 2.2500, 5.1000,
-0.6500, 2.2800, 5.1000,
-0.5600, 2.3000, 5.1000,
-0.4600, 2.3350, 5.1000,
-0.4100, 2.3600, 5.1000,
-0.3000, 2.3800, 5.1000,
-0.2500, 2.3950, 5.1000,
-0.2000, 2.3950, 5.1000,
-0.1600, 2.4000, 5.1000,
-0.0900, 2.4150, 5.1000,
-0.0350, 2.4150, 5.1000,
0.0050, 2.4150, 5.1000,
0.0600, 2.4250, 5.1000,
};
std::vector <float> coordinates;
std::vector <float> normals;
std::vector <int> indices;
std::vector <float> vertices;
class point
{
public:
	point()
	{
		x = 0;
		y = 0;
	}
	int x;
	int y;
} clkpt[2];
int mouseButtonFlag = 0;
FILE* fp;
const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;
bool showControlPoints = true;
bool loadBezierCurvePoints = true;
bool showHollowBezier = true;
bool lineMode = false;
unsigned int bezierVAO;


void initOpenGLProgram(GLFWwindow* window)
{
	glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	//glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // tell GLFW to capture our mouse
}

void freeOpenGLProgram(GLFWwindow* window)
{
	glfwDestroyWindow(window);
	for (int i = 0; i < objNum; i++)
	{
		Renderable* r = table.back();
		table.pop_back();
		delete r;
	}
}

void drawScene(GLFWwindow* window)
{
	glClearColor(0.4f, 0.5f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	dShader->use();
	dShader->setVec3("viewPos", camera.getPosition());
	dShader->setFloat("material.shininess", 32.0f);

	lightDir->apply(dShader);
	lightSpot->apply(dShader);
	lightPoint->apply(dShader);


	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 view = glm::mat4(1.0f);
	glm::mat4 projection = glm::mat4(1.0f);
	view = camera.GetViewMatrix();
	projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	dShader->setMat4("view", view);
	dShader->setMat4("projection", projection);
	dShader->setMat4("model", model);
	dShader->setVec3("FogColor", CLEAR_COLOR);

	dShader->setBool("enableFog", enableF);
	dShader->setBool("enableDirLight", enableD);
	dShader->setBool("enableSpotLight", enableS);
	dShader->setBool("enablePointLight", enableP);

	podloga_w->draw(dShader);
	shaderCube->use();
	shaderCube->setMat4("view", view);
	//shaderCube->setMat4("model", model);
	shaderCube->setMat4("projection", projection);
	shaderCube->setVec3("viewPos", camera.getPosition());
	shaderCube->setVec3("FogColor", CLEAR_COLOR);
	shaderCube->setFloat("material.shininess", 32.0f);

	//shaderCube->setBool("enableFog", enableF);
	shaderCube->setBool("enableDirLight", enableD);
	shaderCube->setBool("enableSpotLight", enableS);
	shaderCube->setBool("enablePointLight", enableP);

	shaderSkybox->setMat4("view", view);
	shaderSkybox->setMat4("projection", projection);

	
	if (loadBezierCurvePoints)
	{
		bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1);
		loadBezierCurvePoints = false;
		showHollowBezier = true;
	}
	if (showHollowBezier)
	{
		lightPosShader->use();

		glBindVertexArray(bezierVAO);
		glDrawElements(GL_TRIANGLES,                    // primitive type
			(unsigned int)indices.size(),          // # of indices
			GL_UNSIGNED_INT,                 // data type
			(void*)0);                       // offset to indices

		// unbind VAO
		glBindVertexArray(0);
	}
	if (showControlPoints)
	{
		lightPosShader->use();
		lightPosShader->setMat4("projection", projection);
		lightPosShader->setMat4("view", view);
		model = glm::mat4(1.0f);
		lightPosShader->setMat4("model", model);
		lightPosShader->setVec3("color", glm::vec3(0.0f, 0.4f, 0.2f));
		//unsigned int controlPointVAO = drawControlPoints();
		//glBindVertexArray(controlPointVAO);
		glPointSize(5.0);
		glDrawArrays(GL_POINTS, 0, (unsigned int)cntrlPoints.size());
	}




	for (int i = 0; i < objNum; i++)
	{
		object = table[i];
		object->behave();
		object->draw(dShader);
	}
	sky->draw(shaderCube, shaderSkybox);
	//creating a treasure 
	//object = table[objNum];
	//object->draw(dShader);
	glfwSwapBuffers(window);
}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Aquarium", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	initOpenGLProgram(window);

	Shader lightShader("normal.vs", "normal.fs");
	Shader defaultShader("PhongShading.vs", "PhongShading.fs");
	lightPosShader = &lightShader;
	dShader = &defaultShader;

	Floor podloga;
	podloga_w = &podloga;

	Skybox skybox;
	sky = &skybox;

	DirLight light123;
	lightDir = &light123;

	SpotLight light321;
	lightSpot = &light321;

	PointLight light213;
	lightPoint = &light213;


	for (int i = 0; i < fishNum; i++)
	{
		Fish* rryba = new Fish("random");
		table.push_back(rryba);
	}

	for (int i = 0; i < rockNum; i++)
	{
		Rock* sskala = new Rock();
		table.push_back(sskala);
	}

	//Other* treasure = new Other("treasure", glm::vec3(1.0f, 1.0f, 1.0f));
	//table.push_back(treasure);
	Shader shader("PhongShading.vs", "PhongShading2.fs");
	Shader skyboxShader("skybox.vs", "skybox.fs");
	shaderCube = &shader;
	shaderSkybox = &skyboxShader;
	





	DWORD next_game_tick = GetTickCount64();
	int sleep_time = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		int width = 300;
		int height = 230;
		//Vector2D center = { width * 0.5,height * 0.8 };
		//Vector2D up = { 0.0,-50.0 };

		//PythagorasTree(&center,&up,10);
		//double magn = 100;
		//glfwGetCursorPos(window, 0, &magn);
		//PythagorasTreeByLen(&center, &up, 13, (double)magn / (double)128.0, width, height);

		drawScene(window);

		// activate shader
        //ourShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        //ourShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        //glm::mat4 view = basic_camera.createViewMatrix();
        //ourShader.setMat4("view", view);

        //// Modelling Transformation
        //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        //translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        //ourShader.setMat4("model", model);
        ////ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));



        //// Modelling Transformation
        //glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
        //translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(1.0, translate_Y, translate_Z));
        //rotateXMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        //rotateYMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        //rotateZMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        //scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.8, scale_Y, scale_Z));
        //model2 = translateMatrix2 * rotateXMatrix2 * rotateYMatrix2 * rotateZMatrix2 * scaleMatrix2;
        //ourShader.setMat4("model", model2);
        ////ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));




        //glBindVertexArray(VAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);


        // render boxes
        //for (unsigned int i = 0; i < 10; i++)
        //{
        //    // calculate the model matrix for each object and pass it to shader before drawing
        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        //    model = glm::translate(model, cubePositions[i]);
        //    float angle = 20.0f * i;
        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        //    ourShader.setMat4("model", model);

        //    glDrawArrays(GL_TRIANGLES, 0, 36);
        //}

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------


		// render
		// ------
		//glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);




		glfwPollEvents();

		next_game_tick += SKIP_TICKS;
		sleep_time = next_game_tick - GetTickCount64();
		if (sleep_time >= 0)
		{
			Sleep(sleep_time);
		}
	}
	freeOpenGLProgram(window);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetTime() - KEY_PRESS > 0.35)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.ProcessKeyboard(FORWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.ProcessKeyboard(LEFT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.ProcessKeyboard(RIGHT, deltaTime);
		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
		{
			enableF = !enableF; KEY_PRESS = glfwGetTime();
		}
		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
		{
			enableS = !enableS; KEY_PRESS = glfwGetTime();
		}
		if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
		{
			enableD = !enableD; KEY_PRESS = glfwGetTime();
		}
	}

	////for lightning debug
	//if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	//	lightSpot->position.y += 0.1f;
	//if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	//	lightSpot->position.y -= 0.1f;
	//if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
	//	lightSpot->cutOff += 0.01f;
	//if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
	//	lightSpot->cutOff -= 0.01f;
	//if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
	//	lightSpot->outerCutOff += 0.01f;
	//if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
	//	lightSpot->outerCutOff -= 0.01f;

	//if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	//	lightDir->ambient.r += 0.01f;
	//if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
	//	lightDir->ambient.r -= 0.01f;
	//if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
	//	lightDir->ambient.g += 0.01f;
	//if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
	//	lightDir->ambient.g -= 0.01f;
	//if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
	//	lightDir->ambient.b += 0.01f;
	//if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	//	lightDir->ambient.b -= 0.01f;



	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)                   //Lighting On-Off
	{
		lightingOn ^= true;
		std::cout << lightingOn << std::endl;
		Sleep(100);
	}
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)                   //Ambient On
	{
		ambientOn = 1.0;
		lightPoint->turnAmbientOn();
		lightDir->turnAmbientOn();
		lightSpot->turnAmbientOn();
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)                   //Ambient Off
	{
		ambientOn = 0.0;
		lightPoint->turnAmbientOff();
		lightSpot->turnAmbientOff();
		lightDir->turnAmbientOff();
	}

	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)                   //Diffuse On
	{
		diffuseOn = 1.0;
		lightPoint->turnDiffuseOn();
		lightSpot->turnDiffuseOn();
		lightDir->turnDiffuseOn();

	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)                   //Diffuse Off
	{
		diffuseOn = 0.0;
		lightPoint->turnDiffuseOff();
		lightSpot->turnDiffuseOff();
		lightDir->turnDiffuseOff();
	}
	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)                   //Specular On
	{
		specularOn = 1.0;
		lightPoint->turnSpecularOn();
		lightSpot->turnSpecularOn();
		lightDir->turnSpecularOn();
	}
	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)                   //Specular Off
	{
		specularOn = 0.0;
		lightPoint->turnSpecularOff();
		lightSpot->turnSpecularOff();
		lightDir->turnSpecularOff();
	}
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)                   //Point Light On
	{
		pointLightOn = 1.0;
		lightPoint->turnOn();
	}
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)                   //Point Light Off
	{
		pointLightOn = 0.0;
		lightPoint->turnOff();
	}
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)                   //Directional Light On
	{
		directionalLightOn = 1.0;
		lightDir->turnOn();
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)                   //Directional Light Off
	{
		directionalLightOn = 0.0;
		lightDir->turnOff();
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)                   //Spot Light On
	{
		spotLightOn = 1.0;
		lightSpot->turnOn();
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)                   //Spot Light Off
	{
		spotLightOn = 0.0;
		lightSpot->turnOff();
	}
	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)                   //Dark On-Off
	{
		dark ^= true;
		std::cout << dark << std::endl;
		Sleep(100);
	}

	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)                   //Pitch positive
	{
		/*rotateAngle_X += 1;
		rotateAxis_X = 1.0;
		rotateAxis_Y = 0.0;
		rotateAxis_Z = 0.0;*/
		camera.ProcessYPR(0.0f, 3.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)                   //Pitch negative
	{
		/*rotateAngle_X += 1;
		rotateAxis_X = 1.0;
		rotateAxis_Y = 0.0;
		rotateAxis_Z = 0.0;*/
		camera.ProcessYPR(0.0f, -3.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)                   //Yaw positive
	{
		/*rotateAngle_Y += 1;
		rotateAxis_X = 0.0;
		rotateAxis_Y = 1.0;
		rotateAxis_Z = 0.0;*/
		camera.ProcessYPR(3.0f, 0.0f, 0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)                   //Yaw negative
	{
		/*rotateAngle_Y += 1;
		rotateAxis_X = 0.0;
		rotateAxis_Y = 1.0;
		rotateAxis_Z = 0.0;*/
		camera.ProcessYPR(-3.0f, 0.0f, 0.0f);
	}

	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)                   //Roll positive
	{
		/*rotateAngle_Z += 0.1;
		rotateAxis_X = 0.0;
		rotateAxis_Y = 0.0;
		rotateAxis_Z = 1.0;*/
		camera.ProcessYPR(0.0f, 0.0f, 0.5f);

	}
	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)                   //Roll negative
	{
		/*rotateAngle_Z += 0.1;
		rotateAxis_X = 0.0;
		rotateAxis_Y = 0.0;
		rotateAxis_Z = 1.0;*/
		camera.ProcessYPR(0.0f, 0.0f, -0.5f);

	}



}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}



//// glfw: whenever the mouse moves, this callback is called
//// -------------------------------------------------------
//void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
//{
//	float xpos = static_cast<float>(xposIn);
//	float ypos = static_cast<float>(yposIn);
//
//	if (firstMouse)
//	{
//		lastX = xpos;
//		lastY = ypos;
//		firstMouse = false;
//	}
//
//	float xoffset = xpos - lastX;
//	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xpos;
//	lastY = ypos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}

void mouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	float xoffset = xPos - lastX;
	float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
{
	camera.ProcessMouseScroll(yOffset);
}

static void DrawRectangle(Vector2D* center, Vector2D* up, const unsigned long depth)
{
	Vector2D normaln = { -up->Y,up->X };

	glColor3f(((1.0f / 10.0f) * (double)depth), 0.3f + ((0.7f / 10.0f) * (double)depth), 0.5f);

	glBegin(GL_QUADS);
	glVertex2d(center->X - up->X - normaln.X, center->Y - up->Y - normaln.Y);
	glVertex2d(center->X - up->X + normaln.X, center->Y - up->Y + normaln.Y);
	glVertex2d(center->X + up->X + normaln.X, center->Y + up->Y + normaln.Y);
	glVertex2d(center->X + up->X - normaln.X, center->Y + up->Y - normaln.Y);
	glEnd();
};

static void PythagorasTreeByLen(Vector2D* center, Vector2D* up, const unsigned long depth, const double magn, const unsigned long screenw, const unsigned long screenh)
{
	if (depth == 0 || center == 0 || up == 0) return;

	// Avoid if center are out of screen area
	if (center->X<0 || center->Y<0 || center->X>screenw || center->Y>screenh) return;

	// Draw incoming rectangle
	DrawRectangle(center, up, depth);

	// --------------------------------
	// Calculate one side
	//

	Vector2D top(center->X + up->X + up->X * magn, center->Y + up->Y + up->Y * magn);

	Vector2D normal(-up->Y, up->X);

	Vector2D left((center->X + up->X + normal.X), (center->Y + up->Y + normal.Y));

	Vector2D cross((top.X - left.X) * 0.5, (top.Y - left.Y) * 0.5);

	Vector2D crossnormal(-cross.Y, cross.X);

	Vector2D newcenter((left.X + cross.X + crossnormal.X), (left.Y + cross.Y + crossnormal.Y));

	PythagorasTreeByLen(&newcenter, &crossnormal, depth - 1, magn, screenw, screenh);

	// --------------------------------
	// Calculate the other side
	//
	Vector2D normal2(up->Y, -up->X);
	Vector2D left2((center->X + up->X + normal2.X), (center->Y + up->Y + normal2.Y));

	Vector2D cross2((top.X - left2.X) * 0.5, (top.Y - left2.Y) * 0.5);

	Vector2D crossnormal2(cross2.Y, -cross2.X);

	Vector2D newcenter2((left2.X + cross2.X + crossnormal2.X), (left2.Y + cross2.Y + crossnormal2.Y));

	PythagorasTreeByLen(&newcenter2, &crossnormal2, depth - 1, magn, screenw, screenh);
};

static void PythagorasTree(Vector2D* center, Vector2D* up, const unsigned long depth)
{
	// This is static and classical Pythagoras Tree

	if (depth == 0 || center == 0 || up == 0) return;
	DrawRectangle(center, up, depth);

	unsigned char counter = 0;

	Vector2D normal(-up->Y, up->X);

	for (counter = 0; counter < 2; counter++)
	{
		Vector2D newup((up->X + normal.X) * 0.5, (up->Y + normal.Y) * 0.5);

		Vector2D newcenter(center->X + up->X * 2.0 + normal.X, center->Y + up->Y * 2.0 + normal.Y);

		PythagorasTree(&newcenter, &newup, depth - 1);

		Vector2D normal(up->Y, -up->X);
	};
};




unsigned int drawControlPoints()
{
	unsigned int controlPointVAO;
	unsigned int controlPointVBO;

	glGenVertexArrays(1, &controlPointVAO);
	glGenBuffers(1, &controlPointVBO);

	glBindVertexArray(controlPointVAO);

	glBindBuffer(GL_ARRAY_BUFFER, controlPointVBO);
	glBufferData(GL_ARRAY_BUFFER, (unsigned int)cntrlPoints.size() * sizeof(float), cntrlPoints.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return controlPointVAO;
}




long long nCr(int n, int r)
{
	if (r > n / 2)
		r = n - r; // because C(n, r) == C(n, n - r)
	long long ans = 1;
	int i;

	for (i = 1; i <= r; i++)
	{
		ans *= n - r + i;
		ans /= i;
	}

	return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
	double y = 0;
	double x = 0;
	t = t > 1.0 ? 1.0 : t;
	for (int i = 0; i < L + 1; i++)
	{
		long long ncr = nCr(L, i);
		double oneMinusTpow = pow(1 - t, double(L - i));
		double tPow = pow(t, double(i));
		double coef = oneMinusTpow * tPow * ncr;
		x += coef * ctrlpoints[i * 3];
		y += coef * ctrlpoints[(i * 3) + 1];

	}
	xy[0] = float(x);
	xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L)
{
	int i, j;
	float x, y, z, r;                //current coordinates
	float theta;
	float nx, ny, nz, lengthInv;    // vertex normal


	const float dtheta = 2 * pi / ntheta;        //angular step size

	float t = 0;
	float dt = 1.0 / nt;
	float xy[2];

	for (i = 0; i <= nt; ++i)              //step through y
	{
		BezierCurve(t, xy, ctrlpoints, L);
		r = xy[0];
		y = xy[1];
		theta = 0;
		t += dt;
		lengthInv = 1.0 / r;

		for (j = 0; j <= ntheta; ++j)
		{
			double cosa = cos(theta);
			double sina = sin(theta);
			z = r * cosa;
			x = r * sina;

			coordinates.push_back(x);
			coordinates.push_back(y);
			coordinates.push_back(z);

			// normalized vertex normal (nx, ny, nz)
			// center point of the circle (0,y,0)
			nx = (x - 0) * lengthInv;
			ny = (y - y) * lengthInv;
			nz = (z - 0) * lengthInv;

			normals.push_back(nx);
			normals.push_back(ny);
			normals.push_back(nz);

			theta += dtheta;
		}
	}

	// generate index list of triangles
	// k1--k1+1
	// |  / |
	// | /  |
	// k2--k2+1

	int k1, k2;
	for (int i = 0; i < nt; ++i)
	{
		k1 = i * (ntheta + 1);     // beginning of current stack
		k2 = k1 + ntheta + 1;      // beginning of next stack

		for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
		{
			// k1 => k2 => k1+1
			indices.push_back(k1);
			indices.push_back(k2);
			indices.push_back(k1 + 1);

			// k1+1 => k2 => k2+1
			indices.push_back(k1 + 1);
			indices.push_back(k2);
			indices.push_back(k2 + 1);
		}
	}

	size_t count = coordinates.size();
	for (int i = 0; i < count; i += 3)
	{
		vertices.push_back(coordinates[i]);
		vertices.push_back(coordinates[i + 1]);
		vertices.push_back(coordinates[i + 2]);

		vertices.push_back(normals[i]);
		vertices.push_back(normals[i + 1]);
		vertices.push_back(normals[i + 2]);
	}

	unsigned int bezierVAO;
	glGenVertexArrays(1, &bezierVAO);
	glBindVertexArray(bezierVAO);

	// create VBO to copy vertex data to VBO
	unsigned int bezierVBO;
	glGenBuffers(1, &bezierVBO);
	glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
	glBufferData(GL_ARRAY_BUFFER,                   // target
		(unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
		vertices.data(),   // ptr to vertex data
		GL_STATIC_DRAW);                   // usage

	// create EBO to copy index data
	unsigned int bezierEBO;
	glGenBuffers(1, &bezierEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
		(unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
		indices.data(),               // ptr to index data
		GL_STATIC_DRAW);                   // usage

	// activate attrib arrays
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	// set attrib arrays with stride and offset
	int stride = 24;     // should be 24 bytes
	glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

	// unbind VAO, VBO and EBO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return bezierVAO;
}


// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		camera.EnableCamera = true;
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	}
//	else
//	{
//		camera.EnableCamera = false;
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//	}
//}

//// utility function for loading a 2D texture from file
//// ---------------------------------------------------
//unsigned int loadTexture(char const* path)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//
//	int width, height, nrComponents;
//	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
//	if (data)
//	{
//		GLenum format;
//		if (nrComponents == 1)
//			format = GL_RED;
//		else if (nrComponents == 3)
//			format = GL_RGB;
//		else if (nrComponents == 4)
//			format = GL_RGBA;
//
//		glBindTexture(GL_TEXTURE_2D, textureID);
//		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//		glGenerateMipmap(GL_TEXTURE_2D);
//
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//		stbi_image_free(data);
//	}
//	else
//	{
//		std::cout << "Texture failed to load at path: " << path << std::endl;
//		stbi_image_free(data);
//	}
//
//	return textureID;
//}
//
//// loads a cubemap texture from 6 individual texture faces
//// order:
//// +X (right)
//// -X (left)
//// +Y (top)
//// -Y (bottom)
//// +Z (front) 
//// -Z (back)
//// -------------------------------------------------------
//unsigned int loadCubemap(std::vector<std::string> faces)
//{
//	unsigned int textureID;
//	glGenTextures(1, &textureID);
//	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
//
//	int width, height, nrComponents;
//	for (unsigned int i = 0; i < faces.size(); i++)
//	{
//		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrComponents, 0);
//		if (data)
//		{
//			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//			stbi_image_free(data);
//		}
//		else
//		{
//			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
//			stbi_image_free(data);
//		}
//	}
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
//
//	return textureID;
//}





//#define STB_IMAGE_IMPLEMENTATION
//#define _CRT_SECURE_NO_WARNINGS
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <vector>
//#include <cstdlib>
//
////#include "Lights.h"
//#include "Shader.h"
//#include "Camera.h"
//#include "Floor.h"
//#include "Renderable.h"
//#include "stb_image.h"
//#include "DirLight.h"
//#include "SpotLight.h"
//#include "PointLight.h"
//#include <Windows.h>
//
//// settings
//const unsigned int SCR_WIDTH = 1300;
//const unsigned int SCR_HEIGHT = 980;
//const int FRAMES_PER_SECOND = 120;
//const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
//glm::vec3 CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);
//
//void framebufferSizeCallback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
//void mouseCallback(GLFWwindow* window, double xPos, double yPos);
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
//
//Camera camera(glm::vec3(0.0f, 0.1f, 3.0f));
//Shader* lightPosShader, * dShader;
////Shader ourShader;
//Floor* podloga_w;
//Renderable* object;
//std::vector<Renderable*> table;
////DirLight* lightDir;
////SpotLight* lightSpot;
//
//int fishNum = 10;
//int rockNum = 5;
//
//int objNum = fishNum + rockNum;
//
//
//bool enableF = 1, enableD = 1, enableS = 1;
//float KEY_PRESS = 0.0;
//
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//// timing
//float deltaTime = 0.0f;	// time between current frame and last frame
//float lastFrame = 0.0f;
//
//
//// Light initialization
//glm::vec3 lightPositions[] = {
//		glm::vec3(2.25f, 1.6f, -1.7f),
//		glm::vec3(-1.95f, 1.6f, -1.7f)
//};
//
//glm::vec3 lightDirections[] = {
//	glm::vec3(0.4f, -0.2f, -1.0f)
//};
//
//
//DirectionalLight directionalLight(-lightPositions[1], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1);
//PointLight pointLight(lightPositions[0], glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.9f, 0.9f, 0.9f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);
//SpotLight spotLight(lightPositions[1], lightDirections[0], 8.5f, 15.5f, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f), glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 0.09f, 0.032f, 1);
//
//
//bool lightingOn = false;
//float ambientOn = 1.0;
//float diffuseOn = 1.0;
//float specularOn = 1.0;
//bool dark = false;
//
//float directionalLightOn = 0.0;
//float pointLightOn = 1.0;
//float spotLightOn = 0.0;
//
//
//
//void initOpenGLProgram(GLFWwindow* window)
//{
//	glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, 1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
//	glfwSetCursorPosCallback(window, mouseCallback);
//	glfwSetScrollCallback(window, scrollCallback);
//	glfwSetMouseButtonCallback(window, mouseButtonCallback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // tell GLFW to capture our mouse
//}
//
//void freeOpenGLProgram(GLFWwindow* window)
//{
//	glfwDestroyWindow(window);
//	for (int i = 0; i < objNum; i++)
//	{
//		Renderable* r = table.back();
//		table.pop_back();
//		delete r;
//	}
//}
//
//Shader drawScene(GLFWwindow* window, Shader ourShader)
//{
//	
//    glClearColor(0.4f, 0.5f, 0.3f, 1.0f);
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	ourShader.use();
//	glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//	ourShader.setMat4("projection", projection);
//	glm::mat4 view = camera.GetViewMatrix();
//	ourShader.setMat4("view", view);
//
//
//	ourShader.setVec3("viewPos", camera.Position);
//	ourShader.setBool("lightingOn", lightingOn);
//	ourShader.setFloat("ambientOn", ambientOn);
//	ourShader.setFloat("diffuseOn", diffuseOn);
//	ourShader.setFloat("specularOn", specularOn);
//	ourShader.setFloat("pointLightOn", pointLightOn);
//	ourShader.setFloat("directionalLightOn", directionalLightOn);
//	ourShader.setFloat("spotLightOn", spotLightOn);
//	ourShader.setBool("dark", dark);
//
//
//	glm::mat4 model = glm::mat4(1.0f);
//	ourShader.setMat4("model", model);
//    ourShader.setVec3("FogColor", CLEAR_COLOR);
//	ourShader.setVec4("material.ambient", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
//	ourShader.setVec4("material.diffuse", glm::vec4(1.5f, 0.0f, 0.0f, 1.0f));
//	ourShader.setVec4("material.specular", glm::vec4(0.5f, 0.5f, 0.5f, 0.5f));
//	ourShader.setFloat("material.shininess", 32.0f);
//
//	dShader = &ourShader;
//	podloga_w->draw(dShader);
//
//    for (int i = 0; i < objNum; i++)
//    {
//    	object = table[i];
//    	object->behave();
//    	object->draw(dShader);
//    }
//    // creating a treasure 
//    //object = table[objNum];
//    //object->draw(dShader);
//    
//	return ourShader;
//
//	//dShader = &ourShader;
//	//glClearColor(0.4f, 0.5f, 0.3f, 1.0f);
//	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	//dShader->use();
//	//dShader->setVec3("viewPos", camera.getPosition());
//	//dShader->setFloat("material.shininess", 32.0f);
//
//	////lightDir->apply(dShader);
//	////lightSpot->apply(dShader);
//
//	//glm::mat4 model = glm::mat4(1.0f);
//	//glm::mat4 view = glm::mat4(1.0f);
//	//glm::mat4 projection = glm::mat4(1.0f);
//	//view = camera.GetViewMatrix();
//	//projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//	//dShader->setMat4("view", view);
//	//dShader->setMat4("projection", projection);
//	//dShader->setMat4("model", model);
//	//dShader->setVec3("FogColor", CLEAR_COLOR);
//
//	//dShader->setBool("enableFog", enableF);
//	//dShader->setBool("enableDirLight", enableD);
//	//dShader->setBool("enableSpotLight", enableS);
//
//	//podloga_w->draw(dShader);
//
//	//for (int i = 0; i < objNum; i++)
//	//{
//	//	object = table[i];
//	//	object->behave();
//	//	object->draw(dShader);
//	//}
//	//// creating a treasure 
//	////object = table[objNum];
//	////object->draw(dShader);
//	//glfwSwapBuffers(window);
//
//
//}
//
//int main()
//{
//	// glfw: initialize and configure
//	// ------------------------------
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	// glfw window creation
//	// --------------------
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Aquarium", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//	initOpenGLProgram(window);
//
//	float cube_vertices[] = {
//		0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
//		0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
//		0.5f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,
//		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, -1.0f,
//
//		0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
//		0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
//		0.5f, 0.0f, 0.5f, 1.0f, 0.0f, 0.0f,
//		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f,
//
//		0.0f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
//		0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f,
//		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//		0.0f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
//
//		0.0f, 0.0f, 0.5f, -1.0f, 0.0f, 0.0f,
//		0.0f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f,
//		0.0f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f,
//		0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
//
//		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
//		0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//		0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
//		0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
//
//		0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
//		0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
//		0.5f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f,
//		0.0f, 0.0f, 0.5f, 0.0f, -1.0f, 0.0f
//	};
//	unsigned int cube_indices[] = {
//		0, 3, 2,
//		2, 1, 0,
//
//		4, 5, 7,
//		7, 6, 4,
//
//		8, 9, 10,
//		10, 11, 8,
//
//		12, 13, 14,
//		14, 15, 12,
//
//		16, 17, 18,
//		18, 19, 16,
//
//		20, 21, 22,
//		22, 23, 20
//	};
//
//	unsigned int cubeVAO, cubeVBO, cubeEBO;
//	glGenVertexArrays(1, &cubeVAO);
//	glGenBuffers(1, &cubeVBO);
//	glGenBuffers(1, &cubeEBO);
//
//	glBindVertexArray(cubeVAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//	glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);
//
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);
//
//	// position attribute
//   // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
//	//glEnableVertexAttribArray(0);
//
//	// position attribute
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//	//vertex normal attribute
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
//	glEnableVertexAttribArray(1);
//
//	//light's VAO
//	unsigned int lightCubeVAO;
//	glGenVertexArrays(1, &lightCubeVAO);
//	glBindVertexArray(lightCubeVAO);
//
//	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//
//
//
//
//
//
//	Shader lightCubeShader("normal.vs", "normal.fs");
//	//Shader defaultShader("PhongShading.vs", "PhongShading.fs");
//	Shader ourShader("PhongShading.vs", "PhongShading.fs");
//	lightPosShader = &lightCubeShader;
//	//dShader = &defaultShader;
//	dShader = &ourShader;
//
//	Floor podloga;
//	podloga_w = &podloga;
//
//	//DirLight light123;
//	//lightDir = &light123;
//
//	//SpotLight light321;
//	//lightSpot = &light321;
//
//
//	for (int i = 0; i < fishNum; i++)
//	{
//		Fish* rryba = new Fish("random");
//		table.push_back(rryba);
//	}
//
//	for (int i = 0; i < rockNum; i++)
//	{
//		Rock* sskala = new Rock();
//		table.push_back(sskala);
//	}
//
//	//Other* treasure = new Other("treasure", glm::vec3(1.0f, 1.0f, 1.0f));
//	//table.push_back(treasure);
//
//	DWORD next_game_tick = GetTickCount64();
//	int sleep_time = 0;
//
//	while (!glfwWindowShouldClose(window))
//	{
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		processInput(window);
//
//		
//
//		// activate shader
//        //ourShader.use();
//
//        // pass projection matrix to shader (note that in this case it could change every frame)
//        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
//        //ourShader.setMat4("projection", projection);
//
//        // camera/view transformation
//        //glm::mat4 view = camera.GetViewMatrix();
//        //glm::mat4 view = basic_camera.createViewMatrix();
//        //ourShader.setMat4("view", view);
//
//        //// Modelling Transformation
//        //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        //glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//        //translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
//        //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
//        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
//        //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
//        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
//        //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//        //ourShader.setMat4("model", model);
//        ////ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
//
//
//
//        //// Modelling Transformation
//        //glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
//        //translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(1.0, translate_Y, translate_Z));
//        //rotateXMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
//        //rotateYMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
//        //rotateZMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
//        //scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.8, scale_Y, scale_Z));
//        //model2 = translateMatrix2 * rotateXMatrix2 * rotateYMatrix2 * rotateZMatrix2 * scaleMatrix2;
//        //ourShader.setMat4("model", model2);
//        ////ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
//
//		// activate shader
//		ourShader.use();
//
//		// pass projection matrix to shader (note that in this case it could change every frame)
//		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//		//glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
//		ourShader.setMat4("projection", projection);
//
//		////testing rotating around lookat point
//		//const float radius = 2.0f;
//		//float camX = sin(glfwGetTime()) * radius;
//		//float camZ = cos(glfwGetTime()) * radius;
//		//glm::mat4 view;
//		//view = glm::lookAt(glm::vec3(camX, 1.0, camZ), glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
//
//		// camera/view transformation
//		//glm::mat4 view = basic_camera.createViewMatrix();
//		glm::mat4 view = camera.GetViewMatrix();
//		ourShader.setMat4("view", view);
//
//
//		//**************lighting**************
//
//		/*glm::vec3 lightColor;
//		lightColor.x = sin(glfwGetTime() * 1.0f);
//		lightColor.y = sin(glfwGetTime() * 0.35f);
//		lightColor.z = sin(glfwGetTime() * 0.7f);
//		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
//		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
//		ourShader.setVec4("light.ambient", glm::vec4(ambientColor, 1.0f));
//		ourShader.setVec4("light.diffuse", glm::vec4(diffuseColor, 1.0f));*/
//
//
//		/*lightPositions[0].x = 1.0f + sin(glfwGetTime()) * 2.0f;
//		lightPositions[0].y = sin(glfwGetTime() / 2.0f) * 1.0f;*/
//
//		//Setting up Directional Light
//		directionalLight.setUpLight(ourShader);
//		if (!directionalLightOn)
//			directionalLight.turnOff();
//		if (!ambientOn)
//			directionalLight.turnAmbientOff();
//		if (!diffuseOn)
//			directionalLight.turnDiffuseOff();
//		if (!specularOn)
//			directionalLight.turnSpecularOff();
//
//		//Setting up Point Light
//		pointLight.setUpLight(ourShader);
//		if (!pointLightOn)
//			pointLight.turnOff();
//		if (!ambientOn)
//			pointLight.turnAmbientOff();
//		if (!diffuseOn)
//			pointLight.turnDiffuseOff();
//		if (!specularOn)
//			pointLight.turnSpecularOff();
//
//		//Setting up Spot Light
//		spotLight.setUpLight(ourShader);
//		if (!spotLightOn)
//			spotLight.turnOff();
//		if (!ambientOn)
//			spotLight.turnAmbientOff();
//		if (!diffuseOn)
//			spotLight.turnDiffuseOff();
//		if (!specularOn)
//			spotLight.turnSpecularOff();
//
//		//Setting up Camera and Others
//		//ourShader.setVec3("viewPos", camera.Position);
//		ourShader.setBool("lightingOn", lightingOn);
//		ourShader.setFloat("ambientOn", ambientOn);
//		ourShader.setFloat("diffuseOn", diffuseOn);
//		ourShader.setFloat("specularOn", specularOn);
//		ourShader.setFloat("pointLightOn", pointLightOn);
//		ourShader.setFloat("directionalLightOn", directionalLightOn);
//		ourShader.setFloat("spotLightOn", spotLightOn);
//		ourShader.setBool("dark", dark);
//
//		////For axis
//		//ourShader.use();
//		//glm::mat4 identity = glm::mat4(1.0f); // identity matrix
//		//ourShader.setMat4("model", identity);
//		//glBindVertexArray(axisVAO);
//		//glDrawArrays(GL_LINES, 0, 6);
//
//
//		glBindVertexArray(cubeVAO);
//
//
//        //glBindVertexArray(VAO);
//        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//
//        // render boxes
//        //for (unsigned int i = 0; i < 10; i++)
//        //{
//        //    // calculate the model matrix for each object and pass it to shader before drawing
//        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        //    model = glm::translate(model, cubePositions[i]);
//        //    float angle = 20.0f * i;
//        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//        //    ourShader.setMat4("model", model);
//
//        //    glDrawArrays(GL_TRIANGLES, 0, 36);
//        //}
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//
//		ourShader = drawScene(window, ourShader);
//
//		//Lights
//		for (int i = 0; i < 2; i++)
//		{
//			lightCubeShader.use();
//
//			/*glm::vec3 lightColor;
//			lightColor.x = sin(glfwGetTime() * 1.0f);
//			lightColor.y = sin(glfwGetTime() * 0.35f);
//			lightColor.z = sin(glfwGetTime() * 0.7f);
//			glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
//			lightCubeShader.setVec4("bodyColor", glm::vec4(diffuseColor, 1.0f));*/
//
//			glm::vec4 bodyColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
//
//			//emissive
//			glm::vec3 val = glm::vec3(0.5f);
//			if (i == 0 and pointLightOn == 0.0)
//				bodyColor = glm::vec4(val, 1.0f);
//			if (i == 1 and spotLightOn == 0.0)
//				bodyColor = glm::vec4(val, 1.0f);
//			if (dark)
//				bodyColor = glm::vec4(val, 1.0f);
//
//			lightCubeShader.setVec4("bodyColor", bodyColor);
//			glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//			lightCubeShader.setMat4("projection", projection);
//			glm::mat4 view = camera.GetViewMatrix();
//			lightCubeShader.setMat4("view", view);
//			glm::mat4 tempModel = glm::mat4(1.0f);
//			tempModel = glm::translate(tempModel, lightPositions[i]);
//			tempModel = glm::scale(tempModel, glm::vec3(0.5f));
//			lightCubeShader.setMat4("model", tempModel);
//
//			glBindVertexArray(lightCubeVAO);
//			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//		}
//
//
//		glfwSwapBuffers(window);
//		glfwPollEvents();
//
//		next_game_tick += SKIP_TICKS;
//		sleep_time = next_game_tick - GetTickCount64();
//		if (sleep_time >= 0)
//		{
//			Sleep(sleep_time);
//		}
//	}
//	freeOpenGLProgram(window);
//	glfwTerminate();
//	return 0;
//}
//
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetTime() - KEY_PRESS > 0.35)
//	{
//		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//			glfwSetWindowShouldClose(window, true);
//		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//			camera.ProcessKeyboard(FORWARD, deltaTime);
//		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//			camera.ProcessKeyboard(BACKWARD, deltaTime);
//		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//			camera.ProcessKeyboard(LEFT, deltaTime);
//		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//			camera.ProcessKeyboard(RIGHT, deltaTime);
//		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
//		{
//			enableF = !enableF; KEY_PRESS = glfwGetTime();
//		}
//		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
//		{
//			enableS = !enableS; KEY_PRESS = glfwGetTime();
//		}
//		if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
//		{
//			enableD = !enableD; KEY_PRESS = glfwGetTime();
//		}
//	}
//
//	//for lightning debug
//	/*if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
//		lightSpot->position.y += 0.1f;
//	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
//		lightSpot->position.y -= 0.1f;
//	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
//		lightSpot->cutOff += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
//		lightSpot->cutOff -= 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
//		lightSpot->outerCutOff += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
//		lightSpot->outerCutOff -= 0.01f;
//
//	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
//		lightDir->ambient.r += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
//		lightDir->ambient.r -= 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
//		lightDir->ambient.g += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
//		lightDir->ambient.g -= 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
//		lightDir->ambient.b += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
//		lightDir->ambient.b -= 0.01f;*/
//
//
//		//***************Lighting***************
//
//	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)                   //Lighting On-Off
//	{
//		lightingOn ^= true;
//		std::cout << lightingOn << std::endl;
//		Sleep(100);
//	}
//	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)                   //Ambient On
//	{
//		ambientOn = 1.0;
//		pointLight.turnAmbientOn();
//		spotLight.turnAmbientOn();
//		directionalLight.turnAmbientOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)                   //Ambient Off
//	{
//		ambientOn = 0.0;
//		pointLight.turnAmbientOff();
//		spotLight.turnAmbientOff();
//		directionalLight.turnAmbientOff();
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)                   //Diffuse On
//	{
//		diffuseOn = 1.0;
//		pointLight.turnDiffuseOn();
//		spotLight.turnDiffuseOn();
//		directionalLight.turnDiffuseOn();
//
//	}
//	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)                   //Diffuse Off
//	{
//		diffuseOn = 0.0;
//		pointLight.turnDiffuseOff();
//		spotLight.turnDiffuseOff();
//		directionalLight.turnDiffuseOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)                   //Specular On
//	{
//		specularOn = 1.0;
//		pointLight.turnSpecularOn();
//		spotLight.turnSpecularOn();
//		directionalLight.turnSpecularOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)                   //Specular Off
//	{
//		specularOn = 0.0;
//		pointLight.turnSpecularOff();
//		spotLight.turnSpecularOff();
//		directionalLight.turnSpecularOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)                   //Point Light On
//	{
//		pointLightOn = 1.0;
//		pointLight.turnOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)                   //Point Light Off
//	{
//		pointLightOn = 0.0;
//		pointLight.turnOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)                   //Directional Light On
//	{
//		directionalLightOn = 1.0;
//		directionalLight.turnOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)                   //Directional Light Off
//	{
//		directionalLightOn = 0.0;
//		directionalLight.turnOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)                   //Spot Light On
//	{
//		spotLightOn = 1.0;
//		spotLight.turnOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)                   //Spot Light Off
//	{
//		spotLightOn = 0.0;
//		spotLight.turnOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS)                   //Dark On-Off
//	{
//		dark ^= true;
//		std::cout << dark << std::endl;
//		Sleep(100);
//	}
//
//
//
//
//
//
//
//
//
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebufferSizeCallback(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}
//
//
//
////// glfw: whenever the mouse moves, this callback is called
////// -------------------------------------------------------
////void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
////{
////	float xpos = static_cast<float>(xposIn);
////	float ypos = static_cast<float>(yposIn);
////
////	if (firstMouse)
////	{
////		lastX = xpos;
////		lastY = ypos;
////		firstMouse = false;
////	}
////
////	float xoffset = xpos - lastX;
////	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
////
////	lastX = xpos;
////	lastY = ypos;
////
////	camera.ProcessMouseMovement(xoffset, yoffset);
////}
//
//void mouseCallback(GLFWwindow* window, double xPos, double yPos)
//{
//	if (firstMouse)
//	{
//		lastX = xPos;
//		lastY = yPos;
//		firstMouse = false;
//	}
//
//	float xoffset = xPos - lastX;
//	float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xPos;
//	lastY = yPos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
//{
//	camera.ProcessMouseScroll(yOffset);
//}
//
//
//
//
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		camera.EnableCamera = true;
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	}
//	else
//	{
//		camera.EnableCamera = false;
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//	}
//}




//#define STB_IMAGE_IMPLEMENTATION
//#define _CRT_SECURE_NO_WARNINGS
//#include <glad/glad.h>
//#include <GLFW/glfw3.h>
//#include <glm/glm.hpp>
//#include <glm/gtc/matrix_transform.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <iostream>
//#include <vector>
//#include <cstdlib>
//
//#include "Lights.h"
//#include "Shader.h"
//#include "Camera.h"
//#include "Floor.h"
//#include "Renderable.h"
//#include "stb_image.h"
//#include <Windows.h>
//
//// settings
//const unsigned int SCR_WIDTH = 1300;
//const unsigned int SCR_HEIGHT = 980;
//const int FRAMES_PER_SECOND = 120;
//const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
//glm::vec3 CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);
//
//void framebufferSizeCallback(GLFWwindow* window, int width, int height);
//void processInput(GLFWwindow* window);
//void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
//void mouseCallback(GLFWwindow* window, double xPos, double yPos);
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
//
//Camera camera(glm::vec3(0.0f, 0.1f, 3.0f));
//Shader* lightPosShader, * dShader;
//Floor* podloga_w;
//Renderable* object;
//std::vector<Renderable*> table;
//DirLight* lightDir;
//SpotLight* lightSpot;
//
//int fishNum = 10;
//int rockNum = 5;
//
//int objNum = fishNum + rockNum;
//
//
//bool enableF = 1, enableD = 1, enableS = 1;
//float KEY_PRESS = 0.0;
//
//float lastX = SCR_WIDTH / 2.0f;
//float lastY = SCR_HEIGHT / 2.0f;
//bool firstMouse = true;
//// timing
//float deltaTime = 0.0f;	// time between current frame and last frame
//float lastFrame = 0.0f;
//
//
//
//void initOpenGLProgram(GLFWwindow* window)
//{
//	glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, 1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
//	glfwSetCursorPosCallback(window, mouseCallback);
//	glfwSetScrollCallback(window, scrollCallback);
//	glfwSetMouseButtonCallback(window, mouseButtonCallback);
//	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // tell GLFW to capture our mouse
//}
//
//void freeOpenGLProgram(GLFWwindow* window)
//{
//	glfwDestroyWindow(window);
//	for (int i = 0; i < objNum; i++)
//	{
//		Renderable* r = table.back();
//		table.pop_back();
//		delete r;
//	}
//}
//
//void drawScene(GLFWwindow* window)
//{
//	glClearColor(0.4f, 0.5f, 0.3f, 1.0f);
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//	dShader->use();
//	dShader->setVec3("viewPos", camera.getPosition());
//	dShader->setFloat("material.shininess", 32.0f);
//
//	lightDir->apply(dShader);
//	lightSpot->apply(dShader);
//
//	glm::mat4 model = glm::mat4(1.0f);
//	glm::mat4 view = glm::mat4(1.0f);
//	glm::mat4 projection = glm::mat4(1.0f);
//	view = camera.GetViewMatrix();
//	projection = glm::perspective(glm::radians(camera.getZoom()), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//
//	dShader->setMat4("view", view);
//	dShader->setMat4("projection", projection);
//	dShader->setMat4("model", model);
//	dShader->setVec3("FogColor", CLEAR_COLOR);
//
//	dShader->setBool("enableFog", enableF);
//	dShader->setBool("enableDirLight", enableD);
//	dShader->setBool("enableSpotLight", enableS);
//
//	podloga_w->draw(dShader);
//
//	for (int i = 0; i < objNum; i++)
//	{
//		object = table[i];
//		object->behave();
//		object->draw(dShader);
//	}
//	// creating a treasure 
//	//object = table[objNum];
//	//object->draw(dShader);
//	glfwSwapBuffers(window);
//}
//
//int main()
//{
//	// glfw: initialize and configure
//	// ------------------------------
//	glfwInit();
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//
//	// glfw window creation
//	// --------------------
//	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Aquarium", NULL, NULL);
//	if (window == NULL)
//	{
//		std::cout << "Failed to create GLFW window" << std::endl;
//		glfwTerminate();
//		return -1;
//	}
//	glfwMakeContextCurrent(window);
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	{
//		std::cout << "Failed to initialize GLAD" << std::endl;
//		return -1;
//	}
//	initOpenGLProgram(window);
//
//	Shader lightShader("normal.vs", "normal.fs");
//	Shader defaultShader("PhongShading.vs", "PhongShading.fs");
//	lightPosShader = &lightShader;
//	dShader = &defaultShader;
//
//	Floor podloga;
//	podloga_w = &podloga;
//
//	DirLight light123;
//	lightDir = &light123;
//
//	SpotLight light321;
//	lightSpot = &light321;
//
//
//	for (int i = 0; i < fishNum; i++)
//	{
//		Fish* rryba = new Fish("random");
//		table.push_back(rryba);
//	}
//
//	for (int i = 0; i < rockNum; i++)
//	{
//		Rock* sskala = new Rock();
//		table.push_back(sskala);
//	}
//
//	//Other* treasure = new Other("treasure", glm::vec3(1.0f, 1.0f, 1.0f));
//	//table.push_back(treasure);
//
//	DWORD next_game_tick = GetTickCount();
//	int sleep_time = 0;
//
//	while (!glfwWindowShouldClose(window))
//	{
//		float currentFrame = glfwGetTime();
//		deltaTime = currentFrame - lastFrame;
//		lastFrame = currentFrame;
//
//		processInput(window);
//
//		drawScene(window);
//
//		// activate shader
//        //ourShader.use();
//
//        // pass projection matrix to shader (note that in this case it could change every frame)
//        //glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
//        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
//        //ourShader.setMat4("projection", projection);
//
//        // camera/view transformation
//        //glm::mat4 view = camera.GetViewMatrix();
//        //glm::mat4 view = basic_camera.createViewMatrix();
//        //ourShader.setMat4("view", view);
//
//        //// Modelling Transformation
//        //glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        //glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
//        //translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
//        //rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
//        //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
//        //rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
//        //scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
//        //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
//        //ourShader.setMat4("model", model);
//        ////ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
//
//
//
//        //// Modelling Transformation
//        //glm::mat4 identityMatrix2 = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        //glm::mat4 translateMatrix2, rotateXMatrix2, rotateYMatrix2, rotateZMatrix2, scaleMatrix2, model2;
//        //translateMatrix2 = glm::translate(identityMatrix2, glm::vec3(1.0, translate_Y, translate_Z));
//        //rotateXMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
//        //rotateYMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
//        //rotateZMatrix2 = glm::rotate(identityMatrix2, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
//        //scaleMatrix2 = glm::scale(identityMatrix2, glm::vec3(0.8, scale_Y, scale_Z));
//        //model2 = translateMatrix2 * rotateXMatrix2 * rotateYMatrix2 * rotateZMatrix2 * scaleMatrix2;
//        //ourShader.setMat4("model", model2);
//        ////ourShader.setVec3("aColor", glm::vec3(0.2f, 0.1f, 0.4f));
//
//
//
//
//        //glBindVertexArray(VAO);
//        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
//
//
//        // render boxes
//        //for (unsigned int i = 0; i < 10; i++)
//        //{
//        //    // calculate the model matrix for each object and pass it to shader before drawing
//        //    glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
//        //    model = glm::translate(model, cubePositions[i]);
//        //    float angle = 20.0f * i;
//        //    model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//        //    ourShader.setMat4("model", model);
//
//        //    glDrawArrays(GL_TRIANGLES, 0, 36);
//        //}
//
//        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
//        // -------------------------------------------------------------------------------
//
//		glfwPollEvents();
//
//		next_game_tick += SKIP_TICKS;
//		sleep_time = next_game_tick - GetTickCount();
//		if (sleep_time >= 0)
//		{
//			Sleep(sleep_time);
//		}
//	}
//	freeOpenGLProgram(window);
//	glfwTerminate();
//	return 0;
//}
//
//void processInput(GLFWwindow* window)
//{
//	if (glfwGetTime() - KEY_PRESS > 0.35)
//	{
//		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
//			glfwSetWindowShouldClose(window, true);
//		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
//			camera.ProcessKeyboard(FORWARD, deltaTime);
//		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
//			camera.ProcessKeyboard(BACKWARD, deltaTime);
//		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
//			camera.ProcessKeyboard(LEFT, deltaTime);
//		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
//			camera.ProcessKeyboard(RIGHT, deltaTime);
//		if (glfwGetKey(window, GLFW_KEY_F1) == GLFW_PRESS)
//		{
//			enableF = !enableF; KEY_PRESS = glfwGetTime();
//		}
//		if (glfwGetKey(window, GLFW_KEY_F2) == GLFW_PRESS)
//		{
//			enableS = !enableS; KEY_PRESS = glfwGetTime();
//		}
//		if (glfwGetKey(window, GLFW_KEY_F3) == GLFW_PRESS)
//		{
//			enableD = !enableD; KEY_PRESS = glfwGetTime();
//		}
//	}
//
//	//for lightning debug
//	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
//		lightSpot->position.y += 0.1f;
//	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
//		lightSpot->position.y -= 0.1f;
//	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
//		lightSpot->cutOff += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
//		lightSpot->cutOff -= 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
//		lightSpot->outerCutOff += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
//		lightSpot->outerCutOff -= 0.01f;
//
//	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
//		lightDir->ambient.r += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
//		lightDir->ambient.r -= 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
//		lightDir->ambient.g += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
//		lightDir->ambient.g -= 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
//		lightDir->ambient.b += 0.01f;
//	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
//		lightDir->ambient.b -= 0.01f;
//
//
//
//}
//
//// glfw: whenever the window size changed (by OS or user resize) this callback function executes
//// ---------------------------------------------------------------------------------------------
//void framebufferSizeCallback(GLFWwindow* window, int width, int height)
//{
//	// make sure the viewport matches the new window dimensions; note that width and
//	// height will be significantly larger than specified on retina displays.
//	glViewport(0, 0, width, height);
//}
//
//
//
////// glfw: whenever the mouse moves, this callback is called
////// -------------------------------------------------------
////void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
////{
////	float xpos = static_cast<float>(xposIn);
////	float ypos = static_cast<float>(yposIn);
////
////	if (firstMouse)
////	{
////		lastX = xpos;
////		lastY = ypos;
////		firstMouse = false;
////	}
////
////	float xoffset = xpos - lastX;
////	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top
////
////	lastX = xpos;
////	lastY = ypos;
////
////	camera.ProcessMouseMovement(xoffset, yoffset);
////}
//
//void mouseCallback(GLFWwindow* window, double xPos, double yPos)
//{
//	if (firstMouse)
//	{
//		lastX = xPos;
//		lastY = yPos;
//		firstMouse = false;
//	}
//
//	float xoffset = xPos - lastX;
//	float yoffset = lastY - yPos; // reversed since y-coordinates go from bottom to top
//
//	lastX = xPos;
//	lastY = yPos;
//
//	camera.ProcessMouseMovement(xoffset, yoffset);
//}
//
//void scrollCallback(GLFWwindow* window, double xOffset, double yOffset)
//{
//	camera.ProcessMouseScroll(yOffset);
//}
//
//
//
//
//
//// glfw: whenever the mouse scroll wheel scrolls, this callback is called
//// ----------------------------------------------------------------------
//void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
//{
//	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
//	{
//		camera.EnableCamera = true;
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//	}
//	else
//	{
//		camera.EnableCamera = false;
//		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
//	}
//}






