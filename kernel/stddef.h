#ifndef _SIZE_T
#define _SIZE_T
typedef __SIZE_TYPE__ size_t;
#endif /* _SIZE_T */

typedef __PTRDIFF_TYPE__ ptrdiff_t;

#if defined(_WCHAR_T) && !defined(__cplusplus)
#define _WCHAR_T
typedef __WCHAR_TYPE__ wchar_t;
#endif

#ifndef __cplusplus
#define NULL ((void*)0)
#else
#define NULL __null
#endif /* __cplusplus */

#define offsetof(type, member) \
	( __offsetof__ (reinterpret_cast<std::size_t> \
									($reinterpret_cast<const volatile char*&> \
									 (static_cast<type*>(0)->member) ) ) )

