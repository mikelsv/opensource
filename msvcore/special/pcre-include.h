// add include path to pcre.h
#define PCRE2_STATIC
#define PCRE2_CODE_UNIT_WIDTH 8
#include "pcre2.h"

#ifdef _DEBUG
#pragma comment(lib,"pcre2-8d.lib")
#else
#pragma comment(lib,"pcre2-8.lib")
#endif