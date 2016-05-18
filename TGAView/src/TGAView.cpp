#include "TGAView.h"
#include "mutex"

#include <fstream>
#include <iostream>

TGAView::TGAView(unsigned int width, unsigned int height):resolution_(Resolution(width, height))
{
    std::unique_lock<std::mutex> lock_gurad(matrixLock_);
    data = new float[width*height*3];
    for(unsigned int x = 0; x < width; ++x)
        for(unsigned int y = 0; y < height; ++y)
        {
            data[width*y*3+x*3] = 0.0f;
            data[width*y*3+x*3 + 1] = 0.0f;
            data[width*y*3+x*3 + 2] = 1.0f;
        }
}

TGAView::~TGAView()
{
    std::unique_lock<std::mutex> lock_gurad(matrixLock_);
    delete[] data;
}

void TGAView::UpdatePixel(unsigned int x, unsigned int y, Color color)
{
    std::unique_lock<std::mutex> lock_gurad(matrixLock_);
    if(color.B > 1)
        color.B = 1;
    if(color.R > 1)
        color.R = 1;
    if(color.G > 1)
        color.G = 1;
    data[3*(resolution_.Width*y + x) + 0] = color.R;
    data[3*(resolution_.Width*y + x) + 1] = color.G;
    data[3*(resolution_.Width*y + x) + 2] = color.B;
}

Color TGAView::GetPixelColor(unsigned int x, unsigned int y)
{
    return Color(data[3*(resolution_.Width*y + x) + 0],
            data[3*(resolution_.Width*y + x) + 1],
            data[3*(resolution_.Width*y + x) + 2]);
}


Resolution TGAView::GetResolution() const
{
    return resolution_;
}

const void TGAView::Export(std::__cxx11::string exportPath)
{
    std::ofstream image(exportPath.c_str(), std::ios::out | std::ios::binary);
    if(!image.is_open())
    {
        std::cerr << "Can't create file for rendered image" << std::endl;
    }
    image.put(0);
    image.put(0);
    image.put(2);
    image.put(0);
    image.put(0);
    image.put(0);
    image.put(0);
    image.put(0);
    image.put(0);
    image.put(0);
    image.put(0);
    image.put(0);
    image.put((this->resolution_.Width & 0x00FF));
    image.put((this->resolution_.Width & 0xFF00) / 256);
    image.put((this->resolution_.Height & 0x00FF));
    image.put((this->resolution_.Height & 0xFF00) / 256);
    image.put(24);
    image.put(0);
    for(int i = 0; i < resolution_.Width * resolution_.Height; ++i)
    {
        image.put((int)std::round((data[i*3 + 2]*255)));
        image.put((int)std::round((data[i*3 + 1]*255)));
        image.put((int)std::round((data[i*3]*255)));
    }
    image.close();
}

const float*TGAView::GetData() const
{
    return data;
}
