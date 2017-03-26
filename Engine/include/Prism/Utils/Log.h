#pragma once
#include <iostream>

enum PR_PRISM_LOG_LEVEL {
	PR_LOG_LEVEL_TRIVIAL,
	PR_LOG_LEVEL_LOW,
	PR_LOG_LEVEL_MEDIUM,
	PR_LOG_LEVEL_HIGH,
	PR_LOG_LEVEL_CRITICAL
};

/**	CLogger
*******************************************************************************/
class PR_CLogger {
public:
	static bool HandleAssert( char* file, int line, char* msg );
	static PR_PRISM_LOG_LEVEL	sm_LogLevel;

public:
	PR_CLogger( PR_PRISM_LOG_LEVEL level );
	~PR_CLogger( );

	template<typename TValueType>
	PR_CLogger& operator <<( const TValueType& value ) { Write( value ); return *this; }

private:
	template<typename TValueType>
	void Write( const TValueType& value ) {
		if (m_Level < sm_LogLevel)
			return;

		std::cout << value;
	}

	const PR_PRISM_LOG_LEVEL		m_Level;
};

#ifdef PRISM_DEBUG
#define PR_ASSERT_MSG(expr, msg)		if (!(expr)) PR_CLogger::HandleAssert(__FILE__, __LINE__, msg)
#define PR_ASSERT(expr)					PR_ASSERT_MSG(expr, "No message.");
#else
#define PR_ASSERT_MSG(expr, msg)		((void)0);
#define PR_ASSERT(expr)					((void)0);
#endif