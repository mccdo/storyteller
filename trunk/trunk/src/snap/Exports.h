#pragma once

#if defined(_MSC_VER)
#pragma warning( disable : 4503 )
#pragma warning( disable : 4251 )
#endif

#if defined(_MSC_VER) || defined(__CYGWIN__) || defined(__MINGW32__) || defined( __BCPLUSPLUS__)  || defined( __MWERKS__)
    #  if defined( SNAP_LIBRARY_STATIC )
    #    define SNAP_EXPORT
    #    define SNAP_LOCAL
    #  elif defined( SNAP_LIBRARY )
    #    define SNAP_EXPORT   __declspec(dllexport)
    #    define SNAP_LOCAL
    #  else
    #    define SNAP_EXPORT   __declspec(dllimport)
    #    define SNAP_LOCAL
    #  endif
#else
  #if __GNUC__ >= 4
    # if defined( SNAP_LIBRARY_STATIC )
    #    define SNAP_EXPORT
    #    define SNAP_LOCAL
    # else
    #    define SNAP_EXPORT   __attribute__ ((visibility ("default")))
    #    define SNAP_LOCAL   __attribute__ ((visibility ("hidden")))
    # endif
  #else
    #  define SNAP_EXPORT
    #  define SNAP_LOCAL
  #endif
#endif
