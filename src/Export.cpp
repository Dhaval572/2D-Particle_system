// Export.cpp
#include "Export.h"

void ParticleSaver::SavePreset(ParticleSystem& system)  // Correct implementation
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

void ParticleSaver::LoadPreset(ParticleSystem& system) 
{
    if (!SavedPreset.has_value()) return;
    
    auto& preset = SavedPreset.value();
    
    system.max_particles = preset.max_particles;
    system.position = preset.position;
    system.e_EmitterType = preset.e_EmitterType;
    system.e_ParticleType = preset.e_ParticleType;
    system.emission_rate = preset.emission_rate;
    system.velocity = preset.velocity;
    system.velocity_variation = preset.velocity_variation;
    system.acceleration = preset.acceleration;
    system.start_color = preset.start_color;
    system.end_color = preset.end_color;
    system.min_life = preset.min_life;
    system.max_life = preset.max_life;
    system.min_size = preset.min_size;
    system.max_size = preset.max_size;
    system.rotation_speed = preset.rotation_speed;
    system.line_length = preset.line_length;
    system.circle_radius = preset.circle_radius;
    system.rect_size = preset.rect_size;
    system.b_Active = preset.b_Active;
    
    system.Clear();
}