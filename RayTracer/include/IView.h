#ifndef I_VIEW_H
#define I_VIEW_H

#include "Base.h"

class IView
{
    public:
        virtual ~IView() {};
        virtual void UpdatePixel(unsigned int x, unsigned int y, Point color) = 0;
};

#endif
