#pragma once 
#define foreach(it,con) for(auto it=con.begin();it!=con.end();it++)
#define times(__i__,COUNT) for(int __i__=0;__i__<COUNT;__i__++)



#define MEASURE_FUNCTION_TIME
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