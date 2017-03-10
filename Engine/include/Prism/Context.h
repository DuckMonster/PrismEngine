#pragma once
#include <SFML/Window.hpp>
#include <glm/vec2.hpp>
#include <Prism/IProgram.h>

namespace prism {
	class CContext {
	public:
		static CContext* Instance( );
		static CContext* Create( const sf::VideoMode& videoMode, const char* title, const sf::ContextSettings& settings );
		static const std::string& GetWorkingDirectory( );

	private:
		static CContext*	sm_instance;
		static std::string	sm_workDir;

	protected:
		CContext( );

	public:
		void Run( IProgram& program );
		glm::ivec2 GetWindowSize( );
		glm::ivec2 GetContextSize( );

	private:
		void CreateWindow( const sf::VideoMode& videoMode, const char* title, const sf::ContextSettings& settings );

	private:
		sf::Window*		m_Window;
	};
}