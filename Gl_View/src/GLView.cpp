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

void GLView::UpdatePixel(unsigned int x, unsigned int y, Color color)
{
    std::unique_lock<std::mutex> lock_gurad(matrixLock_);
    data[3*(resolution_.Width*y + x) + 0] = color.R;
    data[3*(resolution_.Width*y + x) + 1] = color.G;
    data[3*(resolution_.Width*y + x) + 2] = color.B;
}

Color GLView::GetPixelColor(unsigned int x, unsigned int y)
{
    return Color(data[3*(resolution_.Width*y + x) + 0],
            data[3*(resolution_.Width*y + x) + 1],
            data[3*(resolution_.Width*y + x) + 2]);
}


Resolution GLView::GetResolution() const
{
    return resolution_;
}

const float*GLView::GetData() const
{
    return data;
}
