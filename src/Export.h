#pragma once
#include <optional>
#include <fstream>
#include "ParticleSystem.h"

class ParticleSystem;
class ParticleSaver
{
public:
	void Export(ParticleSystem& system,const char* path);
};