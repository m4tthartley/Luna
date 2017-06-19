#include "video3.h"
#include "shader.h"

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"
#include <iostream>

GLuint Video3::vertexBuffer = 0;
GLuint Video3::indexBuffer = 0;
GLuint Video3::textureBuffer = 0;
GLuint Video3::elipseVertexBuffer = 0;
GLuint Video3::elipseIndexBuffer = 0;

glm::mat4 Video3::modelMatrix = glm::mat4();

void Video3::init () {
	
	int indicies [] = {
		0, 1, 2, 0, 2, 3
	};
	
	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	
	double verticies[] = {
		-0.5, -0.5, 0.0,
		0.5, -0.5, 0.0,
		0.5, 0.5, 0.0,
		-0.5, 0.5, 0.0
	};
	
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticies), verticies, GL_STATIC_DRAW);
	
	double textureCoords[] = {
		0.0, 0.0,
		1.0, 0.0,
		1.0, 1.0,
		0.0, 1.0
	};
	
	glGenBuffers(1, &textureBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, textureBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoords), textureCoords, GL_STATIC_DRAW);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexPointer(3, GL_DOUBLE, 0, 0);
	
	// Elipse
	{
		const int detail = 50;
		//int testdetail = 4;
		int location = 0;
		double vertices[detail*3+3];
		
		for (int i = 0; i < detail; i++) {
			
			double rad = (((double)i / (double)detail) * 360)*(3.14159 / 180.0);
			vertices[location] = (cos(rad)*500);
			vertices[location+1] = (sin(rad)*500);
			vertices[location+2] = 0;
			location += 3;
		}
		
//		for (int i = 0; i < 50; i++) {
//			std::cout << vertices[i*3] << ", " << vertices[i*3+1] << ", " << vertices[i*3+2] << std::endl;
//		}
		
		vertices[150] = 0;
		vertices[151] = 0;
		vertices[152] = 0;
		
		glGenBuffers(1, &elipseVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, elipseVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		int indexLocation = 0;
		int indices[50*3];
		
		for (int i = 0; i < detail; i++) {
			
			indices[indexLocation] = 50;
			indices[indexLocation+1] = i==49 ? 0 : i+1;
			indices[indexLocation+2] = i;
			indexLocation += 3;
		}
		
//		for (int i = 0; i < 150; i++) {
//			std::cout << indices[i] << std::endl;
//		}
		
		glGenBuffers(1, &elipseIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elipseIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);
	}
}

void Video3::renderQuad (GLuint shader, double x, double y, double z, double width, double height) {
	
	Shader::use(shader);
	
	GLuint u_position = Shader::uniformLocation(shader, "u_position");
	//Shader::uniform3(u_position, x, y, 0);
	GLuint u_scale = Shader::uniformLocation(shader, "u_scale");
	//Shader::uniform3(u_scale, width, height, 1);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexPointer(3, GL_DOUBLE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Video3::renderSprite (GLuint shader, GLuint texture, double sx, double sy, double sw, double sh, double x, double y, double z, double scale, double angle) {
	
	glBindTexture(GL_TEXTURE_2D, texture);
	int tw;
	int th;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &tw);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &th);
	
	Shader::use(shader);
	
	GLuint u_texture = Shader::uniformLocation(shader, "u_texture");
//	Shader::uniform4(u_texture, sw/tw, sh/th, sx/tw, sy/th);
	glm::mat4 textureMatrix = glm::translate(glm::mat4(1.0), glm::vec3(sx/tw, sy/th, 0)) * glm::scale(glm::mat4(1.0), glm::vec3(sw/tw, sh/th, 1));
	glUniformMatrix4fv(u_texture, 1, GL_FALSE, glm::value_ptr(textureMatrix));
	
	// Model matrix
	if(angle != 0.0) rotate(angle);
	modelMatrix = glm::translate(glm::mat4(), glm::vec3(x, y, 0)) * modelMatrix;
	GLuint u_modelMatrix = Shader::uniformLocation(shader, "u_modelMatrix");
	glUniformMatrix4fv(u_modelMatrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	loadIdentity();
	
	GLuint u_position = Shader::uniformLocation(shader, "u_position");
	//Shader::uniform3(u_position, 0, 0, 0);
	GLuint u_scale = Shader::uniformLocation(shader, "u_scale");
	//Shader::uniform3(u_scale, sw*scale, sh*scale, 1);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexPointer(3, GL_DOUBLE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void Video3::renderElipse (GLuint shader, double x, double y, double width, double height, double detail) {
	
	Shader::use(shader);
	
	GLuint u_position = Shader::uniformLocation(shader, "u_position");
	//Shader::uniform3(u_position, x, y, 0);
	GLuint u_scale = Shader::uniformLocation(shader, "u_scale");
	//Shader::uniform3(u_scale, width, height, 1);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, elipseVertexBuffer);
	glVertexPointer(3, GL_DOUBLE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elipseIndexBuffer);
	glDrawElements(GL_TRIANGLES, 150, GL_UNSIGNED_INT, 0);
}

void Video3::loadIdentity () {
	
	modelMatrix = glm::mat4();
}

void Video3::translate (double x, double y, double z) {
	
	modelMatrix *= glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

void Video3::scale (double x, double y, double z) {
	
	modelMatrix *= glm::scale(glm::mat4(), glm::vec3(x, y, z));
}

void Video3::rotate (double rotation) {
	
	modelMatrix *= glm::rotate(glm::mat4(), (float)rotation, glm::vec3(0, 0, 1));
}

// Lua

int Video3::lua_renderQuad (lua_State* l) {
	
	GLuint shader = lua_tonumber(l, 1);
	double x = lua_tonumber(l, 2);
	double y = lua_tonumber(l, 3);
	double z = lua_tonumber(l, 4);
	double width = lua_tonumber(l, 5);
	double height = lua_tonumber(l, 6);
	
	renderQuad(shader, x, y, z, width, height);
	
	return 0;
}

int Video3::lua_renderSprite (lua_State* l) {
	
	int args = lua_gettop(l);
	
	GLuint shader = lua_tonumber(l, 1);
	double x = lua_tonumber(l, 3);
	double y = lua_tonumber(l, 4);
	double z = lua_tonumber(l, 5);
	double scale = lua_tonumber(l, 6);
	double angle = lua_tonumber(l, 7);
	
//	int i = 7;
//	
//	angle = lua_tonumber(l, 7);
//	if(angle != NULL) i++;
//	if(angle = lua_tonumber(l, 7)) {
//		std::cout << "angle is " << angle << std::endl;
//		i++;
//	}
	
	luaL_checktype(l, 2, LUA_TTABLE);
	lua_getfield(l, 2, "texture");
	lua_getfield(l, 2, "sx");
	lua_getfield(l, 2, "sy");
	lua_getfield(l, 2, "sw");
	lua_getfield(l, 2, "sh");
	
	GLuint texture = lua_tonumber(l, args+1);
	double sx = lua_tonumber(l, args+2);
	double sy = lua_tonumber(l, args+3);
	double sw = lua_tonumber(l, args+4);
	double sh = lua_tonumber(l, args+5);
	
	//std::cout << texture << " " << sw << " " << sh << std::endl;
	
	renderSprite(shader, texture, sx, sy, sw, sh, x, y, z, scale, angle);
	
	return 0;
}

int Video3::lua_renderElipse (lua_State* l) {
	
	GLuint shader = lua_tonumber(l, 1);
	double x = lua_tonumber(l, 2);
	double y = lua_tonumber(l, 3);
	double width = lua_tonumber(l, 4);
	double height = lua_tonumber(l, 5);
	double detail = lua_tonumber(l, 6);
	
	renderElipse(shader, x, y, width, height, detail);
	
	return 0;
}

int Video3::lua_loadIdentity (lua_State* l) {
	
	loadIdentity();
	
	return 0;
}

int Video3::lua_translate (lua_State* l) {
	
	double x = lua_tonumber(l, 1);
	double y = lua_tonumber(l, 2);
	double z = lua_tonumber(l, 3);
	
	translate(x, y, z);
	
	return 0;
}

int Video3::lua_scale (lua_State* l) {
	
	double x = lua_tonumber(l, 1);
	double y = lua_tonumber(l, 2);
	double z = lua_tonumber(l, 3);
	
	scale(x, y, z);
	
	return 0;
}

int Video3::lua_rotate (lua_State* l) {
	
	double rotation = lua_tonumber(l, 1);
	
	rotate(rotation);
	
	return 0;
}