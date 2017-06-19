#include "matrix.h"

glm::mat4 Matrix::modelMatrix = glm::mat4();

void Matrix::identity () {
	
	modelMatrix = glm::mat4();
}

void Matrix::translate (double x, double y, double z) {
	
	modelMatrix *= glm::translate(glm::mat4(), glm::vec3(x, y, z));
}

void Matrix::rotate (double angle, double x, double y, double z) {
	
	modelMatrix *= glm::rotate(glm::mat4(), (float)angle, glm::vec3(x, y, z));
}

void Matrix::scale (double x, double y, double z) {
	
	modelMatrix *= glm::scale(glm::mat4(), glm::vec3(x, y, z));
}

// Lua
int Matrix::lua_identity (lua_State* l) {
	
	identity();
	
	return 0;
}

int Matrix::lua_translate (lua_State* l) {
	
	translate( lua_tonumber(l, 1), lua_tonumber(l, 2), lua_tonumber(l, 3) );
	
	return 0;
}

int Matrix::lua_rotate (lua_State* l) {
	
	rotate( lua_tonumber(l, 1), lua_tonumber(l, 2), lua_tonumber(l, 3), lua_tonumber(l, 4) );
	
	return 0;
}

int Matrix::lua_scale (lua_State* l) {
	
	scale( lua_tonumber(l, 1), lua_tonumber(l, 2), lua_tonumber(l, 3) );
	
	return 0;
}