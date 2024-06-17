
#pragma once
#include "Config.hpp"
#include "Math.hpp"
#include "Texture.hpp"
#include "Batch.hpp"
#include "Mesh.hpp"
#include "File.hpp"
#include "Utils.hpp"
class Node;




struct CORE_PUBLIC Script
{
        Script(){id=0;name="";node=nullptr;};
        virtual ~Script(){};

        virtual void Load(){};
        virtual void Update(float dt)=0;
        virtual void UnLoad(){};

        int id;
        std::string name;
        Node *node;

       
};


class CORE_PUBLIC Node
{

    private:
        std::vector<Script *> scripts;


    

    public:
    enum  NodeType
    {
        DEFAULT=0,
        STATIC,
        ENTITY,
        JOINT    
    };

    Vec3 position;
    Vec3 scale;
    Quaternion orientation;

    Mat4 AbsoluteTransformation;
    Mat4 LocalWorld;

    Node *parent;
    
    bool shadow;
    bool visible;
    bool active;
    bool done;
    u64  id{0};
    NodeType type;

    std::string name;
    std::string parentName;

    Node();
    virtual ~Node();

    Script *AddScript(Script *s);
    bool HasScript(const std::string &s);
    bool RemoveScript(const std::string &s);

    void SetParent(Node *p)
    {
        parent = p;
    }




    void SetPosition(const Vec3 &p) 
    {
        position = p;
    }

    void SetScale(const Vec3 &s)
    {
        scale = s;
    }

    void SetRotation(const Vec3 &q)
    {
        orientation.rotate(q.x, q.y, q.z);
    }

    void SetRotation( float x, float y, float z)
    {
       orientation.setEuler(Vec3(ToRadians(x), ToRadians(y), ToRadians(z)));
    }

    void Rotate(float angle, float x, float y, float z)
    {
        Quaternion q;
        q.rotate(angle, x, y, z);
        orientation *= q ;
    }

    void SetRotation(const Quaternion &q)
    {
        orientation = q;
    }

    void SetPosition(float x, float y, float z)
    {
        position.set(x, y, z);
    }

    void SetScale(float x, float y, float z)
    {
        scale.set(x, y, z);
    }

    const Mat4 GetRelativeTransformation();

    void UpdateAbsolutePosition()
    {

        AbsoluteTransformation = GetRelativeTransformation();
    }

    virtual void Update()
    {
        UpdateAbsolutePosition();
        for (u32 i = 0; i < scripts.size(); i++)
        {
            scripts[i]->Update(1);
        }
    }
    virtual void Render()
    {
        
    }
    
    virtual void Release()
    {
      
    }

    void setLocalPosition(const Vec3 &p)
    {
        position = p;
    }

    void setLocalRotation(const Quaternion &q)
    {
        orientation = q;
    }
    
    Vec3 getLocalPosition()
    {
        return position;
    }

    Quaternion getLocalRotation()
    {
        return orientation;
    }
    Vec3 getWorldPosition()
    {
             return Mat4::Transform(AbsoluteTransformation, position);
    }
    void SetName(const std::string &n){name = n;}
};



class CORE_PUBLIC Joint : public Node
{
    public:
        Mat4 offset;
        Joint() : Node()    {  type = Node::DEFAULT;     }
        virtual ~Joint()    {       }
        void Render() override;

};


class CORE_PUBLIC Model 
{

    public:
        Model();
        virtual ~Model();


        Mesh *AddMesh(const VertexFormat& vertexFormat,u32 material =0, bool dynamic = false);
        Material *AddMaterial();

        bool Load(const std::string &fileName,const VertexFormat& vertexFormat);


        Material *GetMaterial(u32 index) { return m_materials[index]; }
        Mesh *GetMesh(u32 index) { return m_meshs[index]; }

        void Render();

        void RenderNoMaterial();

        void SetTexture(int layer, Texture2D *texture);
        void SetTextureNormal(int layer, Texture2D *texture);
        void SetCullFace(int layer, bool value);
        void SetName(const std::string &name) { m_fileName = name; }
        std::string GetName() const { return m_fileName; }

        void Transform(const Mat4 &transform);
        void TransformTexture(const Mat4 &transform);
        void TransformTextureByLayer(int layer,const Mat4 &transform);

        void CalculateBox();

        void Release();

        

        BoundingBox GetBoundingBox() { return m_boundingBox; }

        Vec3 GetSize();

    private:
         friend class Scene;
         std::string             m_fileName{"Model"};
        std::vector<Mesh*>       m_meshs;
        std::vector<Material*>   m_materials;
        BoundingBox              m_boundingBox;
};


class CORE_PUBLIC StaticNode : public Node
{

    public:
        StaticNode();
        virtual ~StaticNode();
        void AddModel(Model *model);
        void Render() override;
        void RenderNoMaterial();
        void Release() override;

    private:
        std::vector<Model*>       m_models;
        
};