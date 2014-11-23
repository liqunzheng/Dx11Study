#ifdef RENDERD3D11_EXPORTS
#define RENDERD3D11_API __declspec(dllexport)
#else
#define RENDERD3D11_API __declspec(dllimport)
#endif