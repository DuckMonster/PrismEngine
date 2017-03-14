#include "Prism/Asset/Asset.h"

//--------------------------------------------------- Manager
/**	GetAssetDirectory
*******************************************************************************/
const std::string PR_CAsset::GetAssetDirectory( ) {
	return sm_AssetDirectory;
}

/**	Hash String
*******************************************************************************/
unsigned int PR_CAsset::HashString( const char * str ) {
	unsigned int hash = sm_HashSeed;
	while (*str) {
		hash *= 101 + *str;
		str++;
	}

	return hash;
}

const unsigned int PR_CAsset::sm_HashSeed = 0xf75ac213;
std::string PR_CAsset::sm_AssetDirectory = "./Assets/";
std::map<unsigned int, PR_CAsset*> PR_CAsset::sm_AssetMap;
//---------------------------------------------------

/**	Constructor
*******************************************************************************/
PR_CAsset::PR_CAsset( ) :
	m_AssetPath( "" ) {
}

/**	Destructor
*******************************************************************************/
PR_CAsset::~PR_CAsset( ) {
	Release( );
}