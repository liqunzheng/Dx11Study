#ifdef LEE_COMMON_EXPORTS
#define LEE_COMMON_API __declspec(dllexport)
#else
#define LEE_COMMON_API __declspec(dllimport)
#endif