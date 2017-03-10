#include "Prism/Asset/Asset.h"
using namespace prism;

//--------------------------------------------------- Manager
/**	GetAssetDirectory
*******************************************************************************/
const std::string CAsset::GetAssetDirectory( ) {
	return sm_AssetDirectory;
}

/**	Hash String
*******************************************************************************/
unsigned int prism::CAsset::HashString( const char * str ) {
	unsigned int hash = HASH_SEED;
	while (*str) {
		hash *= 101 + *str;
		str++;
	}

	return hash;
}

std::string CAsset::sm_AssetDirectory = "./Assets/";
std::map<unsigned int, CAsset*> CAsset::sm_AssetMap;
//---------------------------------------------------

/**	Constructor
*******************************************************************************/
prism::CAsset::CAsset( ) :
	m_AssetPath( "" ) {
}

/**	Destructor
*******************************************************************************/
prism::CAsset::~CAsset( ) {
	Release( );
}