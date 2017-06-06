#include "Prism/Resource/Resource.h"

//--------------------------------------------------- Manager
/**	GetResourceDirectory
*******************************************************************************/
const std::string PR_CResource::GetResourceDirectory( ) {
	return sm_AssetDirectory;
}

/**	SetResourceDirectory
*******************************************************************************/
void PR_CResource::SetResourceDirectory( const std::string & directory ) {
	sm_AssetDirectory = directory;
}

std::string PR_CResource::sm_AssetDirectory = "./Assets/";
//---------------------------------------------------

/**	Constructor
*******************************************************************************/
PR_CResource::PR_CResource( ) :
	m_Name( "" ) {
}

/**	Destructor
*******************************************************************************/
PR_CResource::~PR_CResource( ) {
}

/**	Register Asset
*******************************************************************************/
void PR_CResource::Register( const std::string & name ) {
	m_Name = name;
	PR_CLogger( PR_LOG_LEVEL_MEDIUM ) << "Asset \"" << name << "\" loaded";
}
