#pragma once

inline ImVec4 ColorToImVec4(const Color &C)
{
	return ImVec4
	(
		static_cast<float>(C.r) / 255.0f,
		static_cast<float>(C.g) / 255.0f,
		static_cast<float>(C.b) / 255.0f,
		static_cast<float>(C.a) / 255.0f
	);
}

inline Color ImVec4ToColor(const ImVec4 &V)
{
	return Color
	{
		static_cast<unsigned char>(V.x * 255.0f),
		static_cast<unsigned char>(V.y * 255.0f),
		static_cast<unsigned char>(V.z * 255.0f),
		static_cast<unsigned char>(V.w * 255.0f)
	};
}
