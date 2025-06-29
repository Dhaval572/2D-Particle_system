#pragma once

inline ImVec4 ColorToImVec4(const Color &c)
{
	return ImVec4(
		static_cast<float>(c.r) / 255.0f,
		static_cast<float>(c.g) / 255.0f,
		static_cast<float>(c.b) / 255.0f,
		static_cast<float>(c.a) / 255.0f);
}

inline Color ImVec4ToColor(const ImVec4 &v)
{
	return Color{
		static_cast<unsigned char>(v.x * 255.0f),
		static_cast<unsigned char>(v.y * 255.0f),
		static_cast<unsigned char>(v.z * 255.0f),
		static_cast<unsigned char>(v.w * 255.0f)};
}
