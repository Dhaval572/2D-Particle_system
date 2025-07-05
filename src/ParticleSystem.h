#pragma once
#include "ParticleEnums.h"
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <vector>
#include <random>
#include <raymath.h>
#include "Utility.h"

class ParticleSaver;
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
	bool b_Active;
};

class ParticleSystem
{
private:
	friend class ParticleSaver;
	std::vector<t_Particle> particles;
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist;

	// Emitter properties
	EmitterType e_EmitterType;
	ParticleType e_ParticleType;
	Texture2D particle_texture;
	bool use_texture;
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

	bool b_Active;
	int max_particles;

	Vector2 GetEmissionPoint();
	void EmitParticle();
	void DrawEmitterShape() const;
	void Clear();
	int GetParticleCount() const;
	friend void DrawParticleSystemUI(ParticleSystem& ps);

public:
	Vector2 position;

	ParticleSystem();
	~ParticleSystem();
	void Update(float dt);
	void Draw();

	// Texture management
	bool LoadTexture(const char* filename);
	void UnloadTexture();
	void SetUseTexture(bool use);
	bool IsUsingTexture() const;

	// Texture access for UI preview
	const Texture2D& GetTexture() const { return particle_texture; }
};