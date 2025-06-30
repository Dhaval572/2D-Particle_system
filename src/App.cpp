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
	MaximizeWindow();

	SetTargetFPS(60);
	rlImGuiSetup(true);
	ImCustomTheme();

	particle_system.position.x = GetScreenWidth() * 0.05f + GetScreenWidth() * 0.6f / 2.0f;
	particle_system.position.y = GetScreenHeight() * 0.1f + GetScreenHeight() * 0.8f / 2.0f;
}

void App::Run()
{
	Init();

	while (!WindowShouldClose())
	{
		if (IsWindowResized())
		{
			particle_system.position.x = GetScreenWidth() * 0.05f + GetScreenWidth() * 0.6f / 2.0f;
			particle_system.position.y = GetScreenHeight() * 0.1f + GetScreenHeight() * 0.8f / 2.0f;
		}

		float delta_time = GetFrameTime();
		Update(delta_time);
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
