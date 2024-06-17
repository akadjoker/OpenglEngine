#pragma once


#include <SDL2/SDL.h>
#include "Config.hpp"
#include "Input.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "OpenGL.h"

class  CORE_PUBLIC  Device 
{
public:
    Device();
    virtual ~Device();

    bool Create(int width, int height,const char* title,bool vzync=false);
 
    bool Run();

    void Close();

    void Flip();

   
    int GetWidth() const { return m_width; }
    int GetHeight() const { return m_height; }


    void Wait(float ms);
    void SetTargetFPS(int fps);
    int  GetFPS(void);
    float GetFrameTime(void);
    double GetTime(void);
    u32  GetTicks(void) ;

    void SetShouldClose(bool close) { m_shouldclose = close; }

    void SetCloseKey(Sint32 key) { m_closekey = key; }


    SDL_Window*   GetWindow() const { return m_window; }

    static Device& Instance();
    static Device* InstancePtr();
   
private:
    int m_width;
    int m_height;    
    bool m_shouldclose ;
    SDL_Window   *m_window;
    SDL_GLContext m_context;
    static Device* m_singleton;
    double m_current;                 
    double m_previous;                  
    double m_update;                    
    double m_draw;                       
    double m_frame;   
    double m_target;
    bool m_ready;
    Sint32 m_closekey;


};


enum class TRANSFORM_STATE
{
    PROJECTION=0,
    VIEW,
    MODEL
};

enum class CullMode
{
    FRONT=0x0404,
    BACK,
    FRONT_AND_BACK
};



enum class BlendMode
{
    
    BLEND=0,
    ADD,
    MULTIPLY,
    SCREEN,
    SUBTRACT,
    REPLACE,
    LIGHTEN,
    DARKEN,
    LIGHT,
    COUNT
    
};




class  CORE_PUBLIC  Driver 
{
    public:

    Driver();
    virtual ~Driver();


    u32  GetWidth() const { return m_width; }
    u32  GetHeight() const { return m_height; }
    

     void SetCubeTexture(u32 unit, u32 texture);
     void SetDepthTest(bool enable);
     void SetDepthWrite(bool enable);
     void SetDepthClamp(bool enable);
     
     void SetCullFace(bool enable);
     void SetBlend(bool enable);
     void SetBlendMode(BlendMode blendMode);
     void SetScissorTest(bool enable);
     void SetStencilTest(bool enable);
     void SetColorMask(bool r, bool g, bool b, bool a);
     void SetDepthMask(bool enable);
     void SetStencilMask(bool enable);
     void SetBlendFunc(u32 src, u32 dst);
     void SetCullFaceMode(u32 mode);
     void SetFrontFace(u32 mode);
     void SetDepthFunc(u32 func);
     void SetStencilFunc(u32 func, u32 ref, u32 mask);
     void SetStencilOp(u32 sfail, u32 dpfail, u32 dppass);
     void SetScissor(u32 x, u32 y, u32 width, u32 height);
     IntRect GetScissor();
     IntRect GetViewport();
     void SetViewport(u32 x, u32 y, u32 width, u32 height);
     void SetClearColor(u8 r, u8 g, u8 b, u8 a);
     void SetClearColor(f32 r, f32 g, f32 b, f32 a);
     Color GetClearColor();
     void Clear();

     void ResetStats();
     
     void SaveViewport();
     void RestoreViewport();


     void SetShader(u32 shader);
     void SetShader(Shader *shader);
     
     void SetTexture(Texture2D *texture, u32 unit);
     void SetTextureId(u32 unit, u32 texture);



     u32 GetTotalTriangles();
     u32 GetTotalDrawCalls();
     u32 GetTotalVertex();
     u32 GetTotalTextures();
     u32 GetTotalPrograms();


     void  DrawElements(u32 mode, u32 count, u32 type,const void *indices);
     void  DrawArrays(u32 mode, u32 first, u32 count);

    static Driver& Instance();
    static Driver* InstancePtr();

    private:
    friend class Device;



     void Init();
     void resize (int width, int height);


    int m_width;
    int m_height;

    Shader *m_currentShader;
    Texture2D *m_currentTexture[8];
     u32 currentShader;
     u32 currentTexture[8];
     u32 currentCubeTexture[8];
     bool depthTest;
     bool depthWrite;
     bool cullFace;
     bool blend;
     bool scissorTest;
     bool stencilTest;
     bool colorMask[4];
     bool depthMask;
     bool stencilMask;
     bool justDepth;
     bool depthClamp;
     u32 blendSrc;
     u32 blendDst;
     u32 cullFaceMode;
     u32 frontFace;
     u32 depthFunc;
     u32 stencilFunc;
     u32 stencilRef;
     u32 stencilMaskRef;
     IntRect scissor;
     IntRect viewport;
     IntRect save_viewport;
     BlendMode currentMode;
     u32 totalTextures;
     u32 totalCubeTextures;
     u32 totalShaders;
     u32 totalTraingles;
     u32 totalDrawCalls;
     u32 totalVertex;

     Color clearColor;
 
     bool stateMode;
     static Driver* m_singleton;
};


