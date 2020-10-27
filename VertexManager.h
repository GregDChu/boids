#ifndef H_VERTEX_MANAGER
#define H_VERTEX_MANAGER

/* VERTEX GROUP DEFINITIONS */
#define DEFAULT_BUFFER_SLOTS 10
#define DEFAULT_ATTRIB_SLOTS 10

enum AttribType
{
	EMPTY, POS, COLOR, TEXTURE, NORM, INDEX, OTHER
};

typedef struct VertexGroup *VGroup;
typedef struct VertexGroup
{
	unsigned int attribs;
	AttribType attribTypes[DEFAULT_ATTRIB_SLOTS];
	unsigned int attribGroupSize[DEFAULT_ATTRIB_SLOTS];
	unsigned int elements;
	void *data;
} *VGroup;

VGroup newVGroup(unsigned int vcount, void *data);
VGroup newIGroup(unsigned int icount, void *data);
int defineAttribute(VGroup group, AttribType type, unsigned int elements);

/* VERTEX GROUP MANAGER */
#define VERTEX_DIMENSIONS 4

class VertexManager
{
public:
	static unsigned int bufferID[DEFAULT_BUFFER_SLOTS];
	static unsigned int vertaoID[DEFAULT_BUFFER_SLOTS];
	static VGroup vertGroups[DEFAULT_BUFFER_SLOTS];
	static int activeBuffers;
	int loadVertGroup(VGroup group);
	int loadIndexGroup(VGroup group);
	void close();
};

#endif