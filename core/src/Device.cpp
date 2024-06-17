
#include "pch.h"
#include "Device.hpp"



double GetTime()
{
    return  static_cast<double> (SDL_GetTicks())/1000;
}


void glDebugOutput(u32 source,
                   u32 type,
                   u32 id,
                   u32 severity,
                   s32 length,
                   const char *message,
                   const void *userParam)
{
    (void)length;
    (void)userParam;
    (void)severity;
    GLuint ignore_ids[2] = { 131185,1 }; //1 shader ???

    for (int i = 0 ; i <(int) ARRAY_SIZE_IN_ELEMENTS(ignore_ids) ; i++) 
    {
        if (id == ignore_ids[i]) 
        {
            return;
        }
    }

  //  SDL_LogWarn(1,"!!! Debug callback !!!\n");
    SDL_LogDebug(2,"Debug message: id %d, %s", id, message);

    std::string msg = std::string(message);
    std::string src ;
    std::string stype;

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             src=("API"); break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   src=("Window System"); break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: src=("Shader Compiler"); break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     src=("Third Party"); break;
    case GL_DEBUG_SOURCE_APPLICATION:     src=("Application"); break;
    case GL_DEBUG_SOURCE_OTHER:           src=("Other"); break;
    }

  //  printf("Error type: ");
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               stype =("Error"); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: stype =("Deprecated Behaviour"); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  stype =("Undefined Behaviour"); break;
    case GL_DEBUG_TYPE_PORTABILITY:         stype =("Portability"); break;
    case GL_DEBUG_TYPE_PERFORMANCE:         stype =("Performance"); break;
    case GL_DEBUG_TYPE_MARKER:              stype =("Marker"); break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          stype =("Push Group"); break;
    case GL_DEBUG_TYPE_POP_GROUP:           stype =("Pop Group"); break;
    case GL_DEBUG_TYPE_OTHER:               stype =("Other"); break;
    }

    LogError("Source: [%s]: Type: {%s}: Message: (%s) ID - %d", src.c_str(),stype.c_str(),msg.c_str(),id);

}


//*************************************************************************************************
// Device
//*************************************************************************************************

Device*  Device::m_singleton = 0x0;


  

Device& Device::Instance()
{
    DEBUG_BREAK_IF(!m_singleton);
    return (*m_singleton);
}
Device* Device::InstancePtr()
{
    return m_singleton;

}

Device::Device():m_width(0), m_height(0)
{
    LogInfo("[DEVICE] Initialized.");
    m_singleton = this;
    m_shouldclose = false;
    m_window = NULL;
    m_context = NULL;
    m_current = 0;
    m_previous = 0;
    m_update = 0;
    m_draw = 0;
    m_frame = 0;
    m_target = 0;
    m_ready = false;
    
}

Device::~Device()
{
    LogInfo("[DEVICE] Destroyed.");
    m_singleton = 0x0;
    Close();
}

bool Device::Create(int width, int height,const char* title,bool vzync)
{
   
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
       LogError( "SDL could not initialize! SDL_Error: %s" , SDL_GetError());
       return false;
    }
    else
    {

            m_current = 0;
            m_previous = 0;
            m_update = 0;
            m_draw = 0;
            m_frame = 0;
            SetTargetFPS(60*10);
            m_closekey = 256;
            m_width = width;
            m_height = height;

             m_current = GetTime();
            m_draw = m_current - m_previous;
            m_previous = m_current;
            m_frame = m_update + m_draw;



        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
       SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);

        m_window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
        if (m_window == NULL)
        {
            LogError( "Window could not be created! SDL_Error: %s" , SDL_GetError());
            return false;
        }
            LogInfo( "[DEVICE] Created.");

            // Logger::Instance();
            // System::Instance();
            // Driver::Instance();
                             
                               
         

            m_context = SDL_GL_CreateContext(m_window);
            if (m_context == NULL)
            {
                LogError( "OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
                return false ;
            } else
            {
                 LogInfo( "[DEVICE] OpenGL context created!");
                           
                if (vzync)
                    SDL_GL_SetSwapInterval(1);
                else
                    SDL_GL_SetSwapInterval(0) ;
                   if (initOpenGLExtensions(false))
                    {
 
                  
                                glEnable(GL_DEBUG_OUTPUT);
                                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                                glDebugMessageCallback(glDebugOutput, nullptr);
                                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
                                LogInfo("[DEVICE] Vendor  :  %s",glGetString(GL_VENDOR));
                                LogInfo("[DEVICE] Renderer:  %s",glGetString(GL_RENDERER));
                                LogInfo("[DEVICE] Version :  %s",glGetString(GL_VERSION));

                                LogInfo("[DEVICE] GLSL Version: %s",glGetString(GL_SHADING_LANGUAGE_VERSION));

                                Driver::Instance().Init();

                              
                                TextureManager::Instance().Init();
                                          Driver::Instance().resize(width, height);
                            
                                glEnable(GL_MULTISAMPLE);

                                

                        


                    } else 
                    {
                        LogError( "Failed to load OpenGL extensions");
                        return false;
                    }
                                
                }         
              m_ready = true;
   

    }

   



    return true;

}



void Device::Wait(float ms)
{
    SDL_Delay(ms);
}




int Device::GetFPS(void)
{
    #define FPS_CAPTURE_FRAMES_COUNT    30      // 30 captures
    #define FPS_AVERAGE_TIME_SECONDS   0.5f     // 500 millisecondes
    #define FPS_STEP (FPS_AVERAGE_TIME_SECONDS/FPS_CAPTURE_FRAMES_COUNT)

    static int index = 0;
    static float history[FPS_CAPTURE_FRAMES_COUNT] = { 0 };
    static float average = 0, last = 0;
    float fpsFrame = GetFrameTime();

    if (fpsFrame == 0) return 0;

    if ((GetTime() - last) > FPS_STEP)
    {
        last = (float)GetTime();
        index = (index + 1)%FPS_CAPTURE_FRAMES_COUNT;
        average -= history[index];
        history[index] = fpsFrame/FPS_CAPTURE_FRAMES_COUNT;
        average += history[index];
    }

    return (int)roundf(1.0f/average);
}

void Device::SetTargetFPS(int fps)
{
    if (fps < 1) m_target = 0.0;
    else m_target = 1.0/(double)fps;

    
}

float Device::GetFrameTime(void)
{
    return (float)m_frame;
}

double Device::GetTime(void)
{
    return (double) SDL_GetTicks()/1000.0;
}

u32 Device::GetTicks(void)
{
    return SDL_GetTicks();
}


bool Device::Run()
{       
   if (!m_ready) return false;

        m_current = GetTime();            // Number of elapsed seconds since InitTimer()
        m_update = m_current - m_previous;
        m_previous = m_current;
  

    SDL_Event event;
 
    while (SDL_PollEvent(&event) != 0)
    {
        
        switch (event.type)
        {
            case SDL_QUIT:
            {
                m_shouldclose = true;
                break;
            }
            case SDL_WINDOWEVENT:
            {
                switch (event.window.event)
                {
                    case SDL_WINDOWEVENT_RESIZED:
                    {
                        m_width = event.window.data1;
                        m_height = event.window.data2;
                            Driver::Instance().resize(m_width, m_height);
                      
                        break;
                    }
                }
                break;
            }
            case SDL_KEYDOWN:
            {

                
                
               // LogWarning("[DEVICE] Key %d  %d",Keyboard::toKey(event.key.keysym.scancode),m_closekey);
                if (Keyboard::toKey(event.key.keysym.scancode)==m_closekey)
                {
                    m_shouldclose = true;
                    break;
                }
               
               Keyboard::setKeyState(event.key.keysym.scancode, true);
        
                break;
            }
            
            case SDL_KEYUP:
            {
               
                Keyboard::setKeyState(event.key.keysym.scancode, false);
                break;
            
            }
            break;
            case SDL_MOUSEBUTTONDOWN:
            {
               
                int btn = event.button.button - 1;
                if (btn == 2) btn = 1;
                else if (btn == 1) btn = 2;
                Mouse::setMouseButton(btn, true);
            }break;
            case SDL_MOUSEBUTTONUP:
            {
                int btn = event.button.button - 1;
                if (btn == 2) btn = 1;
                else if (btn == 1) btn = 2;
                Mouse::setMouseButton(btn, false);
                
                break;
            }
            case SDL_MOUSEMOTION:
            {
               Mouse::setMousePosition(event.motion.x, event.motion.y,event.motion.xrel, event.motion.yrel);
            break;   
            }
            
            case SDL_MOUSEWHEEL:
            {
                Mouse::setMouseWheel(event.wheel.x, event.wheel.y);
                break;
            }
        }

    }
    

    return !m_shouldclose;
}

void Device::Close()
{ 
    if (!m_ready) return;

    m_ready = false;



    SDL_GL_DeleteContext(m_context);
    SDL_DestroyWindow(m_window);

    m_window = NULL;
    LogInfo( "[DEVICE] closed!");
    SDL_Quit();
}



void Device::Flip()
{
   

    SDL_GL_SwapWindow(m_window);

    m_current = GetTime();
    m_draw = m_current - m_previous;
    m_previous = m_current;
    m_frame = m_update + m_draw;



    // Wait for some milliseconds...
    if (m_frame < m_target)
    {
        Wait((float)(m_target -m_frame)*1000.0f);

        m_current = GetTime();
        double waitTime = m_current - m_previous;
        m_previous = m_current;

        m_frame += waitTime;      // Total frame time: update + draw + wait
    }

    // std::string fps= std::to_string(GetFPS());
    // std::string title = std::string("FPS: ") + fps;
    // SDL_SetWindowTitle(m_window,  title.c_str());

    Keyboard::Update();
    Mouse::Update();

}



//*************************************************************************************************
// Driver
//*************************************************************************************************

 Driver*  Driver::m_singleton = 0x0;


Driver& Driver::Instance()
{
    DEBUG_BREAK_IF(!m_singleton);
    return *m_singleton;
}
Driver* Driver::InstancePtr()
{
    return m_singleton;

}

Driver::Driver()
{
    if (m_singleton!=0x0)
    {
        LogError("[DRIVER] Already initialized.");
        return;
    }
    m_singleton = this;
    Init();
}

Driver::~Driver()
{
    LogInfo("[DRIVER] Destroyed.");
    m_singleton = 0x0;
}

void Driver::Init()
{
    LogInfo("[DRIVER] Initialized.");
     currentShader = 0;
    for (int i = 0; i < 8; i++)
    {
        currentTexture[i] = 0;
        currentCubeTexture[i] = 0;
        m_currentTexture[i] = nullptr;
    }
    depthTest = false;
    depthWrite = false;
    cullFace = false;
    blend = false;
    scissorTest = false;
    stencilTest = false;
    depthClamp = false;
    for (int i = 0; i < 4; i++)
    {
        colorMask[i] = true;
    }
    totalTextures=0;
    totalCubeTextures=0;
    totalShaders=0;
    totalTraingles=0;
    totalDrawCalls=0;
    totalVertex=0;
    depthMask = true;
    
    stencilMask = false;
    justDepth = false;
    stateMode=false;
    blendSrc = 0;
    blendDst = 0;
    cullFaceMode = 0;
    frontFace = 0;
    depthFunc = 0;
    stencilFunc = 0;
    stencilRef = 0;
    stencilMaskRef = 0xFF;


    currentMode = BlendMode::COUNT;



    clearColor.Set(0, 0, 0, 1);
    viewport.Set(0, 0, 0, 0);
    scissor.Set(0, 0, 0, 0);

    m_currentShader = nullptr;
    
}

void Driver::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

u32 Driver::GetTotalDrawCalls()
{
    return totalDrawCalls;
}

u32 Driver::GetTotalVertex()
{
    return totalVertex;
}

u32 Driver::GetTotalTextures()
{
    return totalTextures;
}


u32 Driver::GetTotalPrograms()
{
    return totalShaders;
}

u32 Driver::GetTotalTriangles()
{
    return totalTraingles;
}



void Driver::ResetStats()
{
totalTextures=0;
totalCubeTextures=0;
totalShaders=0;
totalTraingles=0;
totalDrawCalls=0;
totalVertex=0;
}



void Driver::SetShader(u32 shader)
{
    if (!stateMode)
    {
        glUseProgram(shader);
        totalShaders++;
        currentShader = shader;
        return;
    }
    if (currentShader != shader )
    {
        glUseProgram(shader);
        totalShaders++;
        currentShader = shader;
    }
}

void Driver::SetTextureId(u32 unit, u32 texture)
{
     if (!stateMode)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_2D, texture);
        totalTextures++;
        currentTexture[unit] = texture;
        return;
    
    }
if (currentTexture[unit] != texture )
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, texture);
    totalTextures++;
    currentTexture[unit] = texture;
}
}

void Driver::SetCubeTexture(u32 unit, u32 texture)
{
     if (!stateMode)
    {
        glActiveTexture(GL_TEXTURE0 + unit);
        glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
        totalCubeTextures++;
        currentCubeTexture[unit] = texture;
        return;
    
    }

if (currentCubeTexture[unit] != texture )
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
    totalCubeTextures++;
    currentCubeTexture[unit] = texture;
}

}

void Driver::SetDepthTest(bool enable)
{

     if (!stateMode)
    {
        if (enable)
        {
            glEnable(GL_DEPTH_TEST);
        }
        else
        {
            glDisable(GL_DEPTH_TEST);
        }
        depthTest = enable;
        return;
    }


if (depthTest != enable)
{
    if (enable)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    depthTest = enable;
}
}

void Driver::SetDepthWrite(bool enable)
{
     if (!stateMode)
    {
        if (depthWrite != enable)
        {
            glDepthMask(enable);
            depthWrite = enable;
        }
        return;
    }
    if (depthWrite != enable)
    {
        glDepthMask(enable);
        depthWrite = enable;
    }
}

void Driver::SetDepthClamp(bool enable)
{

     if (!stateMode)
    {
        if (enable)
        {
            glDisable(GL_DEPTH_CLAMP);
        }
        else
        {
            glEnable(GL_DEPTH_CLAMP);
        }
        return;
    }
    if (depthClamp != enable)
    {
        if (enable)
        {
            glDisable(GL_DEPTH_CLAMP);
        }
        else
        {
            glEnable(GL_DEPTH_CLAMP);
        }
        depthClamp = enable;
    }
}

void Driver::SetCullFace(bool enable)
{

    if (!stateMode)
    {
        if (enable)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
        cullFace = enable;
        return;
    }

    if (cullFace != enable)
    {
        if (enable)
        {
            glEnable(GL_CULL_FACE);
        }
        else
        {
            glDisable(GL_CULL_FACE);
        }
        cullFace = enable;
    }
}

void Driver::SetBlend(bool enable)
{

     if (!stateMode)
    {
        if (enable)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        blend = enable;
        return;
    }

    if (blend != enable )
    {
        if (enable)
        {
            glEnable(GL_BLEND);
        }
        else
        {
            glDisable(GL_BLEND);
        }
        blend = enable;
    }
}

void Driver::SetScissorTest(bool enable)
{
     if (!stateMode)
    {
        if (enable)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
        scissorTest = enable;
        return;
    }
        
    if (scissorTest != enable)
    {
        if (enable)
        {
            glEnable(GL_SCISSOR_TEST);
        }
        else
        {
            glDisable(GL_SCISSOR_TEST);
        }
        scissorTest = enable;
    }
}

void Driver::SetStencilTest(bool enable)
{
     if (!stateMode)
    {
        if (enable)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }
        stencilTest = enable;
        return;
    }
    if (stencilTest != enable )
    {
        if (enable)
        {
            glEnable(GL_STENCIL_TEST);
        }
        else
        {
            glDisable(GL_STENCIL_TEST);
        }
        stencilTest = enable;
    }
}

void Driver::SetColorMask(bool r, bool g, bool b, bool a)
{

    if (!stateMode)
    {
        if (colorMask[0] != r || colorMask[1] != g || colorMask[2] != b || colorMask[3] != a )
        {
            glColorMask(r, g, b, a);
            colorMask[0] = r;
            colorMask[1] = g;
            colorMask[2] = b;
            colorMask[3] = a;
        }
        return;
    }

    if (colorMask[0] != r || colorMask[1] != g || colorMask[2] != b || colorMask[3] != a )
    {
        glColorMask(r, g, b, a);
        colorMask[0] = r;
        colorMask[1] = g;
        colorMask[2] = b;
        colorMask[3] = a;
    }
}

void Driver::SetDepthMask(bool enable)
{
     if (!stateMode)
    {
        if (depthMask != enable)
        {
            glDepthMask(enable);
            depthMask = enable;
        }
        return;
    }
    if (depthMask != enable  )
    {
        glDepthMask(enable);
        depthMask = enable;
    }
}

void Driver::SetStencilMask(bool enable)
{
     if (!stateMode)
    {
        if (stencilMask != enable)
        {
            glStencilMask(enable);
            stencilMask = enable;
        }
        return;
    }
    if (stencilMask != enable)
    {
        glStencilMask(enable);
        stencilMask = enable;
    }
}

void Driver::SetBlendFunc(u32 src, u32 dst)
{
     if (!stateMode)
    {
        if (blendSrc != src || blendDst != dst )
        {
            glBlendFunc(src, dst);
            blendSrc = src;
            blendDst = dst;
        }
        return;
    }
    if (blendSrc != src || blendDst != dst )
    {
        glBlendFunc(src, dst);
        blendSrc = src;
        blendDst = dst;
    }
}

void Driver::SetCullFaceMode(u32 mode)
{
     if (!stateMode)
    {
        if (cullFaceMode != mode )
        {
            glCullFace(mode);
            cullFaceMode = mode;
        }
        return;
    }
    if (cullFaceMode != mode )
    {
        glCullFace(mode);
        cullFaceMode = mode;
    }
}

void Driver::SetFrontFace(u32 mode)
{
     if (!stateMode)
    {
        if (frontFace != mode)
        {
            glFrontFace(mode);
            frontFace = mode;
        }
        return;
    }
    if (frontFace != mode)
    {
        glFrontFace(mode);
        frontFace = mode;
    }
}

void Driver::SetDepthFunc(u32 func)
{
     if (!stateMode)
    {
        if (depthFunc != func)
        {
            glDepthFunc(func);
            depthFunc = func;
        }
        return;
    }

    if (depthFunc != func)
    {
        glDepthFunc(func);
        depthFunc = func;
    }
}

void Driver::SetStencilFunc(u32 func, u32 ref, u32 mask)
{
     if (!stateMode)
    {
        if (stencilFunc != func || stencilRef != ref || stencilMaskRef != mask)
        {
            glStencilFunc(func, ref, mask);
            stencilFunc = func;
            stencilRef = ref;
            stencilMaskRef = mask;
        }
        return;
    }
    if (stencilFunc != func || stencilRef != ref || stencilMaskRef != mask)
    {
        glStencilFunc(func, ref, mask);
        stencilFunc = func;
        stencilRef = ref;
        stencilMaskRef = mask;
    }
}

void Driver::SetStencilOp(u32 sfail, u32 dpfail, u32 dppass)
{
glStencilOp(sfail, dpfail, dppass);
}

void Driver::SetScissor(u32 x, u32 y, u32 width, u32 height)
{

int yy = m_height - y - height;

glScissor(x, yy, width, height);
scissor.Set(x, y, width, height);
}

IntRect Driver::GetScissor()
{
return scissor;
}

IntRect Driver::GetViewport()
{
return viewport;

}

void Driver::SetViewport(u32 x, u32 y, u32 width, u32 height)
{
glViewport(x, y, width, height);
viewport.Set(x, y, width, height);
}

void Driver::SaveViewport()
{
     save_viewport = viewport;
}

void Driver::RestoreViewport()
{
    SetViewport(save_viewport.x, save_viewport.y, save_viewport.width, save_viewport.height);
}



void Driver::SetClearColor(f32 r, f32 g, f32 b, f32 a)
{
    clearColor.Set(r * 255, g * 255, b * 255, a * 255);
    glClearColor(r, g, b, a);

}

void Driver::SetClearColor(u8 r, u8 g, u8 b, u8 a)
{
    clearColor.Set(r, g, b, a);
    float floatRed = static_cast<float>(r) / 255.0f;
    float floatGreen = static_cast<float>(g) / 255.0f;
    float floatBlue = static_cast<float>(b) / 255.0f;
    float floatAlpha = static_cast<float>(a) / 255.0f;

    glClearColor(floatRed, floatGreen, floatBlue, floatAlpha);
}

Color Driver::GetClearColor()
{
    return clearColor;
}



void Driver::Clear()
{
    u32 mask = GL_COLOR_BUFFER_BIT;
    if (depthMask)
    {
        mask |= GL_DEPTH_BUFFER_BIT;
    }
    if (stencilMask)
    {
        mask |= GL_STENCIL_BUFFER_BIT;
    }

    glClear(mask);
}

 void  Driver::DrawElements(u32 mode, u32 count, u32 type,const void *indices)
{
    glDrawElements(mode, count, type, indices);
    if (mode == GL_TRIANGLES)
    {
		totalTraingles += count / 3;
	}

    totalDrawCalls++;
    totalVertex += count;
}

void Driver::DrawArrays(u32 mode, u32 first, u32 count)
{
    glDrawArrays(mode, first, count);
    if (mode == GL_TRIANGLES)
    {
        totalTraingles += count / 3;
    }
    totalDrawCalls++;
    totalVertex += count;
}

void Driver::SetBlendMode(BlendMode blendMode)
{
    if (!blend) return ;
    if (currentMode == blendMode) return;

    switch (blendMode)
    {
    case BlendMode::BLEND:
        SetBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case BlendMode::ADD:
        SetBlendFunc(GL_ONE, GL_ONE);
        break;
    case BlendMode::MULTIPLY:
        SetBlendFunc(GL_DST_COLOR, GL_ZERO);
        break;
    case BlendMode::SCREEN:
        SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
        break;
    case BlendMode::SUBTRACT:
        SetBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);
        break;
    case BlendMode::REPLACE:
        SetBlendFunc(GL_ONE, GL_ZERO);
        break;
    case BlendMode::LIGHTEN:
        SetBlendFunc(GL_ONE, GL_ONE);
        break;
    case BlendMode::DARKEN:
        SetBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case BlendMode::LIGHT:
        SetBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        break;
    case BlendMode::COUNT:
        break;
    }
    currentMode = blendMode;
}