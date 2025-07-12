#pragma once
#include "ParticleSystem.h"
#include "ImGuiCustomTheme.h"

// Debug inheritance 
class Demo : public ParticleSystem
{
public:
	Demo() : ParticleSystem()
	{
		max_particles = 100;
	}
};

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

	Demo d;
};