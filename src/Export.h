#pragma once
#include <fstream> // IWYU pragma: keep
#include <filesystem> // IWYU pragma: keep
#include "ParticleSystem.h"

class ParticleSystem;
struct t_ParticleSaver
{
	void Export(ParticleSystem& system, const char* path);
};