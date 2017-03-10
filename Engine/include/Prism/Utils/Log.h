#pragma once
#include <iostream>

namespace prism {
	bool HandleAssert( char* file, int line, char* msg );

	class CLogger {
	public:
		enum PRISM_LOG_LEVEL {
			LOG_LEVEL_TRIVIAL,
			LOG_LEVEL_LOW,
			LOG_LEVEL_MEDIUM,
			LOG_LEVEL_HIGH,
			LOG_LEVEL_CRITICAL
		};

	public:
		CLogger( PRISM_LOG_LEVEL level );
		~CLogger( );

		template<typename TValueType>
		CLogger& operator <<( const TValueType& value ) { Write( value ); return *this; }

	private:
		template<typename TValueType>
		void Write( const TValueType& value ) { std::cout << value; }

		const PRISM_LOG_LEVEL		m_Level;
	};
}

#ifdef PRISM_DEBUG
#define PRISM_ASSERT_MSG(expr, msg)		if (!(expr)) prism::HandleAssert(__FILE__, __LINE__, msg)
#define PRISM_ASSERT(expr)				PRISM_ASSERT_MSG("No message.");
#else
#define PRISM_ASSERT_MSG(expr, msg)		((void)0);
#define PRISM_ASSERT(expr)				((void)0);
#endif