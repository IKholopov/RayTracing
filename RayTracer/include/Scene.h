#ifndef SCENE_H
#define SCENE_H

#include "Base.h"

class Scene
{
    public:
        Scene(Box size, Camera camera);
    private:
        IView* view;
        Camera camera_;


};

#endif
