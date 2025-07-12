#pragma once
#include "ParticleEnums.h"
#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <vector>
#include <random>
#include <raymath.h>
#include "Utility.h"
#include "Export.h"
#include "tinyfiledialogs.h"

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
protected:
	friend class ParticleSaver;
	std::vector<t_Particle> particles;
	std::mt19937 rng;
	std::uniform_real_distribution<float> dist;

	// Emitter properties
	EmitterType e_EmitterType;
	ParticleType e_ParticleType;
	Texture2D particle_texture;
	bool b_UseTexture;
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

	// For texture
	Rectangle texture_source_rect;
	Vector2 texture_half_size;
	Vector2 texture_center;
	int tex_width;
	int tex_height;
	int new_width;
	int new_height;

	int original_tex_width;
	int original_tex_height;
	float tex_size_percent;
	bool b_TextureDataCached;

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
	bool b_LoadTexture(const char* filename);
	void UnloadTexture();
	void SetUseTexture(bool use);
	bool b_IsUsingTexture() const;
};