#pragma once
#include <string>
#include <map>
#include <Prism/Utils/Log.h>

namespace prism {
	const unsigned int HASH_SEED = 0xf75ac213;

	class CAsset {
		//--------------------------------------------------- Manager
	public:
		static const std::string GetAssetDirectory( );
		template<typename TAssetType>
		static TAssetType* Load( const char* assetPath );

	private:
		static unsigned int HashString( const char* str );

		static std::string						sm_AssetDirectory;
		static std::map<unsigned int, CAsset*>	sm_AssetMap;

		//--------------------------------------------------- Base class
	protected:
		CAsset( );
	public:
		virtual ~CAsset( );
		virtual void Release( ) {}

	protected:
		virtual bool Load( const std::string& path ) { m_AssetPath = path; return true; }
		const std::string& GetAssetPath( ) { return m_AssetPath; }

	private:
		std::string		m_AssetPath;
	};

	/**	Asset loading function
	*******************************************************************************/
	template<typename TAssetType>
	inline TAssetType* CAsset::Load( const char* assetPath ) {
		std::string filePath( GetAssetDirectory( ) );
		filePath += assetPath;

		// Hash the file
		unsigned int fileHash = HashString( filePath.c_str( ) );

		// Asset already loaded
		if (sm_AssetMap.find( fileHash ) != sm_AssetMap.end( ))
			return (TAssetType*)sm_AssetMap[fileHash];

		// New asset
		CAsset* assetPtr = new TAssetType;
		if (!assetPtr->Load( filePath )) {
			// Load failed, clean up and return NULL
			CLogger( CLogger::LOG_LEVEL_HIGH ) << "Failed to load \"" << filePath << "\"";

			delete assetPtr;
			return NULL;
		}

		CLogger( CLogger::LOG_LEVEL_MEDIUM ) << "\"" << filePath << "\" loaded [ " << fileHash << " ]";
		sm_AssetMap[fileHash] = assetPtr;
		return (TAssetType*)assetPtr;
	}
}