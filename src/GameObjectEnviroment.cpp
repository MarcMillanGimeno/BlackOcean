#include "GameObjectEnviroment.h"
#include "managers.h"

void GameObjectEnviroment::render() {
	Camera* camera = g_scene.camera;
	glDisable(GL_DEPTH_TEST);//tell openGL to ignore depth test
	glCullFace(GL_FRONT);
	this->model.setTranslation(camera->eye.x, camera->eye.y, camera->eye.z);
	//build a rotation matrix - angle is incremented in update()
	Matrix44 mvp = this->getGlobalMatrix() * camera->viewprojection_matrix;
	//enable shader and set uniforms
	this->shader->enable();
	this->shader->setMatrix44("u_mvp", mvp);
	this->shader->setTexture("u_texture_diffuse", this->texture);

	//call render function for mesh, passing shader as param
	//mesh->render() binds the relevant attributes, and drawing code
	mesh->render(GL_TRIANGLES, shader);
	

	//disable the shader
	this->shader->disable();
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST); //re-enable depth testing
}
void GameObjectEnviroment::update(float dt) {}
