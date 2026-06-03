/*******************************************************************************************
*
*   raylib [core] example - Basic window
*
*   Welcome to raylib!
*
*   To test examples, just press F6 and execute raylib_compile_execute script
*   Note that compiled executable is placed in the same folder as .c file
*
*   You can find all basic examples on C:\raylib\raylib\examples folder or
*   raylib official webpage: www.raylib.com
*
*   Enjoy using raylib. :)
*
*   This example has been created using raylib 1.0 (www.raylib.com)
*   raylib is licensed under an unmodified zlib/libpng license (View raylib.h for details)
*
*   Copyright (c) 2014 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "Critter.h"

#include "raylib.h"
#include "raymath.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <random>
#include <vector>



int main(int argc, char* argv[])
{
    using Timer = std::chrono::high_resolution_clock;

    SetTraceLogLevel(LOG_ERROR);

    /*Timer::time_point programStartTime = Timer::now();*/


    // Initialization
    //--------------------------------------------------------------------------------------
    int screenWidth = 1280;
    int screenHeight = 720;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    //SetTargetFPS(60);
    //--------------------------------------------------------------------------------------

    srand(time(NULL));


    Critter critters[1000]; 

    // create some critters
    const int CRITTER_COUNT = 1000;
    const int MAX_VELOCITY = 80;

    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        // create a random direction vector for the velocity
        Vector2 velocity = { -100+(rand()%200), -100+(rand()%200) };
        // normalize and scale by a random speed
        velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

        // create a critter in a random location
        critters[i].Init(
            { (float)(5+rand() % (screenWidth-10)), (float)(5+(rand() % screenHeight-10)) },
            velocity,
            12, "res/10.png");
    }


    Critter destroyer;
    Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
    velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);
    destroyer.Init(Vector2{ (float)(screenWidth >> 1), (float)(screenHeight >> 1) }, velocity, 20, "res/9.png");

    float timer = 1;
    Vector2 nextSpawnPos = destroyer.GetPosition();

    std::vector<double> drawTimes;
    std::vector<double> frameTimes;
    std::vector<double> updateCritterTimes;
    std::vector<double> collisionTimes;


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        Timer::time_point frameBegin = Timer::now();
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        float delta = GetFrameTime();

        // update the destroyer
        destroyer.Update(delta);
        // check each critter against screen bounds
        if (destroyer.GetX() < 0) {
            destroyer.SetX(0);
            destroyer.SetVelocity(Vector2{ -destroyer.GetVelocity().x, destroyer.GetVelocity().y });
        }
        if (destroyer.GetX() > screenWidth) {
            destroyer.SetX(screenWidth);
            destroyer.SetVelocity(Vector2{ -destroyer.GetVelocity().x, destroyer.GetVelocity().y });
        }
        if (destroyer.GetY() < 0) {
            destroyer.SetY(0);
            destroyer.SetVelocity(Vector2{ destroyer.GetVelocity().x, -destroyer.GetVelocity().y });
        }
        if (destroyer.GetY() > screenHeight) {
            destroyer.SetY(screenHeight);
            destroyer.SetVelocity(Vector2{ destroyer.GetVelocity().x, -destroyer.GetVelocity().y });
        }

        Timer::time_point updateCritterBegin = Timer::now();

        // update the critters
        // (dirty flags will be cleared during update)
        for (int i = 0; i < CRITTER_COUNT; i++)
        {
            critters[i].Update(delta);

            // check each critter against screen bounds
            if (critters[i].GetX() < 0) {
                critters[i].SetX(0);
                critters[i].SetVelocity(Vector2{ -critters[i].GetVelocity().x, critters[i].GetVelocity().y });
            }
            if (critters[i].GetX() > screenWidth) {
                critters[i].SetX(screenWidth);
                critters[i].SetVelocity(Vector2{ -critters[i].GetVelocity().x, critters[i].GetVelocity().y });
            }
            if (critters[i].GetY() < 0) {
                critters[i].SetY(0);
                critters[i].SetVelocity(Vector2{ critters[i].GetVelocity().x, -critters[i].GetVelocity().y });
            }
            if (critters[i].GetY() > screenHeight) {
                critters[i].SetY(screenHeight);
                critters[i].SetVelocity(Vector2{ critters[i].GetVelocity().x, -critters[i].GetVelocity().y });
            }

            // kill any critter touching the destroyer
            // simple circle-to-circle collision check
            float dist = Vector2Distance(critters[i].GetPosition(), destroyer.GetPosition());
            if (dist < critters[i].GetRadius() + destroyer.GetRadius())
            {
                critters[i].Destroy();
                // this would be the perfect time to put the critter into an object pool
            }
        }

        Timer::time_point updateCritterEnd = Timer::now();
        Timer::duration updateCritterTime = updateCritterEnd - updateCritterBegin;
        std::chrono::duration<double, std::milli> milliUpdateCritter = updateCritterTime;
        updateCritterTimes.push_back(milliUpdateCritter.count());

        Timer::time_point collisionBegin = Timer::now();

        // check for critter-on-critter collisions
        for (int i = 0; i < CRITTER_COUNT; i++)
        {            
            for (int j = 0; j < CRITTER_COUNT; j++){
                if (i == j || critters[i].IsDirty()) // note: the other critter (j) could be dirty - that's OK
                    continue;
                // check every critter against every other critter
                float dist = Vector2Distance(critters[i].GetPosition(), critters[j].GetPosition());
                if (dist < critters[i].GetRadius() + critters[j].GetRadius())
                {
                    // collision!
                    // do math to get critters bouncing
                    Vector2 normal = Vector2Normalize( Vector2Subtract(critters[j].GetPosition(), critters[i].GetPosition()));

                    // not even close to real physics, but fine for our needs
                    critters[i].SetVelocity(Vector2Scale(normal, -MAX_VELOCITY));
                    // set the critter to *dirty* so we know not to process any more collisions on it
                    critters[i].SetDirty(); 

                    // we still want to check for collisions in the case where 1 critter is dirty - so we need a check 
                    // to make sure the other critter is clean before we do the collision response
                    if (!critters[j].IsDirty()) {
                        critters[j].SetVelocity(Vector2Scale(normal, MAX_VELOCITY));
                        critters[j].SetDirty();
                    }
                    break;
                }
            }
        }

        Timer::time_point collisionEnd = Timer::now();
        Timer::duration collisionTime = collisionEnd - collisionBegin;
        std::chrono::duration<double, std::milli> milliCollision = collisionTime;
        collisionTimes.push_back(milliCollision.count());

        timer -= delta;
        if (timer <= 0)
        {
            timer = 1;

            // find any dead critters and spit them out (respawn)
            for (int i = 0; i < CRITTER_COUNT; i++)
            {
                if (critters[i].IsDead())
                {
                    Vector2 normal = Vector2Normalize(destroyer.GetVelocity());

                    // get a position behind the destroyer, and far enough away that the critter won't bump into it again
                    Vector2 pos = destroyer.GetPosition();
                    pos = Vector2Add(pos, Vector2Scale(normal, -50));
                    // its pretty ineficient to keep reloading textures. ...if only there was something else we could do
                    critters[i].Init(pos, Vector2Scale(normal, -MAX_VELOCITY), 12, "res/10.png");
                    break;
                }
            }
            nextSpawnPos = destroyer.GetPosition();
        }

        // Draw
        //----------------------------------------------------------------------------------
        Timer::time_point drawBegin = Timer::now();
        BeginDrawing();

        ClearBackground(RAYWHITE);

        // draw the critters
        for (int i = 0; i < CRITTER_COUNT; i++)
        {
            critters[i].Draw();
        }
        // draw the destroyer
        // (if you're wondering why it looks a little odd when sometimes critters are destroyed when they're not quite touching the 
        // destroyer, it's because the origin is at the top-left. ...you could fix that!)
        destroyer.Draw();

        DrawFPS(10, 10);
        //DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        Timer::time_point drawEnd = Timer::now();
        Timer::duration drawTime = drawEnd - drawBegin;
        std::chrono::duration<double, std::milli> milliDraw = drawTime;
        drawTimes.push_back(milliDraw.count());
        /*std::cout << "Draw time: " << milliTime.count() << "\n";*/
        //----------------------------------------------------------------------------------
        Timer::time_point frameEnd = Timer::now();
        Timer::duration frameTime = frameEnd - frameBegin;
        std::chrono::duration<double, std::milli> milliFrame = frameTime;
        frameTimes.push_back(milliFrame.count());
    }

    for (int i = 0; i < CRITTER_COUNT; i++)
    {
        critters[i].Destroy();
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------   
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

   /* Timer::time_point programEndTime = Timer::now();
    Timer::duration programRunningTime = programEndTime - programStartTime;
    std::chrono::duration<double, std::milli> milliTime = programRunningTime;
    std::cout << "Running time of program: " << milliTime.count() << "\n";*/

    //mean - Arithmetic Mean
    //(v1 + v2 + v3 + ... + vn) / n
    double meanDrawTime = 0.0;
    for (double dTime : drawTimes)
    {
        meanDrawTime += dTime;
    }
    meanDrawTime /= drawTimes.size();

    std::cout << "Mean Draw time: " << meanDrawTime << "ms \n";

    std::fstream frameTimeLog{ "./frametime.csv", std::ios::out | std::ios::ate };
    if (!frameTimeLog.is_open())
    {
        return -1;
    }
    frameTimeLog << "frametime, drawtime, crtupdate, crtcoll" << "\n";

    for (std::size_t i = 0u; i < frameTimes.size(); ++i)
    {
        double fTime = frameTimes[i];
        double dTime = drawTimes[i];
        double ucTime = updateCritterTimes[i];
        double colTime = collisionTimes[i];
        frameTimeLog << fTime << "," << dTime
        << "," << ucTime << "," <<colTime << "\n";
    }

    return 0;
}