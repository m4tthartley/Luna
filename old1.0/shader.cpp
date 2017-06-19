#include "shader.h"
#include <string>
#include <iostream>
#include <vector>
#include "stdio.h"

void Shader::printLog (GLuint shader, std::string name) {
	
	GLint maxlength = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxlength);
	
	if(maxlength > 0) {
		GLchar* info = new GLchar[maxlength];
		glGetShaderInfoLog(shader, maxlength, &maxlength, &info[0]);
	
		std::cout << "shader " << name << "(" << shader << ") " << info << std::endl;
		delete [] info;
	}
}

//void Shader::compileDefault () {
//	
//	const char* vertexShader =
//		"varying vec4 vertexColor;"
//		"uniform vec3 u_position;"
//		"void main () {"
//		"gl_Position = gl_ModelViewProjectionMatrix*(gl_Vertex+vec4(u_position, 0));"
//		"vertexColor = vec4(1.0, 1.0, 1.0, 1.0);"
//		"}"
//	;
//	
//	const char* fragmentShader =
//		"varying vec4 vertexColor;"
//		"void main () {"
//		"gl_FragColor = vertexColor;"
//		"}"
//	;
//	
//	GLuint defaultVertexShader;
//	GLuint defaultFragmentShader;
//	defaultVertexShader = glCreateShader(GL_VERTEX_SHADER);
//	defaultFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
//	
//	glShaderSource(defaultVertexShader, 1, &vertexShader, 0);
//	glShaderSource(defaultFragmentShader, 1, &fragmentShader, 0);
//	
//	glCompileShader(defaultVertexShader);
//	printLog(defaultVertexShader);
//	glCompileShader(defaultFragmentShader);
//	printLog(defaultFragmentShader);
//	
//	GLuint defaultShader = glCreateProgram();
//	
//	glAttachShader(defaultShader, defaultVertexShader);
//	glAttachShader(defaultShader, defaultFragmentShader);
//	glLinkProgram(defaultShader);
//	printLog(defaultFragmentShader);
//	
//	glUseProgram(defaultShader);
//}

GLuint Shader::from_file (const char *filename) {
	
	char *buffer = 0;
	long length;
	FILE *f = fopen(filename, "rb");
	
	if (f) {
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char*)malloc(length);
		if (buffer) {
			fread(buffer, 1, length, f);
		}
		fclose(f);
	}
	
	if (buffer) {
		const char* vertex_source =
			"#version 120\n"

			"varying vec4 v_color;\n"
			"varying vec4 v_texcoord;\n"
			"uniform mat4 u_modelMatrix;\n"

			"void main () {\n"
			
				"gl_Position = gl_ModelViewProjectionMatrix * u_modelMatrix * gl_Vertex;\n"
				"v_color = gl_Color;\n"
				"v_texcoord = gl_MultiTexCoord0;\n"
			"}\n"
		;
		const char* fragment_source = buffer;
		
		GLuint vs = glCreateShader(GL_VERTEX_SHADER);
		GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(vs, 1, &vertex_source, 0);
		glShaderSource(fs, 1, &fragment_source, (GLint*)&length);

		glCompileShader(vs);
		printLog(vs, "default");
		glCompileShader(fs);
		printLog(fs, filename);

		GLuint shader = glCreateProgram();

		glAttachShader(shader, vs);
		glAttachShader(shader, fs);
		glLinkProgram(shader);
		printLog(shader, filename);

		return shader;
	}
	
	return 0;
}

void Shader::use (GLuint shader) {
	
	glUseProgram(shader);
}

GLuint Shader::uniformLocation (GLuint shader, std::string uniform) {
	
	return glGetUniformLocation(shader, uniform.c_str());
}

void Shader::uniform3 (GLuint shader, std::string location, double v0, double v1, double v2) {
	
	glUseProgram(shader);
	glUniform3f(glGetUniformLocation(shader, location.c_str()), v0, v1, v2);
}

void Shader::uniform4 (GLuint shader, std::string location, double v0, double v1, double v2, double v3) {
	
	glUseProgram(shader);
	glUniform4f(glGetUniformLocation(shader, location.c_str()), v0, v1, v2, v3);
}

void Shader::uniform1 (GLuint shader, std::string location, double v0) {
	
	glUseProgram(shader);
	glUniform1i(glGetUniformLocation(shader, location.c_str()), v0);
}

// Lua

int Shader::lua_from_file (lua_State* l) {
	
	const char *filename = lua_tostring(l, 1);
	
	GLuint shader = from_file(filename);
	
	//lua_pushnumber(l, shader);
	
	lua_createtable(l, 0, 2);
	lua_pushnumber(l, shader);
	lua_setfield(l, -2, "shader");
	
//	lua_getfield(l, LUA_GLOBALSINDEX, table.c_str());
//	if(!lua_istable(l, -1)) {
//		
//		lua_createtable(l, 0, 1);
//		lua_setfield(l, LUA_GLOBALSINDEX, table.c_str());
//		lua_pop(l, 1);
//		lua_getfield(l, LUA_GLOBALSINDEX, table.c_str());
//	}
	
	lua_pushstring(l, "uniform");
	lua_pushcfunction(l, lua_uniform);
	lua_settable(l, -3);
	lua_pop(l, 0);
	
	lua_pushstring(l, "use");
	lua_pushcfunction(l, lua_use);
	lua_settable(l, -3);
	lua_pop(l, 0);
	
	return 1;
}

int Shader::lua_use (lua_State* l) {
	
	luaL_checktype(l, 1, LUA_TTABLE);
	lua_getfield(l, 1, "shader");
	GLuint shader = lua_tonumber(l, lua_gettop(l)+1);
	
	glUseProgram(shader);
	
	return 0;
}

int Shader::lua_uniformLocation (lua_State* l) {
	
	GLuint shader = lua_tonumber(l, 1);
	std::string uniform = lua_tostring(l, 2);
	
	GLuint ul = uniformLocation(shader, uniform);
	
	lua_pushnumber(l, ul);
	
	return 1;
}

int Shader::lua_uniform1(lua_State* l) {
	
	GLuint shader = lua_tonumber(l, 1);
	std::string location = lua_tostring(l, 2);
	double v0 = lua_tonumber(l, 3);
	
	uniform1(shader, location, v0);
	
	return 0;
}

int Shader::lua_uniform2(lua_State* l) {
	
	GLuint shader = lua_tonumber(l, 1);
	std::string location = lua_tostring(l, 2);
	double v0 = lua_tonumber(l, 3);
	
	uniform1(shader, location, v0);
	
	return 0;
}

int Shader::lua_uniform3(lua_State* l) {
	
	GLuint shader = lua_tonumber(l, 1);
	std::string location = lua_tostring(l, 2);
	double v0 = lua_tonumber(l, 3);
	double v1 = lua_tonumber(l, 4);
	double v2 = lua_tonumber(l, 5);
	
	uniform3(shader, location, v0, v1, v2);
	
	return 0;
}

int Shader::lua_uniform4(lua_State* l) {
	
	GLuint shader = lua_tonumber(l, 1);
	std::string location = lua_tostring(l, 2);
	double v0 = lua_tonumber(l, 3);
	double v1 = lua_tonumber(l, 4);
	double v2 = lua_tonumber(l, 5);
	double v3 = lua_tonumber(l, 6);
	
	uniform4(shader, location, v0, v1, v2, v3);
	
	return 0;
}

int Shader::lua_uniform (lua_State *l) {
	
	int top = lua_gettop(l);
	std::string uniform = lua_tostring(l, 2);
	float *values = new float[top-2];
	for (int i = 0; i < top-2; i++) {
		values[i] = lua_tonumber(l, 3+i);
	}
	
	luaL_checktype(l, 1, LUA_TTABLE);
	lua_getfield(l, 1, "shader");
	GLuint shader = lua_tonumber(l, top+1);
	
	if (top == 3) {
		glUseProgram(shader);
		glUniform1f(glGetUniformLocation(shader, uniform.c_str()), values[0]);
	}
	if (top == 4) {
		glUseProgram(shader);
		glUniform2f(glGetUniformLocation(shader, uniform.c_str()), values[0], values[1]);
	}
	if (top == 5) {
		glUseProgram(shader);
		glUniform3f(glGetUniformLocation(shader, uniform.c_str()), values[0], values[1], values[2]);
	}
	if (top == 6) {
		glUseProgram(shader);
		glUniform4f(glGetUniformLocation(shader, uniform.c_str()), values[0], values[1], values[2], values[3]);
		//std::cout << values[0] << " " << values[1] << " " << values[2] << " " << values[3] << std::endl;
	}
	
	return 0;
}