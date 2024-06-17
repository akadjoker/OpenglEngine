
#include "pch.h"
#include "Animation.hpp"


bool Animation::Save(const std::string &path)
{

        std::string final_path =path+ this->name + ".anim";

        FileStream stream(final_path, "wb");
        if (!stream.IsOpen())
        {
            LogError("Cannot save %s", final_path.c_str());
            return false;
        }

        stream.WriteChar('A');
        stream.WriteChar('H');
        stream.WriteChar('3');
        stream.WriteChar('D');
        stream.WriteInt(2024);
        stream.WriteUTFString(name);
        stream.WriteDouble(duration);
        stream.WriteDouble(fps);
        u32 countFrames = frames.size();
        stream.WriteInt(countFrames);

        for (u32 i = 0; i < countFrames; i++)
        {
            Frame *frame = frames[i];
            std::string name = frame->name;
            stream.WriteUTFString(name);
            stream.WriteInt(frame->keys.numPositionKeys());
            stream.WriteInt(frame->keys.numRotationKeys());

            for (u32 j = 0; j < frame->keys.numPositionKeys(); j++)
            {
                stream.WriteFloat(frame->keys.positionKeyFrames[j].frame);
                stream.WriteFloat(frame->keys.positionKeyFrames[j].pos.x);
                stream.WriteFloat(frame->keys.positionKeyFrames[j].pos.y);
                stream.WriteFloat(frame->keys.positionKeyFrames[j].pos.z);
            }
            for (u32 j = 0; j < frame->keys.numRotationKeys(); j++)
            {
                stream.WriteFloat(frame->keys.rotationKeyFrames[j].frame);
                stream.WriteFloat(frame->keys.rotationKeyFrames[j].rot.x);
                stream.WriteFloat(frame->keys.rotationKeyFrames[j].rot.y);
                stream.WriteFloat(frame->keys.rotationKeyFrames[j].rot.z);
                stream.WriteFloat(frame->keys.rotationKeyFrames[j].rot.w);
            }


        }

    stream.Close();

    return true;

}

bool Animation::Load(const std::string &fileName)
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
    stream.ReadChar();

    u32 version = stream.ReadInt();

    if (version != 2024)
    {
        LogError("Unsupported version %d", version);
        return false;
    }

    name = stream.ReadUTFString();
    duration = stream.ReadDouble();
    fps = stream.ReadDouble();

    LogInfo("Loading animation (%s) %f %f", name.c_str(), fps, duration);

    u32 countFrames = stream.ReadInt();

    for (u32 i = 0; i < countFrames; i++)
    {
        
        std::string name = stream.ReadUTFString();
        Frame *frame = AddFrame(name);

        u32 positions = stream.ReadInt();
        u32 rotations = stream.ReadInt();
        for (u32 j = 0; j < positions; j++)
        {
            float time = stream.ReadFloat();
            Vec3 pos;
            pos.x = stream.ReadFloat();
            pos.y = stream.ReadFloat();
            pos.z = stream.ReadFloat();
            frame->keys.AddPositionKeyFrame(time, pos);
        }
        for (u32 j = 0; j < rotations; j++)
        {
            float time = stream.ReadFloat();
            Quaternion rot;
            rot.x = stream.ReadFloat();
            rot.y = stream.ReadFloat();
            rot.z = stream.ReadFloat();
            rot.w = stream.ReadFloat();
            frame->keys.AddRotationKeyFrame(time, rot);
        }

    }

    stream.Close();
    return true;
           
}


void Animation::Force()
{ 

    if (frames.size() == 0)
    {
        return;
    }



        for (u32 i = 0; i < frames.size(); i++)
        {
            Frame *b = frames[i];
            if (b->keys.numPositionKeys() > 0)
            {
                b->pos = true;
            } 

            if (b->keys.numRotationKeys() > 0)
            {
              b->rot = true;
            }
        }

    

}


void Animation::Update(float elapsed)
{

    if (frames.size() == 0)
    {
        return;
    }
    if (state == Stoped)
    {
        return;
    }
    isEnd = false;
    


    currentTime += elapsed * fps;
    

    switch (mode)
    {
        
        case LOOP:
        {
            state = Playing;
            if (currentTime >= duration)
            {
              //  OnEnd();
                OnStart();
                isEnd = true;
            }
            currentTime = fmod(currentTime, duration);
            if (currentTime < 0.0f)
            {
                currentTime += duration;
            }
                
            
        }
        break;
        case PINGPONG:
        {
            state = Playing;
            currentTime = fmod(currentTime, duration *2);
            if (currentTime > (duration * 2))
            {
                isEnd = true;
                OnEnd();
          
            }
            if (currentTime < 0.0f)
            {
                currentTime += duration * 2;

            }

            if (currentTime > duration)
            {
                currentTime = duration-(currentTime-duration);
                fps = -fps;
            }
            
        }
        break;
        case ONESHOT:
        {
            if (currentTime >= duration)
            {
                LogWarning("Stop Animation : %s. Duration : %f. CurrentTime : %f", name.c_str(), duration, currentTime);

                Stop();
                OnEnd();
            }
        }
    
    }



        for (u32 i = 0; i < frames.size(); i++)
        {
            Frame *b = frames[i];
        

            if (b->keys.numPositionKeys() > 0)
            {
                 b->position    = b->keys.AnimatePosition(currentTime);
                 b->pos = true;
            } 

            if (b->keys.numRotationKeys() > 0)
            {
                 b->orientation = b->keys.AnimateRotation(currentTime);
                 b->rot = true;
            }

    }
        OnLoop(currentTime);

}

 //****************************************************************************
void Animator::SaveAllFrames(const std::string &path)
{
    for (u32 i = 0; i < m_animations.size(); i++)
    {
        m_animations[i]->Save(path);
    }
}

Animation * Animator::LoadAnimation(const std::string &name)
{
    Animation *a = new Animation();
    if (a->Load(name))
    {
        
        m_animations.push_back(a);
        m_animations_map[a->name] = a;
        return a;
    }

    delete a;
    return nullptr;
}
void Animator::Update(float elapsed)
{
        if (blending)
        {
            blendFactor += elapsed / blendTime;
    
            if (blendFactor >= 0.99f)
            {
                blendFactor = 1.0f;
                blending = false;
                updateAnim(elapsed);  
                return;
            }   
            
            updateTrans(blendFactor);
            return;
        }

      
  
        
            
     updateAnim(elapsed);    

       
}
 

Animation * Animator::AddAnimation(const std::string &name)
{
    Animation *a = new Animation();
    a->name = name; 

    for (size_t i = 0; i < entity->joints.size(); ++i) 
    {
        a->AddFrame(entity->joints[i]->name);
    }

    m_animations.push_back(a);
    m_animations_map[name] = a;
    return a;
}


void Animator::beginTrans()
{

    if (!currentAnimation) return;


        for (size_t i = 0; i < entity->joints.size(); i++) 
        {

            Frame *frame = currentAnimation->GetFrame(i);
            if (frame)
            {
                if (frame->keys.positionKeyFrames.size() != 0 && !frame->IgnorePosition)
                {
                    frame->src_pos = entity->joints[i]->getLocalPosition();
                  
                    frame->dest_pos = frame->keys.positionKeyFrames[0].pos;

                }
                if (frame->keys.rotationKeyFrames.size() != 0 && !frame->IgnoreRotation)
                {
                     frame->src_rot = entity->joints[i]->getLocalRotation();
                     frame->dest_rot = frame->keys.rotationKeyFrames[0].rot;
                }
                    
            }
        }
}

void Animator::updateTrans(float blend)
{

    if (!currentAnimation) return;
    for (size_t i = 0; i < entity->joints.size(); i++) 
    {   
        Joint *joint = entity->joints[i];

        Frame *frame = currentAnimation->GetFrame(i);
        if (frame)
        {
            if (frame->pos && !frame->IgnorePosition) joint->setLocalPosition( Vec3::Lerp(frame->src_pos,frame->dest_pos,blend) );
        
            if (frame->rot && !frame->IgnoreRotation)   joint->setLocalRotation( Quaternion::Slerp(frame->src_rot,frame->dest_rot,blend) );
        }
    }
}

void Animator::updateAnim(float elapsed)
{
    if (!currentAnimation) return;
        currentAnimation->Update(elapsed);
    


    if (currentAnimation->IsEnded())
    {
       OnEnd(currentAnimation->name);
       return;
    };

    OnLoop(currentAnimation->name, currentAnimation->currentTime); 

    for (size_t i = 0; i < entity->joints.size(); i++) 
    {
        Frame *frame = currentAnimation->GetFrame(i);
        Joint *joint = entity->joints[i];
        if (frame)
        {
            if (frame->pos && !frame->IgnorePosition) 
            {
               Vec3 pos = Vec3::Lerp(joint->position, frame->position,elapsed * 0.5f);  
               entity->joints[i]->setLocalPosition(pos);
            }
            if (frame->rot && !frame->IgnoreRotation) entity->joints[i]->setLocalRotation(frame->orientation);
        }
    }
}

  bool Animator::Play(const std::string &name, int mode,  float blendTime )
    {
        if (m_animations_map.find(name) == m_animations_map.end())
        {
            LogWarning("Animator: Animation %s not found", name.c_str());
            return false;
        }

        if (currentAnimation)
        {
            if (currentAnimationName == name && currentAnimation->state == Animation::Playing)
            {

                return false;
            }
        }



        currentAnimationName = name;
        if (currentAnimation)
        {
            currentAnimation->Stop();
        }
       
        currentAnimation =  m_animations_map[name];
        currentAnimation->Play(mode, currentAnimation->fps); 
        currentAnimation->Force();

        OnStart(name);

        this->blendTime = blendTime;
        blendFactor = 0.0f; 
        blending = true;

        beginTrans();
 
    
        return true;
    }

void Animator::Stop()
{

    if (currentAnimation)
    {
        currentAnimation->Stop();
    }
}
bool Animator::IsEnded() 
{

    if (currentAnimation)
    {
        return currentAnimation->IsEnded();
    }
    return true;

}
bool Animator::IsPlaying()
{

    if (currentAnimation)
    {
        return currentAnimation->state == Animation::Playing;
    }
    return false;
}


void Animator::SetIgnorePosition(const std::string &name, bool ignore)
{

    for (size_t i = 0; i < m_animations.size(); i++)
    {
        Frame *frame = m_animations[i]->GetFrame(name);
        if (frame)
        {
            frame->IgnorePosition = ignore;
        }

    }
}
void Animator::SetIgnoreRotation(const std::string &name, bool ignore)
{

    for (size_t i = 0; i < m_animations.size(); i++)
    {
        Frame *frame = m_animations[i]->GetFrame(name);
        if (frame)
        {
            frame->IgnoreRotation = ignore;
        }
    }

} 

//******************************************************************************


bool  Entity::Save(const std::string &name)
{


        FileStream stream(name, "wb");
        if (!stream.IsOpen())
        {
            LogError("Cannot save %s", name.c_str());
            return false;
        }

        stream.WriteChar('A');
        stream.WriteChar('H');
        stream.WriteChar('3');
        stream.WriteChar('D');
        stream.WriteInt(2024);

        u32 material = materials.size();

        stream.WriteInt(material);

        for (u32 i = 0; i < material; i++)
        {
           Material *mat = materials[i];
           stream.WriteUTFString(mat->name);
           stream.WriteUTFString(mat->diffuse);
        }

        u32 coutMesh = surfaces.size();
        stream.WriteInt(coutMesh);
        for (u32 i = 0; i < coutMesh; i++)
        {
            SkinSurface *surf = surfaces[i];
            stream.WriteUTFString(surf->name);
            stream.WriteInt(surf->material);
            stream.WriteInt(surf->vertices.size());
            stream.WriteInt(surf->faces.size()/3);

            for (u32 j = 0; j < surf->vertices.size(); j++)
            {
                stream.WriteFloat(surf->vertices[j].pos.x);
                stream.WriteFloat(surf->vertices[j].pos.y);
                stream.WriteFloat(surf->vertices[j].pos.z);

                stream.WriteFloat(surf->vertices[j].normal.x);
                stream.WriteFloat(surf->vertices[j].normal.y);
                stream.WriteFloat(surf->vertices[j].normal.z);

                stream.WriteFloat(surf->vertices[j].uv.x);
                stream.WriteFloat(surf->vertices[j].uv.y);

                stream.WriteFloat(surf->vertices[j].weights[0]);
                stream.WriteInt(surf->vertices[j].bones[0]);

                stream.WriteFloat(surf->vertices[j].weights[1]);
                stream.WriteInt(surf->vertices[j].bones[1]);

                stream.WriteFloat(surf->vertices[j].weights[2]);
                stream.WriteInt(surf->vertices[j].bones[2]);

                stream.WriteFloat(surf->vertices[j].weights[3]);
                stream.WriteInt(surf->vertices[j].bones[3]);

            }

            for (u32 j = 0; j < (surf->faces.size()/3); j++)
            {
                stream.WriteInt(surf->faces[j*3+0]);
                stream.WriteInt(surf->faces[j*3+1]);
                stream.WriteInt(surf->faces[j*3+2]);
            }

            
        }

        u32 countJoints = joints.size();
        stream.WriteInt(countJoints);
        for (u32 i = 0; i < countJoints; i++)
        {
            Joint *joint = joints[i]; 
            std::string name = joint->name;
            std::string parent = joint->parentName;

            stream.WriteUTFString(name);
            stream.WriteUTFString(parent);

            stream.WriteFloat(joint->position.x);
            stream.WriteFloat(joint->position.y);
            stream.WriteFloat(joint->position.z);

            stream.WriteFloat(joint->orientation.x);
            stream.WriteFloat(joint->orientation.y);
            stream.WriteFloat(joint->orientation.z);
            stream.WriteFloat(joint->orientation.w);

            stream.WriteFloat(joint->scale.x);
            stream.WriteFloat(joint->scale.y);
            stream.WriteFloat(joint->scale.z);

            
             stream.WriteFloat(joint->offset.x[0]);
             stream.WriteFloat(joint->offset.x[1]);
             stream.WriteFloat(joint->offset.x[2]);
             stream.WriteFloat(joint->offset.x[3]);

             stream.WriteFloat(joint->offset.x[4]);
             stream.WriteFloat(joint->offset.x[5]);
             stream.WriteFloat(joint->offset.x[6]);
             stream.WriteFloat(joint->offset.x[7]);      

     
             stream.WriteFloat(joint->offset.x[8]);
             stream.WriteFloat(joint->offset.x[9]);
             stream.WriteFloat(joint->offset.x[10]);
             stream.WriteFloat(joint->offset.x[11]);

             stream.WriteFloat(joint->offset.x[12]);
             stream.WriteFloat(joint->offset.x[13]);
             stream.WriteFloat(joint->offset.x[14]);
             stream.WriteFloat(joint->offset.x[15]);

            




        }

        stream.Close();
        return true;
        

}

bool Entity::Load(const std::string &name)
{

    FileStream stream(name, "rb");
    if (!stream.IsOpen())
    {
        LogError("Cannot load %s", name.c_str());
        return false;
    }

    stream.ReadChar();
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
        Material *mat = new Material();
        mat->name    = stream.ReadUTFString();
        mat->diffuse = stream.ReadUTFString();

        Logger::Instance().Info("Material %s texture %s",mat->name.c_str(),mat->diffuse.c_str());
        mat->texture = TextureManager::Instance().Load(mat->diffuse.c_str());
        materials.push_back(mat);
    }

    u32 countSurfaces = stream.ReadInt();
    for (u32 i = 0; i < countSurfaces; i++)
    {

        SkinSurface *surf    = AddSurface();
        surf->name       = stream.ReadUTFString();
        surf->material   = stream.ReadInt();
        u32 countVertices = stream.ReadInt();
        u32 countFaces = stream.ReadInt();

        LogInfo("Surface %s %d %d %d", surf->name.c_str(), surf->material, countVertices, countFaces);

        for (u32 j = 0; j < countVertices; j++)
        {
            SkinVertex vertex;

            vertex.pos.x = stream.ReadFloat();
            vertex.pos.y = stream.ReadFloat();
            vertex.pos.z = stream.ReadFloat();

            vertex.normal.x = stream.ReadFloat();
            vertex.normal.y = stream.ReadFloat();
            vertex.normal.z = stream.ReadFloat();

            vertex.uv.x = stream.ReadFloat();
            vertex.uv.y = stream.ReadFloat();

            vertex.weights[0] = stream.ReadFloat();
            vertex.bones[0] = stream.ReadInt();

            vertex.weights[1] = stream.ReadFloat();
            vertex.bones[1] = stream.ReadInt();

            vertex.weights[2] = stream.ReadFloat();
            vertex.bones[2] = stream.ReadInt();

            vertex.weights[3] = stream.ReadFloat();
            vertex.bones[3] = stream.ReadInt();
            surf->AddVertex(vertex);

        }
 
        for (u32 j = 0; j < countFaces; j++)
        {
            int a = stream.ReadInt();
            int b = stream.ReadInt();
            int c = stream.ReadInt();
            surf->AddFace(a, b, c);
        }

         surf->Build();


    }
 

    u32 countJoints = stream.ReadInt();
    for (u32 i = 0; i < countJoints; i++)
    {
        Joint *node = new Joint();
        node->name =  stream.ReadUTFString();
        node->parentName = stream.ReadUTFString();

      //  LogInfo("Joint %s parent %s", node->name.c_str(), node->parentName.c_str());

    

        

	   


        node->position.x = stream.ReadFloat();
        node->position.y = stream.ReadFloat();
        node->position.z = stream.ReadFloat();

        node->orientation.x = stream.ReadFloat();
        node->orientation.y = stream.ReadFloat();
        node->orientation.z = stream.ReadFloat();
        node->orientation.w = stream.ReadFloat();


        node->scale.x = stream.ReadFloat();
        node->scale.y = stream.ReadFloat();
        node->scale.z = stream.ReadFloat();

        float m [16];

        m[0]  = stream.ReadFloat();   
        m[1]  = stream.ReadFloat();
        m[2]  = stream.ReadFloat();
        m[3]  = stream.ReadFloat();

        m[4]  = stream.ReadFloat();
        m[5]  = stream.ReadFloat();
        m[6]  = stream.ReadFloat();
        m[7]  = stream.ReadFloat();

        m[8]  = stream.ReadFloat();
        m[9]  = stream.ReadFloat();
        m[10] = stream.ReadFloat();
        m[11] = stream.ReadFloat();

        m[12] = stream.ReadFloat();
        m[13] = stream.ReadFloat();
        m[14] = stream.ReadFloat();
        m[15] = stream.ReadFloat();

        node->offset.set(m);


      //  node->parent = this;

        joints.push_back(node);

        for (u32 i = 0; i < joints.size(); i++)
        {
            if (joints[i]->name == node->parentName)
            {
                node->parent = joints[i];
                break;
            }
        }
        node->Update();
    }

    stream.Close();

    return true;


}

void Entity::Release() 
{
    Node::Release();

    if (animator != nullptr)
    {
        delete animator;
        animator = nullptr;
    }

    for (u32 i = 0; i < materials.size(); i++)
    {
        materials[i]->Release();
        delete materials[i];
    }
    for (u32 i = 0; i < joints.size(); i++)
    {
        joints[i]->Release();
        delete joints[i];
    }

    for (u32 i = 0; i < surfaces.size(); i++)
    {
        surfaces[i]->Release();
        delete surfaces[i];
    }
}

Animation *Entity::LoadAnimation(const std::string &name)
{
    return animator->LoadAnimation(name);
}

void Entity::Play(const std::string &name, int mode, float blendTime)
{

    animator->Play(name, mode, blendTime);
}

void Entity::SetTexture(int layer, Texture2D *texture)
{
    DEBUG_BREAK_IF(layer >= (int)materials.size());
    materials[layer]->texture = texture;
}

void Entity::Render() 
{

    for (u32 i = 0; i < surfaces.size(); i++)
    {
        SkinSurface *surf = surfaces[i];
        Material *material = materials[surf->material];
        material->Bind();
        surf->Render();
    }

 
    Node::Render();
    

    for (u32 i = 0; i < joints.size(); i++)
    {
        joints[i]->Render();
    }
}

void Entity::RenderNoMaterial()
{
     for (u32 i = 0; i < surfaces.size(); i++)
    {
        SkinSurface *surf = surfaces[i];
        surf->Render();
    }

 
    Node::Render();
    

    for (u32 i = 0; i < joints.size(); i++)
    {
        joints[i]->Render();
    }
}
