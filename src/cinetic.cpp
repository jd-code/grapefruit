
#include "grapefruit.h"

namespace grapefruit
{
    Mvmt::Mvmt (TDObj &td)
    {	
	ptd = &td;
	lmvmt_id = lmvmt_end;
    }
    
    Mvmt::~Mvmt (void)
    {	if (lmvmt_id != lmvmt_end)
	    finish ();
    }
    
    void Mvmt::start (void)
    {	
	lmvmt_id = lmvmt.insert (lmvmt_end, this);
	lmvmt_idtdid = ptd->lmvmtid.insert ( ptd->lmvmtid.end(), lmvmt_id);
	qfmvmt.push (this);
    }
    
    void Mvmt::finish (void)
    {
	if (lmvmt_id != lmvmt_end) {
	    ptd->lmvmtid.erase (lmvmt_idtdid);
	    lmvmt.erase (lmvmt_id);
	    lmvmt_id = lmvmt_end;
	    pa_finish.doit();
	} else
	    bzouzerr << " attempt to finish a Mvmt that was not even started" << endl ;
    }
    



    Mv_Spin::Mv_Spin (TDObj &td, Vector3 &axis, GLfloat speed, Uint32 duration) : Mvmt (td)
    {	speed1k = (2.0 * M_PI * speed) / 1000.0;
	Mv_Spin::axis = axis;
	Mv_Spin::duration = duration;
    }

    void Mv_Spin::firststep (void)
    {
	if (duration != 0)
	    t_end = curtime + duration;
	else
	    t_end = 0;
    }

    int Mv_Spin::step (void)
    {
	Uint32 dt = curtime - lasttime;
	ptd->rotate (axis, speed1k * dt);
	lasttime = curtime;

	if (t_end && (curtime > t_end))	// did we reach end of movement ?
	    return -1;
	else
	    return 0;
    }
}

