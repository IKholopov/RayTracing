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
        Scene(Camera* camera, IView* view, IGeometryHierarchy* hierarchy,
              std::vector<PointLight*> lights,
              std::vector<ISceneObject*> objects,
              std::vector<IMaterial*> materials);
        Scene(Camera* camera, IView* view, IGeometryHierarchy* hierarchy,
              std::vector<PointLight*> lights, LightReference reference,
              std::vector<ISceneObject*> objects,
              std::vector<IMaterial*> materials);
        ~Scene();
        void RenderScene();
        void RenderPixel(unsigned int x, unsigned int y);
        void SetView(IView* view);
        void SetConfig(RenderConfig config);
        const std::vector<ISceneObject*>& GetGeometry() const;
        const std::vector<PointLight*>& GetLights() const;
        const std::vector<IMaterial*>& GetMaterials() const;
        const Camera* GetCamera() const;
        const LightReference& GetLightReference() const;
    private:
        IGeometryHierarchy* hierarchy_;
        IView* view_;
        Camera* camera_;
        std::vector<PointLight*> lights_;
        LightReference reference_;
        RenderConfig config_;
        Color EmitLights(CollisionData& collision);
        std::vector<ISceneObject*> objects_;
        std::vector<IMaterial*> materials_;
};

#endif
