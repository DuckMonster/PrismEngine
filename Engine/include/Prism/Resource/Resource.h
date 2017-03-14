#pragma once
#include <string>
#include <vector>
#include <Prism/Utils/Log.h>

/**	CResource
*******************************************************************************/
class PR_CResource {
	//--------------------------------------------------- Manager
public:
	static const std::string GetResourceDirectory( );
	template<typename TResourceType>
	static TResourceType* Load( const char* ResourcePath );
	template<typename TResourceType>
	static TResourceType* Create( );

private:
	static std::string								sm_AssetDirectory;
	static std::vector<PR_CResource*>				sm_ResourceList;

	//--------------------------------------------------- Base class
protected:
	PR_CResource( );
public:
	virtual ~PR_CResource( );

protected:
	virtual bool Load( const std::string& path ) = 0;
	virtual bool Create( ) = 0;
	virtual void Delete( ) = 0;

	const std::string& GetResourcePath( ) { return m_ResourcePath; }

private:
	std::string		m_ResourcePath;
};

/**	Resource loading function
*******************************************************************************/
template<typename TResourceType>
inline TResourceType* PR_CResource::Load( const char* ResourcePath ) {
	std::string filePath( GetResourceDirectory( ) );
	filePath += ResourcePath;

	// New Resource
	PR_CResource* ResourcePtr = new TResourceType;
	if (!ResourcePtr->Load( filePath )) {
		// Load failed, clean up and return NULL
		PR_CLogger( PR_LOG_LEVEL_HIGH ) << "Failed to load \"" << filePath << "\"";

		ResourcePtr->Delete( );
		delete ResourcePtr;
		return NULL;
	}

	PR_CLogger( PR_LOG_LEVEL_MEDIUM ) << "\"" << filePath << "\" loaded";
	return (TResourceType*)ResourcePtr;
}

/**	Resource creating function
*******************************************************************************/
template<typename TResourceType>
inline TResourceType * PR_CResource::Create( ) {
	// New Resource
	PR_CResource* ResourcePtr = new TResourceType;
	if (!ResourcePtr->Create()) {
		// Load failed, clean up and return NULL
		PR_CLogger( PR_LOG_LEVEL_HIGH ) << "Failed to create resource";

		ResourcePtr->Delete( );
		delete ResourcePtr;
		return NULL;
	}

	PR_CLogger( PR_LOG_LEVEL_MEDIUM ) << "Resource created";
	return (TResourceType*)ResourcePtr;
}
