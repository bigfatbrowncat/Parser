#ifndef COMPLEXPARSER_H_
#define COMPLEXPARSER_H_

#include <complex>

using namespace std;

typedef complex<double> d_complex;

class ComplexParser : public ConstantParser<d_complex>
{
public:
	virtual d_complex parse(string str) const { return atof(str.c_str()); }
	virtual ~ComplexParser() {}
};




#endif /* COMPLEXPARSER_H_ */
