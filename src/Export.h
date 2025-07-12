#pragma once
#include <optional>
#include "ParticleSystem.h"

class ParticleSystem;
struct ParticlePreset
{
	int max_particles;
	Vector2 position;
	EmitterType e_EmitterType;
	ParticleType e_ParticleType;

	float emission_rate;
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
	float line_length;
	float circle_radius;
	Vector2 rect_size;
	bool b_Active;
};

class ParticleSaver
{
private:
	std::optional<ParticlePreset> SavedPreset;

public:
	void SavePreset(ParticleSystem& system);
	void LoadPreset(ParticleSystem& system);
};