#include "shapebuffer.h"
#include "shader.h"
#include "matrix.h"
#include "epic_space_game.h"
#include "engine.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

#include <iostream>

ShapeBuffer::ShapeBuffer (GLuint vbo, GLuint ibo)
: vbo(vbo)
, ibo(ibo)
{
}

ShapeBuffer::ShapeBuffer (double width, double height) {
	
	double vertices[] = {
		-width/2, -height/2, 0.0,
		width/2, -height/2, 0.0,
		width/2, height/2, 0.0,
		-width/2, height/2, 0.0,
	};
	
	int indices[] = {
		0, 1, 2, 0, 2, 3
	};
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void ShapeBuffer::render (GLuint shader, double x, double y, double z, double angle) {
	
	Engine::addqps(1);
	
	Shader::use(shader);
	
	glm::mat4 matrix = glm::mat4();
	matrix *= glm::translate(glm::mat4(), glm::vec3(x, y, z));
	if(angle != 0) matrix *= glm::rotate(glm::mat4(), (float)angle, glm::vec3(0, 0, 1));
	
	GLuint u_modelMatrix = Shader::uniformLocation(shader, "u_modelMatrix");
	glUniformMatrix4fv(u_modelMatrix, 1, GL_FALSE, glm::value_ptr(matrix * Matrix::modelMatrix));
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_DOUBLE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

// Lua
int ShapeBuffer::lua_shapeBuffer (lua_State* l) {
	
	ShapeBuffer buffer = ShapeBuffer((double)lua_tonumber(l, 1), (double)lua_tonumber(l, 2));
	
	lua_createtable(l, 0, 2);
	lua_pushnumber(l, buffer.vbo);
	lua_setfield(l, -2, "vbo");
	lua_pushnumber(l, buffer.ibo);
	lua_setfield(l, -2, "ibo");
	
	return 1;
}

int ShapeBuffer::lua_renderShapeBuffer (lua_State* l) {
	
	int top = lua_gettop(l);
	luaL_checktype(l, 2, LUA_TNUMBER);
	GLuint shader = lua_tonumber(l, 2);
	double x = lua_tonumber(l, 3);
	double y = lua_tonumber(l, 4);
	double z = lua_tonumber(l, 5);
	double angle = lua_tonumber(l, 6);
	
	luaL_checktype(l, 1, LUA_TTABLE);
	lua_getfield(l, 1, "vbo");
	lua_getfield(l, 1, "ibo");
	
	ShapeBuffer buffer = ShapeBuffer((GLuint)lua_tonumber(l, top+1), (GLuint)lua_tonumber(l, top+2));
	
	buffer.render(shader, x, y, z, angle);
	
	return 0;
}