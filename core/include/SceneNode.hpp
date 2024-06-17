
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
        virtual void Update()=0;
        virtual void UnLoad(){};

        int id;
        std::string name;
        Node *node;

       
};


class CORE_PUBLIC Node
{

    public:

    Vec3 position;
    Vec3 scale;
    Quaternion orientation;

    Mat4 AbsoluteTransformation;
    Mat4 LocalWorld;

    Node *parent;
    std::vector<Script *> scripts;

    bool shadow;
    bool visible;
    bool active;
    bool done;
    u64  id{0};

    std::string name;
    std::string parentName;

    Node()
    {
        name = "";
        parent = nullptr;
        position.set(0, 0, 0);
        scale.set(1, 1, 1);
        orientation.identity();
        AbsoluteTransformation.identity();
        shadow = false;
        visible = true;
        active = true;
        done = false;
        
    }

    Script *AddScript(Script *s)
    {
        s->node = this;
        s->Load();
        scripts.push_back(s);
        return s;
    }

    virtual ~Node()
    {
        for (u32 i = 0; i < scripts.size(); i++)
        {
            
            scripts[i]->UnLoad();
            scripts[i]->node = nullptr;
            delete scripts[i];
        }
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
        //Quaternion q;
        //q.rotate(Vec3(T(x), ToRadians(y), ToRadians(z)));
        orientation.setEuler(Vec3(ToRadians(x), ToRadians(y), ToRadians(z)));
        // orientation.rotate(Vec3(ToRadians(x), ToRadians(y), ToRadians(z)));
    }

    void Rotate(float angle, float x, float y, float z)
    {
        Quaternion q;
        q.rotate(angle, x, y, z);
        orientation *= q ;

        // orientation.rotate(angle, x, y, z) * orientation;
        //  q.rotate(Vec3(T(x), ToRadians(y), ToRadians(z)));
        //  orientation.rotate(angle, x, y, z);
        //   orientation.rotate(Vec3(ToRadians(x), ToRadians(y), ToRadians(z)));
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

    

    const Mat4 GetRelativeTransformation()
    {
        
        // Mat4 mScale = Mat4::Scale(scale.x, scale.y, scale.z);
        // Mat4 mRotate = Mat4::Rotate(orientation);
        // Mat4 mTranslate = Mat4::Translate(position.x, position.y, position.z);


        // LocalWorld =mTranslate * mRotate *  mScale;

         LocalWorld = Mat4::Scale( scale.x, scale.y, scale.z );
         LocalWorld.rotate(orientation);
         LocalWorld.translate( position.x, position.y, position.z );

        if (parent != nullptr)
        {
            Mat4 m_absTrans;
            Mat4::fastMult43(m_absTrans, parent->AbsoluteTransformation, LocalWorld);
            return m_absTrans;
        }
        else
        {
            return LocalWorld;
        }
    }

    void UpdateAbsolutePosition()
    {

        AbsoluteTransformation = GetRelativeTransformation();
    }

    virtual void Update()
    {
        UpdateAbsolutePosition();
        for (u32 i = 0; i < scripts.size(); i++)
        {
            scripts[i]->Update();
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

    // void Rotate(float angle,  float x, float y, float z)
    // {

    //     orientation.rotate(angle, x, y, z);
    // }


    void SetName(const std::string &n){name = n;}
};



class CORE_PUBLIC Joint : public Node
{
    public:
        Mat4 offset;
        Joint() : Node()    {       }
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