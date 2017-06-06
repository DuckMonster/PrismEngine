#include <Prism/Context.h>
#include <GL/glew.h>
#include <chrono>
#include <iostream>

//--------------------------------------------------- Instance
PR_CContext* PR_CContext::Instance( ) {
	return sm_instance;
}

PR_CContext* PR_CContext::Create( const sf::VideoMode& videoMode, const char* title, const sf::ContextSettings& settings ) {
	sm_instance = new PR_CContext;
	sm_instance->CreateWindow( videoMode, title, settings );

	return sm_instance;
}

const std::string& PR_CContext::GetWorkingDirectory( ) {
	// Fetch working directory
	if (sm_workDir.empty( )) {
		char dirBuf[1024];
		sm_workDir = dirBuf;
	}

	return sm_workDir;
}

PR_CContext* PR_CContext::sm_instance = NULL;
std::string PR_CContext::sm_workDir = "";
//---------------------------------------------------

/**	Constructor
*******************************************************************************/
PR_CContext::PR_CContext( ) {
}

/**	Create window
*******************************************************************************/
void PR_CContext::CreateWindow( const sf::VideoMode& videoMode, const char* title, const sf::ContextSettings& settings ) {
	m_Window = new sf::Window( videoMode, title, sf::Style::Default, settings );
	m_Window->setFramerateLimit( 120 );
	glewInit( );
}

/**	Run
*******************************************************************************/
void PR_CContext::Run( PR_IProgram& program ) {
	using namespace std::chrono;
	time_point<high_resolution_clock> point_a = high_resolution_clock::now( );

	program.Init( );

	glClearColor( 0.1f, 0.1f, 0.1f, 1.f );

	while (m_Window->isOpen( )) {
		sf::Event wndEvent;

		while (m_Window->pollEvent( wndEvent )) {
			switch (wndEvent.type) {
			case sf::Event::Closed:
				m_Window->close( );
				break;

			case sf::Event::KeyPressed:
				if (wndEvent.key.code == sf::Keyboard::Escape)
					m_Window->close( );

				break;
			}
		}

		// Get delta time
		time_point<high_resolution_clock> point_b = high_resolution_clock::now( );
		double delta = (double)duration_cast<std::chrono::microseconds>(point_b - point_a).count( ) / 1e6;

		point_a = point_b;

		program.Update( delta );

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
		program.Render( delta );

		m_Window->display( );
	}
}

/**	Get Window Size
*******************************************************************************/
glm::ivec2 PR_CContext::GetWindowSize( ) {
	auto windowSize = m_Window->getSize( );
	return glm::ivec2( windowSize.x, windowSize.y );
}

/**	Get Context Size
*******************************************************************************/
glm::ivec2 PR_CContext::GetContextSize( ) {
	return GetWindowSize( );
}
