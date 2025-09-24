#pragma once
#define GET_MACRO(_1,_2,NAME,...) NAME
#define forit(...) GET_MACRO(__VA_ARGS__, forit2, forit1)(__VA_ARGS__)
#define forit2(iteratorName, range) for(int iteratorName = 0; iteratorName < range; iteratorName++)
#define forit1(range) for(int _ = 0; _ < range; _++)