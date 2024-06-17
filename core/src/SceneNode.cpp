 
#include "pch.h"
#include "SceneNode.hpp"


void Joint::Render() 
{
//     batch.SetColor(0, 1, 1);
//     batch.BeginTransform(AbsoluteTransformation);
//     batch.Cube(Vec3(0, 0, 0), 0.01, 0.01, 0.01, false);
//     batch.EndTransform();

//     Vec3 pos;
//     Vec3 parentPos;

//     pos = Mat4::Transform(AbsoluteTransformation, pos);
//     if (parent)
//     {
//         parentPos = Mat4::Transform(parent->AbsoluteTransformation, parentPos);
//     }

//     batch.SetColor(1, 0, 0);
//     batch.Line3D(pos, parentPos);
//     batch.Cube(pos, 0.1, 0.1, 0.1, false);
}

//********************************************************************************************************************
//MODEL
//********************************************************************************************************************

 Model::Model()
 {
    
 }

Model::~Model()
{

    Release();
}



Mesh *Model::AddMesh(const VertexFormat& vertexFormat,u32 material , bool dynamic )
{

    Mesh *mesh = new Mesh(vertexFormat, material, dynamic);

    m_meshs.push_back(mesh);

    return mesh;

}

Material *Model::AddMaterial()
{

    Material *material = new Material();

    m_materials.push_back(material);

    return material;

}

bool Model::Load(const std::string &fileName,const VertexFormat& vertexFormat)
{


    FileStream stream(fileName, "rb");
    if (!stream.IsOpen())
    {
        LogError("Cannot load %s", fileName.c_str());
        return false;
    }

    stream.ReadChar();
    stream.ReadChar();
    stream.ReadChar();


    u32 version = stream.ReadInt();

    if (version != 2024)
    {
        LogError("Unsupported version %d", version);
        return false;
    }

    u32 countMaterial = stream.ReadInt();

    for (u32 i = 0; i < countMaterial; i++)
    {
        Material *mat = AddMaterial();
        mat->name    = stream.ReadUTFString();
        mat->diffuse = stream.ReadUTFString();

        mat->texture = TextureManager::Instance().Load(mat->diffuse.c_str());

        Logger::Instance().Info("Material %s texture %s",mat->name.c_str(),mat->diffuse.c_str());

        
    }

    u32 countSurfaces = stream.ReadInt();
    for (u32 i = 0; i < countSurfaces; i++)
    {

        
        std::string name       = stream.ReadUTFString();
        int material   = stream.ReadInt();
        u32 countVertices = stream.ReadInt();
        u32 countFaces = stream.ReadInt();

        Mesh *surf = AddMesh(vertexFormat, material);

        LogInfo("Surface %s %d %d %d", name.c_str(), material, countVertices, countFaces);

        for (u32 j = 0; j < countVertices; j++)
        {
            Vec3 pos;
            Vec3 normal;
            Vec2 uv;

            pos.x = stream.ReadFloat();
            pos.y = stream.ReadFloat();
            pos.z = stream.ReadFloat();

            normal.x = stream.ReadFloat();
            normal.y = stream.ReadFloat();
            normal.z = stream.ReadFloat();

            uv.x = stream.ReadFloat();
            uv.y = stream.ReadFloat();
            surf->AddVertex(pos, uv,normal);

            

        }
 
        for (u32 j = 0; j < countFaces; j++)
        {
            int a = stream.ReadInt();
            int b = stream.ReadInt();
            int c = stream.ReadInt();
            surf->AddFace(a, b, c);
        }
        surf->CalculateTangents();
        surf->Upload(); 
        surf->CalculateBoundingBox();    
        m_boundingBox.Merge(surf->GetBoundingBox());


    }

    return true;
}
void Model::Release()
{

    for (u32 i = 0; i < m_meshs.size(); i++)
    {
        m_meshs[i]->Release();
        delete m_meshs[i];
    }
    m_meshs.clear();

    for (u32 i = 0; i < m_materials.size(); i++)
    {
        m_materials[i]->Release();
        delete m_materials[i];
    }
    m_materials.clear();

}

Vec3 Model::GetSize()
{
    float x = m_boundingBox.max.x - m_boundingBox.min.x;
    float y = m_boundingBox.max.y - m_boundingBox.min.y;
    float z = m_boundingBox.max.z - m_boundingBox.min.z;

    return Vec3(x, y, z);
}

void Model::Render() 
{

    for (u32 i = 0; i < m_meshs.size(); i++)
    {
        Material *mat = m_materials[m_meshs[i]->GetMaterial()];
        mat->Bind();
        m_meshs[i]->Render();
    }

}

void Model::RenderNoMaterial()
{

    for (u32 i = 0; i < m_meshs.size(); i++)
    {
        m_meshs[i]->Render();
    }
}

void Model::SetTexture(int layer, Texture2D *texture)
{
    DEBUG_BREAK_IF(layer >= (int)m_materials.size());
    m_materials[layer]->texture = texture;
}

void Model::SetTextureNormal(int layer, Texture2D *texture)
{
    DEBUG_BREAK_IF(layer >= (int)m_materials.size());
    m_materials[layer]->normal = texture;
}
void Model::SetCullFace(int layer, bool value)
{
    DEBUG_BREAK_IF(layer >= (int)m_materials.size());
    m_materials[layer]->cullFace = value;
}

void Model::Transform(const Mat4 &transform)
{

    for (u32 i = 0; i < m_meshs.size(); i++)
    {
        m_meshs[i]->Transform(transform);
    }
}

void Model::TransformTextureByLayer(int layer, const Mat4 &transform)
{

    DEBUG_BREAK_IF(layer >= (int)m_materials.size());
    m_meshs[layer]->TransformTexture(transform);
}

void Model::TransformTexture(const Mat4 &transform)
{

    for (u32 i = 0; i < m_meshs.size(); i++)
    {
        m_meshs[i]->TransformTexture(transform);
    }
}

void Model::CalculateBox()
{

    m_boundingBox.Clear();
    for (u32 i = 0; i < m_meshs.size(); i++)
    {
        m_boundingBox.Merge(m_meshs[i]->GetBoundingBox());
    }
}

//********************************************************************************************************************
//STATICNODE
//********************************************************************************************************************

StaticNode::StaticNode()
{
    type = Node::DEFAULT;
}

StaticNode::~StaticNode()
{
   //LogInfo("delete StaticNode %s", name.c_str());
    Release();
}

void StaticNode::AddModel(Model *model)
{
    m_models.push_back(model);
}

void StaticNode::Render()
{
    Node::Render();
    for (u32 i = 0; i < m_models.size(); i++)
    {
        m_models[i]->Render();
    }
}

void StaticNode::RenderNoMaterial()
{

    for (u32 i = 0; i < m_models.size(); i++)
    {
        m_models[i]->RenderNoMaterial();
    }
}

void StaticNode::Release()
{
    Node::Release();
    m_models.clear();
}
