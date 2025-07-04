﻿#include "ParticleSystem.h"

ParticleSystem::ParticleSystem()
	: max_particles(1000),
	rng(std::random_device{}()),
	dist(0.0f, 1.0f),
	position({}),
	e_EmitterType(POINT),
	e_ParticleType(CIRCULER),
	particle_texture({}),
	use_texture(false),
	emission_rate(50.0f),
	emission_timer(0.0f),
	velocity({ 0, -50 }),
	velocity_variation({ 20, 20 }),
	acceleration({ 0, 98 }),
	start_color(DARKGREEN),
	end_color(GREEN),
	min_life(1.0f),
	max_life(3.0f),
	min_size(2.0f),
	max_size(8.0f),
	rotation_speed(0.0f),
	line_length(100.0f),
	circle_radius(50.0f),
	rect_size({ 100, 100 }),
	b_Active(true)
{
	particles.reserve(max_particles);
}

ParticleSystem::~ParticleSystem()
{
	UnloadTexture();
}

bool ParticleSystem::LoadTexture(const char* filename)
{
	// Unload existing texture first
	UnloadTexture();

	particle_texture = LoadTextureFromImage(LoadImage(filename));

	if (particle_texture.id > 0)
	{
		use_texture = true;
		return true;
	}

	use_texture = false;
	return false;
}

void ParticleSystem::UnloadTexture()
{
	if (particle_texture.id > 0)
	{
		::UnloadTexture(particle_texture);
		particle_texture = {};
	}
	use_texture = false;
}

void ParticleSystem::SetUseTexture(bool use)
{
	use_texture = use && (particle_texture.id > 0);
}

bool ParticleSystem::IsUsingTexture() const
{
	return use_texture && (particle_texture.id > 0);
}

Vector2 ParticleSystem::GetEmissionPoint()
{
	switch (e_EmitterType)
	{
	case POINT:
		return position;

	case LINE:
	{
		float t = dist(rng);
		return
		{
			position.x + (t - 0.5f) * line_length,
			position.y
		};
	}

	case CIRCLE:
	{
		float angle = (dist(rng) * 2.0f - 1.0f) * PI; // [-PI, PI]
		float radius = dist(rng) * circle_radius;	  // [0, circle_radius]
		return
		{
			position.x + cosf(angle) * radius,
			position.y + sinf(angle) * radius
		};
	}

	case RECTANGLE:
	{
		return
		{
			position.x + (dist(rng) - 0.5f) * rect_size.x,
			position.y + (dist(rng) - 0.5f) * rect_size.y
		};
	}

	default:
		return position;
	}
}

void ParticleSystem::EmitParticle()
{
	if (particles.size() >= max_particles)
		return;

	t_Particle p;
	p.position = GetEmissionPoint();
	p.velocity =
	{
		velocity.x + (dist(rng) - 0.5f) * velocity_variation.x,
		velocity.y + (dist(rng) - 0.5f) * velocity_variation.y
	};
	p.acceleration = acceleration;
	p.color = start_color;
	p.life = p.max_life = min_life + dist(rng) * (max_life - min_life);
	p.size = min_size + dist(rng) * (max_size - min_size);
	p.rotation = 0;
	p.rotation_speed = rotation_speed + (dist(rng) - 0.5f) * 2.0f;
	p.b_Active = true;
	particles.push_back(p);
}

void ParticleSystem::Update(float dt)
{
	if (!b_Active)
		return;

	// Emit new particles
	emission_timer += dt;
	while (emission_timer >= 1.0f / emission_rate)
	{
		EmitParticle();
		emission_timer -= 1.0f / emission_rate;
	}

	// Update existing particles
	for (auto it = particles.begin(); it != particles.end(); ++it)
	{
		t_Particle& p = *it;

		if (!p.b_Active || p.life <= 0)
		{
			it = particles.erase(it);
			continue;
		}

		// Update physics
		p.velocity.x += p.acceleration.x * dt;
		p.velocity.y += p.acceleration.y * dt;
		p.position.x += p.velocity.x * dt;
		p.position.y += p.velocity.y * dt;

		p.rotation += p.rotation_speed * dt;
		p.life -= dt;

		// Interpolate color and apply alpha based on life
		float t = 1.0f - (p.life / p.max_life);
		p.color.r = Clamp(start_color.r * (1.0f - t) + end_color.r * t, 0, 255);
		p.color.g = Clamp(start_color.g * (1.0f - t) + end_color.g * t, 0, 255);
		p.color.b = Clamp(start_color.b * (1.0f - t) + end_color.b * t, 0, 255);
		p.color.a = Clamp(255.0f * (p.life / p.max_life), 0, 255);
	}
}

void ParticleSystem::DrawEmitterShape()
{
	Color shape_color = { 128, 128, 128, 100 }; // Gray with transparency

	switch (e_EmitterType)
	{
	case POINT:
		DrawCircleV(position, 3, shape_color);
		break;

	case LINE:
		DrawLineEx
		(
			{ position.x - line_length / 2, position.y },
			{ position.x + line_length / 2, position.y },
			2, shape_color
		);
		break;

	case CIRCLE:
		DrawCircleLinesV(position, circle_radius, shape_color);
		break;

	case RECTANGLE:
		DrawRectangleLinesEx
		(
			{
				position.x - rect_size.x / 2,
				position.y - rect_size.y / 2,
				rect_size.x, rect_size.y
			},
			2, shape_color
		);
		break;
	}
}

void ParticleSystem::Draw()
{
	Rectangle draw_area =
	{
		GetScreenWidth() * 0.05f,
		GetScreenHeight() * 0.1f,
		GetScreenWidth() * 0.6f,
		GetScreenHeight() * 0.8f
	};

	DrawRectangleLinesEx(draw_area, 2, GRAY);
	BeginScissorMode(draw_area.x, draw_area.y, draw_area.width, draw_area.height);

	for (const auto& p : particles)
	{
		if (!p.b_Active)
			continue;

		// If using texture, draw texture instead of geometric shapes
		if (IsUsingTexture())
		{
			Rectangle source = { 0, 0, (float)particle_texture.width, (float)particle_texture.height };
			Rectangle dest = {
				p.position.x - p.size / 2,
				p.position.y - p.size / 2,
				p.size,
				p.size
			};
			Vector2 origin = { p.size / 2, p.size / 2 };

			DrawTexturePro(particle_texture, source, dest, origin, p.rotation * RAD2DEG, p.color);
		}
		else
		{
			// Draw geometric shapes as before
			switch (e_ParticleType)
			{
			case CIRCULER:
			{
				DrawCircleV(p.position, p.size, p.color);
				break;
			}

			case SQUARE:
			{
				Rectangle rect =
				{
					p.position.x - p.size / 2,
					p.position.y - p.size / 2,
					p.size,
					p.size
				};

				DrawRectanglePro
				(
					rect,
					{ p.size / 2, p.size / 2 },
					p.rotation * RAD2DEG,
					p.color
				);
				break;
			}

			case TRIANGLE:
			{
				DrawPoly
				(
					p.position,
					3,
					p.size,
					p.rotation * RAD2DEG,
					p.color
				);
				break;
			}

			case K_CHAR:
			{
				Vector2 vertices[6] =
				{
					{-p.size / 2, p.size / 2},	// Top-left (vertical line top)
					{-p.size / 2, -p.size / 2}, // Bottom-left (vertical line bottom)
					{-p.size / 2, 0},			// Middle-left (center junction)
					{p.size / 2, p.size / 2},	// Top-right (upper diagonal end)
					{p.size / 2, -p.size / 2},	// Bottom-right (lower diagonal end)
					{-p.size / 2, 0}			// Middle-left (center junction - repeated for connection)
				};

				// Define the lines that make up the K shape
				int lines[][2] =
				{
					{0, 1}, // Vertical line (left side)
					{2, 3}, // Upper diagonal
					{2, 4}	// Lower diagonal
				};

				// Draw each line of the K
				for (int i = 0; i < 3; ++i)
				{
					Vector2& v1 = vertices[lines[i][0]];
					Vector2& v2 = vertices[lines[i][1]];

					// Apply rotation transformation
					Vector2 a =
					{
						p.position.x + v1.x * cosf(p.rotation) - v1.y * sinf(p.rotation),
						p.position.y + v1.x * sinf(p.rotation) + v1.y * cosf(p.rotation)
					};
					Vector2 b =
					{
						p.position.x + v2.x * cosf(p.rotation) - v2.y * sinf(p.rotation),
						p.position.y + v2.x * sinf(p.rotation) + v2.y * cosf(p.rotation)
					};

					DrawLineEx(a, b, 2, p.color);
				}
				break;
			}
			}
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

void DrawParticleSystemUI(ParticleSystem& ps)
{
	float s_width = static_cast<float>(GetScreenWidth());
	float s_height = static_cast<float>(GetScreenHeight());
	ImGui::SetNextWindowPos(ImVec2(s_width * 0.654f, s_height * 0.06f));
	ImGui::SetNextWindowSize(ImVec2(s_width * 0.35f, s_height * 0.9f));
	ImGui::Begin
	(
		"Particle System Editor",
		nullptr,
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove
	);

	// Settings
	ImGui::Checkbox("Active", &ps.b_Active);
	ImGui::SliderFloat("Emission Rate", &ps.emission_rate, 1.0f, 100.0f);
	ImGui::SliderInt("Max Particles", &ps.max_particles, 100, 5000);

	ImGui::Separator();

	// Texture settings
	ImGui::Text("Texture Settings");

	// Static buffer for texture path input
	static char texture_path[512] = "";
	static bool load_failed = false;
	static char error_message[256] = "";

	// Texture path input field
	ImGui::InputText("Texture Path", texture_path, sizeof(texture_path));

	// Load and Clear buttons
	if (ImGui::Button("Load Texture"))
	{
		if (strlen(texture_path) > 0)
		{
			if (ps.LoadTexture(texture_path))
			{
				load_failed = false;
				strcpy(error_message, "");
			}
			else
			{
				load_failed = true;
				snprintf(error_message, sizeof(error_message), "Failed to load: %s", texture_path);
			}
		}
		else
		{
			load_failed = true;
			strcpy(error_message, "Please enter a texture path");
		}
	}

	ImGui::SameLine();
	if (ImGui::Button("Clear Texture"))
	{
		ps.UnloadTexture();
		load_failed = false;
		strcpy(error_message, "");
	}

	// Use texture checkbox
	bool use_texture = ps.IsUsingTexture();
	if (ImGui::Checkbox("Use Texture", &use_texture))
	{
		ps.SetUseTexture(use_texture);
	}

	// Status display
	if (ps.IsUsingTexture())
	{
		ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Texture loaded and active");
	}
	else if (load_failed)
	{
		ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "%s", error_message);
	}
	else
	{
		ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.7f, 1.0f), "No texture loaded");
	}

	ImGui::Text("Supported: PNG, JPG, BMP, etc");

	ImGui::Separator();
	ImGui::Text("Emitter");
	static const char* s_EMITTER_TYPES[] =
	{
		"Point", "Line", "Circle", "Rectangle"
	};
	ImGui::Combo
	(
		"Type",
		reinterpret_cast<int*>(&ps.e_EmitterType),
		s_EMITTER_TYPES,
		IM_ARRAYSIZE(s_EMITTER_TYPES)
	);

	// Only show particle type selection if not using texture
	if (!ps.IsUsingTexture())
	{
		static const char* s_PARTICLE_TYPES[] =
		{
			"Circular", "Square", "Triangle", "K-Symbol"
		};
		ImGui::Combo
		(
			"Particle Type",
			reinterpret_cast<int*>(&ps.e_ParticleType),
			s_PARTICLE_TYPES,
			IM_ARRAYSIZE(s_PARTICLE_TYPES)
		);
	}

	Rectangle draw_area =
	{
		GetScreenWidth() * 0.05f,
		GetScreenHeight() * 0.1f,
		GetScreenWidth() * 0.6f,
		GetScreenHeight() * 0.8f
	};

	ImGui::SliderFloat
	(
		"Emitter X",
		&ps.position.x,
		draw_area.x,
		draw_area.x + draw_area.width
	);
	ImGui::SliderFloat
	(
		"Emitter Y",
		&ps.position.y,
		draw_area.y,
		draw_area.y + draw_area.height
	);

	switch (ps.e_EmitterType)
	{
	case LINE:
		ImGui::SliderFloat("Line Length", &ps.line_length, 10.0f, 300.0f);
		break;

	case CIRCLE:
		ImGui::SliderFloat("Circle Radius", &ps.circle_radius, 10.0f, 200.0f);
		break;

	case RECTANGLE:
		ImGui::SliderFloat2("Rectangle Size", (float*)&ps.rect_size, 10.0f, 300.0f);
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

	ImVec4 start_col = ColorToImVec4(ps.start_color);
	if (ImGui::ColorEdit4("Start Color", reinterpret_cast<float*>(&start_col)))
	{
		ps.start_color = ImVec4ToColor(start_col);
	}

	ImVec4 end_col = ColorToImVec4(ps.end_color);
	if (ImGui::ColorEdit4("End Color", reinterpret_cast<float*>(&end_col)))
	{
		ps.end_color = ImVec4ToColor(end_col);
	}

	if (ImGui::Button("Save Preset"))
	{
	}

	if (ImGui::Button("Load Preset"))
	{
	}

	ImGui::Text("Active Particles: %d", ps.GetParticleCount());
	ImGui::End();
}