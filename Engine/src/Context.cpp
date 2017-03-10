#include <Prism/Context.h>
#include <GL/glew.h>
#include <chrono>

using namespace prism;

//--------------------------------------------------- Instance
CContext* CContext::Instance( ) {
	return sm_instance;
}

CContext* CContext::Create( const sf::VideoMode& videoMode, const char* title, const sf::ContextSettings& settings ) {
	sm_instance = new CContext;
	sm_instance->CreateWindow( videoMode, title, settings );

	return sm_instance;
}

const std::string& CContext::GetWorkingDirectory( ) {
	// Fetch working directory
	if (sm_workDir.empty( )) {
		char dirBuf[1024];
		sm_workDir = dirBuf;
	}

	return sm_workDir;
}

CContext* CContext::sm_instance = NULL;
std::string CContext::sm_workDir = "";
//---------------------------------------------------

/**	Constructor
*******************************************************************************/
CContext::CContext( ) {
}

/**	Create window
*******************************************************************************/
void CContext::CreateWindow( const sf::VideoMode& videoMode, const char* title, const sf::ContextSettings& settings ) {
	m_Window = new sf::Window( videoMode, title, sf::Style::Default, settings );
	glewInit( );
}

/**	Run
*******************************************************************************/
void CContext::Run( IProgram& program ) {
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

		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		program.Render( delta );

		m_Window->display( );
	}
}

/**	Get Window Size
*******************************************************************************/
glm::ivec2 CContext::GetWindowSize( ) {
	auto windowSize = m_Window->getSize( );
	return glm::ivec2( windowSize.x, windowSize.y );
}

/**	Get Context Size
*******************************************************************************/
glm::ivec2 prism::CContext::GetContextSize( ) {
	return GetWindowSize( );
}
