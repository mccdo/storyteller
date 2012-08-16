#pragma once

#if defined(_MSC_VER)
#pragma warning( disable : 4503 )
#pragma warning( disable : 4251 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( STORYTELLER_LIBRARY_STATIC )
    #    define STORYTELLER_EXPORT
    #    define STORYTELLER_LOCAL
    #  elif defined( STORYTELLER_LIBRARY )
    #    define STORYTELLER_EXPORT   __declspec(dllexport)
    #    define STORYTELLER_LOCAL
    #  else
    #    define STORYTELLER_EXPORT   __declspec(dllimport)
    #    define STORYTELLER_LOCAL
    #  endif
#else
  #if __GNUC__ >= 4
    # if defined( STORYTELLER_LIBRARY_STATIC )
    #    define STORYTELLER_EXPORT
    #    define STORYTELLER_LOCAL
    # else
    #    define STORYTELLER_EXPORT   __attribute__ ((visibility ("default")))
    #    define STORYTELLER_LOCAL   __attribute__ ((visibility ("hidden")))
    # endif
  #else
    #  define STORYTELLER_EXPORT
    #  define STORYTELLER_LOCAL
  #endif
#endif
