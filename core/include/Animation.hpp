#pragma once

#include <SDL2/SDL.h>
#include "Config.hpp"
#include "Math.hpp"
#include "Mesh.hpp"
#include "File.hpp"
#include "Utils.hpp"
#include "SceneNode.hpp"


class Node;
class Joint;
class KeyFrame;
class Animator;
class Entity;

struct PosKeyFrame
{
    Vec3 pos;
    float frame;

    PosKeyFrame(const Vec3 &p, float f)
    {
        pos.set(p.x, p.y, p.z);
        frame = f;
    }
};

struct RotKeyFrame
{
    Quaternion rot;
    float frame;
    RotKeyFrame(const Quaternion &r, float f)
    {
        rot.set(r.x, r.y, r.z, r.w);
        frame = f;
    }
};



struct KeyFrame
{

  
    std::vector<PosKeyFrame> positionKeyFrames;
    std::vector<RotKeyFrame> rotationKeyFrames;

 

    KeyFrame()
    {
       
    }



     KeyFrame(KeyFrame *t)
    {
        if ((!t) || (t->numPositionKeys() == 0) || (t->numRotationKeys() == 0))
        {

            LogError("Null animation pointer or nor frames");
            return;
        }

     
       
        for (u32 i = 0; i < t->numPositionKeys(); i++)
        {
            positionKeyFrames.push_back(t->positionKeyFrames[i]);
            
        }

        
        for (u32 i = 0; i < t->numRotationKeys(); i++)
        {
            rotationKeyFrames.push_back(t->rotationKeyFrames[i]);
        }
    }

    ~KeyFrame()
    {
    }

    void AddPositionKeyFrame(float frame, const Vec3 &pos)
    {
        positionKeyFrames.push_back(PosKeyFrame(pos, frame));
    }

    void AddRotationKeyFrame(float frame, const Quaternion &rot)
    {

        rotationKeyFrames.push_back(RotKeyFrame(rot, frame));
    }

    int GetPositionIndex(float animationTime)
    {
         // SDL_Log("KeyFrame time %f  %ld",animationTime,positionKeyFrames.size());
        for (u32 index = 0; index < positionKeyFrames.size() - 1; ++index)
        {
            if (animationTime < positionKeyFrames[index + 1].frame)
                return index;
        }
      
        DEBUG_BREAK_IF(true);
        return 0;
    }

    int GetRotationIndex(float animationTime)
    {
        for (u32 index = 0; index < rotationKeyFrames.size() - 1; ++index)
        {
            if (animationTime < rotationKeyFrames[index + 1].frame)
                return index;
        }
     //   SDL_Log("KeyFrame time %f  %ld",animationTime,rotationKeyFrames.size());
        DEBUG_BREAK_IF(true);
        return 0;
    }

    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    Quaternion AnimateRotation(float movetime)
    {
        if (rotationKeyFrames.size() == 1)
        {
            return rotationKeyFrames[0].rot;
        }
        int currentIndex = GetRotationIndex(movetime);
        int nextIndex = currentIndex + 1;

        float factor = GetScaleFactor(rotationKeyFrames[currentIndex].frame, rotationKeyFrames[nextIndex].frame, movetime);

      //    SDL_Log(" %f %f %f",rotationKeyFrames[currentIndex].frame, rotationKeyFrames[nextIndex].frame,movetime);

        return Quaternion::Slerp(rotationKeyFrames[currentIndex].rot, rotationKeyFrames[nextIndex].rot, factor);
    }
    Vec3 AnimatePosition(float movetime)
    {
        if (positionKeyFrames.size() == 1)
        {
            return positionKeyFrames[0].pos;
        }
        int currentIndex = GetPositionIndex(movetime);
        int nextIndex = currentIndex + 1;

        float factor = GetScaleFactor(positionKeyFrames[currentIndex].frame, positionKeyFrames[nextIndex].frame, movetime);
        return Vec3::Lerp(positionKeyFrames[currentIndex].pos, positionKeyFrames[nextIndex].pos, factor);
    }

    u32 numRotationKeys() const { return rotationKeyFrames.size(); }
    u32 numPositionKeys() const { return positionKeyFrames.size(); }

    void setPositionKey(int frame, const Vec3 &p)
    {
        positionKeyFrames[frame].pos = p;
    }

    void setRotationKey(int frame, const Quaternion &q)
    {
        rotationKeyFrames[frame].rot = q;
    }

    
};


struct Frame
{
    std::string name;
    Vec3 position;
    Quaternion orientation;
    KeyFrame   keys;
    Vec3 src_pos,dest_pos;
	Quaternion src_rot,dest_rot;
    bool pos;
    bool rot;
    bool IgnorePosition;
    bool IgnoreRotation;
    Frame()
    {
       pos = false;
       rot = false;
       IgnorePosition = false;
       IgnoreRotation = false;
    }
};



class CORE_PUBLIC Animation
{
    public:
    enum{LOOP=0,		PINGPONG=1,	ONESHOT=2};
    enum{Stoped=0, Looping=1, Playing=2 };
    std::string name;
    std::vector<Frame*> frames;
    std::map<std::string, Frame*> framesMap;
    u64 n_frames;
    u64 state;
    u64 method;
    float currentTime;
    float duration;
    float fps;
    int mode;
    bool isEnd;

    std::function<void()> OnStart;
    std::function<void()> OnEnd;
    std::function<void(float)> OnLoop;




    Animation()
    {
        state = Stoped;
        method = 0;
        currentTime = 0.0f;
        fps = 30.0f;
        mode = LOOP;
        isEnd = false;
        OnStart = [](){};
        OnEnd = [](){};
        OnLoop = [](float time){(void)time;};
    }

    void Force();


    bool Save(const std::string &name);
    bool Load(const std::string &name);

    

    bool Play(u32 mode, float fps)
    {
        if (state == Stoped)
        {
            state = Playing;
            this->mode = mode;
            this->fps = fps;
            currentTime = 0.0f;
            OnStart();
            return true;
        }
        return false;
    }



    bool Stop()
    {
        if (state == Playing)
        {
            state = Stoped;
            currentTime = 0.0f;
            isEnd = true;
      
            return true;
        }
        return false;
    }

    Frame *AddFrame(std::string name)
    {
        Frame *frame = new Frame();
        frame->name = name;
        frames.push_back(frame);
        framesMap[name] = frame;
        return frame;
    }

    bool IsEnded()
    {
        if (state == Stoped)
        {
            return true;
        }
        return isEnd;
    }

    ~Animation()
    {
        for (u32 i = 0; i < frames.size(); i++)
        {
            delete frames[i];
        }
        frames.clear();

    }

    Frame *GetFrame(std::string name)
    {
        if (framesMap.find(name) == framesMap.end())
        {
            return NULL;
        }
        return framesMap[name];
    }

    Frame *GetFrame(int index)
    {
        if (index < 0 || index >= (int)frames.size())
        {
            return NULL;
        }
        return frames[index];
    }

    void Update(float elapsed);


        

};




class CORE_PUBLIC Animator
{
  public:




    Animator(Entity *parent)
    {
        currentAnimation = nullptr;
 
        blendFactor = 0.0f;
        blendTime = 0.0f;
        blending = false;
        currentAnimationName = "";

        entity = parent;
     
        OnStart = [](const std::string &){};
        OnLoop = [](const std::string &,float time){(void)time;};
        OnEnd = [](const std::string &){};

     }

    ~Animator()
    {
        for (u32 i = 0; i < m_animations.size(); i++)
        {
            delete m_animations[i];
        }
        
    }

    void Update(float elapsed);

    void SaveAllFrames(const std::string &path);

    
    Animation * LoadAnimation(const std::string &name);

    Animation * AddAnimation(const std::string &name);


    Animation * GetAnimation(const std::string &name)
    {
        if (m_animations_map.find(name) == m_animations_map.end())
        {
            return NULL;
        }
        return m_animations_map[name];
    }

    Animation * GetAnimation(int index)
    {
        if (index < 0 || index >= (int)m_animations.size())
        {
            return NULL;
        }
        return m_animations[index];
    }



    u32 numAnimations() const { return m_animations.size(); }

    void SetIgnorePosition(const std::string &name, bool ignore);
    void SetIgnoreRotation(const std::string &name, bool ignore);




   


    bool Play(const std::string &name, int mode=Animation::LOOP,  float blendTime = 0.25f);

    void Stop();
    bool IsEnded() ;
    bool IsPlaying();

    std::string GetCurrentAnimationName()
    {
        std::string s("");
        if (!currentAnimation)
            return s;
        return currentAnimation->name; 
  }

  float GetCurrentFrame()
  {
    if (!currentAnimation) return 0.0f;
    return currentAnimation->currentTime;
 }




    std::function<void(const std::string &)> OnStart;
    std::function<void(const std::string &)> OnEnd;
    std::function<void(const std::string &, float)> OnLoop;


private:
    
    std::vector<Animation *> m_animations;
    std::map<std::string, Animation *> m_animations_map;
    Entity * entity;

    Animation* currentAnimation;

    std::string currentAnimationName;

    float blendFactor;
    float blendTime;
    bool blending;


    void beginTrans();
    void updateTrans(float blend);
    void updateAnim(float elapsed);





};


  







class CORE_PUBLIC Entity : public Node
{
    public:
    std::vector<Joint *> joints;
    std::vector<SkinSurface *> surfaces;
    std::vector<Material *> materials;
    std::vector<Mat4> bones;

    Animator *animator;

    Entity()
    {

       animator = new Animator(this);

        bones.reserve(80);

        for (int i = 0; i < 80; i++)
        {
            bones.push_back(Mat4::Identity());
        }


    }

    void UpdateAnimation(float dt)
    {

        //   SDL_Log("CurrentTime : %f Duration : %f TicksPerSecond : %f DeltaTime : %f",m_CurrentTime,m_Duration,m_TicksPerSecond,m_DeltaTime);
        animator->Update(dt);
        for (u32 i = 0; i < joints.size(); i++)
        {

            Joint *b = joints[i];
            b->Update();
            Mat4::fastMult43(bones[i], b->AbsoluteTransformation, b->offset);
        }
    }


    Animation * LoadAnimation(const std::string &name);

    void Play(const std::string &name, int mode=Animation::LOOP,  float blendTime = 0.25f);

    void SetTexture(int layer = 0, Texture2D *texture = nullptr);

    Material *AddMaterial()
    {
        Material *material = new Material();
        materials.push_back(material);
        return material;
    }

    void AddJoint(Joint *joint)
    {
        joints.push_back(joint);
    }

    SkinSurface *AddSurface()
    {
        SkinSurface *surface = new SkinSurface();
        surfaces.push_back(surface);
        return surface;
    }

    Joint *GetJoint(int id) { return joints[id]; }
    Joint *FindJoint(const char *name)
    {
        for (u32 i = 0; i < joints.size(); i++)
        {
            if (strcmp(joints[i]->name.c_str(), name) == 0)
            {
                return joints[i];
            }
        }
        return nullptr;
    }
    int GetJointIndex(const char *name)
    {
        for (u32 i = 0; i < joints.size(); i++)
        {
            if (strcmp(joints[i]->name.c_str(), name) == 0)
            {
                return i;
            }
        }
        return -1;
    }
    void Render() override;

    void RenderNoMaterial();



    void Release() override;

    
    bool Save(const std::string &name);

    bool Load(const std::string &name);
 
};
