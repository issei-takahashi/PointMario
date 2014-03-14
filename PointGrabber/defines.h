#pragma once 
#define foreach(it,con) for(auto it=con.begin();it!=con.end();it++)
#define rforeach(it,con) for(auto it=con.rbegin();it!=con.rend();it++)
#define times(__i__,BEGIN,END) for(int __i__=BEGIN;__i__<END;__i__++)

/* if version of C++ is greater than C++11 */
#ifdef CPP_11
#else
#define final
#endif


//#define MEASURE_FUNCTION_TIME
//#define SHOW_IMAGE !((VTK_MAJOR_VERSION == 5)&&(VTK_MINOR_VERSION <= 4))
#define SHOW_IMAGE 1

#define SHOW_FPS 1
#if SHOW_FPS
#define FPS_CALC(_WHAT_) \
	do \
{ \
	static unsigned count = 0;\
	static double last = pcl::getTime ();\
	double now = pcl::getTime (); \
	++count; \
	if (now - last >= 1.0) \
	{ \
	count = 0; \
	last = now; \
	} \
}while(false)
#else
#define FPS_CALC(_WHAT_) \
	do \
{ \
}while(false)
#endif