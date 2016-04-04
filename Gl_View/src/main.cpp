#include <GL/glut.h>
#include <cstdlib>

const unsigned int W = 640;
const unsigned int H = 480;

void display()
{
    glClearColor( 0, 0, 0, 1 );
    glClear( GL_COLOR_BUFFER_BIT );

    unsigned int data[W][H][3];
    for( size_t y = 0; y < H; ++y )
    {
        for( size_t x = 0; x < W; ++x )
        {
            data[x][y][0] = ( rand() % 255 ) * 255 * 255 * 255;
            data[x][y][1] = (rand() % 255 ) * 255 * 255 * 255;
            data[x][y][2] = ( rand() % 255 ) * 255 * 255 * 255;
        }
    }

    glDrawPixels( W, H, GL_RGB, GL_UNSIGNED_INT, data );

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
