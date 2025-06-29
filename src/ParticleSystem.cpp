#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
	: maxParticles(1000),
	  rng(std::random_device{}()),
	  dist(0.0f, 1.0f),
	  position({400, 300}),
	  emitterType(POINT),
	  emissionRate(50.0f),
	  emissionTimer(0.0f),
	  velocity({0, -50}),
	  velocityVariation({20, 20}),
	  acceleration({0, 98}),
	  startColor(RED),
	  endColor(ORANGE),
	  minLife(1.0f),
	  maxLife(3.0f),
	  minSize(2.0f),
	  maxSize(8.0f),
	  rotationSpeed(0.0f),
	  lineLength(100.0f),
	  circleRadius(50.0f),
	  rectSize({100, 100}),
	  active(true)
{
	particles.reserve(maxParticles);
}

Vector2 ParticleSystem::GetEmissionPoint()
{
	switch (emitterType)
	{
	case POINT:
		return position;

	case LINE:
	{
		float t = dist(rng);
		return {
			position.x + (t - 0.5f) * lineLength,
			position.y};
	}

	case CIRCLE:
	{
		float angle = dist(rng) * 2.0f * PI;
		float radius = dist(rng) * circleRadius;
		return {
			position.x + cosf(angle) * radius,
			position.y + sinf(angle) * radius};
	}

	case RECTANGLE:
	{
		return {
			position.x + (dist(rng) - 0.5f) * rectSize.x,
			position.y + (dist(rng) - 0.5f) * rectSize.y};
	}

	default:
		return position;
	}
}

void ParticleSystem::EmitParticle()
{
	if (particles.size() >= maxParticles)
		return;

	Particle p;
	p.position = GetEmissionPoint();
	p.velocity = {
		velocity.x + (dist(rng) - 0.5f) * velocityVariation.x,
		velocity.y + (dist(rng) - 0.5f) * velocityVariation.y};
	p.acceleration = acceleration;
	p.color = startColor;
	p.life = p.maxLife = minLife + dist(rng) * (maxLife - minLife);
	p.size = minSize + dist(rng) * (maxSize - minSize);
	p.rotation = 0;
	p.rotationSpeed = rotationSpeed + (dist(rng) - 0.5f) * 2.0f;
	p.active = true;

	particles.push_back(p);
}

void ParticleSystem::Update(float deltaTime)
{
	if (!active)
		return;

	// Emit new particles
	emissionTimer += deltaTime;
	while (emissionTimer >= 1.0f / emissionRate)
	{
		EmitParticle();
		emissionTimer -= 1.0f / emissionRate;
	}

	// Update existing particles
	for (auto it = particles.begin(); it != particles.end(); ++it)
	{
		Particle &p = *it;

		if (!p.active || p.life <= 0)
		{
			it = particles.erase(it);
			continue;
		}

		// Update physics
		p.velocity.x += p.acceleration.x * deltaTime;
		p.velocity.y += p.acceleration.y * deltaTime;
		p.position.x += p.velocity.x * deltaTime;
		p.position.y += p.velocity.y * deltaTime;

		p.rotation += p.rotationSpeed * deltaTime;
		p.life -= deltaTime;

		// This makes color transparent
		float t = 1.0f - (p.life / p.maxLife);
		p.color.r = Clamp(startColor.r * (1.0f - t) + endColor.r * t, 0, 255);
		p.color.g = Clamp(startColor.g * (1.0f - t) + endColor.g * t, 0, 255);
		p.color.b = Clamp(startColor.b * (1.0f - t) + endColor.b * t, 0, 255);
		p.color.a = Clamp(255.0f * (p.life / p.maxLife), 0, 255);
	}
}

void ParticleSystem::DrawEmitterShape()
{
	Color shapeColor = {128, 128, 128, 100}; // Gray with transparency

	switch (emitterType)
	{
	case POINT:
		DrawCircleV(position, 3, shapeColor);
		break;

	case LINE:
		DrawLineEx(
			{position.x - lineLength / 2, position.y},
			{position.x + lineLength / 2, position.y},
			2, shapeColor);
		break;

	case CIRCLE:
		DrawCircleLinesV(position, circleRadius, shapeColor);
		break;

	case RECTANGLE:
		DrawRectangleLinesEx(
			{position.x - rectSize.x / 2, position.y - rectSize.y / 2, rectSize.x, rectSize.y},	
			2, shapeColor);
		break;
	}
}

void ParticleSystem::Draw()
{

	Rectangle drawArea =
		{
			GetScreenWidth() * 0.05f,
			GetScreenHeight() * 0.1f,
			GetScreenWidth() * 0.6f,
			GetScreenHeight() * 0.8f};

	DrawRectangleLinesEx(drawArea, 2, GRAY);
	BeginScissorMode(drawArea.x, drawArea.y, drawArea.width, drawArea.height);

	for (const auto &p : particles)
	{
		if (!p.active)
			continue;

		// Draw particle as a circle (you can customize this)
		// DrawCircleV(p.position, p.size, p.color);

		// Alternative: Draw as rotated rectangle
		// Rectangle rect = {p.position.x - p.size / 2, p.position.y - p.size / 2, p.size, p.size};
		// DrawRectanglePro(rect, {p.size / 2, p.size / 2}, p.rotation * RAD2DEG, p.color);

		// Draw as triangle
		Vector2 vertices[3] = {
			{0, -p.size / 2},
			{-p.size / 2, p.size / 2},
			{p.size / 2, p.size / 2},
		};
		for (int i = 0; i < 3; ++i)
		{
			Vector2 &v1 = vertices[i];
			Vector2 &v2 = vertices[(i + 1) % 3];
			DrawLineEx(
				{p.position.x + v1.x * cosf(p.rotation) - v1.y * sinf(p.rotation),
				 p.position.y + v1.x * sinf(p.rotation) + v1.y * cosf(p.rotation)},
				{p.position.x + v2.x * cosf(p.rotation) - v2.y * sinf(p.rotation),
				 p.position.y + v2.x * sinf(p.rotation) + v2.y * cosf(p.rotation)},
				2, p.color);
		}
	}

	EndScissorMode();
	DrawEmitterShape();
}

void ParticleSystem::Clear()
{
	particles.clear();
}

int ParticleSystem::GetParticleCount() const
{
	return particles.size();
}

// ImGui interface for particle system
void DrawParticleSystemUI(ParticleSystem &ps)
{
	float screenW = static_cast<float>(GetScreenWidth());
	float screenH = static_cast<float>(GetScreenHeight());
	ImGui::SetNextWindowPos(ImVec2(screenW * 0.7f, screenH * 0.06f));
	ImGui::SetNextWindowSize(ImVec2(screenW * 0.30f, screenH * 0.9f));
	ImGui::Begin("Particle System Editor", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

	// Basic controls
	ImGui::Checkbox("Active", &ps.active);
	ImGui::SliderFloat("Emission Rate", &ps.emissionRate, 1.0f, 200.0f);
	ImGui::SliderInt("Max Particles", &ps.maxParticles, 100, 5000);

	ImGui::Separator();

	ImGui::Text("Emitter");
	static const char *emitterTypes[] = {"Point", "Line", "Circle", "Rectangle"};
	int currentType = static_cast<int>(ps.emitterType);

	ImGui::Combo("Type", &currentType, emitterTypes, IM_ARRAYSIZE(emitterTypes));
	ps.emitterType = static_cast<EmitterType>(currentType);

	Rectangle drawArea = {
		GetScreenWidth() * 0.05f,
		GetScreenHeight() * 0.1f,
		GetScreenWidth() * 0.6f,
		GetScreenHeight() * 0.8f};

	// Slider inputs for position
	ImGui::SliderFloat("Emitter X", &ps.position.x, drawArea.x, drawArea.x + drawArea.width);
	ImGui::SliderFloat("Emitter Y", &ps.position.y, drawArea.y, drawArea.y + drawArea.height);

	// Emitter shape properties
	switch (ps.emitterType)
	{
	case LINE:
		ImGui::SliderFloat("Line Length", &ps.lineLength, 10.0f, 300.0f);
		break;

	case CIRCLE:
		ImGui::SliderFloat("Circle Radius", &ps.circleRadius, 10.0f, 200.0f);
		break;

	case RECTANGLE:
		ImGui::SliderFloat2("Rectangle Size", (float *)&ps.rectSize, 10.0f, 300.0f);
		break;
	}

	ImGui::Separator();
	ImGui::Text("Particle Properties");
	ImGui::SliderFloat("Velocity X", &ps.velocity.x, -200.0f, 200.0f);
	ImGui::SliderFloat("Velocity Y", &ps.velocity.y, -200.0f, 200.0f);

	ImGui::SliderFloat("Velocity Variation X", &ps.velocityVariation.x, 0.0f, 100.0f);
	ImGui::SliderFloat("Velocity Variation Y", &ps.velocityVariation.y, 0.0f, 100.0f);

	ImGui::SliderFloat("Acceleration X", &ps.acceleration.x, -500.0f, 500.0f);
	ImGui::SliderFloat("Acceleration Y", &ps.acceleration.y, -500.0f, 500.0f);

	ImGui::SliderFloat("Min Life", &ps.minLife, 0.1f, 5.0f);
	ImGui::SliderFloat("Max Life", &ps.maxLife, 0.1f, 10.0f);

	ImGui::SliderFloat("Min Size", &ps.minSize, 1.0f, 20.0f);
	ImGui::SliderFloat("Max Size", &ps.maxSize, 1.0f, 50.0f);

	ImGui::SliderFloat("Rotation Speed", &ps.rotationSpeed, -10.0f, 10.0f);

	ImGui::Separator();
	ImGui::Text("Colors");

	ImVec4 startCol = ColorToImVec4(ps.startColor);
	if (ImGui::ColorEdit4("Start Color", reinterpret_cast<float *>(&startCol)))
	{
		ps.startColor = ImVec4ToColor(startCol);
	}

	ImVec4 endCol = ColorToImVec4(ps.endColor);
	if (ImGui::ColorEdit4("End Color", reinterpret_cast<float *>(&endCol)))
	{
		ps.endColor = ImVec4ToColor(endCol);
	}

	ImGui::Separator();
	if (ImGui::Button("Clear Particles"))
	{
		ps.Clear();
	}

	ImGui::Text("Active Particles: %d", ps.GetParticleCount());
	ImGui::End();
}