
#pragma once

#include <SDL2/SDL.h>
#include <vector>
#include <string>
#include "Config.hpp"
#include "Math.hpp"
#include "Texture.hpp"
#include "SceneNode.hpp"
#include "Animation.hpp"
#include "Buffer.hpp"




class   Scene
{
public:
    Scene();
    ~Scene();

    static Scene& Instance();
    static Scene* InstancePtr();

    Entity * LoadEntity(const std::string &name,bool castShadow=false);
    Model  * LoadModel(const std::string &name);

    StaticNode *CreateStaticNode(const std::string &name="Node",bool castShadow=false);

    Model *CreateCube (float width, float height, float length);
    Model *CreatePlane(int stacks, int slices, int tilesX, int tilesY,float uvTileX =1.0f, float uvTileY =1.0f);
    Model *CreateQuad(float x, float y, float w, float h);
   
 


    void Init();

    void Render();

    void Update(float elapsed);


    void Release();

    void SetViewMatrix(const Mat4 &m) { viewMatrix = m; }
    void SetProjectionMatrix(const Mat4 &m) { projectionMatrix = m; }
    void SetCameraPosition(const Vec3 &p) { cameraPosition = p; }

  
  
    

private:
    static Scene* m_singleton;
    std::vector<Entity*> m_entities;
    std::vector<StaticNode*> m_nodes;
    std::vector<Model*> m_models;
    std::vector<Vec3> m_lights;
    Shader m_modelShader;
    Shader m_depthShader;

    Shader m_debugShader;

    Shader m_quadShader;




    Mat4 viewMatrix;
    Mat4 projectionMatrix;
    Vec3 cameraPosition;
    Vec3 lightPos;


    bool LoadModelShader();
    bool LoadDepthShader();
    void RenderLight(int index);
};