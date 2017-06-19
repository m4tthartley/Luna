#include "epic_space_game.h"
#include "shader.h"
#include "engine.h"
#include <cstdlib>
#include <iostream>

#include "../glm/glm.hpp"
#include "../glm/gtc/matrix_transform.hpp"
#include "../glm/gtc/type_ptr.hpp"

GLuint star_map_vbo;
GLuint star_map_colors;
GLuint star_map_ibo;
int indices_to_render;
int star_count;

void create_star_map (int stars, int width, int height, int min_star_size, int max_star_size) {

	star_count = stars;
	int star_vertex_count = stars*(4*3);
	int star_index_count = stars*6;
	int star_color_count = stars*16;
	indices_to_render = star_index_count;
	double vertices[10000*12];
	int indices[10000*6];
	double colors[10000*16];
	
	for (int i = 0; i < star_vertex_count; i+=12) {
		double x = (rand()%(width*3))-(width*1.5);
		double y = (rand()%(height*3))-(height*1.5);
		//double x = (i/12)*10;
		//double y = (i/12)*10;
		
		vertices[i] = x; vertices[i+1] = y; vertices[i+2] = 0;
		vertices[i+3] = x+1; vertices[i+4] = y; vertices[i+5] = 0;
		vertices[i+6] = x+1; vertices[i+7] = y+1; vertices[i+8] = 0;
		vertices[i+9] = x; vertices[i+10] = y+1; vertices[i+11] = 0;
	}
	
	for (int i = 0; i < star_color_count; i+=16) {
		double color = (double)(rand() % 100) / 100;
		
		colors[i] = color; colors[i+1] = color; colors[i+2] = color; colors[i+3] = 1;
		colors[i+4] = color; colors[i+5] = color; colors[i+6] = color; colors[i+7] = 1;
		colors[i+8] = color; colors[i+9] = color; colors[i+10] = color; colors[i+11] = 1;
		colors[i+12] = color; colors[i+13] = color; colors[i+14] = color; colors[i+15] = 1;
	}
	
	int index_location = 0;
	for (int i = 0; i < stars*4; i+=4) {
		indices[index_location] = i;
		indices[index_location+1] = i+1;
		indices[index_location+2] = i+2;
		indices[index_location+3] = i;
		indices[index_location+4] = i+2;
		indices[index_location+5] = i+3;
		index_location+=6;
	}
	
//	for (int i = 0; i < star_vertex_count; i++) {
//		std::cout << vertices[i] << std::endl;
//	}
//	for (int i = 0; i < star_index_count; i++) {
//		std::cout << indices[i] << std::endl;
//	}
	
//	double vertices[] = {
//		-width/2, -height/2, 0.0,
//		width/2, -height/2, 0.0,
//		width/2, height/2, 0.0,
//		-width/2, height/2, 0.0,
//	};
	
//	int indices[] = {
//		0, 1, 2, 0, 2, 3
//	};
	
	glGenBuffers(1, &star_map_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, star_map_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &star_map_colors);
	glBindBuffer(GL_ARRAY_BUFFER, star_map_colors);
	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
	
	glGenBuffers(1, &star_map_ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_map_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void render_star_map (GLuint shader, double x, double y, double z) {
	
	Engine::addqps(star_count);
	
	Shader::use(shader);
	
	glm::mat4 matrix = glm::mat4();
	matrix *= glm::translate(glm::mat4(), glm::vec3(x, y, z));
	
	GLuint u_modelMatrix = Shader::uniformLocation(shader, "u_modelMatrix");
	glUniformMatrix4fv(u_modelMatrix, 1, GL_FALSE, glm::value_ptr(matrix));
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, star_map_vbo);
	glVertexPointer(3, GL_DOUBLE, 0, 0);
	
	glEnableClientState(GL_COLOR_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, star_map_colors);
	glColorPointer(4, GL_DOUBLE, 0, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, star_map_ibo);
	glDrawElements(GL_TRIANGLES, indices_to_render, GL_UNSIGNED_INT, 0);
}

int lua_create_star_map (lua_State *l) {
	
	create_star_map(
					lua_tonumber(l, 1),
					lua_tonumber(l, 2),
					lua_tonumber(l, 3),
					lua_tonumber(l, 4),
					lua_tonumber(l, 5)
					);
	
	return 0;
}

int lua_render_star_map (lua_State *l) {
	
	render_star_map(lua_tonumber(l, 1), lua_tonumber(l, 2), lua_tonumber(l, 3), lua_tonumber(l, 4));
	
	return 0;
}

// Minimap

GLuint fbo;
GLuint fbo_depth;
GLuint fbo_texture;
int fbo_width = 300;
int fbo_height = 200;
bool framebuffer_rendered = false;

void init_minimap_fbo () {
	
	glGenRenderbuffers(1, &fbo_depth);
	
	glBindRenderbuffer(GL_RENDERBUFFER, fbo_depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, fbo_width, fbo_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	
	glGenTextures(1, &fbo_texture);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, fbo_width, fbo_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	glBindTexture(GL_TEXTURE_2D, 0);
	
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo_texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo_depth);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Error while creating framebuffer" << std::endl;
	}
}

void begin_render_minimap () {
	
	if (!framebuffer_rendered) {
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, fbo_width, fbo_height, 0, -100, 100);
		glMatrixMode(GL_MODELVIEW);
	
	glEnable(GL_DEPTH_TEST);
	glPushAttrib(GL_VIEWPORT_BIT);
	glViewport(0, 0, 300, 200);
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
//	glColor4d(1.0, 0.0, 1.0, 1.0);
//	glBegin(GL_QUADS);
//	glVertex2d(0, 0);
//	glVertex2d(150, 0);
//	glVertex2d(150, 200);
//	glVertex2d(0, 200);
//	glEnd();
//	glColor4d(0.0, 1.0, 1.0, 1.0);
//	glBegin(GL_QUADS);
//	glVertex2d(150, 0);
//	glVertex2d(300, 0);
//	glVertex2d(300, 200);
//	glVertex2d(150, 200);
//	glEnd();
//	
//	glColor4d(1.0, 0.0, 1.0, 1.0);
//	glBegin(GL_QUADS);
//	glVertex2d(0, 0);
//	glVertex2d(50, 0);
//	glVertex2d(50, 50);
//	glVertex2d(0, 50);
//	glEnd();
	}
}

void end_render_minimap () {

	if (!framebuffer_rendered) {
		glPopAttrib();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		
		//framebuffer_rendered = true;
		
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1280, 720, 0, -100, 100);
		glMatrixMode(GL_MODELVIEW);
	}
	
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fbo_texture);
	//std::cout << fbo_texture << std::endl;
	
	glBegin(GL_QUADS);
	glTexCoord2d(0, 1);
	glVertex2d(0, 720-200);
	glTexCoord2d(1, 1);
	glVertex2d(300, 720-200);
	glTexCoord2d(1, 0);
	glVertex2d(300, 720);
	glTexCoord2d(0, 0);
	glVertex2d(0, 720);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, 0);
}

int lua_begin_render_minimap (lua_State *l) {
	
	begin_render_minimap();
	
	return 0;
}

int lua_end_render_minimap (lua_State *l) {
	
	end_render_minimap();
	
	return 0;
}

int lua_init_minimap_fbo (lua_State *l) {
	
	init_minimap_fbo();
	
	return 0;
}







