#include <Prism\Utils\Log.h>
using namespace prism;

/**	Handle Assert
*******************************************************************************/
bool prism::HandleAssert( char* file, int line, char* msg ) {
	using namespace std;

	cout << "ASSERTION FAIL\n" << file << " ( " << line << " ):\n" << msg << "\n";
	abort( );

	return false;
}

//--------------------------------------------------- Logger

/**	Constructor
*******************************************************************************/
prism::CLogger::CLogger( PRISM_LOG_LEVEL level ) :
	m_Level( level ) {

	(*this) << "[PRISM " << level << "]> ";
}

/**	Destructor
*******************************************************************************/
prism::CLogger::~CLogger( ) {
	(*this) << "$\n";
}