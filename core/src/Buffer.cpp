
#include "pch.h"
#include "Buffer.hpp"


bool TextureBuffer::Init(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;

        
   
   
    glGenTextures(1, &map);
    glBindTexture(GL_TEXTURE_2D, map);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);


    glGenFramebuffers(1, &frameFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, frameFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, map, 0);
   


        



    glGenRenderbuffers(1, &renderFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, renderFBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderFBO);

        





     GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
     glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LogError("Could not create FrameBuffer");
        DEBUG_BREAK_IF(true);
        return false;
    }



    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;

}

void TextureBuffer::Begin() 
{
    glBindFramebuffer(GL_FRAMEBUFFER, frameFBO);
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glViewport(0, 0, width, height);
    
}

void TextureBuffer::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void TextureBuffer::Release()
{
    glDeleteFramebuffers(1, &frameFBO);
    glDeleteFramebuffers(1, &renderFBO);
    glDeleteTextures(1, &map);
}

void TextureBuffer::BindTexture(int layer)
{

    glActiveTexture(GL_TEXTURE0 + layer);
    glBindTexture(GL_TEXTURE_2D, map);

}



//*******************************************************************************
//
//*******************************************************************************

bool DepthBuffer::Init(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    
    
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture

    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    float borderColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);  

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void DepthBuffer::Begin()
{
    
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
    
}

void DepthBuffer::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void DepthBuffer::BindTexture(int layer )
{
    glActiveTexture(GL_TEXTURE0 + layer);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}

void DepthBuffer::Release()
{
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthMap);
    
}




//*******************************************************************************
//CubeBuffer
//*******************************************************************************

bool CubeBuffer::Init(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    

    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture

    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
    {
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void CubeBuffer::Begin()
{
    
      //  Driver::Instance().SaveViewport();
        Driver::Instance().SetViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
    
}

void CubeBuffer::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
   // Driver::Instance().RestoreViewport();
}

void CubeBuffer::BindTexture(int layer )
{
    glActiveTexture(GL_TEXTURE0 + layer);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
}

void CubeBuffer::Release()
{
    glDeleteFramebuffers(1, &depthMapFBO);
    glDeleteTextures(1, &depthCubemap);
    
}
//*******************************************************************************
//
//*******************************************************************************



bool MultiBuffer::Init(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
     
    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    
    // position color buffer
    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
    
    // normal color buffer
    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
  
    // color + specular color buffer
    glGenTextures(1, &gAlbedoSpec);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);


    unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
    glDrawBuffers(3, attachments);
    
    // create and attach depth buffer (renderbuffer)
    
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        LogError("Framebuffer not complete!");
        return false;
    }
        
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void MultiBuffer::Begin()
{
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void MultiBuffer::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MultiBuffer::Release()
{
    glDeleteFramebuffers(1, &gBuffer);
    glDeleteRenderbuffers(1, &rboDepth);
    glDeleteTextures(1, &gPosition);
    glDeleteTextures(1, &gNormal);
    glDeleteTextures(1, &gAlbedoSpec);
}

void MultiBuffer::Render(int Width, int Height)
{
        glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); 
        glBlitFramebuffer(0, 0, width, height, 0, 0, Width, Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void MultiBuffer::Bind()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, gPosition);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, gNormal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
}


//*******************************************************************************
//
// RenderQuad
//
//*******************************************************************************




bool RenderQuad::Init(unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;


    float quadVertices[] = 
    {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

    

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glBindVertexArray(0);
        return true;


}

void RenderQuad::Render()
{
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void RenderQuad::Release()
{
    glDeleteVertexArrays(1, &quadVAO);
    glDeleteBuffers(1, &quadVBO);

}

void RenderQuad::Render(float x, float y, float size)
{

    float w = size * 0.5f;
    float h = size * 0.5f;
    float quadVertices[] =
    {
            // positions        // texture Coords
             x-w,  y+h, 0.0f, 0.0f, 1.0f,
             x-w,  y-h, 0.0f, 0.0f, 0.0f,
             x+w,  y+h, 0.0f, 1.0f, 1.0f,
             x+w,  y-h, 0.0f, 1.0f, 0.0f,
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

//****************************************************************************************
//
//********************************************************************************

void TextureCascade::Init(u32 count,u32 width, u32 height)
{
    m_width = width;
    m_height = height;
    m_count = count;
    m_textures = new u32[count];
    glGenTextures(count, m_textures);

    for (u32 i = 0; i < count; i++)
    {
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
    }
}

void TextureCascade::Bind(u32 start)
{
    for (int i = 0; i < m_count; i++)
    {
        glActiveTexture(GL_TEXTURE0 + start + i);
        glBindTexture(GL_TEXTURE_2D, m_textures[i]);
    }


}

void TextureCascade::Set( u32 id)
{
        DEBUG_BREAK_IF(id >= m_count);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_textures[id]);
    
}

void TextureCascade::Release()
{
    for (u32 i = 0; i < m_count; i++)
    {
        glDeleteTextures(1, &m_textures[i]);
    }
    delete[] m_textures;
}

//****************************************************************************************
//
//********************************************************************************



bool CascadeShadow::Init(u32 count,unsigned int width, unsigned int height)
{
    this->width = width;
    this->height = height;
    m_count = count;
    textures.Init(count, width, height);
    
    
    glGenFramebuffers(1, &depthMapFBO);
    // create depth texture

    

    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[0], 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);


    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) 
    {
        LogError("Could not create FrameBuffer");
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return true;
}

void CascadeShadow::Begin()
{
    
        glViewport(0, 0, width, height);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        
    
}

void CascadeShadow::Set(u32 index)
{
    
    
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, textures[index], 0);
    glClear(GL_DEPTH_BUFFER_BIT);

    
    
}



void CascadeShadow::End()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CascadeShadow::BindTextures(u32 start)
{
    textures.Bind(start);       
}
void CascadeShadow::BindTexture(u32 id)
{
    textures.Set(id);       
}

void CascadeShadow::Release()
{
    glDeleteFramebuffers(1, &depthMapFBO);
    textures.Release();
    
}
