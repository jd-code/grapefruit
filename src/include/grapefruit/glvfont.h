
#ifndef   GRAPEFRUIT_H_INC
#warning agrume.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    AGTD_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef GLVFONT_H_INC
#define GLVFONT_H_INC

#include <matcalc/matcalc.h>

namespace grapefruit
{
    Vector3 render_string (const string &s, double addstep=0);
    Vector3 compute_string_size (const string &s, double addstep=0);
    int loadglvfont (void);
}

#endif // GLVFONT_H_INC

