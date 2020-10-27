#ifndef H_SHADER_MANAGER
#define H_SHADER_MANAGER

#include <string>

#define SHADER_KEYWORD "#shader"
#define VS_KEYWORD "vertex"
#define FS_KEYWORD "fragment"
#define DEFAULT_SHADER_COUNT 5
#define MAX_SHADER_SETS 10

enum ShaderType
{
	NONE = -1, VERTEX = 0, FRAGMENT = 1
};

typedef struct ShaderSet *SSet;
typedef struct ShaderSet
{
	unsigned int programID;
	std::string vertexShader;
	std::string fragmentShader;
} *SSet;

class ShaderManager
{
public:
	static unsigned int shaders;
	static SSet shaderSets[MAX_SHADER_SETS];
	int loadShaders(SSet source);
	void close();
private:
	int createShader(unsigned int programID, unsigned int type, const char* source);
};

SSet newShaderSet(const std::string& path);

#endif