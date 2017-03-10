#include "stdafx.h"
#include "Prism/Context.h"
#include "Program.h"

int main( ) {
	CProgram program;

	prism::CContext::Create( sf::VideoMode( 1024, 768 ), "PrismTest - Main", sf::ContextSettings( ) );
	prism::CContext::Instance( )->Run( program );
}