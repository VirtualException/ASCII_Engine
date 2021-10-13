/*
 * ######################################
 * #  Virtual Exception's ASCII-ENGINE  #
 * # Main header file - Just an example #
 * ######################################
*/

struct v3
{
	float x, y, z;
};

struct triangle
{
	struct v3 p[3];
};

struct mesh
{
	struct triangle* tris;
};

struct mat4x4
{
	float m[4][4];
};

struct mesh meshCube; 
struct mat4x4 matProj, matRotZ, matRotX;
float fTheta;

void MultiplyMatrixVector(struct v3* i, struct v3* o, struct mat4x4* mat);
void render();
void drawTriangle(struct triangle* tri);
