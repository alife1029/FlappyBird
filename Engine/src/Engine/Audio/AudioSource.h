#pragma once

#include <glm/vec3.hpp>

namespace Engine
{
	class AudioSource
	{
	public:
		AudioSource();
		~AudioSource();

		void Play(const unsigned int bufferToPlay);

	private:
		unsigned int m_Source = 0;
		float m_Pitch = 1.0f;
		float m_Gain = 1.0f;
		glm::vec3 m_Position{ 0.0f };
		glm::vec3 m_Velocity{ 0.0f };
		bool m_Loop = false;
		unsigned int m_Buffer = 0;
	};
}
