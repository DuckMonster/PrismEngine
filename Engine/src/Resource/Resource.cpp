#include "Prism/Resource/Resource.h"

//--------------------------------------------------- Manager
/**	GetResourceDirectory
*******************************************************************************/
const std::string PR_CResource::GetResourceDirectory( ) {
	return sm_AssetDirectory;
}

std::string PR_CResource::sm_AssetDirectory = "./Assets/";
std::vector<PR_CResource*> PR_CResource::sm_ResourceList;
//---------------------------------------------------

/**	Constructor
*******************************************************************************/
PR_CResource::PR_CResource( ) :
	m_ResourcePath( "" ) {
}

/**	Destructor
*******************************************************************************/
PR_CResource::~PR_CResource( ) {
}