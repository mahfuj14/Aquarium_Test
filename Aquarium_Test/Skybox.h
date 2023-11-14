#ifndef SKYBOX_H
#define SKYBOX_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Renderable.h"
#include <iostream>
#include <vector>

class Skybox {
public:
    unsigned int cubeVAO, cubeVBO;
    unsigned int skyboxVAO, skyboxVBO;
    const unsigned int SCR_WIDTH = 1300;
    const unsigned int SCR_HEIGHT = 980;
    std::string parentDir = "F:/4.2/CG LAB/Lab5/Reflection_refraction/Reflection_refraction/";
    std::vector<std::string> faces
    {
        parentDir + "resources/textures/skybox/right.jpg",
        parentDir + "resources/textures/skybox/left.jpg",
        parentDir + "resources/textures/skybox/top.jpg",
        parentDir + "resources/textures/skybox/bottom.jpg",
        parentDir + "resources/textures/skybox/front.jpg",
        parentDir + "resources/textures/skybox/back.jpg"
    };
	Skybox();
    ~Skybox();
    //unsigned int cubemapTexture = loadCubemap(faces);
    unsigned int cubemapTexture;
    void draw(Shader* shader, Shader* skyboxShader);	//draw skybox in loop function
};
#endif