#include "App.h"

void App::Shutdown()
{
	rlImGuiShutdown();
	CloseWindow();
}

void App::Init()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(1200, 700, "Particle System Demo");
	SetTargetFPS(60);
	rlImGuiSetup(true);
	ImCustomTheme();

	particle_system.position = {100, 350};
}

void App::Run()
{
	Init();

	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();
		Update(deltaTime);
		Render();
	}
}

void App::Update(float dt)
{
	particle_system.Update(dt);
}

void App::Render()
{
	BeginDrawing();
	ClearBackground(BLACK);

	particle_system.Draw();

	rlImGuiBegin();
	DrawParticleSystemUI(particle_system);
	rlImGuiEnd();

	EndDrawing();
}

App::~App()
{
	Shutdown();
}