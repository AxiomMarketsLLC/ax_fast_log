#ifndef __NOTTRIVIAL__
#define __NOTTRIVIAL__
#include <stdio.h>

class NotTrivial {
    public:
     ~NotTrivial();
};
NotTrivial::~NotTrivial() {
	fprintf(stdout,"");
}

#endif
