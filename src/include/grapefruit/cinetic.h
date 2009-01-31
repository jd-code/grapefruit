
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
#include <queue>

namespace grapefruit
{
    class TDObj;
    class Mvmt;
    typedef list<Mvmt*> LPMvmt;
    typedef queue<Mvmt*> QPMvmt;

    class Mvmt
    {
	protected:

     static LPMvmt lmvmt;				    //!< the global list of Mvmt who subscrided
     static LPMvmt::iterator lmvmt_end;			    //!< the end of the lmvmt list (for speed? is it safe on all platform JDJDJDJD?)
     static QPMvmt qfmvmt;				    //!< the global queue of Mvmt subscrided to their first step

	    LPMvmt::iterator lmvmt_id;			    //!< our id in the lmvmt list
	    list<LPMvmt::iterator>::iterator lmvmt_idtdid;  //!< our id in the bound-to-us-td's list of Mvmt

     static Uint32 curtime;
	    Uint32 lasttime;

	    TDObj * ptd;				    //!< the td we move

     static void hippity (void);    // JDJDJDJD mettre un nom plus digne plus tard...

	public:
	    ActionPool pa_finish;			    //!< the Action to be performed at finishing the Mvmt
	    bool delete_after_finish;			    //!< shall the MVMT be deleted after finish

	    virtual ~Mvmt (void);
	    Mvmt (TDObj &td);				    //!< Mvmt shall not exist without being bound to a td
	    void start (void);
	    void finish (void);
	    virtual void firststep (void) = 0;
	    virtual int step (void) = 0;

	friend void TDrender (void);

     static inline LPMvmt::iterator getlmvmtend (void) { return lmvmt.end(); };	// JDJDJDJD probably not safe on all STLs
    };

    class Mv_Spin : public Mvmt
    {
	    GLfloat speed1k;	//!< the spin-speed in radian per millisecond
	    Uint32 duration;	//!< the movement duration if any or zero
	    Uint32 t_end;	//!< when the movement must end or 0 for non-ending
	    Vector3 axis;	//!< the axis of the spin movement

	public:
	    Mv_Spin (TDObj &td, Vector3 &axe, GLfloat speed, Uint32 duration);
	    virtual void firststep (void);
	    virtual int step (void);
    };

#ifdef AGCINETIC_H_GLOBINST
    GRAPEFRUIT_H_SCOPE LPMvmt Mvmt::lmvmt;					//!< the global list of Mvmt who subscrided
    GRAPEFRUIT_H_SCOPE LPMvmt::iterator Mvmt::lmvmt_end = Mvmt::getlmvmtend ();	//!< the end of the lmvmt list (JDJDJDJD cf above)
    GRAPEFRUIT_H_SCOPE QPMvmt Mvmt::qfmvmt;					//!< the global queue of Mvmt's first step
    GRAPEFRUIT_H_SCOPE Uint32 Mvmt::curtime = 0;
#endif
    
}   // namespace grapefruit

#endif	// AGCINETIC_H_HEADER

