#include "App.h"

void App::Shutdown()
{
	rlImGuiShutdown();
	CloseWindow();
}

void App::Init()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(0, 0, "Particle System Demo");
	MaximizeWindow();

	SetTargetFPS(60);
	rlImGuiSetup(true);
	ImCustomTheme();

	d.position.x = GetScreenWidth() * 0.05f + GetScreenWidth() * 0.6f / 2.0f;
	d.position.y = GetScreenHeight() * 0.1f + GetScreenHeight() * 0.8f / 2.0f;
}

void App::Run()
{
	Init();

	while (!WindowShouldClose())
	{
		if (IsWindowResized())
		{
			d.position.x =
				GetScreenWidth() * 0.05f + GetScreenWidth() * 0.6f / 2.0f;

			d.position.y =
				GetScreenHeight() * 0.1f + GetScreenHeight() * 0.8f / 2.0f;
		}

		float delta_time = GetFrameTime();
		Update(delta_time);
		Render();
	}
}

void App::Update(float dt)
{
	d.Update(dt);
}

void App::Render()
{
	BeginDrawing();
	ClearBackground(BLACK);

	d.Draw();

	rlImGuiBegin();
	DrawParticleSystemUI(d);
	rlImGuiEnd();

	EndDrawing();
}

App::~App()
{
	Shutdown();
}