#pragma once

#include <queue>
#include <bitset>

namespace Engine
{
	class Keyboard
	{
		friend class Window;
	public:
		class Event
		{
		public:
			enum class Type { Press, Release, Invalid };
		
		private:
			Type m_Type;
			unsigned char m_Code;
		
		public:
			Event() noexcept;
			Event(Type type, unsigned char code) noexcept;
			bool IsPress() const noexcept;
			bool IsRelease() const noexcept;
			bool IsValid() const noexcept;
			unsigned char GetCode() const noexcept;
		};

	public:
		Keyboard() = default;
		Keyboard(const Keyboard&) = delete;
		Keyboard& operator=(const Keyboard&) = delete;

		bool IsKeyPressed(unsigned char keycode) const noexcept;
		bool IsKeyJustPressed(unsigned char keycode) const noexcept;
		char ReadChar() noexcept;

		void EnableAutorepeat() noexcept;
		void DisableAutorepeat() noexcept;
		bool IsAutorepeatEnabled() const noexcept;

	private:
		void OnKeyPress(unsigned char keycode) noexcept;
		void OnKeyRelease(unsigned char keycode) noexcept;
		void OnChar(char character) noexcept;
		void ClearState() noexcept;

		Event ReadKey() noexcept;
		bool IsEmpty() const noexcept;
		void FlushKey() noexcept;
		bool IsCharEmpty() const noexcept;
		void FlushChar() noexcept;
		void Flush() noexcept;

	private:
		template<typename T>
		inline static void TrimBuffer(std::queue<T>& buffer) noexcept;

	public:
		static constexpr unsigned int s_KeyCount = 256u;

	private:
		static constexpr unsigned int s_BufferSize = 16u;
		bool m_AutorepeatEnabled = false;
		std::bitset<s_KeyCount> m_KeyStates;
		std::bitset<s_KeyCount> m_LastStates;
		std::queue<Event> m_KeyBuffer;
		std::queue<char> m_CharBuffer;
	};

	template<typename T>
	inline void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept
	{
		while (buffer.size() > s_BufferSize) buffer.pop();
	}
}
