#include "Lights.h"





DirLight::DirLight()
{
	ambient = glm::vec3(31.0f, 181.0f, 198.0f) * glm::vec3(1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f);
	diffuse = glm::vec3(13.0f, 56.0f, 71.0f) * glm::vec3(1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f);
	specular = glm::vec3(48.0f, 88.0f, 102.0f) * glm::vec3(1.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f);

	direction = glm::vec3(0.0f, -10.0f, 0.2f);
}

DirLight::~DirLight()
{
	//
}

void DirLight::apply(Shader* sp)
{
	sp->setVec3("dirLight.direction", direction);
	sp->setVec3("dirLight.ambient", ambient * ambientOn* isOn);
	sp->setVec3("dirLight.diffuse", diffuse * diffuseOn * isOn);
	sp->setVec3("dirLight.specular", specular * specularOn* isOn);



}

//
//

SpotLight::SpotLight()
{
	position = glm::vec3(0.0f, 10.0f, 0.0f);
	direction = glm::vec3(0.0f, -1.0f, 0.0f);

	cutOff = glm::cos(glm::radians(10.5f));
	outerCutOff = glm::cos(glm::radians(12.0f));

	ambient = glm::vec3(0.2f, 0.2f, 0.2f);
	diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
	specular = glm::vec3(1.0f, 1.0f, 1.0f);

	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032;
}

SpotLight::~SpotLight()
{
	//
}

void SpotLight::apply(Shader* sp)
{
	sp->setVec3("spotLight.position", position);
	sp->setVec3("spotLight.direction", direction);
	sp->setVec3("spotLight.ambient", ambient * ambientOn* isOn);
	sp->setVec3("spotLight.diffuse", diffuse * diffuseOn* isOn);
	sp->setVec3("spotLight.specular", specular * specularOn * isOn);

	sp->setFloat("spotLight.cutOff", cutOff);
	sp->setFloat("spotLight.outerCutOff", outerCutOff);
	sp->setFloat("spotLight.constant", constant);
	sp->setFloat("spotLight.linear", linear);
	sp->setFloat("spotLight.quadratic", quadratic);
}

PointLight::PointLight() {
	position = glm::vec3(0.0f, 10.0f, 0.0f);
	ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	diffuse = glm::vec3(0.9f, 0.9f, 0.9f);
	specular = glm::vec3(1.0f, 1.0f, 1.0f);
	constant = 1.0f;
	linear = 0.09f;
	quadratic = 0.032f;
}
void PointLight::apply(Shader* sp) {
	sp->setVec3("pointLight.ambient", ambient * ambientOn * isOn);
	sp->setVec3("pointLight.diffuse", diffuse * diffuseOn * isOn);
	sp->setVec3("pointLight.specular", specular * specularOn * isOn);
	sp->setVec3("pointLight.position", position);
	sp->setFloat("pointLight.constant", constant);
	sp->setFloat("pointLight.linear", linear);
	sp->setFloat("pointLight.quadratic", quadratic);
}
PointLight::~PointLight() {
	//
}









//PointLight::PointLight(float ambR, float ambG, float ambB, float diffR, float diffG, float diffB, float specR, float specG, float specB, float constant, float linear, float quadratic) {
//
//    ambient = glm::vec3(ambR, ambG, ambB);
//    diffuse = glm::vec3(diffR, diffG, diffB);
//    specular = glm::vec3(specR, specG, specB);
//    k_c = constant;
//    k_l = linear;
//    k_q = quadratic;
//    lightNumber = num;
//}
//void setUpPointLight(Shader& lightingShader)
//{
//    lightingShader.use();
//
//    if (lightNumber == 1) {
//        lightingShader.setVec3("pointLights[0].position", position);
//        lightingShader.setVec3("pointLights[0].ambient", ambientOn * ambient);
//        lightingShader.setVec3("pointLights[0].diffuse", diffuseOn * diffuse);
//        lightingShader.setVec3("pointLights[0].specular", specularOn * specular);
//        lightingShader.setFloat("pointLights[0].k_c", k_c);
//        lightingShader.setFloat("pointLights[0].k_l", k_l);
//        lightingShader.setFloat("pointLights[0].k_q", k_q);
//    }
//
//    else if (lightNumber == 2)
//    {
//        lightingShader.setVec3("pointLights[1].position", position);
//        lightingShader.setVec3("pointLights[1].ambient", ambientOn * ambient);
//        lightingShader.setVec3("pointLights[1].diffuse", diffuseOn * diffuse);
//        lightingShader.setVec3("pointLights[1].specular", specularOn * specular);
//        lightingShader.setFloat("pointLights[1].k_c", k_c);
//        lightingShader.setFloat("pointLights[1].k_l", k_l);
//        lightingShader.setFloat("pointLights[1].k_q", k_q);
//    }
//    else if (lightNumber == 3)
//    {
//        lightingShader.setVec3("pointLights[2].position", position);
//        lightingShader.setVec3("pointLights[2].ambient", ambientOn * ambient);
//        lightingShader.setVec3("pointLights[2].diffuse", diffuseOn * diffuse);
//        lightingShader.setVec3("pointLights[2].specular", specularOn * specular);
//        lightingShader.setFloat("pointLights[2].k_c", k_c);
//        lightingShader.setFloat("pointLights[2].k_l", k_l);
//        lightingShader.setFloat("pointLights[2].k_q", k_q);
//    }
//    else
//    {
//        lightingShader.setVec3("pointLights[3].position", position);
//        lightingShader.setVec3("pointLights[3].ambient", ambientOn * ambient);
//        lightingShader.setVec3("pointLights[3].diffuse", diffuseOn * diffuse);
//        lightingShader.setVec3("pointLights[3].specular", specularOn * specular);
//        lightingShader.setFloat("pointLights[3].k_c", k_c);
//        lightingShader.setFloat("pointLights[3].k_l", k_l);
//        lightingShader.setFloat("pointLights[3].k_q", k_q);
//    }
//}
//
//
//PointLight::~PointLight() {}
//
//void PointLight::apply(Shader* sp) {
//
//
//
//}