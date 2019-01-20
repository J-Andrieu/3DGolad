#ifndef GRAPHICS_HEADERS_H
#define GRAPHICS_HEADERS_H

#define GL_DO_NOT_WARN_IF_MULTI_GL_VERSION_HEADERS_INCLUDED

#if defined(__APPLE__) || defined(MACOSX)
#include <OpenGL/gl3.h>
#include <OpenGL/GLU.h>
#else //linux as default
#include <GL/glew.h>
//#include <GL/glu.h>
#endif

// GLM
#define GLM_FORCE_RADIANS
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>
#include <thread>
#include <atomic>

#define INVALID_UNIFORM_LOCATION 0x7fffffff

typedef struct Vertex {
	glm::vec3 m_vertex;
	glm::vec3 m_normal;
	glm::vec2 m_texture;

	Vertex(const glm::vec3 & vertex, const glm::vec3 & normal, const glm::vec2 & texture) :
			m_vertex(vertex), m_normal(normal), m_texture(texture) {
	}
} Vertex;

enum ObjType {
	DEAD = 0,
	P1_ALIVE_FUTURE = 1,
	P2_ALIVE_FUTURE = 2,
	P1_ALIVE = 3,
	P2_ALIVE = 4,
	P1_DEAD_FUTURE = 5,
	P2_DEAD_FUTURE = 6,
	P1_ALIVE_MARKED = 7,
	P2_ALIVE_MARKED = 8,
	P1_DEAD_MARKED = 9,
	P2_DEAD_MARKED = 10,
  NUM_TYPES
};

enum BoardSides{
	FLOOR = 0,
	ROOF = 1,
	NORTH = 2,
	SOUTH = 3,
	EAST = 4,
	WEST = 5,
	NUM_SIDES
};

enum CommandLineFlags {
    LAUNCHFILE = 1,
    AUTOPLAY = 1 << 1,
    NUM_OBJECTS = 1 << 2,
    AMBIENT_LEVEL = 1 << 3,
    VERTEX_SHADER = 1 << 4,
    FRAGMENT_SHADER = 1 << 5,
    WINDOW_SIZE = 1 << 6,
    FULL_SCREEN = 1 << 7,
    MENU_SIZE = 1 << 8,
    OBJ_FILE = 1 << 9,
};

typedef struct CommandLineArgs {
    uint m_FLAGS = 0;
    std::string m_launchFile;
    float m_autoplayInterval;
    uint m_numObjects;//n, where number of objects is nxnx6 
    glm::vec3 m_ambientLevel;
    std::string m_vertexShaderPath;
    std::string m_fragmentShaderPath;
    glm::uvec2 m_windowSize;
    bool m_fullScreen;
    glm::uvec2 m_menuSize;
    std::string m_objectFilePath;
} CommandLineArgs;

typedef struct ObjectInfo {
	std::string m_objFile;
	glm::vec3 m_scale;
	glm::vec3 m_rotation;
	glm::vec3 m_diffuseLevel;
	glm::vec3 m_specularLevel;
	float m_shininess;
} ObjectInfo;

typedef struct BoardInfo {
	std::string m_name;
	glm::uvec2 m_size;
	glm::vec3 m_startingLoc;
	glm::vec3 m_changeRow;
	glm::vec3 m_changeCol;
} BoardInfo;

typedef struct GameInfo {
	ObjectInfo m_object;
	std::string m_textures[ObjType::NUM_TYPES];
	glm::vec3 m_ambientLevel;
        float m_autoplayInterval;
	BoardInfo m_sides[BoardSides::NUM_SIDES];
} GameInfo;

typedef struct BasicTransform {
            glm::vec3 m_pos;
            glm::vec3 m_rot;
} BasicTransform;

#endif /* GRAPHICS_HEADERS_H */
