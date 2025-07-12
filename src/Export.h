#pragma once
#include <optional>
#include <fstream>
#include "ParticleSystem.h"

class ParticleSystem;
struct t_ParticleSaver
{
	void Export(ParticleSystem& system,const char* path);
};