#pragma once
#include <rlimGui.h>
#include "ParticleSystem.h"
#include "ImGuiCustomTheme.h"

class App
{
public:
	~App();
	void Run();

private:
	void Init();
	void Update(float dt);
	void Render();
	void Shutdown();

	ParticleSystem particleSystem;
};