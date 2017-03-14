#include <Prism/Resource/MeshResource.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

/**	Constructor
*******************************************************************************/
PR_CMeshResource::PR_CMeshResource( ) :
	PR_CResource( ),
	m_objectHandle( -1 ),
	m_positionHandle( -1 ), m_normalHandle( -1 ), m_uvHandle( -1 ),
	m_elementsHandle( -1 ),
	m_vertexCount( 0 ) {

}

/**	Destructor
*******************************************************************************/
PR_CMeshResource::~PR_CMeshResource( ) {

}

/**	Load
*******************************************************************************/
bool PR_CMeshResource::Load( const std::string& path ) {
	// Gen buffers
	glGenVertexArrays( 1, &m_objectHandle );
	glGenBuffers( 1, &m_positionHandle );
	glGenBuffers( 1, &m_normalHandle );
	glGenBuffers( 1, &m_uvHandle );
	glGenBuffers( 1, &m_elementsHandle );

	// Load mesh data
	if (!LoadAssimp( path.c_str( ) ))
		return false;

	// Bind vertex objects
	glBindVertexArray( m_objectHandle );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elementsHandle );

	glBindBuffer( GL_ARRAY_BUFFER, m_positionHandle );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_normalHandle );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_uvHandle );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	glBindVertexArray( 0 );

	return true;
}

/**	Create
*******************************************************************************/
bool PR_CMeshResource::Create( ) {
	// Gen buffers
	glGenVertexArrays( 1, &m_objectHandle );
	glGenBuffers( 1, &m_positionHandle );
	glGenBuffers( 1, &m_normalHandle );
	glGenBuffers( 1, &m_uvHandle );
	glGenBuffers( 1, &m_elementsHandle );

	// Bind vertex objects
	glBindVertexArray( m_objectHandle );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elementsHandle );

	glBindBuffer( GL_ARRAY_BUFFER, m_positionHandle );
	glEnableVertexAttribArray( 0 );
	glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_normalHandle );
	glEnableVertexAttribArray( 1 );
	glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );

	glBindBuffer( GL_ARRAY_BUFFER, m_uvHandle );
	glEnableVertexAttribArray( 2 );
	glVertexAttribPointer( 2, 2, GL_FLOAT, GL_FALSE, 0, 0 );

	glBindVertexArray( 0 );

	return true;
}

/**	Release
*******************************************************************************/
void PR_CMeshResource::Delete( ) {
	if (m_objectHandle == -1)
		return;

	glDeleteVertexArrays( 1, &m_objectHandle );
	glDeleteBuffers( 1, &m_positionHandle );
	glDeleteBuffers( 1, &m_normalHandle );
	glDeleteBuffers( 1, &m_uvHandle );
	glDeleteBuffers( 1, &m_elementsHandle );
}

/**	Render
*******************************************************************************/
void PR_CMeshResource::Render( ) {
	glBindVertexArray( m_objectHandle );
	glDrawElements( GL_TRIANGLES, m_vertexCount, GL_UNSIGNED_INT, 0 );
	glBindVertexArray( 0 );
}

/**	Load Assimp
*******************************************************************************/
bool PR_CMeshResource::LoadAssimp( const char* fileName ) {
	using namespace std;
	using namespace glm;

	// Create importer
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile( fileName, aiProcess_Triangulate );
	if (!scene) {
		cout << "Failed to load " << fileName << ": " << importer.GetErrorString( ) << "\n";
		return false;
	}

	std::vector<aiVector3D> vertices;
	std::vector<aiVector3D> normals;
	std::vector<aiVector2D> uvs;
	std::vector<size_t> indicies;

	size_t elem_offset = 0;

	//--------------------------------------------------- Load all meshes
	for (size_t m = 0; m < scene->mNumMeshes; m++) {
		aiMesh* fbxMesh = scene->mMeshes[m];

		for (size_t i = 0; i < fbxMesh->mNumVertices; i++) {
			// Position
			vertices.push_back( fbxMesh->mVertices[i] );

			// Normal
			if (fbxMesh->HasNormals( ))
				normals.push_back( fbxMesh->mNormals[i] );

			// UV
			if (fbxMesh->HasTextureCoords( 0 )) {
				aiVector3D v = fbxMesh->mTextureCoords[0][i];
				uvs.push_back( aiVector2D( v.x, v.y ) );
			}
		}

		// Indicies
		for (size_t f = 0; f < fbxMesh->mNumFaces; f++) {
			aiFace face = fbxMesh->mFaces[f];

			for (size_t i = 0; i < face.mNumIndices; i++) {
				indicies.push_back( face.mIndices[i] + elem_offset );
			}
		}

		elem_offset += fbxMesh->mNumVertices;
	}

	glBindVertexArray( 0 );

	//--------------------------------------------------- Upload all data
	glBindBuffer( GL_ARRAY_BUFFER, m_positionHandle );
	glBufferData( GL_ARRAY_BUFFER, sizeof( aiVector3D ) * vertices.size( ), &(vertices[0].x), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, m_normalHandle );
	glBufferData( GL_ARRAY_BUFFER, sizeof( aiVector3D ) * normals.size( ), &(normals[0].x), GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, m_uvHandle );
	glBufferData( GL_ARRAY_BUFFER, sizeof( aiVector2D ) * uvs.size( ), &(uvs[0].x), GL_STATIC_DRAW );

	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, m_elementsHandle );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, sizeof( size_t ) * vertices.size( ), &(indicies[0]), GL_STATIC_DRAW );

	m_vertexCount = indicies.size( );

	return true;
}
