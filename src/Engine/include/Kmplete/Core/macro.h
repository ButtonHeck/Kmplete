#pragma once

#define KMPLETE_M_CONCAT_(x, y) x ## y
#define KMPLETE_M_CONCAT(x, y) KMPLETE_M_CONCAT_(x, y)
#define KMPLETE_M_STRINGIFY_(x) #x
#define KMPLETE_M_STRINGIFY(x) KMPLETE_M_STRINGIFY_(x)

#define KMPLETE_M_NUM_ARGS_(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, TOTAL, ...) TOTAL
#define KMPLETE_M_NUM_ARGS(...) KMPLETE_M_NUM_ARGS_(__VA_ARGS__, 12_, 11_, 10_, 9_, 8_, 7_, 6_, 5_, 4_, 3_, 2_, 1_)
#define KMPLETE_M_DISPATCH_VA(macro, ...) KMPLETE_M_CONCAT(macro, KMPLETE_M_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__) 
