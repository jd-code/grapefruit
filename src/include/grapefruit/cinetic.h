
#ifndef   GRAPEFRUIT_H_INC
#warning cinetic.h should not be included as is. grapefruit.h should be included instead
#else  // GRAPEFRUIT_H_INC
#ifdef	    GRAPEFRUIT_H_GLOBINST
#define	    AGCINETIC_H_GLOBINST
#endif //   GRAPEFRUIT_H_GLOBINST
#endif // GRAPEFRUIT_H_INC

#ifndef AGCINETIC_H_HEADER
#define AGCINETIC_H_HEADER


#include <matcalc/matcalc.h>

namespace grapefruit
{
    class TDObj;
    class Mvmt;
    typedef list<Mvmt*> LPMvmt;

    class Mvmt
    {
	public:
	protected:
     static LPMvmt lmvmt;				    //!< the list of movement who subscrided
     static LPMvmt::iterator lmvmt_end;			    //!< the end of the lmvmt list (for speed? is it safe on all platform JDJDJDJD?)
	    LPMvmt::iterator lmvmt_id;			    //!< our id in the lmvmt list
	    list<LPMvmt::iterator>::iterator lmvmt_idtdid;  //!< our id in the bound-to-us-td's list of mvmt

     static Uint32 curtime;
	    Uint32 lasttime;

	    TDObj * ptd;				    //!< the td we move

	public:
	    virtual ~Mvmt (void);
	    Mvmt (TDObj &td);				    //!< we shall not exist without being bound to a td
	    void start (void);
	    void finish (void);
	    virtual int step (void) = 0;

	friend void TDrender (void);
	friend inline LPMvmt::iterator getlmvmtend (void) { return lmvmt.end(); };
    };

    class Mv_Spin : public Mvmt
    {
	    GLfloat speed1k;	//!< the spin-speed in radian per millisecond
	    Uint32 t_end;	//!< when the movement must end or 0 for non-ending
	    Vector3 axis;	//!< the axis of the spin movement

	public:
	    Mv_Spin (TDObj &td, Vector3 &axe, GLfloat speed, Uint32 duration);
	    virtual int step (void);
    };

#ifdef AGCINETIC_H_GLOBINST
    GRAPEFRUIT_H_SCOPE LPMvmt Mvmt::lmvmt;  //!< the list of movement we subscrided to
    GRAPEFRUIT_H_SCOPE LPMvmt::iterator Mvmt::lmvmt_end = getlmvmtend ();
    GRAPEFRUIT_H_SCOPE Uint32 Mvmt::curtime = 0;
#endif
    
}   // namespace grapefruit

#endif	// AGCINETIC_H_HEADER
