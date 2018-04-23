#version 330
 
// the following attributes are supported by this engine
// Mesh::render searches for, and binds them, if they exist
in vec3 a_vertex;
in vec2 a_uv;


uniform mat4 u_mvp;

//Varying (de un shader a otro)
out vec2 v_uv;

void main()
{
	v_uv = a_uv;

	//calculate the position of the vertex using the matrices
	gl_Position = u_mvp * vec4( a_vertex, 1.0 );

}



