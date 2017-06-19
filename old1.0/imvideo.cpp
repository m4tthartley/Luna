#include "imvideo.h"
#include "engine.h"
#include "shader.h"

#include <iostream>

// Create vertex, index and color buffer object
int imvideo_create_buffer (GLuint texture, float *vertices, float *colors, float *texcoords, int *indices, int vertex_count, int index_count) {
	
	im_buffer buffer;
	buffer.vertex_buffer = vertices;
	buffer.color_buffer = colors;
	buffer.texture_buffer = texcoords;
	buffer.index_buffer = indices;
	
	buffer.vertex_count = vertex_count;
	buffer.index_count = index_count;
	buffer.texture = texture;
	
	int i = buffer_count;
	buffers[i] = buffer;
	buffer_count++;
	
	return i;
}

// Render im_buffer
void imvideo_render_buffer (im_buffer buffer, float x, float y, float z, float angle) {
	
	Engine::addqps(1);
	
	glTranslatef(x, y, z);
	glRotatef(angle, 0, 0, 1);
	
	if (!buffer.texture) {
		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
	
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < buffer.index_count; i++) {
			int index = buffer.index_buffer[i];
			glColor4f(buffer.color_buffer[index*4],
					  buffer.color_buffer[index*4+1],
					  buffer.color_buffer[index*4+2],
					  buffer.color_buffer[index*4+3]);
			glVertex3f(buffer.vertex_buffer[index*3],
					   buffer.vertex_buffer[index*3+1],
					   buffer.vertex_buffer[index*3+2]);
		}
		glEnd();
	}
	else {
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, buffer.texture);
		
		glBegin(GL_TRIANGLES);
		for (int i = 0; i < buffer.index_count; i++) {
			int index = buffer.index_buffer[i];
			glColor4f(buffer.color_buffer[index*4],
					  buffer.color_buffer[index*4+1],
					  buffer.color_buffer[index*4+2],
					  buffer.color_buffer[index*4+3]);
			glTexCoord2f(buffer.texture_buffer[index*2],
						 buffer.texture_buffer[index*2+1]);
			glVertex3f(buffer.vertex_buffer[index*3],
					   buffer.vertex_buffer[index*3+1],
					   buffer.vertex_buffer[index*3+2]);
		}
		glEnd();
	}
	
	glLoadIdentity();
}



int lua_create_buffer (lua_State *l) {
	
	GLuint texture = lua_tonumber(l, 1);
	luaL_checktype(l, 2, LUA_TTABLE);
	luaL_checktype(l, 3, LUA_TTABLE);
	luaL_checktype(l, 4, LUA_TTABLE);
	luaL_checktype(l, 5, LUA_TTABLE);
	int vertex_count = lua_tonumber(l, 6);
	int index_count = lua_tonumber(l, 7);
	int vcount = vertex_count;
	int ccount = vertex_count+(vertex_count/3);
	int tcount = vertex_count-(vertex_count/3);
	int icount = index_count;
	float *vertices = new float[vcount];
	float *colors = new float[ccount];
	float *texcoords = new float[tcount];
	int *indices = new int[icount];
	
	lua_pushnil(l);
	for (int i = 0; i < vcount; i++) {
		if (!lua_next(l, 2)) break;
		vertices[i] = lua_tonumber(l, -1);
		lua_pop(l, 1);
	}
	
	lua_pushnil(l);
	for (int i = 0; i < ccount; i++) {
		if (!lua_next(l, 3)) break;
		colors[i] = lua_tonumber(l, -1);
		lua_pop(l, 1);
	}
	
	lua_pushnil(l);
	for (int i = 0; i < tcount; i++) {
		if (!lua_next(l, 4)) break;
		texcoords[i] = lua_tonumber(l, -1);
		lua_pop(l, 1);
	}
	
	lua_pushnil(l);
	for (int i = 0; i < icount; i++) {
		if (!lua_next(l, 5)) break;
		indices[i] = lua_tonumber(l, -1);
		lua_pop(l, 1);
	}
	
	int i = imvideo_create_buffer(texture, vertices, colors, texcoords, indices, vertex_count, index_count);
	lua_pushnumber(l, i);
	
	return 1;
}

int lua_render_buffer (lua_State *l) {
	
	imvideo_render_buffer(buffers[lua_tonumber(l, 1)],
						  lua_tonumber(l, 2),
						  lua_tonumber(l, 3),
						  lua_tonumber(l, 4),
						  lua_tonumber(l, 5));
	
	return 0;
}