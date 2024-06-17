#pragma once



const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  58.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


struct Camera
{

        Camera()
        {
            position = Vec3(0.0f, 1.0f, -5.0f);
            front = Vec3(0.0f, 0.0f, 1.0f);
            right = Vec3(1.0f, 0.0f, 0.0f);
            worldUp = Vec3(0.0f, 1.0f, 0.0f);
            up = Vec3(0.0f, 1.0f, 0.0f);
            Yaw = 90.0f;
            Pitch = 0.0f;
            Zoom = 45.0f;
            near = 0.1f;
            far = 2000.0f;
            movementSpeed = SPEED;
            mouseSensitivity = SENSITIVITY;
            
        }
        Camera(Vec3 position, Vec3 target)
        {
            this->position = position;
            front = Vec3::Normalize(target - position);
            right = Vec3(1.0f, 0.0f, 0.0f);
            worldUp = Vec3(0.0f, 1.0f, 0.0f);
            up = Vec3(0.0f, 1.0f, 0.0f);
            Yaw = 0.0f;
            Pitch = 0.0f;
            Zoom = 45.0f;
            near = 0.1f;
            far = 2000.0f;
            movementSpeed = SPEED;
            mouseSensitivity = SENSITIVITY;
    
        }
 

        void Init(Vec3 position, Vec3 target)
        {
            this->position = position;
            front = Vec3::Normalize(target - position);
            right = Vec3(1.0f, 0.0f, 0.0f);
            worldUp = Vec3(0.0f, 1.0f, 0.0f);
            up = Vec3(0.0f, 1.0f, 0.0f);
            Yaw = 0.0f;
            Pitch = 0.0f;
            Zoom = 45.0f;
            near = 0.1f;
            far = 2000.0f;
            movementSpeed = SPEED;
            mouseSensitivity = SENSITIVITY;
        }
       
        Mat4 GetViewMatrix()
        {
                  
            update();
            return  Mat4::LookAt(position, position +  front, up);
            
        }

        Mat4 GetProjectionMatrix(float aspect)
        {
            return Mat4::Perspective(Zoom, aspect, near, far);
        }

    void ProcessMouseScroll(float yoffset)
    {
       Zoom -= (float)yoffset;
        if (Zoom < 45.0f)
            Zoom = 45.0f;
        if (Zoom > 95.0f)
            Zoom = 95.0f;
    }

     void ProcessKeyboard(int direction, float deltaTime)
    {
        float velocity = movementSpeed * deltaTime;
        if (direction == 0)
            position += front * velocity;
        if (direction == 1)
            position -= front * velocity;
        if (direction == 2)
            position -= right * velocity;
        if (direction == 3)
            position += right * velocity;
       
    
    }


    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true)
    {
        xoffset *= mouseSensitivity;
        yoffset *= mouseSensitivity;

        Yaw   += xoffset;
        Pitch += yoffset;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        

    }

    void SetZoom(float zoom)
    {
        Zoom = zoom;
    }

    float getZoom() const
    {
        return Zoom;
    }

    void setNearClip(float near)
    {
        this->near = near;
    }

    void setFarClip(float far)
    {
        this->far = far;
    }

    float getNearClip() const
    {
        return near;
    }

    float getFarClip() const
    {
        return far;
    }


    void MouseView(float dt)
    {
                 int xposIn, yposIn;
                 u32 IsMouseDown = SDL_GetMouseState(&xposIn, &yposIn);
                  if ( IsMouseDown & SDL_BUTTON(SDL_BUTTON_LEFT) )
                  {
                          float xpos = static_cast<float>(xposIn);
                          float ypos = static_cast<float>(yposIn);

                          if (firstMouse)
                          {
                              lastX = xpos;
                              lastY = ypos;
                              firstMouse = false;
                          }

                          float xoffset = xpos - lastX;
                          float yoffset = lastY - ypos; 

                          lastX = xpos;
                          lastY = ypos;

                       ProcessMouseMovement(xoffset, yoffset);
                  }
                  else
                  {
                      firstMouse = true;
                  }
    }

    Vec3 position;
    Vec3 front;
    Vec3 up;
    Vec3 right; 
    float Yaw;
    float Pitch;
    private:
   
    Vec3 worldUp;

    float movementSpeed;
    float mouseSensitivity;

    float Zoom;
    float near;
    float far;

    
    Mat4 view;

    float lastX{0};
    float lastY{0};
    bool firstMouse{true};

    void update()
    {
        Vec3 direction;
        direction.x = cos(ToRadians(Yaw)) * cos(ToRadians(Pitch));
        direction.y = sin(ToRadians(Pitch));
        direction.z = sin(ToRadians(Yaw)) * cos(ToRadians(Pitch));
        front = Vec3::Normalize(direction);

        right =Vec3::Normalize(Vec3::Cross(front, worldUp));
        up    =Vec3::Normalize(Vec3::Cross(right, front));
    
    }


};
