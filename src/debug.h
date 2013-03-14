#ifndef _DEBUG_PRINT_H_
#define _DEBUG_PRINT_H_

#define DEBUG
#ifdef DEBUG
#define debug_print(x,...)\
	printf(x,##__VA_ARGS__);
#define debug_print_s(x,...)\
        printf("%s() :%d "x"\n",__func__,__LINE__,##__VA_ARGS__);
#else
#define debug_print_s(x,...)
#define debug_print(x,...) 
#endif

#define PRINT_VALUE(a,b) \
    printf("%s():%d" #a " is " #b "\n",__func__,__LINE__,a)

#endif
