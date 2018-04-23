#include "managers.h"
#include "shader.h"

using namespace std;

AABB::AABB() {
	mesh_box = new Mesh();
	min = Vector3(0.0, 0.0,0.0 );
	max = Vector3(0.0, 0.0, 0.0);
	center = Vector3(0.0, 0.0, 0.0);
}

void AABB::setCenter() {
	center = Vector3((min.x + max.x)/2, (min.y + max.y) / 2, (min.z + max.z) / 2);
}

void AABB::collisionBox() {
	mesh_box->vertices.clear();
	mesh_box->normals.clear();
	mesh_box->uvs.clear();
	mesh_box->colors.clear();
	mesh_box->indices.clear();

	mesh_box->vertices.push_back(Vector3(min.x, max.y, min.z));
	mesh_box->vertices.push_back(Vector3(max.x, max.y, min.z));
	mesh_box->vertices.push_back(Vector3(min.x, min.y, min.z));
	mesh_box->vertices.push_back(Vector3(max.x, min.y, min.z));
	mesh_box->vertices.push_back(Vector3(min.x, max.y, max.z));
	mesh_box->vertices.push_back(Vector3(max.x, max.y, max.z));
	mesh_box->vertices.push_back(Vector3(min.x, min.y, max.z));
	mesh_box->vertices.push_back(Vector3(max.x, min.y, max.z));

	mesh_box->indices.push_back(0);
	mesh_box->indices.push_back(2);
	mesh_box->indices.push_back(1);
	mesh_box->indices.push_back(1);
	mesh_box->indices.push_back(3);
	mesh_box->indices.push_back(2);

	mesh_box->indices.push_back(0);
	mesh_box->indices.push_back(4);
	mesh_box->indices.push_back(5);
	mesh_box->indices.push_back(0);
	mesh_box->indices.push_back(5);
	mesh_box->indices.push_back(1);

	mesh_box->indices.push_back(1);
	mesh_box->indices.push_back(5);
	mesh_box->indices.push_back(3);
	mesh_box->indices.push_back(3);
	mesh_box->indices.push_back(5);
	mesh_box->indices.push_back(7);

	mesh_box->indices.push_back(2);
	mesh_box->indices.push_back(3);
	mesh_box->indices.push_back(7);
	mesh_box->indices.push_back(2);
	mesh_box->indices.push_back(7);
	mesh_box->indices.push_back(6);

	mesh_box->indices.push_back(0);
	mesh_box->indices.push_back(4);
	mesh_box->indices.push_back(2);
	mesh_box->indices.push_back(4);
	mesh_box->indices.push_back(2);
	mesh_box->indices.push_back(6);

	mesh_box->indices.push_back(4);
	mesh_box->indices.push_back(5);
	mesh_box->indices.push_back(0);
	mesh_box->indices.push_back(5);
	mesh_box->indices.push_back(0);
	mesh_box->indices.push_back(7);

	mesh_box->genBuffers();
}

void AABB::drawCollisionModel() {
	Camera* camera = g_scene.camera;
	
	Matrix44 model;

	model.setScale(1.0f, 1.0f, 1.0f);

	Matrix44 mvp = model * camera->viewprojection_matrix;
	
	Shader *shader = new Shader();

	string vert = "data/shaders/simple_bullet.vert";
	string frag = "data/shaders/simple_bullet.frag";

	if (!shader->load(vert, frag))
	{
		//if your program quits straight after opening, the shader probably hasn't compiled,
		//if you put a breakpoint here you'll be able to read the compilation error in the console
		exit(0);
	}

	shader->enable();
	shader->setMatrix44("u_mvp", mvp);

	mesh_box->render(GL_TRIANGLES, shader);
}