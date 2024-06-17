
#include "pch.h"
#include "Mesh.hpp"

void Material::Release()
{
    if (texture != nullptr)
    {
        texture = nullptr;
    }
    if (normal != nullptr)
    {
        normal = nullptr;
    }
}
void Material::Bind()
{   
    Driver::Instance().SetCullFace(cullFace);
    
    if (texture != nullptr)
    {
        texture->Use(0);
    }
    if (normal != nullptr)
    {
        normal->Use(1);
    }
}


//*******************************************************
//
//*******************************************************



VertexFormat::VertexFormat(const Element* elements, unsigned int elementCount)
    : _vertexSize(0)
{
    for (unsigned int i = 0; i < elementCount; ++i)
    {
        Element element;
        memcpy(&element, &elements[i], sizeof(Element));
        _elements.push_back(element);
        _vertexSize += element.size * sizeof(float);
    }
}

VertexFormat::~VertexFormat(){}

const VertexFormat::Element& VertexFormat::getElement(unsigned int index) const{return _elements[index];}

unsigned int VertexFormat::getElementCount() const{    return (unsigned int)_elements.size();}
unsigned int VertexFormat::getVertexSize() const{    return _vertexSize;}

bool VertexFormat::operator == (const VertexFormat& f) const
{
    if (_elements.size() != f._elements.size())
        return false;
    for (size_t i = 0, count = _elements.size(); i < count; ++i)
    {
        if (_elements[i] != f._elements[i])
            return false;
    }

    return true;
}
bool VertexFormat::operator != (const VertexFormat& f) const{    return !(*this == f);}
VertexFormat::Element::Element() :    usage(POSITION), size(0){}
VertexFormat::Element::Element(Usage usage, unsigned int size) :    usage(usage), size(size){}
bool VertexFormat::Element::operator == (const VertexFormat::Element& e) const{    return (size == e.size && usage == e.usage);}
bool VertexFormat::Element::operator != (const VertexFormat::Element& e) const{    return !(*this == e);}

//*******************************************************
//
//*******************************************************



Mesh::Mesh(const VertexFormat &vertexFormat, u32 material, bool dynamic)
{
    m_vertexFormat = vertexFormat;
    m_material = material;
    m_dynamic = dynamic;
    flags = VBO_POSITION | VBO_INDICES;
    m_stride = vertexFormat.getVertexSize();
    isDirty = true;
    IBO=0;
    VAO=0;
    m_name = "Mesh";

    Init();
   
    
}

Mesh::~Mesh()
{
    Release();
}

void Mesh::Transform(const Mat4 &transform)
{

    bool hasNormals = normals.size() == positions.size();
    for (u32 i = 0; i < positions.size(); ++i) 
    {
        positions[i] = Mat4::Transform(transform,positions[i]);
     
        if (hasNormals) 
        {
            normals[i] = Mat4::TranformNormal(transform,normals[i]);
        }
        
    }
    flags |= VBO_POSITION;
    if (hasNormals) 
    {
        flags |= VBO_NORMAL;
    }
    CalculateBoundingBox();

    isDirty = true;
}

void Mesh::TransformTexture(const Mat4 &transform)
{

    for (u32 i = 0; i < texCoords.size(); ++i) 
    {
        Vec3 tex;
        tex.set(texCoords[i].x,texCoords[i].y,0.0f);

        tex = Mat4::Transform(transform,tex);

        texCoords[i].set(tex.x,tex.y);
    }
    flags |= VBO_TEXCOORD0;
    isDirty = true;
}

void Mesh::Init()
{

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    isDirty = true;


     for (u32 j = 0; j <m_vertexFormat.getElementCount(); ++j)
    {
            const VertexFormat::Element& e = m_vertexFormat.getElement(j);           
            if (e.usage == VertexFormat::POSITION) 
            {
                flags |= VBO_POSITION;
                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j, 3, GL_FLOAT, GL_FALSE, (GLint)sizeof(Vec3), 0);

                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "POSITION";
            //    Log(1,"POSITION");
                
                AddBuffer(buffer);
            }
            else
            if (e.usage == VertexFormat::TEXCOORD0) 
            {
                flags |= VBO_TEXCOORD0;

                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j, 2, GL_FLOAT, GL_FALSE, (GLint)sizeof(Vec2), 0);
                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "TEXCOORD0";
              //  Log(1,"TEXCOORD0");
                AddBuffer(buffer);

                

            }
            else if (e.usage == VertexFormat::NORMAL) 
            {
                flags |= VBO_NORMAL;
                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j, 3, GL_FLOAT, GL_FALSE, (GLint)sizeof(Vec3), 0);
                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "NORMAL";
            //   Log(1,"NORMAL");
                AddBuffer(buffer);
            } else
                
            if (e.usage == VertexFormat::TANGENT) 
            {
                flags |= VBO_TANGENT;
               
                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
    
                glEnableVertexAttribArray(j);
                glVertexAttribPointer(j, 3, GL_FLOAT, GL_FALSE, (GLint)sizeof(Vec3), 0);

                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "TANGENT";
                
                AddBuffer(buffer);
            } else          
            if (e.usage == VertexFormat::COLOR)
            {
                flags |= VBO_COLOR;
                VertexBuffer * buffer = new VertexBuffer();
                glGenBuffers(1, &buffer->id);
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
           
                glEnableVertexAttribArray(j);
               // glVertexAttribPointer(j, (GLint)e.size, GL_FLOAT, GL_FALSE, (GLint)e.size * sizeof(float), (void*)(0));
                glVertexAttribPointer(j, 4, GL_UNSIGNED_BYTE, GL_TRUE, 0, 0);
                buffer->size  = e.size;
                buffer->usage = e.usage;
                buffer->name = "COLOR";

           
                AddBuffer(buffer);
            } 
        }

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Mesh::FlipFaces()
{
    const u32 idxcnt = (u32)indices.size();
    if (idxcnt == 0) return;
    
     unsigned int *idx = indices.data();

    for (u32 i = 0; i < idxcnt; i += 3) 
    {
       const unsigned int tmp = idx[i+1];
        idx[i+1] = idx[i + 2];
        idx[i + 2] = tmp;
    }
    isDirty = true;
    flags |= VBO_INDICES;

}

void Mesh::TexturePlanarMapping(float resolution)
{

     const u32 idxcnt = (u32)indices.size();
     const u32 texcnt = (u32)texCoords.size();


    if (idxcnt == 0) return;
    if (texcnt < positions.size()) 
    {
        for (size_t i = 0; i < positions.size(); ++i) 
        {
            texCoords.push_back(Vec2(0.0f, 0.0f));
        }
    }
    


    for (u32 i = 0; i < idxcnt; i += 3) 
    {
        Plane3D plane(positions[indices[i]], positions[indices[i + 1]], positions[indices[i + 2]]);
        Vec3 normal = plane.normal;
        normal.x = fabs(normal.x);
        normal.y = fabs(normal.y);
        normal.z = fabs(normal.z);

        if (normal.x > normal.y && normal.x > normal.z) 
        {
            for (u32 j = 0; j < 3; ++j) 
            {
                texCoords[indices[i + j]].x = positions[indices[i + j]].y * resolution;
                texCoords[indices[i + j]].y = positions[indices[i + j]].z * resolution;
            }
        } else 
        if (normal.y > normal.x && normal.y > normal.z) 
        {
            for (u32 j = 0; j < 3; ++j) 
            {
                texCoords[indices[i + j]].x = positions[indices[i + j]].x * resolution;
                texCoords[indices[i + j]].y = positions[indices[i + j]].z * resolution;
            }
        } else 
        {
            for (u32 j = 0; j < 3; ++j) 
            {
                texCoords[indices[i + j]].x = positions[indices[i + j]].x * resolution;
                texCoords[indices[i + j]].y = positions[indices[i + j]].y * resolution;
            }
        }
           
    }



    flags |= VBO_TEXCOORD0;
    isDirty = true;
}
void Mesh::TexturePlanarMapping(float resolutionS, float resolutionT, u8 axis, const Vec3& offset)
{
     const u32 idxcnt = (u32)indices.size();
     const u32 texcnt = (u32)texCoords.size();
     if (idxcnt == 0 || texcnt==0) return;


    for (u32 i = 0; i < idxcnt; i += 3)
    {

        if (axis==0)
        {
                for (u32 j = 0; j < 3; ++j) 
                {
                    texCoords[indices[i + j]].x =0.5f +(positions[indices[i + j]].z + offset.z) * resolutionS;
                    texCoords[indices[i + j]].y =0.5f -(positions[indices[i + j]].y + offset.y) * resolutionT;
                

                }
        } else if (axis==1)
        {
                for (u32 j = 0; j < 3; ++j) 
                {
                    texCoords[indices[i + j]].x =0.5f +(positions[indices[i + j]].x + offset.x) * resolutionS;
                    texCoords[indices[i + j]].y =1.0f -(positions[indices[i + j]].z + offset.z) * resolutionT;
                }
        } else if (axis==2)
        {
                for (u32 j = 0; j < 3; ++j) 
                {
                    texCoords[indices[i + j]].x =0.5f +(positions[indices[i + j]].x + offset.x) * resolutionS;
                    texCoords[indices[i + j]].y =0.5f -(positions[indices[i + j]].y + offset.y) * resolutionT;
                }
        }
    
    }



    flags |= VBO_TEXCOORD0;
    isDirty = true;
}

void Mesh::Upload()
{

    for (u32 i = 0; i < buffers.size(); ++i) 
    {
        VertexBuffer *buffer = buffers[i];

      //  Log(1,"Upload usage %d id %d size %d name %s",buffer->usage, buffer->id ,buffer->size, buffer->name.c_str());
        if (buffer->usage == VertexFormat::POSITION) 
        {
            if (flags & VBO_POSITION) 
            {
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(Vec3), positions.data(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
             //  Log(1,"Upload POSITION  ");
            }
        }
        else if (buffer->usage == VertexFormat::TEXCOORD0) 
        {
            if (flags & VBO_TEXCOORD0) 
            {
                if (texCoords.size() != positions.size()) 
                {
                    for (size_t i = 0; i < positions.size(); ++i) 
                    {
                        texCoords.push_back(Vec2(0.0f, 0.0f));
                    }
                }
             //   Log(1,"Upload TEXCOORD0  ");
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glBufferData(GL_ARRAY_BUFFER, texCoords.size() * sizeof(Vec2), texCoords.data(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            }

        }
        else if (buffer->usage == VertexFormat::NORMAL) 
        {
            if (flags & VBO_NORMAL) 
            {

                if (normals.size() != positions.size()) 
                {
                    for (size_t i = 0; i < positions.size(); ++i) 
                    {
                        normals.push_back(Vec3(0.0f, 0.0f, 0.0f));
                    }
                }

            //    Log(1,"Upload NORMAL  ");
                glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
                glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(Vec3), normals.data(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
            }
        }
        else if (buffer->usage == VertexFormat::COLOR) 
        {

            if (colors.size() != positions.size()) 
            {
                for (size_t i = 0; i < positions.size(); ++i) 
                {
                    colors.push_back(255);colors.push_back(255);colors.push_back(255);colors.push_back(255);                
                }
            }

            glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
            glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(unsigned char), colors.data(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
        else if (buffer->usage == VertexFormat::TANGENT) 
        {
            if (flags & VBO_TANGENT) 
            {
                if (tangents.size() != positions.size()) 
                {
                    for (size_t i = 0; i < positions.size(); ++i) 
                    {
                        tangents.push_back(Vec3(0.0f, 0.0f, 0.0f));
                    }
                }

            }
     
            glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
            glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(Vec3), &tangents[0], m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        } else if (buffer->usage == VertexFormat::BITANGENT) 
        {
            if (flags & VBO_BITANGENT) 
            {
                if (bitangents.size() != positions.size()) 
                {
                    for (size_t i = 0; i < positions.size(); ++i) 
                    {
                        bitangents.push_back(Vec3(0.0f, 0.0f, 0.0f));
                    }
                }
            }
   
            glBindBuffer(GL_ARRAY_BUFFER, buffer->id);
            glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(Vec3), &bitangents[0], m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
        }
    }

    if (flags & VBO_INDICES) 
    {
       // Log(1,"Upload INDICES  ");
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), m_dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    flags = 0;
    isDirty = false;

}

void Mesh::Release()
{

    if (VAO != 0) 
    {
        glDeleteVertexArrays(1, &VAO);
    }
    if (IBO != 0) 
    {
        glDeleteBuffers(1, &IBO);
    }
    for (u32 i = 0; i < buffers.size(); ++i) 
    {
        VertexBuffer *buffer = buffers[i];
    
        glDeleteBuffers(1, &buffer->id);
        buffer->id = 0;
        delete buffer;
    }

    buffers.clear();

}

void Mesh::Render(u32 mode,u32 count)
{
    if (isDirty) 
    {
        Upload();
    }

  

    glBindVertexArray(VAO);
    glDrawElements(mode, count, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Render(u32 mode)
{
   
    this->Render(mode, this->GetIndexCount());
}
void Mesh::Render()
{
    this->Render(GL_TRIANGLES, this->GetIndexCount());
}

const void* Mesh::GetVertices() const
{
	return positions.data();
}
void* Mesh::GetVertices()
{
	return positions.data();
}

void* Mesh::GetIndices()
{
    	return indices.data();

}
const void* Mesh::GetIndices() const
{
		return indices.data();
}


int Mesh::AddVertex(const Vec3 &position)
{
    positions.push_back(position);
    flags |= VBO_POSITION;
    texCoords.push_back(Vec2(1.0f));
    normals.push_back(Vec3(1.0f));

    m_boundingBox.AddPoint(position);
    isDirty = true;
    return (int)positions.size() - 1;
}

int Mesh::AddVertex(float x, float y, float z)
{
    Vec3 position(x, y, z);
    positions.push_back(position);
    texCoords.push_back(Vec2(1.0f));
    normals.push_back(Vec3(1.0f));
    flags |= VBO_POSITION;
    isDirty = true;
    m_boundingBox.AddPoint(position);
    return (int)positions.size() - 1;
}

int Mesh::AddVertex(const Vec3 &position,const Vec2 &texCoord)
{
    positions.push_back(position);
    texCoords.push_back(texCoord);
    m_boundingBox.AddPoint(position);
    flags |= VBO_POSITION | VBO_TEXCOORD0;
     isDirty = true;
    return (int)positions.size() - 1;
}

int Mesh::AddVertex(float x, float y, float z, float u, float v)
{
    Vec3 position(x, y, z);
    positions.push_back(position);
    m_boundingBox.AddPoint(position);
    texCoords.push_back(Vec2(u, v));
    flags |= VBO_POSITION | VBO_TEXCOORD0;
     isDirty = true;
    return (int)positions.size() - 1;
}

int Mesh::AddVertex(const Vec3 &position,const Vec2 &texCoord,const Vec3 &normal)
{

    positions.push_back(position);
    m_boundingBox.AddPoint(position);
    texCoords.push_back(texCoord);
    normals.push_back(normal);
    flags |= VBO_POSITION | VBO_TEXCOORD0 | VBO_NORMAL;
     isDirty = true;
    return (int)positions.size() - 1;
}

int Mesh::AddVertex(float x, float y, float z, float u, float v, float nx, float ny, float nz)
{
     Vec3 position(x, y, z);
    positions.push_back(position);
    m_boundingBox.AddPoint(position);

    texCoords.push_back(Vec2(u, v));
    normals.push_back(Vec3(nx, ny, nz));
    flags |= VBO_POSITION | VBO_TEXCOORD0 | VBO_NORMAL;
     isDirty = true;
    return (int)positions.size() - 1;
}



void Mesh::VertexPosition(u32 index, const Vec3 &position)
{
    m_boundingBox.AddPoint(position);
    positions[index] = position;
    flags |= VBO_POSITION;
     isDirty = true;
}

void Mesh::VertexPosition(u32 index, float x, float y, float z)
{
    positions[index] = Vec3(x, y, z);
     m_boundingBox.AddPoint(positions[index]);
    flags |= VBO_POSITION;
     isDirty = true;
}

void Mesh::VertexNormal(u32 index, const Vec3 &normal)
{
    normals[index] = normal;
    flags |= VBO_NORMAL;
     isDirty = true;
}

void Mesh::VertexNormal(u32 index, float x, float y, float z)
{
    normals[index] = Vec3(x, y, z);
    flags |= VBO_NORMAL;
     isDirty = true;
}

void Mesh::VertexTexCoord(u32 index, const Vec2 &texCoord)
{
    texCoords[index] = texCoord;
    flags |= VBO_TEXCOORD0;
     isDirty = true;
}

void Mesh::VertexTexCoord(u32 index, float u, float v)
{
    texCoords[index] = Vec2(u, v);
    flags |= VBO_TEXCOORD0;
     isDirty = true;
}

int Mesh::AddFace(u32 v0, u32 v1, u32 v2)
{
    indices.push_back(v0);
    indices.push_back(v1);
    indices.push_back(v2);
    flags |= VBO_INDICES;
     isDirty = true;
    return (int)indices.size() - 3;
}



void Mesh::CalculateNormals()
{

    if (indices.size() > 0) 
    {

        if (normals.size() != positions.size()) 
        {
            for (size_t i = 0; i < positions.size(); ++i) 
            {
                normals.push_back(Vec3(0.0f, 0.0f, 0.0f));
            }
        }
        for (u32 i = 0; i < GetIndexCount(); i += 3)
        {
            Plane3D plane = Plane3D(positions[indices[i]], positions[indices[i + 1]], positions[indices[i + 2]]);
          
            Vec3 normal = plane.normal;
            normals[indices[i]] = normal;
            normals[indices[i + 1]] = normal;
            normals[indices[i + 2]] = normal;

        }


         flags |= VBO_NORMAL;
         isDirty = true;
    }

       
}

void Mesh::Clear()
{
    positions.clear();
    normals.clear();
    texCoords.clear();
    indices.clear();
    colors.clear();
    m_boundingBox.Clear();
}

void Mesh::CalculateSmothNormals(bool angleWeighted)
{

    if (normals.size() != positions.size()) 
    {
        for (size_t i = 0; i < positions.size(); ++i) 
        {
            normals.push_back(Vec3(0.0f, 0.0f, 0.0f));
        }
    }

    if (indices.size() > 0) 
    {
        if (normals.size() != positions.size()) 
        {
            for (size_t i = 0; i < positions.size(); ++i) 
            {
                normals.push_back(Vec3(0.0f, 0.0f, 0.0f));
            }
        }
        for (size_t i = 0; i < indices.size(); i += 3) 
        {
            Vec3 v0 = positions[indices[i + 0]];
            Vec3 v1 = positions[indices[i + 1]];
            Vec3 v2 = positions[indices[i + 2]];

            Vec3 normal = Vec3::Normalize(Vec3::Cross(v1 - v0, v2 - v0));
         
            Vec3 weight(1.f, 1.f, 1.f);
            if (angleWeighted)
            {
                weight = Vec3::GetAngleWeights(v0, v1, v2);
            }



            normals[indices[i + 0]] += weight.x*normal.x;
            normals[indices[i + 1]] += weight.y*normal.y;
            normals[indices[i + 2]] += weight.z*normal.z;
        }

        for (size_t i = 0; i < normals.size(); ++i) 
        {
            normals[i] = Vec3::Normalize(normals[i]);
        }
          flags |= VBO_NORMAL;
           isDirty = true;
    }
  
}

void Mesh::CalculateTangents()
{

    tangents.resize(positions.size(), Vec3(0.0f));
    bitangents.resize(positions.size(), Vec3(0.0f));

    for (size_t i = 0; i < indices.size(); i += 3) 
    {
        unsigned int index0 = indices[i];
        unsigned int index1 = indices[i + 1];
        unsigned int index2 = indices[i + 2];

        Vec3 edge1 = positions[index1] - positions[index0];
        Vec3 edge2 = positions[index2] - positions[index0];

        Vec2 deltaUV1 = texCoords[index1] - texCoords[index0];
        Vec2 deltaUV2 = texCoords[index2] - texCoords[index0];

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        Vec3 tangent(
            f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x),
            f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y),
            f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z)
        );

        Vec3 bitangent(
            f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x),
            f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y),
            f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z)
        );

        tangent.normalize();
        bitangent.normalize();

        tangents[index0] += tangent;
        tangents[index1] += tangent;
        tangents[index2] += tangent;

        bitangents[index0] += bitangent;
        bitangents[index1] += bitangent;
        bitangents[index2] += bitangent;
    }

    // Ortogonalizar e normalizar os vetores tangentes e bitangentes
    for (size_t i = 0; i < positions.size(); ++i) 
    {
        const Vec3& normal = normals[i];
        const Vec3& tangent = tangents[i];
        const Vec3& bitangent = bitangents[i];

        // Projeta a tangente e a bitangente no plano tangente (ortogonal a normal)
        tangents[i] = (tangent - normal * normal.dot(tangent)).normalized();

        // Calcula a direção correta para a bitangente
        bitangents[i] = (bitangent - normal * normal.dot(bitangent) - tangent * tangent.dot(bitangent)).normalized();
    }


}
void Mesh::CalculateBoundingBox()
{
    m_boundingBox.Clear();
    for (size_t i = 0; i < positions.size(); ++i) 
    {
        m_boundingBox.AddPoint(positions[i]);
    }
}

//*******************************************************
//
//*******************************************************

void SetVertexBoneData(SkinVertex &vertex, int boneID, float weight)
{
    for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
    {
        if (vertex.bones[i] < 0)
        {
            vertex.weights[i] = weight;
            vertex.bones[i] = boneID;
            break;
        }
    }
}


//*******************************************************
void SkinSurface::Build()
{

glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

glBindVertexArray(VAO);
glBindBuffer(GL_ARRAY_BUFFER, VBO);

glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SkinVertex), &vertices[0], GL_STATIC_DRAW);

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces.size() * sizeof(unsigned int), &faces[0], GL_STATIC_DRAW);

glEnableVertexAttribArray(0);
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void *)0);

// uv
glEnableVertexAttribArray(1);
glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void *)offsetof(SkinVertex, uv));


// normal

glEnableVertexAttribArray(2);
glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void *)offsetof(SkinVertex, normal));

// ids
glEnableVertexAttribArray(5);
glVertexAttribIPointer(5, 4, GL_INT, sizeof(SkinVertex), (void *)offsetof(SkinVertex, bones));

// weights
glEnableVertexAttribArray(6);
glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(SkinVertex), (void *)offsetof(SkinVertex, weights));


glBindVertexArray(0);
}

void SkinSurface::Release()
{
glDeleteVertexArrays(1, &VAO);
glDeleteBuffers(1, &VBO);
glDeleteBuffers(1, &EBO);
}

void SkinSurface::Render()
{
glBindVertexArray(VAO);
glDrawElements(GL_TRIANGLES, faces.size(), GL_UNSIGNED_INT, 0);
glBindVertexArray(0);
}