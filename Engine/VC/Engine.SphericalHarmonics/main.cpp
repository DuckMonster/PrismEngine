#include "Prism/Context.h"
#include "Program.h"

int main( ) {
	CProgram program;

	sf::ContextSettings settings;

	settings.depthBits = 24;
	settings.stencilBits = 8;
	//settings.antialiasingLevel = 8;

	PR_CContext::Create( sf::VideoMode( 1024, 768 ), "PrismTest - Spherical Harmonics", settings );
	PR_CContext::Instance( )->Run( program );
}