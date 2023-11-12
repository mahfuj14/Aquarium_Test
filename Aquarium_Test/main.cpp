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
//	////for lightning debug
//	//if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
//	//	lightSpot->position.y += 0.1f;
//	//if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
//	//	lightSpot->position.y -= 0.1f;
//	//if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
//	//	lightSpot->cutOff += 0.01f;
//	//if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
//	//	lightSpot->cutOff -= 0.01f;
//	//if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
//	//	lightSpot->outerCutOff += 0.01f;
//	//if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
//	//	lightSpot->outerCutOff -= 0.01f;
//
//	//if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
//	//	lightDir->ambient.r += 0.01f;
//	//if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
//	//	lightDir->ambient.r -= 0.01f;
//	//if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
//	//	lightDir->ambient.g += 0.01f;
//	//if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
//	//	lightDir->ambient.g -= 0.01f;
//	//if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
//	//	lightDir->ambient.b += 0.01f;
//	//if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
//	//	lightDir->ambient.b -= 0.01f;
//
//
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
//		//pointLight->turnAmbientOn();
//		lightDir->turnAmbientOn();
//		lightSpot->turnAmbientOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)                   //Ambient Off
//	{
//		ambientOn = 0.0;
//		//pointLight.turnAmbientOff();
//		lightSpot->turnAmbientOff();
//		lightDir->turnAmbientOff();
//	}
//
//	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS)                   //Diffuse On
//	{
//		diffuseOn = 1.0;
//		//pointLight.turnDiffuseOn();
//		lightSpot->turnDiffuseOn();
//		lightDir->turnDiffuseOn();
//
//	}
//	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS)                   //Diffuse Off
//	{
//		diffuseOn = 0.0;
//		//pointLight.turnDiffuseOff();
//		lightSpot->turnDiffuseOff();
//		lightDir->turnDiffuseOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS)                   //Specular On
//	{
//		specularOn = 1.0;
//		//pointLight.turnSpecularOn();
//		lightSpot->turnSpecularOn();
//		lightDir->turnSpecularOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_6) == GLFW_PRESS)                   //Specular Off
//	{
//		specularOn = 0.0;
//		//pointLight.turnSpecularOff();
//		lightSpot->turnSpecularOff();
//		lightDir->turnSpecularOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)                   //Point Light On
//	{
//		pointLightOn = 1.0;
//		//pointLight.turnOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)                   //Point Light Off
//	{
//		pointLightOn = 0.0;
//		//pointLight.turnOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)                   //Directional Light On
//	{
//		directionalLightOn = 1.0;
//		lightDir->turnOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)                   //Directional Light Off
//	{
//		directionalLightOn = 0.0;
//		lightDir->turnOff();
//	}
//	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)                   //Spot Light On
//	{
//		spotLightOn = 1.0;
//		lightSpot->turnOn();
//	}
//	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)                   //Spot Light Off
//	{
//		spotLightOn = 0.0;
//		lightSpot->turnOff();
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




#define STB_IMAGE_IMPLEMENTATION
#define _CRT_SECURE_NO_WARNINGS
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
#include <Windows.h>

// settings
const unsigned int SCR_WIDTH = 1300;
const unsigned int SCR_HEIGHT = 980;
const int FRAMES_PER_SECOND = 120;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;
glm::vec3 CLEAR_COLOR = glm::vec3(0.2f, 0.3f, 0.3f);

void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

Camera camera(glm::vec3(0.0f, 0.1f, 3.0f));
Shader* lightPosShader, * dShader;
Floor* podloga_w;
Renderable* object;
std::vector<Renderable*> table;
DirLight* lightDir;
SpotLight* lightSpot;

int fishNum = 10;
int rockNum = 5;

int objNum = fishNum + rockNum;


bool enableF = 1, enableD = 1, enableS = 1;
float KEY_PRESS = 0.0;

float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



void initOpenGLProgram(GLFWwindow* window)
{
	glClearColor(CLEAR_COLOR.x, CLEAR_COLOR.y, CLEAR_COLOR.z, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetScrollCallback(window, scrollCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // tell GLFW to capture our mouse
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

	podloga_w->draw(dShader);

	for (int i = 0; i < objNum; i++)
	{
		object = table[i];
		object->behave();
		object->draw(dShader);
	}
	// creating a treasure 
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

	DirLight light123;
	lightDir = &light123;

	SpotLight light321;
	lightSpot = &light321;


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

	DWORD next_game_tick = GetTickCount();
	int sleep_time = 0;

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

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

		glfwPollEvents();

		next_game_tick += SKIP_TICKS;
		sleep_time = next_game_tick - GetTickCount();
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

	//for lightning debug
	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		lightSpot->position.y += 0.1f;
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		lightSpot->position.y -= 0.1f;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		lightSpot->cutOff += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		lightSpot->cutOff -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		lightSpot->outerCutOff += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		lightSpot->outerCutOff -= 0.01f;

	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
		lightDir->ambient.r += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
		lightDir->ambient.r -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
		lightDir->ambient.g += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
		lightDir->ambient.g -= 0.01f;
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		lightDir->ambient.b += 0.01f;
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
		lightDir->ambient.b -= 0.01f;



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





// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		camera.EnableCamera = true;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
	else
	{
		camera.EnableCamera = false;
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}
}






