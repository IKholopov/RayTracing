#ifndef TGA_VIEW_H
#define TGA_VIEW_H

#include <mutex>

#include "Base.h"
#include "IView.h"

class TGAView:public IView
{
    public:
        TGAView(unsigned int width, unsigned int height);
        ~TGAView();

        void UpdatePixel(unsigned int x, unsigned int y, Color color);
        Color GetPixelColor(unsigned int x, unsigned int y);
        Resolution GetResolution();

        const float* GetData() const;
        Resolution GetResolution() const;
        const void Export(std::string exportPath);
    private:
        std::mutex matrixLock_;
        Resolution resolution_;
        float* data;

};

#endif
