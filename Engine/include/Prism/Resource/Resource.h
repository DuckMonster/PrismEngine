#pragma once
#include <string>
#include <Prism/Utils/Log.h>

/**	CResource
*******************************************************************************/
class PR_CResource {
	//--------------------------------------------------- Manager
public:
	static const std::string GetResourceDirectory( );

private:
	static std::string								sm_AssetDirectory;
	//--------------------------------------------------- Base class

public:
	PR_CResource( );
	virtual ~PR_CResource( );

protected:
	void Register( const std::string& name );
	const std::string& GetName( ) { return m_Name; }

private:
	std::string m_Name;
};