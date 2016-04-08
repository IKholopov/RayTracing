#include "GLView.h"
#include "mutex"

GLView::GLView(unsigned int width, unsigned int height):resolution_(Resolution(width, height))
{
    std::unique_lock<std::mutex> lock_gurad(matrixLock_);
    data = new float[width*height*3];
    for(unsigned int x = 0; x < width; ++x)
        for(unsigned int y = 0; y < height; ++y)
            for(unsigned int c = 0; c < 3; ++c)
                data[width*y*3+x*3+c] = 0.5f;
}

GLView::~GLView()
{
    std::unique_lock<std::mutex> lock_gurad(matrixLock_);
    delete[] data;
}

void GLView::UpdatePixel(unsigned int x, unsigned int y, Point color)
{
    std::unique_lock<std::mutex> lock_gurad(matrixLock_);
    data[3*(resolution_.Width*y + x) + 0] = color.X;
    data[3*(resolution_.Width*y + x) + 1] = color.Y;
    data[3*(resolution_.Width*y + x) + 2] = color.Z;
}

Resolution GLView::GetResolution() const
{
    return resolution_;
}

const float*GLView::GetData() const
{
    return data;
}
