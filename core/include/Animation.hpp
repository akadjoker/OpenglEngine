#pragma once

#include <SDL2/SDL.h>
#include "Config.hpp"
#include "Math.hpp"
#include "Mesh.hpp"
#include "File.hpp"
#include "Utils.hpp"
#include "SceneNode.hpp"

const u32 MAX_BONES = 80;

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

    KeyFrame() {}
    KeyFrame(KeyFrame *t);

    ~KeyFrame() {}

    int GetPositionIndex(float animationTime);
    int GetRotationIndex(float animationTime);
    float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);
    Quaternion AnimateRotation(float movetime);
    Vec3 AnimatePosition(float movetime);
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
    void AddPositionKeyFrame(float frame, const Vec3 &pos)
    {
        positionKeyFrames.push_back(PosKeyFrame(pos, frame));
    }

    void AddRotationKeyFrame(float frame, const Quaternion &rot)
    {

        rotationKeyFrames.push_back(RotKeyFrame(rot, frame));
    }
};

struct Frame
{
    std::string name;
    Vec3 position;
    Quaternion orientation;
    KeyFrame keys;
    Vec3 src_pos, dest_pos;
    Quaternion src_rot, dest_rot;
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
private:
    u64 n_frames;
    u64 state;
    u64 method;
    float currentTime;
    float duration;
    float fps;
    int mode;
    bool isEnd;
    std::vector<Frame *> frames;
    std::map<std::string, Frame *> framesMap;

public:
    enum
    {
        LOOP = 0,
        PINGPONG = 1,
        ONESHOT = 2
    };
    enum
    {
        Stoped = 0,
        Looping = 1,
        Playing = 2
    };
    std::string name;

    Animation(const std::string &name);
    ~Animation();

    void Force();

    bool Save(const std::string &name);
    bool Load(const std::string &name);

    float GetDuration();
    float GetTime();
    float GetFPS();
    int GetMode();
    u64 GetState();

    std::string GetName() const { return name; }

    bool Play(u32 mode, float fps);
    bool Stop();
    bool IsEnded();

    Frame *AddFrame(std::string name);
    Frame *GetFrame(std::string name);
    Frame *GetFrame(int index);

    void Update(float elapsed);
};

class CORE_PUBLIC Animator
{
public:
    Animator(Entity *parent);
    ~Animator();

    void Update(float elapsed);

    void SaveAllFrames(const std::string &path);

    Animation *LoadAnimation(const std::string &name);

    Animation *AddAnimation(const std::string &name);

    Animation *GetAnimation(const std::string &name);

    Animation *GetAnimation(int index);

    u32 numAnimations() const
    {
        return m_animations.size();
    }

    void SetIgnorePosition(const std::string &name, bool ignore);
    void SetIgnoreRotation(const std::string &name, bool ignore);

    bool Play(const std::string &name, int mode = Animation::LOOP, float blendTime = 0.25f);

    void Stop();
    bool IsEnded();
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
        if (!currentAnimation)
            return 0.0f;
        return currentAnimation->GetTime();
    }

private:
    std::vector<Animation *> m_animations;
    std::map<std::string, Animation *> m_animations_map;
    Entity *entity;

    Animation *currentAnimation;

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

    std::vector<Joint *> joints;
    std::vector<SkinSurface *> surfaces;
    std::vector<Material *> materials;
    std::vector<Mat4> bones;
    Animator *animator;

    friend class Animation;
    friend class Animator;

public:
    Entity();

    void UpdateAnimation(float dt);

    Animation *LoadAnimation(const std::string &name);

    void Play(const std::string &name, int mode = Animation::LOOP, float blendTime = 0.25f);

    void SetTexture(int layer = 0, Texture2D *texture = nullptr);

    u32 numJoints() const
    {
        return joints.size();
    }
    u32 numSurfaces() const
    {
        return surfaces.size();
    }
    u32 numBones() const
    {
        return bones.size();
    }
    u32 numMaterials() const
    {
        return materials.size();
    }

    Mat4 GetBone(u32 index) const;

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

    Joint *GetJoint(u32 id);
    Joint *FindJoint(const char *name);
    int GetJointIndex(const char *name);
    void Render() override;
    void RenderNoMaterial();
    void Release() override;

    bool Save(const std::string &name);

    bool Load(const std::string &name);
};
