#ifdef GEOMETRY_EXPORTS
#define GEOMETRY_API __declspec(dllexport)
#else
#define GEOMETRY_API __declspec(dllimport)
#endif