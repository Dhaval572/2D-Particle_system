#include "Export.h"

void ParticleSaver::SavePreset(ParticleSystem& system) 
{
    SavedPreset = ParticlePreset
	{
        system.max_particles,
        system.position,
        system.e_EmitterType,
        system.e_ParticleType,
        system.emission_rate,
        system.velocity,
        system.velocity_variation,
        system.acceleration,
        system.start_color,
        system.end_color,
        system.min_life,
        system.max_life,
        system.min_size,
        system.max_size,
        system.rotation_speed,
        system.line_length,
        system.circle_radius,
        system.rect_size,
        system.b_Active
    };
}

void ParticleSaver::LoadPreset(ParticleSystem& system) const
{
    if (!SavedPreset.has_value())
        return;
    
    const auto& PRESET = SavedPreset.value();
    system.max_particles = PRESET.max_particles;
    system.e_EmitterType = PRESET.e_EmitterType;
    system.e_ParticleType = PRESET.e_ParticleType;
    system.emission_rate = PRESET.emission_rate;
    system.velocity = PRESET.velocity;
    system.velocity_variation = PRESET.velocity_variation;
    system.acceleration = PRESET.acceleration;
    system.start_color = PRESET.start_color;
    system.end_color = PRESET.end_color;
    system.min_life = PRESET.min_life;
    system.max_life = PRESET.max_life;
    system.min_size = PRESET.min_size;
    system.max_size = PRESET.max_size;
    system.rotation_speed = PRESET.rotation_speed;
    system.line_length = PRESET.line_length;
    system.circle_radius = PRESET.circle_radius;
    system.rect_size = PRESET.rect_size;
    system.b_Active = PRESET.b_Active;
    
    system.Clear();
}