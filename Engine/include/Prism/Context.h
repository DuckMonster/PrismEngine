#pragma once
#include <SFML/Window.hpp>
#include <glm/vec2.hpp>
#include <Prism/IProgram.h>

class PR_CContext {
public:
	static PR_CContext* Instance( );
	static PR_CContext* Create( const sf::VideoMode& videoMode, const char* title, const sf::ContextSettings& settings );
	static const std::string& GetWorkingDirectory( );

private:
	static PR_CContext*	sm_instance;
	static std::string	sm_workDir;

protected:
	PR_CContext( );

public:
	void Run( PR_IProgram& program );
	glm::ivec2 GetWindowSize( );
	glm::ivec2 GetContextSize( );

private:
	void CreateWindow( const sf::VideoMode& videoMode, const char* title, const sf::ContextSettings& settings );

private:
	sf::Window*		m_Window;
};
