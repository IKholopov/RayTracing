#ifndef GL_VIEW_H
#define GL_VIEW_H

#include <mutex>

#include "Base.h"
#include "IView.h"

class GLView:public IView
{
    public:
        GLView(unsigned int width, unsigned int height);
        ~GLView();

        void UpdatePixel(unsigned int x, unsigned int y, Color color);
        Color GetPixelColor(unsigned int x, unsigned int y);
        Resolution GetResolution();

        const float* GetData() const;
        Resolution GetResolution() const;
    private:
        std::mutex matrixLock_;
        Resolution resolution_;
        float* data;

};

#endif
