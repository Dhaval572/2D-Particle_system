#include "App.h"

void App::Shutdown()
{
	rlImGuiShutdown();
	CloseWindow();
}

void App::Init()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(0, 0, "Kinesis Particle System");
	MaximizeWindow();
	Image icon = LoadImage("assets/Images/K-icon.png");
	SetWindowIcon(icon);
	UnloadImage(icon);

	SetTargetFPS(60);
	rlImGuiSetup(true);
	ImCustomTheme();

	ps.position.x = GetScreenWidth() * 0.05f + GetScreenWidth() * 0.6f / 2.0f;
	ps.position.y = GetScreenHeight() * 0.1f + GetScreenHeight() * 0.8f / 2.0f;
}

void App::Run()
{
	Init();

	while (!WindowShouldClose())
	{
		if (IsWindowResized())
		{
			ps.position.x =
				GetScreenWidth() * 0.05f + GetScreenWidth() * 0.6f / 2.0f;

			ps.position.y =
				GetScreenHeight() * 0.1f + GetScreenHeight() * 0.8f / 2.0f;
		}

		float delta_time = GetFrameTime();
		Update(delta_time);
		Render();
	}
}

void App::Update(float dt)
{
	ps.Update(dt);
}

void App::Render()
{
	BeginDrawing();
	ClearBackground(BLACK);

	ps.Draw();

	rlImGuiBegin();
	DrawParticleSystemUI(ps);
	rlImGuiEnd();

	EndDrawing();
}

App::~App()
{
	Shutdown();
}