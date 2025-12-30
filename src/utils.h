#ifndef UTILS
#define UTILS

#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <complex>
#include <cstring>
#include <complex>
#include <cassert>
#include <cstdio>


#ifdef DEBUG
	#define dbg_requires(...) assert(__VA_ARGS__)
	#define dbg_ensures(...) assert(__VA_ARGS__)
	#define dbg_assert(...) assert(__VA_ARGS__)
	#define dbg_print(fmt, ...) fprintf(stderr, "[Debug] %s:%d" fmt "\n", __FILE__, __LINE__,  ##__VA_ARGS__)
#else
	#define dbg_requires(...)
	#define dbg_ensures(...)
	#define dbg_assert(...)
	#define dbg_printf(...)
#endif

#endif
