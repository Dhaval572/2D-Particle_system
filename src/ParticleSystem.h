#pragma once
#include <rlImGui.h>
#include <imgui.h>
#include <vector>
#include <random>
#include <raymath.h>
#include <cmath>
#include "Utility.h"

struct Particle
{
	Vector2 position;
	Vector2 velocity;
	Vector2 acceleration;
	Color color;
	float life;
	float maxLife;
	float size;
	float rotation;
	float rotationSpeed;
	bool active;
};

// Emitter types
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
	std::vector<Particle> particles;
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist;

	// Emitter properties
	EmitterType emitterType;
	float emissionRate;
	float emissionTimer;

	// Particle properties
	Vector2 velocity;
	Vector2 velocityVariation;
	Vector2 acceleration;
	Color startColor;
	Color endColor;
	float minLife;
	float maxLife;
	float minSize;
	float maxSize;
	float rotationSpeed;

	// Emitter shape properties
	float lineLength;
	float circleRadius;
	Vector2 rectSize;

	bool active;
	int maxParticles;

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