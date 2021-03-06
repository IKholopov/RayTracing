#ifndef I_VIEW_H
#define I_VIEW_H

#include "Base.h"

class IView
{
    public:
        virtual ~IView() {};
        virtual void UpdatePixel(unsigned int x, unsigned int y, Color color) = 0;
        virtual Color GetPixelColor(unsigned int x, unsigned int y) = 0;
        virtual Resolution GetResolution() const = 0;
};

#endif
