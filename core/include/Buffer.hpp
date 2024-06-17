
#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Config.hpp"
#include "Texture.hpp"
#include "OpenGL.h"
#include "Utils.hpp"
#include "Device.hpp"

class CORE_PUBLIC TextureBuffer
{

    public:

    bool Init(unsigned int width, unsigned int height);
    void Begin();
    void End();
    void BindTexture(int layer );
    void Release();

    private:
        unsigned int frameFBO;
        unsigned int renderFBO;
        unsigned int map;
        unsigned int width;
        unsigned int height;
};

class CORE_PUBLIC DepthBuffer
{
     public:

        bool Init(unsigned int width, unsigned int height);
        void Begin();
        void End();
        void BindTexture(int layer );
        void Release();
    private:
        unsigned int depthMapFBO;
        unsigned int depthMap;
        unsigned int width;
        unsigned int height;
};

class CORE_PUBLIC CubeBuffer
{
     public:

        bool Init(unsigned int width, unsigned int height);
        void Begin();
        void End();
        void BindTexture(int layer );
        void Release();
    private:
        unsigned int depthMapFBO;
        unsigned int depthCubemap;
        unsigned int width;
        unsigned int height;
};



class CORE_PUBLIC MultiBuffer
{
     public:

        bool Init(unsigned int width, unsigned int height);
        void Begin();
        void End();
        
        void Render(int Width, int Height);
        void Release();
        void Bind();
    private:
        unsigned int gBuffer;
        unsigned int rboDepth;
        unsigned int gPosition, gNormal, gAlbedoSpec;
        unsigned int width;
        unsigned int height;
};

class CORE_PUBLIC RenderQuad
{
    public:

    bool Init(unsigned int width, unsigned int height);
    void Render(float x, float y, float size);
    void Render();
    void Release();


    private:
        unsigned int quadVAO;
        unsigned int quadVBO;
        unsigned int width;
        unsigned int height;
        

};


class CORE_PUBLIC TextureCascade
{

    public:
     void Init(u32 count,u32 width, u32 height);
     void Release();
     void Bind(u32 start);
     void Set( u32 id);

     operator u32*() const {return m_textures;}

     private:
        u32 *m_textures;
        u32 m_width;
        u32 m_height;
        u32 m_count;
};


class CORE_PUBLIC CascadeShadow
{

        public:

        bool Init(u32 count,unsigned int width, unsigned int height);
        void Begin();
        void End();
        void Set(u32 index);

        u32 count() const {return m_count;}

        void BindTextures(u32 start);
        void BindTexture(u32 id);
        void Release();
        private:
            unsigned int depthMapFBO;
            unsigned int width;
            unsigned int height;
            u32 m_count;
        TextureCascade textures;
    

};