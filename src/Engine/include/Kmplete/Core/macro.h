#pragma once

#define KMP_M_CONCAT_(x, y) x ## y
#define KMP_M_CONCAT(x, y) KMP_M_CONCAT_(x, y)
#define KMP_M_STRINGIFY_(x) #x
#define KMP_M_STRINGIFY(x) KMP_M_STRINGIFY_(x)

#define KMP_M_NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, TOTAL, ...) TOTAL
#define KMP_M_NUM_ARGS(...) KMP_M_NUM_ARGS_(__VA_ARGS__, 12_, 11_, 10_, 9_, 8_, 7_, 6_, 5_, 4_, 3_, 2_, 1_)
#define KMP_M_DISPATCH_VA(macro, ...) KMP_M_CONCAT(macro, KMP_M_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__) 
