#ifndef ECOSYSTEM_EXPORT_H
#define ECOSYSTEM_EXPORT_H

#ifdef WIN32
    #define DLLEXPORT __declspec(dllexport)
    #define DLLIMPORT __declspec(dllimport)
#else
    #define DLLEXPORT
    #define DLLIMPORT
#endif

#endif /* ECOSYSTEM_EXPORT_H */
