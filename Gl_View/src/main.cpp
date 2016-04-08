#include <GL/glut.h>
#include <cstdlib>

#include "GLView.h"

const unsigned int W = 640;
const unsigned int H = 480;

GLView view(W, H);

void display()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );



    glDrawPixels( W, H, GL_RGB, GL_FLOAT, view.GetData());

    glutSwapBuffers();
}

int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize( W, H );
    glutCreateWindow( "RayTracer" );
    glutDisplayFunc( display );
    glutMainLoop();
    return 0;
}
