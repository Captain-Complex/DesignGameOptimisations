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
#include "DoubleLinkedList.hpp"
#include "SpacialGrid.hpp" 

#include "raylib.h"
#include "raymath.h"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <random>
#include <vector>

int main(int argc, char* argv[])
{
	using Timer = std::chrono::high_resolution_clock;

	SetTraceLogLevel(LOG_ERROR);

	// Initialization
	//--------------------------------------------------------------------------------------
	int screenWidth = 1280;
	int screenHeight = 720;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

	//SetTargetFPS(60);
	//--------------------------------------------------------------------------------------

	srand(time(NULL));

	Critter critters[1000];
	DLinkList<Critter*> deadCritters{};
	DLinkList<Critter*> aliveCritters{};
	// create some critters
	const int CRITTER_COUNT = 1000;
	const int MAX_VELOCITY = 80;

	for (int i = 0; i < CRITTER_COUNT; i++)
	{
		aliveCritters.PushBack(&critters[i]);

		// create a random direction vector for the velocity
		Vector2 velocity = { -100 + (rand() % 200), -100 + (rand() % 200) };
		// normalize and scale by a random speed
		velocity = Vector2Scale(Vector2Normalize(velocity), MAX_VELOCITY);

		// create a critter in a random location
		critters[i].Init(
			{ (float)(5 + rand() % (screenWidth - 10)), (float)(5 + (rand() % screenHeight - 10)) },
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

	const int columnCount = 16;
	const float columnWidth = screenWidth / columnCount;
	const int rowCount = 9;
	const float rowHeight = screenHeight / rowCount;
	SpacialGrid<Critter*> grid{ rowCount,  columnCount, columnWidth, rowHeight };
	
	// Main game loop
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		Timer::time_point frameBegin = Timer::now();

		// Update
		//----------------------------------------------------------------------------------
		// TODO: Update your variables here
		//----------------------------------------------------------------------------------

		float delta = GetFrameTime();
		int fps = GetFPS();

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
		for (Node<Critter*>* critter = aliveCritters.head; critter != nullptr;)
		{
			critter->data->Update(delta);
			float cx = critter->data->GetX();
			float cy = critter->data->GetY();
			// check each critter against screen bounds
			if (cx < 0) {
				critter->data->SetX(0);
				critter->data->SetVelocity(Vector2{ -critter->data->GetVelocity().x, critter->data->GetVelocity().y });
			}
			if (cx > screenWidth) {
				critter->data->SetX(screenWidth);
				critter->data->SetVelocity(Vector2{ -critter->data->GetVelocity().x, critter->data->GetVelocity().y });
			}
			if (cy < 0) {
				critter->data->SetY(0);
				critter->data->SetVelocity(Vector2{ critter->data->GetVelocity().x, -critter->data->GetVelocity().y });
			}
			if (cy > screenHeight) {
				critter->data->SetY(screenHeight);
				critter->data->SetVelocity(Vector2{ critter->data->GetVelocity().x, -critter->data->GetVelocity().y });
			}

			// kill any critter touching the destroyer
			// simple circle-to-circle collision check
			float dist = Vector2Distance(critter->data->GetPosition(), destroyer.GetPosition());
			if (dist < critter->data->GetRadius() + destroyer.GetRadius())
			{
				// this would be the perfect time to put the critter into an object pool
				deadCritters.PushBack(critter->data);
				critter->data->Destroy();
				Node<Critter*>* tempCritter = critter;
				critter = critter->next;
				aliveCritters.Erase(tempCritter);
			}
			else
			{
				int ciMinColumn = static_cast<int>(critter->data->GetX() / columnWidth);
				critter->data->minColumnID = ciMinColumn;
				int ciMaxColumn = static_cast<int>(critter->data->GetX() +
					critter->data->GetRadius() * 2 / columnWidth);
				critter->data->maxColumnID = ciMaxColumn;
				critter = critter->next;
			}

		}

		Timer::time_point updateCritterEnd = Timer::now();
		Timer::duration updateCritterTime = updateCritterEnd - updateCritterBegin;
		std::chrono::duration<double, std::milli> milliUpdateCritter = updateCritterTime;
		updateCritterTimes.push_back(milliUpdateCritter.count());

		Timer::time_point collisionBegin = Timer::now();

		for (Node<Critter*>* critter = aliveCritters.head; critter != nullptr; critter = critter->next)
		{
			grid.Insert(critter->data->GetX(), critter->data->GetY(),
				critter->data->GetRadius() * 2, critter->data->GetRadius() * 2, critter->data);
		}
		
		// check for critter-on-critter collisions

		for (int i = 0; i < grid.Size(); i++)
		{
			DLinkList<Critter*>& cellGrid = grid.cells[i];

			for (Node<Critter*>* critter = cellGrid.head; critter != nullptr; critter = critter->next)
			{
				Vector2 v1 = critter->data->GetPosition();
				float c1Radus = critter->data->GetRadius();
				int ciMinColumn = critter->data->minColumnID;
				int ciMaxColumn = critter->data->maxColumnID;
				
				for (Node<Critter*>* critterj = critter->next; critterj != nullptr; critterj = critterj->next)
				{

					// check every critter against every other critter
					Vector2 v2 = critterj->data->GetPosition();
					
					if (critter->data->IsDirty()) // note: the other critter (j) could be dirty - that's OK
						continue;
					Vector2 diff = Vector2Subtract(v1, v2);
					float distSqr = diff.x * diff.x + diff.y * diff.y;
					float sumOfRadi = c1Radus + critterj->data->GetRadius();
					//float dist = Vector2Distance(critters[i].GetPosition(), critters[j].GetPosition());
					if (distSqr < sumOfRadi * sumOfRadi)
					{
						if (diff.x == 0 && diff.y == 0)
						{
							continue;
						}
						// collision!
						// do math to get critters bouncing
						Vector2 normal = Vector2Normalize(Vector2Subtract(critterj->data->GetPosition(), critter->data->GetPosition()));
                         
						// not even close to real physics, but fine for our needs
						critter->data->SetVelocity(Vector2Scale(normal, -MAX_VELOCITY));
						// set the critter to *dirty* so we know not to process any more collisions on it
						critter->data->SetDirty();

						// we still want to check for collisions in the case where 1 critter is dirty - so we need a check 
						// to make sure the other critter is clean before we do the collision response
						if (!critterj->data->IsDirty()) {
							critterj->data->SetVelocity(Vector2Scale(normal, MAX_VELOCITY));
							critterj->data->SetDirty();
						}
						break;
					}
				}
			}
		}
		grid.Clear();
		Timer::time_point collisionEnd = Timer::now();
		Timer::duration collisionTime = collisionEnd - collisionBegin;
		std::chrono::duration<double, std::milli> milliCollision = collisionTime;
		collisionTimes.push_back(milliCollision.count());

		timer -= delta;
		if (timer <= 0)
		{
			timer = 1;

			// find any dead critters and spit them out (respawn)
			//for (int i = 0; i < deadCritters.listSize; i++)

			if (deadCritters.listSize > 0)
			{

				aliveCritters.PushBack(deadCritters.head->data);
				deadCritters.PopFront();
				Vector2 normal = Vector2Normalize(destroyer.GetVelocity());

				// get a position behind the destroyer, and far enough away that the critter won't bump into it again
				Vector2 pos = destroyer.GetPosition();
				pos = Vector2Add(pos, Vector2Scale(normal, -50));
				// its pretty ineficient to keep reloading textures. ...if only there was something else we could do
				aliveCritters.tail->data->Init(pos, Vector2Scale(normal, -MAX_VELOCITY), 12, "res/10.png");

			}

			nextSpawnPos = destroyer.GetPosition();
		}

		// Draw
		//----------------------------------------------------------------------------------
		Timer::time_point drawBegin = Timer::now();
		BeginDrawing();

		ClearBackground(RAYWHITE);

		// draw the critters
		for (Node<Critter*>* critter = aliveCritters.head; critter != nullptr; critter = critter->next)
		{
			critter->data->Draw();
		}
		// draw the destroyer
		// (if you're wondering why it looks a little odd when sometimes critters are destroyed when they're not quite touching the 
		// destroyer, it's because the origin is at the top-left. ...you could fix that!)
		destroyer.Draw();
		DrawText(TextFormat("%i FPS", fps), 10, 10, 20, RED);

		int destroyerColumn = static_cast<int>(destroyer.GetX() / columnWidth);

		Timer::time_point drawEnd = Timer::now();
		Timer::duration drawTime = drawEnd - drawBegin;
		std::chrono::duration<double, std::milli> milliDraw = drawTime;
		drawTimes.push_back(milliDraw.count());

		EndDrawing();

		/*std::cout << "Draw time: " << milliTime.count() << "\n";*/
		//----------------------------------------------------------------------------------

		Timer::time_point frameEnd = Timer::now();
		Timer::duration frameTime = frameEnd - frameBegin;
		std::chrono::duration<double, std::milli> milliFrame = frameTime;
		frameTimes.push_back(milliFrame.count());
	}

	for (Node<Critter*>* critter = aliveCritters.head; critter != nullptr; critter = critter->next)
	{
		critter->data->Destroy();
	}

	// De-Initialization
	//--------------------------------------------------------------------------------------   
	CloseWindow();        // Close window and OpenGL context
	//--------------------------------------------------------------------------------------

	double meanFrameTime = 0.0;
	for (double fTime : frameTimes)
	{
		meanFrameTime += fTime;
	}
	meanFrameTime /= frameTimes.size();

	std::cout << "Mean frame time: " << meanFrameTime << "ms \n";

	std::fstream frameTimeLog{ "./frametime.csv", std::ios::out | std::ios::ate };
	if (!frameTimeLog.is_open())
	{
		return -1;
	}
	frameTimeLog << "frametime, crtcoll, drawtime, crtupdate " << "\n";

	for (std::size_t i = 0u; i < frameTimes.size(); ++i)
	{
		double fTime = frameTimes[i];
		double dTime = drawTimes[i];
		double ucTime = updateCritterTimes[i];
		double colTime = collisionTimes[i];
		frameTimeLog << fTime << "," << colTime
			<< "," << dTime << "," << ucTime << "\n";
	}

	return 0;
}