#include <GL/glut.h>
#include <cstdlib>
#include <iostream>

#include "GLView.h"
#include "RayTracerFactory.h"
#include "SimpleSerializer.h"
#include "STLBinarySerializer.h"

const unsigned int W = 1280;
const unsigned int H = 720;

GLView* view;

void display()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );


    glDrawPixels( W, H, GL_RGB, GL_FLOAT, view->GetData());

    glutSwapBuffers();
    glutPostRedisplay ();
}

int main( int argc, char **argv )
{
    view = new GLView(W, H);
    if(argc < 2)
    {
        std::cerr << "No scene file to render" << std::endl;
        exit(1);
    }
    Kernel* kernel = RayTracerFactory::Instance()->GetKernel(argv[1], argv[2]);
    kernel->SetView(view);
    kernel->Run();
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
    glutInitWindowSize( W, H );
    glutCreateWindow( "RayTracer" );
    glutDisplayFunc( display );
    glutMainLoop();
    delete kernel;
    return 0;
}
