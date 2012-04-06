#ifndef TESTERTOOLS_H_
#define TESTERTOOLS_H_

#define TEST_FAILED					-1

static const char* test_name;

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
	printf("passed\n", test_name); 								\
	fflush(stdout);


#endif /* TESTERTOOLS_H_ */
