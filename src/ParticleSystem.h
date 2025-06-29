#pragma once
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <vector>
#include <random>
#include <raymath.h>
#include "Utility.h"

struct t_Particle
{
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Color color;
	float life;
	float max_life;
	float size;
	float rotation;
	float rotation_speed;
	bool active;
};

enum ParticleType
{
	CIRCULER,
	SQUARE,
	TRIANGLE
};

enum EmitterType
{
	POINT,
	LINE,
	CIRCLE,
	RECTANGLE
};

// Particle system class
class ParticleSystem
{
private:
	std::vector<t_Particle> particles;
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist;

	// Emitter properties
	EmitterType e_EmitterType;
	ParticleType e_PType;
	float emission_rate;
	float emission_timer;

	// Particle properties
	Vector2 velocity;
	Vector2 velocity_variation;
	Vector2 acceleration;
	Color start_color;
	Color end_color;
	float min_life;
	float max_life;
	float min_size;
	float max_size;
	float rotation_speed;

	// Emitter shape properties
	float line_length;
	float circle_radius;
	Vector2 rect_size;

	bool active;
	int max_particles;

	// Core methods
	Vector2 GetEmissionPoint();
	void EmitParticle();
	void DrawEmitterShape();
	void Clear();
	int GetParticleCount() const;
	friend void DrawParticleSystemUI(ParticleSystem &ps);

public:
	Vector2 position;

	ParticleSystem();
	void Update(float deltaTime);
	void Draw();
};