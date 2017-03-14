#include <Prism\Utils\Log.h>

/**	Handle Assert
*******************************************************************************/
bool PR_CLogger::HandleAssert( char* file, int line, char* msg ) {
	using namespace std;

	cout << "ASSERTION FAIL\n" << file << " ( " << line << " ):\n" << msg << "\n";
	abort( );

	return false;
}

//--------------------------------------------------- Logger

/**	Constructor
*******************************************************************************/
PR_CLogger::PR_CLogger( PR_PRISM_LOG_LEVEL level ) :
	m_Level( level ) {

	(*this) << "[PRISM " << level << "]> ";
}

/**	Destructor
*******************************************************************************/
PR_CLogger::~PR_CLogger( ) {
	(*this) << "$\n";
}