#pragma once


// Detect platform
#if defined( WIN32 ) || defined( _WINDOWS ) || defined( _WIN32 )
#	if !defined( PLATFORM_WIN )
#		define PLATFORM_WIN
#	endif
#define CORE_WINDOWS
#define CORE_WINDOWS_API
#define PLATFORM_DESKTOP
#elif defined( __ANDROID__ )
#	if !defined( PLATFORM_ANDROID )
#		define PLATFORM_ANDROID
#	endif
#define CORE_LINUX
#define CORE_LINUX_API
#define PLATFORM_MOBILE
#elif defined( __EMSCRIPTEN__ )
#	if !defined( PLATFORM_EMSCRIPTEN )
#		define PLATFORM_EMSCRIPTEN
#	endif
#define CORE_LINUX
#define CORE_LINUX_API
#define PLATFORM_WEB
#else
#	if !defined( PLATFORM_LINUX )
#		define PLATFORM_LINUX
#	endif
#define CORE_LINUX
#define CORE_LINUX_API
#define PLATFORM_DESKTOP
#endif

#if defined _WIN32 || defined __CYGWIN__
  #ifdef BUILDING_CORE_DLL
    #ifdef __GNUC__
      #define CORE_PUBLIC __attribute__ ((dllexport))
    #else
      #define CORE_PUBLIC __declspec(dllexport)
    #endif
  #else
    #ifdef __GNUC__
      #define CORE_PUBLIC __attribute__ ((dllimport))
    #else
      #define CORE_PUBLIC __declspec(dllimport)
    #endif
  #endif
  #define CORE_LOCAL
#else
  #if __GNUC__ >= 4
    #define CORE_PUBLIC __attribute__ ((visibility ("default")))
    #define CORE_LOCAL  __attribute__ ((visibility ("hidden")))
  #else
    #define CORE_PUBLIC
    #define CORE_LOCAL
  #endif
#endif


typedef unsigned char u8;
typedef signed char s8;
typedef char c8;
typedef unsigned short u16;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef unsigned long long u64;
typedef long long s64;
typedef float f32;
typedef double f64;

#define DEBUG

#if defined(DEBUG)
	#include <assert.h>
	#define DEBUG_BREAK_IF( _CONDITION_ ) assert( !(_CONDITION_) );
#else
	#define DEBUG_BREAK_IF( _CONDITION_ )
#endif




class Ref
	{
	public:

	Ref()
			: DebugName(0), ReferenceCounter(1)
		{
		}


		virtual ~Ref()
		{
          //  std::cout <<DebugName<<" destroyed" << std::endl;
		}

		void Grab() const { ++ReferenceCounter; }

		bool Drop() const
		{
		

			--ReferenceCounter;
			if (!ReferenceCounter)
			{
				delete this;
				return true;
			}

			return false;
		}

	
		s32 getReferenceCount() const
		{
			return ReferenceCounter;
		}

		const c8* getDebugName() const
		{
			return DebugName;
		}

	protected:

	
		void setDebugName(const c8* newName)
		{
			DebugName = newName;
		}

	private:

		const c8* DebugName;
		mutable s32 ReferenceCounter;
	};