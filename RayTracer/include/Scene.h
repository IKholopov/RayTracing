#ifndef SCENE_H
#define SCENE_H

#include "Base.h"
#include "Camera.h"
#include "IView.h"
#include "IGeometryHierarchy.h"
#include "ThreadPool.h"
#include "RenderConfig.h"

class Scene
{
    public:
        Scene(Camera camera, IView* view, IGeometryHierarchy* hierarchy, std::vector<PointLight*> lights);
        Scene(Camera camera, IView* view, IGeometryHierarchy* hierarchy, std::vector<PointLight*> lights, LightReference reference);
        void RenderScene();
        void RenderPixel(unsigned int x, unsigned int y);
        void SetView(IView* view);
        void SetConfig(RenderConfig config);
    private:
        IGeometryHierarchy* hierarchy_;
        IView* view_;
        Camera camera_;
        std::vector<PointLight*> lights_;
        LightReference reference_;
        RenderConfig config_;
        Color EmitLights(CollisionData& collision);
};

#endif
