#ifndef TESTERTOOLS_H_
#define TESTERTOOLS_H_

#include <stdio.h>
#include <time.h>

#define TEST_FAILED					-1

static const char* test_name;
static clock_t c1, c2;

#define TEST_ASSERT(b, t)										\
	if (!(b))													\
	{															\
		printf("\n\nTest \"%s\" failed: %s\n", test_name, t);	\
		throw TEST_FAILED;										\
	}

#define TEST_FUNCTION(name)	void __TestFunction_##name ()
#define TEST_FUNCTION_RUN(name)									\
	test_name = #name;											\
	printf("Test \"%s\"... ", test_name);						\
	fflush(stdout); 											\
	__TestFunction_##name();									\
	printf("passed\n");			 								\
	fflush(stdout);

#define TEST_FUNCTION_RUN_TIMING(name)							\
	test_name = #name;											\
	printf("Timing test \"%s\"... ", test_name);				\
	fflush(stdout); 											\
	c1 = clock();												\
	__TestFunction_##name();									\
	c2 = clock();												\
	printf("passed in %d msec\n", c2 - c1);						\
	fflush(stdout);


#endif /* TESTERTOOLS_H_ */
