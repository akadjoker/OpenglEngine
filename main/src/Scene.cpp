
#include "pch.h"
#include "Scene.hpp"


const float NEAR_PLANE = 0.1f;
const float FAR_PLANE = 2000.0f;
const bool USE_BLOOM = false;

static VertexFormat::Element VertexElements[] =
    {
        VertexFormat::Element(VertexFormat::POSITION, 3),
        VertexFormat::Element(VertexFormat::TEXCOORD0, 2),
        VertexFormat::Element(VertexFormat::NORMAL, 3),
  
    };   
static int VertexElemntsCount =3;

float lightLinear = 0.0334f;
float lightQuadratic = 0.0510f;
float lightIntensity =1.1679f;
struct Cascade
{
    float splitDepth;
    Mat4 viewProjMatrix;

};

CascadeShadow depthBuffer;
RenderQuad  quadRender;
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
const int SHADOW_MAP_CASCADE_COUNT = 4;
Cascade cascades[SHADOW_MAP_CASCADE_COUNT];
Vec3 lightPosition = Vec3(0.5f, 4.0f,  7.5f);
TextureBuffer renderTexture;

void updateCascades(const Mat4 & view, const Mat4 & proj,const Vec3 & lightPos)
	{
		float cascadeSplits[SHADOW_MAP_CASCADE_COUNT];
     	float cascadeSplitLambda = 0.95f;

        float nearClip = NEAR_PLANE;
		float farClip = FAR_PLANE;
		float clipRange = farClip - nearClip;

		float minZ = nearClip;
		float maxZ = nearClip + clipRange;

		float range = maxZ - minZ;
		float ratio = maxZ / minZ;

        for (u32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
        {
            float p = (i + 1) / static_cast<float>(SHADOW_MAP_CASCADE_COUNT);
			float log = minZ * Pow(ratio, p);
			float uniform = minZ + range * p;
			float d = cascadeSplitLambda * (log - uniform) + uniform;
			cascadeSplits[i] = (d - nearClip) / clipRange;
        }
	
		// Calculate orthographic projection matrix for each cascade
		float lastSplitDist = 0.0;
		for (u32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++) 
        {
			float splitDist = cascadeSplits[i];

			Vec3 frustumCorners[8] = 
            {
				Vec3(-1.0f,  1.0f, 0.0f),
				Vec3( 1.0f,  1.0f, 0.0f),
				Vec3( 1.0f, -1.0f, 0.0f),
				Vec3(-1.0f, -1.0f, 0.0f),
				Vec3(-1.0f,  1.0f,  1.0f),
				Vec3( 1.0f,  1.0f,  1.0f),
				Vec3( 1.0f, -1.0f,  1.0f),
				Vec3(-1.0f, -1.0f,  1.0f),
			};

			// Project frustum corners into world space
			Mat4 invCam =Mat4::Inverse(proj * view);   
			for (u32 j = 0; j < 8; j++) 
            {
				Vec4 invCorner = invCam * Vec4(frustumCorners[j], 1.0f);
                Vec4 div = invCorner / invCorner.w;
				frustumCorners[j] = Vec3(div.x, div.y, div.z);
			}

			for (u32 j = 0; j < 4; j++) 
            {
				Vec3 dist = frustumCorners[j + 4] - frustumCorners[j];
				frustumCorners[j + 4] = frustumCorners[j] + (dist * splitDist);
				frustumCorners[j] = frustumCorners[j] + (dist * lastSplitDist);
			}

			// Get frustum center
			Vec3 frustumCenter = Vec3(0.0f);
			for (u32 j = 0; j < 8; j++) 
            {
				frustumCenter += frustumCorners[j];
			}
			frustumCenter /= 8.0f;

			float radius = 0.0f;
			for (u32 j = 0; j < 8; j++) 
            {
				float distance = Vec3::Length(frustumCorners[j] - frustumCenter);
				radius = Max(radius, distance);
			}
			radius = Ceil(radius * 16.0f) / 16.0f;

			Vec3 maxExtents = Vec3(radius);
			Vec3 minExtents = -maxExtents;

			Vec3 lightDir = Vec3::Normalize(-lightPos);
			Mat4 lightViewMatrix  = Mat4::LookAt(frustumCenter - lightDir * -minExtents.z, frustumCenter, Vec3(0.0f, 1.0f, 0.0f));
			Mat4 lightOrthoMatrix = Mat4::Ortho(minExtents.x, maxExtents.x, minExtents.y, maxExtents.y, 0.0f, maxExtents.z - minExtents.z);


      

			// Store split distance and matrix in cascade
			cascades[i].splitDepth = (NEAR_PLANE + splitDist * clipRange) * -1.0f;
			cascades[i].viewProjMatrix = lightOrthoMatrix * lightViewMatrix;

			lastSplitDist = cascadeSplits[i];
		}
	}

Scene*  Scene::m_singleton = 0x0;


  

Scene& Scene::Instance()
{
    DEBUG_BREAK_IF(!m_singleton);
    return (*m_singleton);
}
Scene* Scene::InstancePtr()
{
    return m_singleton;

}

Scene::Scene()
{
    m_singleton = this;
}

Scene::~Scene()
{
     m_singleton = 0x0;
}

Entity *Scene::LoadEntity(const std::string &name,bool castShadow)
{

    Entity *e = new Entity();
    e->Load(name);
    m_entities.push_back(e);
    e->shadow = castShadow;
    return e;
   
}

Model *Scene::LoadModel(const std::string &name)
{
     
    Model *m = new Model();
    if (m->Load(name,VertexFormat(VertexElements, VertexElemntsCount)))
    {
        m_models.push_back(m);
        return m;
    }
    
    return nullptr;
}

StaticNode *Scene::CreateStaticNode(const std::string &name,bool castShadow)
{

    StaticNode *node = new StaticNode();
    node->SetName(name);
    node->shadow = castShadow;
    m_nodes.push_back(node);
    return node;
}

Model *Scene::CreateCube(float width, float height, float length)
{


    Model *model = new Model();

    float w = width/2;
    float h = height/2;
    float d = length/2;
    
    static Vec3 norms[]=
    {
		Vec3(0,0,-1),Vec3(1,0,0),Vec3(0,0,1),
		Vec3(-1,0,0),Vec3(0,1,0),Vec3(0,-1,0)
	};
	static Vec2 tex_coords[]=
    {
		Vec2(0,0),Vec2(1,0),Vec2(1,1),Vec2(0,1)
	};
	static int verts[]=
    {
		2,3,1,0,3,7,5,1,7,6,4,5,6,2,0,4,6,7,3,2,0,1,5,4
	};

    static BoundingBox box( Vec3(-w,-h,-d),Vec3(w,h,d) );

  

    Material * material= model->AddMaterial();
    material->texture = TextureManager::Instance().GetDefault();
    material->name = "default";


    Mesh *mesh = model->AddMesh(VertexFormat(VertexElements, VertexElemntsCount),0,false);

    

   for (int k=0;k<24;k+=4)
   {
        const Vec3 &normal=norms[k/4];

        for (int i=0;i<4;++i)
        {
            Vec3 pos = box.corner( verts[k+i] );
            mesh->AddVertex(pos,tex_coords[i],normal);
        }

        mesh->AddFace(k,k+1,k+2);
        mesh->AddFace(k,k+2,k+3);

   }

    mesh->Upload();
    m_models.push_back(model);
    return model;    
}

Model *Scene::CreateQuad(float x, float y, float w, float h)
{

    Model *model = new Model();

    Material * material= model->AddMaterial();

    material->texture = TextureManager::Instance().GetDefault();

    material->name = "default";

    Mesh *mesh = model->AddMesh(VertexFormat(VertexElements, VertexElemntsCount),0,false);

    float W = w * 0.5f;
    float H = h * 0.5f;

    mesh->AddVertex(Vec3(x -W, y + H, 0.0f), Vec2(1.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f));
    mesh->AddVertex(Vec3(x +W, y + H, 0.0f), Vec2(0.0f, 1.0f), Vec3(0.0f, 0.0f, -1.0f));
    mesh->AddVertex(Vec3(x +W, y - H, 0.0f), Vec2(0.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
    mesh->AddVertex(Vec3(x -W ,y - H, 0.0f), Vec2(1.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));


    mesh->AddFace(0, 1, 2);
    mesh->AddFace(0, 2, 3);

    mesh->CalculateTangents();
    mesh->Upload();

    m_models.push_back(model);
    mesh->m_boundingBox.Merge(mesh->GetBoundingBox());

    return model;




}


Model *Scene::CreatePlane(int stacks, int slices, int tilesX, int tilesY, float uvTileX , float uvTileY )
{
   
    Model *model = new Model();
   

    Material * material= model->AddMaterial();
    material->texture = TextureManager::Instance().GetDefault();
    material->name = "default";


    Mesh *mesh = model->AddMesh(VertexFormat(VertexElements, VertexElemntsCount),0,false);

    Vec3 center(-999999999.0f, 0.0f, -999999999.0f);



  
  for (int i = 0; i <= stacks; ++i) 
  {
        float y = static_cast<float>(i) / static_cast<float>(stacks)    ;
        for (int j = 0; j <= slices; ++j) 
        {
            float x = static_cast<float>(j) / static_cast<float>(slices) ;
            
               

                float uvX = x * uvTileX;
                float uvY = y * uvTileY;

                float pX = x * tilesX;
                float pY = y * tilesY;

                if (pX>center.x) center.x = pX;
                if (pY>center.z) center.z = pY;
                if (pX<center.x) center.x = pX;
                if (pY<center.z) center.z = pY;

             mesh->AddVertex(x * tilesX, 0.0f, y * tilesY, uvX, uvY, 0.0f, 1.0f, 0.0f);
            
        }
    }

    for (u32 i =0; i < mesh->GetVertexCount(); ++i)
    {
      Vec3 &v = mesh->positions[i];
       v.x -= center.x * 0.5f;
       v.z -= center.z * 0.5f;
    } 
  

    for (int i = 0; i < stacks; ++i) 
    {
        for (int j = 0; j < slices; ++j) 
        {
            u16 index = (slices + 1) * i + j;
            mesh->AddFace(index, index + slices + 1, index + slices + 2);
            mesh->AddFace(index, index + slices + 2, index + 1);
        }
    }
  
  
    if (mesh->normals.size() == mesh->positions.size())
    {

        for (u32 i = 0; i < mesh->GetIndexCount(); i += 3)
        {
            Plane3D plane = Plane3D(mesh->positions[mesh->indices[i]], mesh->positions[mesh->indices[i + 1]], mesh->positions[mesh->indices[i + 2]]);

           Vec3 normal = plane.normal;
            mesh->normals[mesh->indices[i]] = normal;
            mesh->normals[mesh->indices[i + 1]] = normal;
            mesh->normals[mesh->indices[i + 2]] = normal;

        }
    }
    mesh->m_boundingBox.Merge(mesh->GetBoundingBox());
    mesh->CalculateTangents();
    mesh->Upload();
    m_models.push_back(model);

    return model;

}

void Scene::Init()
{

    int width = Device::Instance().GetWidth();
    int height = Device::Instance().GetHeight();


    LoadModelShader();
    
    LoadDepthShader();

    depthBuffer.Init(SHADOW_MAP_CASCADE_COUNT,SHADOW_WIDTH, SHADOW_HEIGHT);

    quadRender.Init(width, height);

    renderTexture.Init(width, height);






   
}


void Scene::Release()
{


    m_quadShader.Release();
    m_debugShader.Release();
    depthBuffer.Release();
    m_depthShader.Release();
    m_modelShader.Release();
    renderTexture.Release();

    

    for (u32 i = 0; i < m_entities.size(); i++)
    {
        delete m_entities[i];
    }

    m_entities.clear();

    for (u32 i = 0; i < m_nodes.size(); i++)
    {
        delete m_nodes[i];
    }
    m_nodes.clear();

    for (u32 i = 0; i < m_models.size(); i++)
    {
        delete m_models[i];
    }

    m_models.clear();

    

}




void Scene::Render()
{
   

        int width = Device::Instance().GetWidth();
        int height = Device::Instance().GetHeight();

        



    
       updateCascades(viewMatrix, projectionMatrix, lightPosition);



        Mat4 lightProjection;
        Mat4 lightView;
        Mat4 lightSpaceMatrix;




        Driver::Instance().SetClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        Driver::Instance().SetBlend(true);
        Driver::Instance().SetBlendMode(BlendMode::BLEND);
        Driver::Instance().SetDepthTest(true);
        Driver::Instance().SetDepthClamp(true);


        m_depthShader.Use();
        glCullFace(GL_FRONT);
        depthBuffer.Begin();

            for (u32 j = 0; j < SHADOW_MAP_CASCADE_COUNT; j++)
            {

                depthBuffer.Set(j);
                lightSpaceMatrix = cascades[j].viewProjMatrix;
                m_depthShader.SetMatrix4("lightSpaceMatrix", &lightSpaceMatrix.c[0][0]);
                m_depthShader.SetInt("isStatic", 1);
                for (u32 i = 0; i < m_nodes.size(); i++)
                {
                    StaticNode *node = m_nodes[i];
                    if (!node->shadow || !node->visible) continue;
                    Mat4 m = m_nodes[i]->GetRelativeTransformation();
                    m_depthShader.SetMatrix4("model", m.x);
                    m_nodes[i]->RenderNoMaterial();
                }

                m_depthShader.SetInt("isStatic", 0);

                for (u32 i = 0; i < m_entities.size(); i++)
                {
                    Entity *entity = m_entities[i];
                    if (!entity->shadow || !entity->visible) continue;
                    Mat4 mat = entity->GetRelativeTransformation();
                    m_depthShader.SetMatrix4("model", mat.x);
                    for (u32 b = 0; b < entity->numBones(); b++)
                    {
                        const Mat4 &m = entity->GetBone(b);
                        m_depthShader.SetMatrix4(System::Instance().TextFormat("Joints[%d]", b), m.x); // model.bones
                    }
                    entity->Render();
                }
            }

        depthBuffer.End();
        glCullFace(GL_BACK);
        
        
        Driver::Instance().SetViewport(0, 0, width, height);
        
        Driver::Instance().SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        Driver::Instance().Clear();
    


    Vec3 lightDir = Vec3::Normalize(Vec3(-20.0f, 50, 20.0f));

    m_modelShader.Use();
    m_modelShader.SetMatrix4("view", viewMatrix.x);
    m_modelShader.SetMatrix4("projection", projectionMatrix.x);
    m_modelShader.SetFloat("viewPos", cameraPosition.x, cameraPosition.y, cameraPosition.z);
    m_modelShader.SetFloat("lightDir",lightDir.x, lightDir.y, lightDir.z);
    m_modelShader.SetFloat("farPlane", FAR_PLANE);

    for (u32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
    {
        m_modelShader.SetMatrix4("cascadeshadows["+std::to_string(i)+"].projViewMatrix", &cascades[i].viewProjMatrix.c[0][0]);
        m_modelShader.SetFloat("cascadeshadows["+std::to_string(i)+"].splitDistance", cascades[i].splitDepth);
        m_modelShader.SetInt("shadowMap["+std::to_string(i)+"]", i + 1);
            
    }
    depthBuffer.BindTextures(1);    




    
    m_modelShader.SetInt("isStatic", 1);


    if (USE_BLOOM)
        renderTexture.Begin();

   
  
      

    
    
    for (u32 i = 0; i < m_nodes.size(); i++)
    {
        if (!m_nodes[i]->visible) continue;
        Mat4 m = m_nodes[i]->GetRelativeTransformation();
        m_modelShader.SetMatrix4("model", m.x);
        m_nodes[i]->Render();
    }

    m_modelShader.SetInt("isStatic", 0);

    for (u32 i = 0; i < m_entities.size(); i++)
    {
        Entity *entity = m_entities[i];
        if (!entity->visible) continue;
        Mat4 mat = entity->GetRelativeTransformation();
        m_modelShader.SetMatrix4("model", mat.x);
        for (u32 b = 0; b < entity->numBones(); b++)
        {
            const Mat4 &m = entity->GetBone(b);
            m_modelShader.SetMatrix4(System::Instance().TextFormat("Joints[%d]", b), m.x); // model.bones
        }
        entity->Render();
    }
    if (USE_BLOOM)
        renderTexture.End();

  
    Driver::Instance().SetDepthTest(true);

if (USE_BLOOM)
{
    renderTexture.BindTexture(0);
    m_quadShader.Use();
    Driver::Instance().SetViewport(0, 0, width, height);
    quadRender.Render();
}


    //  depthBuffer.BindTexture(1);   
    //  quadRender.Render(-0.8f,0.35f,0.4f);

    //  depthBuffer.BindTexture(2);   
    //  quadRender.Render(-0.8f,-0.1f,0.4f);

    //     Driver::Instance().SetBlend(true);

}

void Scene::Update(float elapsed)
{

    for (u32 i = 0; i < m_nodes.size(); i++)
    {
        if (!m_nodes[i]->active) continue;
        m_nodes[i]->Update();
    }
   

    for (u32 i = 0; i < m_entities.size(); i++)
    {
        Entity *entity = m_entities[i];
        if (!entity->active) continue;
        entity->UpdateAnimation(elapsed);
        entity->Update();
    }
}

bool Scene::LoadModelShader()
{
   

   if (!m_modelShader.Load("assets/shaders/default.vert", "assets/shaders/default.frag"))
   {
    DEBUG_BREAK_IF(true);
   }
 
 
    m_modelShader.LoadDefaults();
    m_modelShader.SetInt("diffuseTexture", 0);
    m_modelShader.SetInt("isStatic", 1);
    m_modelShader.SetFloat("viewPos", 0.0f, 0.0f, 0.0f);
    for (u32 i = 0; i < SHADOW_MAP_CASCADE_COUNT; i++)
    {
        m_modelShader.SetInt("shadowMap["+std::to_string(i)+"]", i + 1);
    }

    return false;
}


bool Scene::LoadDepthShader()
{


        LogWarning("Loading Depth Shader");

        if (!m_depthShader.Load("assets/shaders/depth.vert", "assets/shaders/depth.frag"))
        {
            DEBUG_BREAK_IF(true);
            return false;
        }
        m_depthShader.LoadDefaults();
        m_depthShader.SetInt("isStatic", 1);

       


      LogWarning("Loading Debug Depth Shader");
     
       if (!m_debugShader.Load("assets/shaders/debugDepth.vert", "assets/shaders/debugDepth.frag"))
       {
           DEBUG_BREAK_IF(true);
           return false;
       }
        m_debugShader.LoadDefaults();
        m_debugShader.SetInt("depthMap", 0);



        
        LogWarning("Loading Quad Shader");
        if (!m_quadShader.Load("assets/shaders/bloom.vert", "assets/shaders/bloom.frag"))
        {
            DEBUG_BREAK_IF(true);
            return false;
        }
        m_quadShader.LoadDefaults();
        m_quadShader.SetInt("textureMap", 0);

    

        return true;
}



