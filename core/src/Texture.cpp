#include "pch.h"
#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"




const Color Color::WHITE;
const Color Color::GRAY(0.5f,0.5f,0.5f);
const Color Color::BLACK(0.0f,0.0f,0.0f);
const Color Color::RED(1.0f,0.0f,0.0f);
const Color Color::GREEN(0.0f,1.0f,0.0f);
const Color Color::BLUE(0.0f,0.0f,1.0f);
const Color Color::CYAN(0.0f,1.0f,1.0f);
const Color Color::MAGENTA(1.0f,0.0f,1.0f);
const Color Color::YELLOW(1.0f,1.0f,0.0f);
const Color Color::TRANSPARENT(0.0f,0.0f,0.0f,0.0f);

Texture::Texture():Ref(), HorizontalWrap(WrapMode::Repeat),
        VerticalWrap(WrapMode::Repeat),
        MinificationFilter(FilterMode::NearestMipLinear),
        MagnificationFilter(FilterMode::Linear),
        MaxAnisotropic(0.0f)
        {
      

        id = 0;
        width = 0;
        height = 0;
        setDebugName("Texture");
    }

Texture::~Texture()
{
    
    Release();
}

void Texture::SetMinFilter(FilterMode filter)
{
    this->MinificationFilter = filter;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinificationFilter);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::SetMagFilter(FilterMode filter)
{
    this->MagnificationFilter = filter;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagnificationFilter);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::SetWrapS(WrapMode mode)
{
    this->HorizontalWrap = mode;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, HorizontalWrap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::SetWrapT(WrapMode mode)
{
    this->VerticalWrap = mode;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, VerticalWrap);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::SetAnisotropicFiltering(float level)
{
    this->MaxAnisotropic = level;
    if (id != 0)
    {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, MaxAnisotropic);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void Texture::Release()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
        LogInfo("Texture: [ID %i] Release", id);
        id = 0;
    }
}




void Texture::createTexture()
{
    if (id != 0)
    {
        glDeleteTextures(1, &id);
    }
    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_2D, id);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, HorizontalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, VerticalWrap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinificationFilter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagnificationFilter);
    if (MaxAnisotropic > 0.0f)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, MaxAnisotropic);
    }
    
}


void Texture::Use(u32 unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}
void Texture::Update(const Pixmap &pixmap)
{
    if (pixmap.pixels)
    {
        u32 components = pixmap.components;
        u32 width = pixmap.width;
        u32 height = pixmap.height;

        GLenum format=GL_RGBA;
        GLenum glFormat = GL_RGBA;

        switch (components) 
        {
            case STBI_grey:
            {
                format =    GL_R8;
                glFormat = GL_RED;
                break;
            }
            case STBI_grey_alpha:
            {
                format = GL_RG8;
                glFormat = GL_RG;
                break;
            }
            case STBI_rgb:
            {
                format = GL_RGB;
                glFormat = GL_RGB;

                break;
            }
            case STBI_rgb_alpha:
            {
                format = GL_RGBA8;
                glFormat = GL_RGBA;
                break;
            }
        }

        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, pixmap.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        
    }
}
void Texture::Update(const unsigned char *buffer, u16 components, int width, int height)
{
    if (buffer)
    {
        GLenum format=GL_RGBA;
        GLenum glFormat = GL_RGBA;
        switch (components) 
        {
            case STBI_grey:
            {
                format =    GL_R8;
                glFormat = GL_RED;
                break;
            }
            case STBI_grey_alpha:
            {
                format = GL_RG8;
                glFormat = GL_RG;
                break;
            }
            case STBI_rgb:
            {
                format = GL_RGB;
                glFormat = GL_RGB;

                break;
            }
            case STBI_rgb_alpha:
            {
                format = GL_RGBA8;
                glFormat = GL_RGBA;
                break;
            }
        }
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}





Texture2D::Texture2D():Texture()
{

}


Texture2D::Texture2D(const Pixmap &pixmap):Texture()
{

        components = pixmap.components;
        width = pixmap.width;
        height = pixmap.height;



    GLenum format=GL_RGBA;
    GLenum glFormat = GL_RGBA;
    switch (components) 
    {
        case STBI_grey:
        {
            format =    GL_R8;
            glFormat = GL_RED;
            break;
        }
        case STBI_grey_alpha:
        {
            format = GL_RG8;
            glFormat = GL_RG;
            break;
        }
        case STBI_rgb:
        {
            format = GL_RGB;
            glFormat = GL_RGB;

            break;
        }
        case STBI_rgb_alpha:
        {
            format = GL_RGBA8;
            glFormat = GL_RGBA;
            break;
        }
    }

    createTexture();


    
    if (pixmap.pixels)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, pixmap.pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
     if (components==1)
    {
        GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

    } else 
    if (components==2)
    {
        GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    } 
        
    }

  //  glBindTexture(GL_TEXTURE_2D, 0);
 //   Log(0, "TEXTURE2D: [ID %i] Create Opengl Texture2D (%d,%d) bpp:%d", id, width, height, components);
}

bool Texture2D::Load(const Pixmap &pixmap)
{
    components = pixmap.components;
    width = pixmap.width;
    height = pixmap.height;



    GLenum format=GL_RGBA;
    GLenum glFormat = GL_RGBA;
    switch (components) 
    {
        case STBI_grey:
        {
            format =    GL_R8;
            glFormat = GL_RED;
            break;
        }
        case STBI_grey_alpha:
        {
            format = GL_RG8;
            glFormat = GL_RG;
            break;
        }
        case STBI_rgb:
        {
            format = GL_RGB;
            glFormat = GL_RGB;

            break;
        }
        case STBI_rgb_alpha:
        {
            format = GL_RGBA8;
            glFormat = GL_RGBA;
            break;
        }
    }

    createTexture();


    
    if (pixmap.pixels)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, pixmap.pixels);
     glGenerateMipmap(GL_TEXTURE_2D);
    if (components==1)
    {
        GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

    } else 
    if (components==2)
    {
        GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    } 
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    LogInfo( "TEXTURE2D: [ID %i] Create Opengl Texture2D (%d,%d) bpp:%d", id, width, height, components);
    return true;
}


bool Texture2D::Load(const char* file_name)
{
 
    unsigned int bytesRead;
    unsigned char *fileData =  LoadFileData(file_name,&bytesRead);
    if(!fileData)
        return false;



    unsigned char *data =  stbi_load_from_memory(fileData,bytesRead,&width, &height, &components, 0);



    if (data == NULL) 
    {
        LogError("Texture2D: Failed to load image: %s",file_name);
        return false;
    }

      GLenum format=GL_RGBA;
    GLenum glFormat = GL_RGBA;
    switch (components) 
    {
        case STBI_grey:
        {
            format =    GL_R8;
            glFormat = GL_RED;
            break;
        }
        case STBI_grey_alpha:
        {
            format = GL_RG;
            glFormat = GL_RG;
            break;
        }
        case STBI_rgb:
        {
            format = GL_RGB;
            glFormat = GL_RGB;

            break;
        }
        case STBI_rgb_alpha:
        {
            format = GL_RGBA8;
            glFormat = GL_RGBA;
            break;
        }
    }

    createTexture();


    
  
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    if (components==1)
    {
        GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

    } else 
    if (components==2)
    {
        GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
        glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
    } 



 //   glBindTexture(GL_TEXTURE_2D, 0);
 //   Log(0, "TEXTURE2D: [ID %i] Create Opengl Texture2D (%d,%d) bpp:%d", id, width, height, components);


        free(fileData);
        free(data);
        return true;
}

Texture2D::Texture2D(const char* file_name):Texture()
{
    Load(file_name);
}

bool Texture2D::LoadFromMemory(const unsigned char *buffer, u16 components, int width, int height)
{
   
    this->components = components;
    this->width = width;
    this->height = height;



    GLenum format=GL_RGBA;
    GLenum glFormat = GL_RGBA;
    switch (components) 
    {
        case STBI_grey:
        {
            format =    GL_R8;
            glFormat = GL_RED;
            break;
        }
        case STBI_grey_alpha:
        {
            format = GL_RG8;
            glFormat = GL_RG;
            break;
        }
        case STBI_rgb:
        {
            format = GL_RGB;
            glFormat = GL_RGB;

            break;
        }
        case STBI_rgb_alpha:
        {
            format = GL_RGBA8;
            glFormat = GL_RGBA;
            break;
        }
    }

    createTexture();


    
    if (buffer)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, glFormat, GL_UNSIGNED_BYTE, buffer);
         glGenerateMipmap(GL_TEXTURE_2D);
            if (components==1)
            {
                GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_ONE };
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);

            } else 
            if (components==2)
            {
                GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
                glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask);
            } 
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    LogInfo( "TEXTURE2D: [ID %i] Create Opengl Texture2D (%d,%d) bpp:%d", id, width, height, components);
   
    return true;
}




static const GLenum types[6] = {  GL_TEXTURE_CUBE_MAP_POSITIVE_X,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
                                  GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
                                  GL_TEXTURE_CUBE_MAP_NEGATIVE_Z };


CubemapTexture::CubemapTexture(
                               const std::string& PosXFilename,
                               const std::string& NegXFilename,
                               const std::string& PosYFilename,
                               const std::string& NegYFilename,
                               const std::string& PosZFilename,
                               const std::string& NegZFilename):Texture()
{
    

    m_fileNames[0] =  PosXFilename;
    m_fileNames[1] =  NegXFilename;
    m_fileNames[2] =  PosYFilename;
    m_fileNames[3] =  NegYFilename;
    m_fileNames[4] =  PosZFilename;
    m_fileNames[5] =  NegZFilename;


}



bool CubemapTexture::Load()
{
   

    glGenTextures(1, &id);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);

    for (unsigned int i = 0 ; i < ARRAY_SIZE_IN_ELEMENTS(types) ; i++) 
    {
        int Width, Height;
        void* pData = NULL;

        int BPP;
        unsigned char* image_data = stbi_load(m_fileNames[i].c_str(), &Width, &Height, &BPP, 0);

        if (!image_data) 
        {
            LogError("Can't load texture from '%s' - %s\n", m_fileNames[i].c_str(), stbi_failure_reason());

        }


        pData = image_data;

        glTexImage2D(types[i], 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, pData);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        stbi_image_free(image_data);
    }

    return true;
}





Pixmap::Pixmap()
{
    
    width = 0;
    height = 0;
    components = 0;
    pixels = nullptr;
}

Pixmap::~Pixmap()
{
    if (pixels)
    {
        free(pixels);
    }
}

Pixmap::Pixmap(const Pixmap &image, const IntRect &crop)
{
    width = crop.width;
    height = crop.height;
    components = image.components;
    pixels = (unsigned char *)malloc(width * height * components);
    for (int y = (int)crop.y, offsetSize = 0; y < (int)(crop.y + crop.height); y++)
    {
        memcpy(pixels + offsetSize, ((unsigned char *)image.pixels) + (y*image.width + (int)crop.x)*components, (int)crop.width*components);
        offsetSize += ((int)crop.width*components);
    }

}

 Pixmap::Pixmap(int w, int h, int components)
{
  
    width = w;
    height = h;
    this->components = components;
    pixels = (unsigned char *)malloc(w * h * components);
}

 Pixmap::Pixmap(int w, int h, int components, unsigned char *data)
{

    pixels = (unsigned char *)malloc(w * h * components);
    width = w;
    height = h;
    this->components = components;
    memcpy(pixels, data, w * h * components);
}

void Pixmap::Clear()
{
    if (pixels)
    {
        for (int i = 0; i < width * height * components; i++)
        {
            pixels[i] = 0;
        }
    }
  
}


void Pixmap::SetPixel(u32 x, u32 y, u8 r, u8 g, u8 b, u8 a)
{

    if (x >= (u32)width || y >= (u32)height)        return;
    
    
  
   
   if (components == 1)
   {
        Vec3 coln ( (float)r/255.0f, (float)g/255.0f, (float)b/255.0f );
        unsigned char gray = (unsigned char)((coln.x*0.299f + coln.y*0.587f + coln.z*0.114f)*255.0f);
        ((u8 *)pixels)[y*width + x] = gray;

   }
   else if (components == 2)
   {
                 Vec3 coln((float)r/255.0f, (float)g/255.0f, (float)b/255.0f );
            u8 gray = (u8)((coln.x*0.299f + coln.y*0.587f + coln.z*0.114f)*255.0f);

            ((u8*)pixels)[(y*width + x)*2] = gray;
            ((u8*)pixels)[(y*width + x)*2 + 1] = a;

   }
   else if (components == 3)
   {

            ((u8 *)pixels)[(y*width + x)*3]     = r ;
            ((u8 *)pixels)[(y*width + x)*3 + 1] = g;
            ((u8 *)pixels)[(y*width + x)*3 + 2] = b;
            

   }
   else if (components == 4)
   {
            ((u8 *)pixels)[(y*width + x)*4]     = r;
            ((u8 *)pixels)[(y*width + x)*4 + 1] = g;
            ((u8 *)pixels)[(y*width + x)*4 + 2] = b;
            ((u8 *)pixels)[(y*width + x)*4 + 3] = a;
      
   }

}

void Pixmap::SetPixel(u32 x, u32 y, u32 rgba)
{

    if (x >= (u32)width || y >= (u32)height)        return;
    
        u8 r = rgba;
        u8 g = rgba >> 8;
        u8 b = rgba >> 16;
        u8 a = rgba >> 24;
        SetPixel(x,y,r,g,b,a);
}

u32 Pixmap::GetPixel(u32 x, u32 y) const
{
  
    if (x >= (u32)width || y >= (u32)height)        return 0;
   
    if (components == 1)
    {
        return pixels[y* width + x];
    }
    else if (components == 2)
    {
        return pixels[(y* width + x)*2] | (pixels[(y* width + x)*2+1] << 8);
    }
    else if (components == 3)
    {
        return pixels[(y* width + x)*3] | (pixels[(y* width + x)*3+1] << 8) | (pixels[(y* width + x)*3+2] << 16);
    }
    else if (components == 4)
    {
        return pixels[(y* width + x)*4] | (pixels[(y* width + x)*4+1] << 8) | (pixels[(y* width + x)*4+2] << 16) | (pixels[(y* width + x)*4+3] << 24);  
    }
    
    return 0;
}

Color Pixmap::GetPixelColor(u32 x, u32 y) const
{

   Color color = Color::BLACK;
    
    if ((x < (u32)width) &&  (y < (u32)height))
    {

    

   
    if (components == 1)
    {
        color.r =  (u8)     ((u8*)pixels)[y* width + x] ;
        color.g =  (u8 )    ((u8*)pixels)[y* width + x] ;
        color.b =  (u8 )    ((u8*)pixels)[y* width + x] ;
        color.a = 255;
        
    }
    else if (components == 2)
    {
        color.r =  (u8)     ((u8*)pixels)[(y* width + x)*2] ;
        color.g =  (u8 )    ((u8*)pixels)[(y* width + x)*2] ;
        color.b =  (u8 )    ((u8*)pixels)[(y* width + x)*2] ;
        color.a =  (u8 )    ((u8*)pixels)[(y* width + x)*2 + 1] ;
    }
    else if (components == 3)
    {
        color.r =  (u8)     ((u8*)pixels)[(y* width + x)*3] ;
        color.g =  (u8 )    ((u8*)pixels)[(y* width + x)*3 + 1] ;
        color.b =  (u8 )    ((u8*)pixels)[(y* width + x)*3 + 2] ;
        color.a = 255;

    }
    else if (components == 4)
    {
        color.r =  (u8)     ((u8*)pixels)[(y* width + x)*4] ;
        color.g =  (u8 )    ((u8*)pixels)[(y* width + x)*4 + 1] ;
        color.b =  (u8 )    ((u8*)pixels)[(y* width + x)*4 + 2] ;
        color.a =  (u8 )    ((u8*)pixels)[(y* width + x)*4 + 3] ;
       
    }
    }
    return color;
 
}

void Pixmap::Fill(u8 r, u8 g, u8 b, u8 a)
{
    for (u32 y = 0; y < (u32)height; y++)
    {
        for (u32 x = 0; x < (u32)width; x++)
        {
            SetPixel(x, y, r, g, b, a);
        }
    }
}


void Pixmap::Fill(u32 rgba)
{
    for (u32 y = 0; y < (u32)height; y++)
    {
        for (u32 x = 0; x <(u32) width; x++)
        {
            SetPixel(x, y, rgba);
        }
    }
}



bool Pixmap::Load(const char *file_name)
{
    
 

        pixels =  stbi_load(file_name,&width, &height, &components, 0);

        if (  pixels  == nullptr) 
        {
         LogError("Failed to load image: %s",file_name);
        return false;
        }

       // Log(0, "PIXMAP: Load image: %s (%d,%d) bpp:%d", file_name, width, height, components);

        return true;
}

bool Pixmap::LoadFromMemory(const unsigned char *buffer,   unsigned int bytesRead)
{
  
    pixels =  stbi_load_from_memory(buffer,bytesRead,&width, &height, &components, 0);
    if (pixels == nullptr) 
    {
        LogError("Failed to load image from memory");
        return false;
    }
    return true;
}

bool Pixmap::Save(const char* file_name)
{
    if (pixels == nullptr) 
    {
        LogError("Failed to save image: %s",file_name);
        return false;
    }
   
    if (System::Instance().IsFileExtension(file_name,".bmp"))
        return stbi_write_bmp(file_name, width, height, components, pixels);
    else if (System::Instance().IsFileExtension(file_name,".tga"))
        return stbi_write_tga(file_name, width, height, components, pixels);
    else if (System::Instance().IsFileExtension(file_name,".png"))
         return stbi_write_png(file_name, width, height, components, pixels, 0);
    else 
    {
        LogError("Failed to save image: %s ",file_name);
        return false;
    }
    return false;
    
}


void Pixmap::FlipVertical()
{
    if (pixels == nullptr) 
    {
        LogError("Failed to flip image");
        return;
    }
    int rowSize = width * components;
    unsigned char *row = (unsigned char *)malloc(rowSize);
    for (int y = 0; y < height / 2; y++)
    {
        unsigned char *src = ((unsigned char *)pixels) + y * rowSize;
        unsigned char *dst = ((unsigned char *)pixels) + (height - y - 1) * rowSize;
        memcpy(row, src, rowSize);
        memcpy(src, dst, rowSize);
        memcpy(dst, row, rowSize);
    }
    free(row);

}

void Pixmap::FlipHorizontal()
{
    if (pixels == nullptr) 
    {
       LogError("Failed to flip image");
        return;
    }
    int rowSize = width * components;
    unsigned char *row = (unsigned char *)malloc(rowSize);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            unsigned char *src = ((unsigned char *)pixels) + y * rowSize + x * components;
            unsigned char *dst = ((unsigned char *)pixels) + y * rowSize + (width - x - 1) * components;
            memcpy(row, src, components);
            memcpy(src, dst, components);
            memcpy(dst, row, components);
        }
    }
    free(row);
}


//*****************************************************************************
// TextureManager
//*****************************************************************************

TextureManager*  TextureManager::m_singleton = 0x0;




TextureManager& TextureManager::Instance()
{
    DEBUG_BREAK_IF(!m_singleton);
    return *m_singleton;
}
TextureManager* TextureManager::InstancePtr()
{
    return m_singleton;
}



TextureManager::TextureManager()
{
    LogInfo("TextureManager: Create");
    m_defaultTexture = 0x0;
    m_singleton = this;
    m_texturePath = "assets/textures";
     
}

TextureManager::~TextureManager()
{
    LogInfo("TextureManager: Destroy");
    if (m_defaultTexture)
    {
        m_defaultTexture->Drop();
        m_defaultTexture = 0x0;
    }

   Clear();

   m_singleton = 0x0;
}

void TextureManager::Clear()
{
     if (m_defaultTexture)
    {
        m_defaultTexture->Drop();
        m_defaultTexture = 0x0;
    }

    m_loadedTextures.clear();

    //clear (m_textures);

    // m_textures.clear();

    for (auto it = m_textures.begin(); it != m_textures.end(); it++)
    {
        it->second->Drop();
    }

    m_textures.clear();
}

void TextureManager::FlipTextureOnLoad(bool flip)
{
       int isFlip = (flip == true) ? 1 : 0;
      stbi_set_flip_vertically_on_load(isFlip);
}

void TextureManager::Init()
{

    unsigned char checker_data[256*256];
    int x, y, i = 0;
	for (y = 0; y < 256; y++) {
		for (x = 0; x < 256; x++) {
			int k = ((x>>5) & 1) ^ ((y>>5) & 1);
			checker_data[i++] = k ? 255 : 192;
		}
	}

       
        m_defaultTexture = new Texture2D();
        //unsigned char data[4] = { 255, 255, 255, 255 };
       // m_defaultTexture->LoadFromMemory(data, 1, 256,256);

       // unsigned char pixels[4] = {         255, 255, 255, 255      }; 
        Pixmap image(256, 256, 1,checker_data);
        m_defaultTexture->Load(image);
        m_loadedTextures.push_back(m_defaultTexture);

}

Texture2D *TextureManager::Create(const unsigned char *buffer, u16 components, int width, int height)
{
    Texture2D *texture = new Texture2D();
    if (texture->LoadFromMemory(buffer, components, width, height))
    {
        return texture;
    }
    texture->Drop();
    return 0x0;
}

Texture2D *TextureManager::Create(const Pixmap &pixmap)
{
    Texture2D *texture = new Texture2D();
    if (texture->Load(pixmap))
    {
        return texture;
    }
    texture->Drop();
    return 0x0;
}

Texture2D *TextureManager::Create(const char *file_name)
{
    Texture2D *texture = new Texture2D();
    if (texture->Load(file_name))
    {
        return texture;
    }
    texture->Drop();
    return 0x0;
}

Texture2D *TextureManager::Create(u16 components, int width, int height)
{
    Texture2D *texture = new Texture2D();
    if (texture->LoadFromMemory(0, components, width, height))
    {
        return texture;
    }
    texture->Drop();
    return 0x0;
}
    

Texture2D *TextureManager::Get(const Pixmap &pixmap, const char *name)
{
    Texture2D *texture =0x0;
    auto it = m_textures.find(name);
    if (it != m_textures.end())
    {
        return it->second;
    } else
    {
         texture = new Texture2D();
        if (texture->Load(pixmap))
        {
            m_textures.emplace(name, texture);
        } else 
        {
            texture->Drop();
            texture = m_defaultTexture;
        }
    }


    

    return texture;
   
}

Texture2D *TextureManager::Get(const unsigned char *buffer, u16 components, int width, int height, const char *name)
{
    
    Texture2D *texture =0x0;
     auto it = m_textures.find(name);
    if (it != m_textures.end())
    {
        return it->second;
    } else
    {
         texture = new Texture2D();
         if (texture->LoadFromMemory(buffer, components, width, height))
        {
            m_textures.emplace(name, texture);
        } else 
        {
            texture->Drop();
            texture = m_defaultTexture;
        }
    }
    
    return texture;
}

bool TextureManager::Add(Texture2D *texture, const char *name)
{
    if (m_textures.find(name) != m_textures.end())
    {
        return false;
    } else 
    {
        m_textures.emplace(name, texture);
        return true;
    }
}

bool TextureManager::Remove(const char *file_name)
{
     Texture2D *texture =0x0;
     auto it = m_textures.find(file_name);
    if (it != m_textures.end())
    {
        texture = it->second;
        m_textures.erase(it);
        texture->Drop();
        return true;
    }

    for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end(); it++)
    {
        if (*it == texture)
        {
            m_loadedTextures.erase(it);
            return true;
        }
    }

    return false;
}

Texture2D *TextureManager::Get(const char *name)
{
    std::string path = name;
    
    Texture2D *texture=0x0;

    auto it = m_textures.find(path.c_str());
    if (it != m_textures.end())
    {
        LogInfo("TextureManager: Get Texture: %s", path.c_str());
        return it->second;
    } else
    {
        texture = new Texture2D();
        if (texture->Load(path.c_str()))
        {
            m_textures.emplace (path.c_str(),texture);
            m_loadedTextures.push_back(texture);
        } else 
        {
            texture->Drop();
            texture = m_defaultTexture;
        }
    }


    return texture;
}

Texture2D *TextureManager::GetTexture(int id)
{
    if (id >= 0 && id < m_loadedTextures.size())
    {
        return m_loadedTextures[id];
    }
    return nullptr;    
}

Texture2D *TextureManager::Load(const char *name)
{
    // const char * ext      = System::Instance().GetFileExtension(name);
    // const char * fileName = System::Instance().GetFileName(name);

   
    // if (!System::Instance().FileExists( System::Instance().TextFormat("%s%s",m_texturePath.c_str(),fileName).c_str()))
    // {
    //      if (!System::Instance().FileExists( System::Instance().TextFormat("%s%s.%s",m_texturePath.c_str(),fileName,"png").c_str()))
    //      {
    //         LogError("TextureManager: Texture not found: %s", name);
    //         return 0x0;
    // }



    std::string path = m_texturePath+ name;



    
    Texture2D *texture=0x0;

    auto it = m_textures.find(path.c_str());
    if (it != m_textures.end())
    {
        LogInfo("TextureManager: Get Texture: %s", path.c_str());
        
        return it->second;
    } else
    {
        texture = new Texture2D();
        if (texture->Load(path.c_str()))
        {
            m_textures.emplace (path.c_str(),texture);
            m_loadedTextures.push_back(it->second);
        } else 
        {
            texture->Drop();
            texture = m_defaultTexture;
        }
    }

    return texture;
}

bool TextureManager::LoadTexture(const char *name)
{
    

    std::string path = m_texturePath+ name;
    

    auto it = m_textures.find(path.c_str());
    if (it != m_textures.end())
    {
        return true;
    } else
    {
        Texture2D *texture = new Texture2D();
        if (texture->Load(path.c_str()))
        {
            //LogInfo("TextureManager: Get Texture: %s", path.c_str());
            m_textures.emplace (path.c_str(),texture);
            m_loadedTextures.push_back(texture);
            return true;
        } else 
        {
           // LogWarning("TextureManager: Texture not found: %s", name);
            texture->Drop();
        }
    }


    return false;
}
