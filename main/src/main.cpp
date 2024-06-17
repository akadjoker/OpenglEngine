
#include "pch.h"
#include <SDL2/SDL.h>
#include "Core.hpp"
#include "Camera.hpp"
#include "Scene.hpp"


int screenWidth = 1024;
int screenHeight = 768;




int main()
{

    Device device;
    Driver driver;
    TextureManager textureManager;
    Logger logger;
    System system;
    RenderBatch batch;
    Scene scene;


    device = Device::Instance();

    device.Create(screenWidth, screenHeight, "Engine", false);

    scene.Init();

    batch.Init(2, 1024 * 8);

    Camera camera(Vec3(-20.0f, 10.0f, 0.0f), Vec3(0.0f, 0.0f, -1.0f));
    float lastX = screenWidth / 2.0f;
    float lastY = screenHeight / 2.0f;
 
    bool firstMouse = true;


 






    while (device.Run())
    {

    
       float deltaTime = device.GetFrameTime();





        // Driver::Instance().SetBlend(true);
        // Driver::Instance().SetBlendMode(BlendMode::BLEND);
        // Driver::Instance().SetDepthTest(true);
        // Driver::Instance().SetDepthClamp(true);
        // Driver::Instance().SetCullFace(true);
        

        // Driver::Instance().SetClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // Driver::Instance().Clear();


         if (Keyboard::Down(KEY_W))
         {
             camera.ProcessKeyboard(0, deltaTime);
             
         } 
         if (Keyboard::Down(KEY_S))
         {
             camera.ProcessKeyboard(1, deltaTime);
             
         }
         if (Keyboard::Down(KEY_A))
         {
			 camera.ProcessKeyboard(2, deltaTime);
             
		 }
    
         if (Keyboard::Down(KEY_D))
         {
             camera.ProcessKeyboard(3, deltaTime);
             
         }


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

                       camera.ProcessMouseMovement(xoffset, yoffset);


                  }
                  else
                  {
                      firstMouse = true;
                  }

   
        Mat4 proj = camera.GetProjectionMatrix((float)screenWidth / (float)screenHeight) ;
        Mat4 view = camera.GetViewMatrix();
        scene.SetViewMatrix(view);
        scene.SetProjectionMatrix(proj);
        scene.SetCameraPosition( camera.position);
        



         scene.Update(deltaTime);


        
        scene.Render();

        Mat4 mvp = proj * view;
        batch.SetMatrix(mvp);
        batch.Grid(10, 10, true);
        batch.Render();

    

    


        device.Flip();
    }

    scene.Release();

    batch.Release();

    device.Close();
    return 0;
}