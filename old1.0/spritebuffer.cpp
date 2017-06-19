#include "spritebuffer.h"
#include "shader.h"
#include "matrix.h"
#include "engine.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

SpriteBuffer::SpriteBuffer (GLuint texture, GLuint vbo, GLuint ibo)
: texture(texture)
, vbo(vbo)
, ibo(ibo)
{
}

SpriteBuffer::SpriteBuffer (GLuint texture, double sx, double sy, double sw, double sh, double size)
: texture(texture)
{
	glBindTexture(GL_TEXTURE_2D, texture);
	int tw;
	int th;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);
	
	double verticies[] = {
		-((sw*size)/2), -((sh*size)/2), 0.0,
		((sw*size)/2), -((sh*size)/2), 0.0,
		((sw*size)/2), ((sh*size)/2), 0.0,
		-((sw*size)/2), ((sh*size)/2), 0.0,
		
		sx/tw, sy/th,
		(sx+sw)/tw, sy/th,
		(sx+sw)/tw, (sy+sh)/th,
		sx/tw, (sy+sh)/th
	};
	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	
	int indices[] = {
		0, 1, 2, 0, 2, 3
	};
	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void SpriteBuffer::render (GLuint shader, double x, double y, double z, double angle) {
	
	Engine::addqps(1);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	Shader::use(shader);
	
	// Model matrix
	glm::mat4 matrix = glm::mat4();
	
	matrix *= glm::translate(glm::mat4(), glm::vec3(x, y, z));
	if(angle != 0) matrix *= glm::rotate(glm::mat4(), (float)angle, glm::vec3(0, 0, 1));
	
	GLuint u_modelMatrix = Shader::uniformLocation(shader, "u_modelMatrix");
	glUniformMatrix4fv(u_modelMatrix, 1, GL_FALSE, glm::value_ptr(matrix * Matrix::modelMatrix));
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexPointer(3, GL_DOUBLE, 0, 0);
	glTexCoordPointer(2, GL_DOUBLE, 0, (GLvoid*)(sizeof(double)*12));
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//Lua
int SpriteBuffer::lua_spriteBuffer (lua_State*l ) {
	
	SpriteBuffer buffer = SpriteBuffer(lua_tonumber(l, 1),
									   lua_tonumber(l, 2),
									   lua_tonumber(l, 3),
									   lua_tonumber(l, 4),
									   lua_tonumber(l, 5),
									   lua_tonumber(l, 6));
	
	lua_createtable(l, 0, 2);
	lua_pushnumber(l, buffer.texture);
	lua_setfield(l, -2, "texture");
	lua_pushnumber(l, buffer.vbo);
	lua_setfield(l, -2, "vbo");
	lua_pushnumber(l, buffer.ibo);
	lua_setfield(l, -2, "ibo");
	
	return 1;
}

int SpriteBuffer::lua_renderSpriteBuffer (lua_State* l) {
	
	int top = lua_gettop(l);
	luaL_checktype(l, 2, LUA_TNUMBER);
	GLuint shader = lua_tonumber(l, 2);
	double x = lua_tonumber(l, 3);
	double y = lua_tonumber(l, 4);
	double z = lua_tonumber(l, 5);
	double angle = lua_tonumber(l, 6);
	
	luaL_checktype(l, 1, LUA_TTABLE);
	lua_getfield(l, 1, "texture");
	lua_getfield(l, 1, "vbo");
	lua_getfield(l, 1, "ibo");
	
	SpriteBuffer buffer = SpriteBuffer(lua_tonumber(l, top+1), lua_tonumber(l, top+2), lua_tonumber(l, top+3));
	
	buffer.render(shader, x, y, z, angle);
	
	return 0;
}