#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
	: max_particles(1000),
	  rng(std::random_device{}()),
	  dist(0.0f, 1.0f),
	  position({400, 300}),
	  emitterType(POINT),
	  emission_rate(50.0f),
	  emission_timer(0.0f),
	  velocity({0, -50}),
	  velocity_variation({20, 20}),
	  acceleration({0, 98}),
	  start_color(RED),
	  end_color(ORANGE),
	  min_life(1.0f),
	  max_life(3.0f),
	  min_size(2.0f),
	  max_size(8.0f),
	  rotation_speed(0.0f),
	  line_length(100.0f),
	  circle_radius(50.0f),
	  rect_size({100, 100}),
	  active(true)
{
	particles.reserve(max_particles);
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
			position.x + (t - 0.5f) * line_length,
			position.y};
	}

	case CIRCLE:
	{
		float angle = (dist(rng) * 2.0f - 1.0f) * PI; // [-PI, PI]
		float radius = dist(rng) * circle_radius;	  // [0, circle_radius]
		return {
			position.x + cosf(angle) * radius,
			position.y + sinf(angle) * radius};
	}

	case RECTANGLE:
	{
		return {
			position.x + (dist(rng) - 0.5f) * rect_size.x,
			position.y + (dist(rng) - 0.5f) * rect_size.y};
	}

	default:
		return position;
	}
}

void ParticleSystem::EmitParticle()
{
	if (particles.size() >= max_particles)
		return;

	Particle p;
	p.position = GetEmissionPoint();
	p.velocity = {
		velocity.x + (dist(rng) - 0.5f) * velocity_variation.x,
		velocity.y + (dist(rng) - 0.5f) * velocity_variation.y};
	p.acceleration = acceleration;
	p.color = start_color;
	p.life = p.max_life = min_life + dist(rng) * (max_life - min_life);
	p.size = min_size + dist(rng) * (max_size - min_size);
	p.rotation = 0;
	p.rotation_speed = rotation_speed + (dist(rng) - 0.5f) * 2.0f;
	p.active = true;

	particles.push_back(p);
}

void ParticleSystem::Update(float deltaTime)
{
	if (!active)
		return;

	// Emit new particles
	emission_timer += deltaTime;
	while (emission_timer >= 1.0f / emission_rate)
	{
		EmitParticle();
		emission_timer -= 1.0f / emission_rate;
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

		p.rotation += p.rotation_speed * deltaTime;
		p.life -= deltaTime;

		// This makes color transparent
		float t = 1.0f - (p.life / p.max_life);
		p.color.r = Clamp(start_color.r * (1.0f - t) + end_color.r * t, 0, 255);
		p.color.g = Clamp(start_color.g * (1.0f - t) + end_color.g * t, 0, 255);
		p.color.b = Clamp(start_color.b * (1.0f - t) + end_color.b * t, 0, 255);
		p.color.a = Clamp(255.0f * (p.life / p.max_life), 0, 255);
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
			{position.x - line_length / 2, position.y},
			{position.x + line_length / 2, position.y},
			2, shapeColor);
		break;

	case CIRCLE:
		DrawCircleLinesV(position, circle_radius, shapeColor);
		break;

	case RECTANGLE:
		DrawRectangleLinesEx(
			{position.x - rect_size.x / 2, position.y - rect_size.y / 2, rect_size.x, rect_size.y},
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
	ImGui::SliderFloat("Emission Rate", &ps.emission_rate, 1.0f, 200.0f);
	ImGui::SliderInt("Max Particles", &ps.max_particles, 100, 5000);

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
		ImGui::SliderFloat("Line Length", &ps.line_length, 10.0f, 300.0f);
		break;

	case CIRCLE:
		ImGui::SliderFloat("Circle Radius", &ps.circle_radius, 10.0f, 200.0f);
		break;

	case RECTANGLE:
		ImGui::SliderFloat2("Rectangle Size", (float *)&ps.rect_size, 10.0f, 300.0f);
		break;
	}

	ImGui::Separator();
	ImGui::Text("Particle Properties");
	ImGui::SliderFloat("Velocity X", &ps.velocity.x, -200.0f, 200.0f);
	ImGui::SliderFloat("Velocity Y", &ps.velocity.y, -200.0f, 200.0f);

	ImGui::SliderFloat("Velocity Variation X", &ps.velocity_variation.x, 0.0f, 100.0f);
	ImGui::SliderFloat("Velocity Variation Y", &ps.velocity_variation.y, 0.0f, 100.0f);

	ImGui::SliderFloat("Acceleration X", &ps.acceleration.x, -500.0f, 500.0f);
	ImGui::SliderFloat("Acceleration Y", &ps.acceleration.y, -500.0f, 500.0f);

	ImGui::SliderFloat("Min Life", &ps.min_life, 0.1f, 5.0f);
	ImGui::SliderFloat("Max Life", &ps.max_life, 0.1f, 10.0f);

	ImGui::SliderFloat("Min Size", &ps.min_size, 1.0f, 20.0f);
	ImGui::SliderFloat("Max Size", &ps.max_size, 1.0f, 50.0f);

	ImGui::SliderFloat("Rotation Speed", &ps.rotation_speed, -10.0f, 10.0f);

	ImGui::Separator();
	ImGui::Text("Colors");

	ImVec4 startCol = ColorToImVec4(ps.start_color);
	if (ImGui::ColorEdit4("Start Color", reinterpret_cast<float *>(&startCol)))
	{
		ps.start_color = ImVec4ToColor(startCol);
	}

	ImVec4 endCol = ColorToImVec4(ps.end_color);
	if (ImGui::ColorEdit4("End Color", reinterpret_cast<float *>(&endCol)))
	{
		ps.end_color = ImVec4ToColor(endCol);
	}

	ImGui::Separator();
	if (ImGui::Button("Clear Particles"))
	{
		ps.Clear();
	}

	ImGui::Text("Active Particles: %d", ps.GetParticleCount());
	ImGui::End();
}