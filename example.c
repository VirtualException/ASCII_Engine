/*
 * ######################################
 * #  Virtual Exception's ASCII-ENGINE  #
 * #  Main c file - Render a nice cube  #
 * ######################################
*/

#include "engine/ascii_engine.h"
#include "example.h"

void
MultiplyMatrixVector(struct v3* i, struct v3* o, struct mat4x4* mat) {
 
    o->x = i->x * mat->m[0][0] + i->y * mat->m[1][0] + i->z * mat->m[2][0] + mat->m[3][0];
    o->y = i->x * mat->m[0][1] + i->y * mat->m[1][1] + i->z * mat->m[2][1] + mat->m[3][1];
    o->z = i->x * mat->m[0][2] + i->y * mat->m[1][2] + i->z * mat->m[2][2] + mat->m[3][2];
 
    float w = i->x * mat->m[0][3] + i->y * mat->m[1][3] + i->z * mat->m[2][3] + mat->m[3][3];
 
    if (w != 0.0f)
    {
        o->x /= w; o->y /= w; o->z /= w;
    }
}

void
render() {

    for (int i = 0; i < ( 12 )/*TEMP*/; i++) {

        struct triangle triProjected, triTranslated, triRotatedZ, triRotatedZX;
        struct triangle tri = meshCube.tris[i];

        /* Rotate in Z-Axis */
        MultiplyMatrixVector(&tri.p[0], &triRotatedZ.p[0], &matRotZ);
        MultiplyMatrixVector(&tri.p[1], &triRotatedZ.p[1], &matRotZ);
        MultiplyMatrixVector(&tri.p[2], &triRotatedZ.p[2], &matRotZ);

        /* Rotate in X-Axis */
        MultiplyMatrixVector(&triRotatedZ.p[0], &triRotatedZX.p[0], &matRotX);
        MultiplyMatrixVector(&triRotatedZ.p[1], &triRotatedZX.p[1], &matRotX);
        MultiplyMatrixVector(&triRotatedZ.p[2], &triRotatedZX.p[2], &matRotX);

        /* Offset into the screen */
        triTranslated = triRotatedZX;
        triTranslated.p[0].z = triRotatedZX.p[0].z + 3.0f;
        triTranslated.p[1].z = triRotatedZX.p[1].z + 3.0f;
        triTranslated.p[2].z = triRotatedZX.p[2].z + 3.0f;

        /* Project triangles from 3D--> 2D */
        MultiplyMatrixVector(&triTranslated.p[0], &triProjected.p[0], &matProj);
        MultiplyMatrixVector(&triTranslated.p[1], &triProjected.p[1], &matProj);
        MultiplyMatrixVector(&triTranslated.p[2], &triProjected.p[2], &matProj);

        /* Screen scaling */
        triProjected.p[0].x += 1.0f; triProjected.p[0].y += 1.0f;
        triProjected.p[1].x += 1.0f; triProjected.p[1].y += 1.0f;
        triProjected.p[2].x += 1.0f; triProjected.p[2].y += 1.0f;

        triProjected.p[0].x *= 0.5f * (float)COLS;
        triProjected.p[0].y *= 0.5f * (float)ROWS;
        triProjected.p[1].x *= 0.5f * (float)COLS;
        triProjected.p[1].y *= 0.5f * (float)ROWS;
        triProjected.p[2].x *= 0.5f * (float)COLS;
        triProjected.p[2].y *= 0.5f * (float)ROWS;

        /* Draw triangles */
        drawTriangle(&triProjected);
    }

    ascii_render(1);

}

void
drawTriangle(struct triangle* tri) {
    
    ascii_drawLine(
        tri->p[0].x, tri->p[0].y,
        tri->p[1].x, tri->p[1].y,
        150 );
    ascii_drawLine(
        tri->p[1].x, tri->p[1].y,
        tri->p[2].x, tri->p[2].y,
        150 );
    ascii_drawLine(
        tri->p[2].x, tri->p[2].y,
        tri->p[0].x, tri->p[0].y,
        150 );
    
}

int main(int argc, char* argv[]) {

    ascii_init();

    meshCube.tris = (struct triangle[6 * 2]) {
        // SOUTH
        { 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },
        // EAST
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },
        // NORTH
        { 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },
        // WEST
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
        { 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },
        // TOP
        { 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },
        // BOTTOM
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
        { 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },
    };

    // Projection Matrix
    float fNear = 0.1f;
    float fFar = 1000.f;
    float fFov = 70.0f;
    float fAspectRatio = (float)ROWS / (float)ROWS;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);

    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;

	while (true) {

        ascii_clear();

        /* Rotation */
        fTheta += 1.0f * 0.0167f;

        /* Set up rotation matrices */

        // Rotation Z
        matRotZ.m[0][0] = cosf(fTheta);
        matRotZ.m[0][1] = sinf(fTheta);
        matRotZ.m[1][0] = -sinf(fTheta);
        matRotZ.m[1][1] = cosf(fTheta);
        matRotZ.m[2][2] = 1;
        matRotZ.m[3][3] = 1;
        // Rotation X
        matRotX.m[0][0] = 1;
        matRotX.m[1][1] = cosf(fTheta * 0.8f);
        matRotX.m[1][2] = sinf(fTheta * 0.8f);
        matRotX.m[2][1] = -sinf(fTheta * 0.8f);
        matRotX.m[2][2] = cosf(fTheta * 0.8f);
        matRotX.m[3][3] = 1;
        
        render(1000);
	}

    // Just in case...
    ascii_die(1);

    return 0;
}
