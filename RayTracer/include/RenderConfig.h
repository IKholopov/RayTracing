#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

class RenderConfig
{
    public:
        RenderConfig();
        unsigned int reflectionDepth() const;
        void setReflectionDepth(unsigned int reflectionDepth);

        bool refraction() const;
        void setRefraction(bool refraction);

        bool reflection() const;
        void setReflection(bool reflection);

        bool light() const;
        void setLight(bool light);

        unsigned int refractionDepth() const;
        void setRefractionDepth(unsigned int refractionDepth);

    private:
        bool light_;
        bool reflection_;
        bool refraction_;
        unsigned int reflectionDepth_;
        unsigned int refractionDepth_;
};

#endif
