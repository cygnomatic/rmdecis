//
// Created by catslashbin on 23-2-1.
//

#ifndef CYGNOIDES_DECISION_TRIG_FITTING_H
#define CYGNOIDES_DECISION_TRIG_FITTING_H

#include <kiss_fftr.h>

/**
 * A C++ wrapper for kissfft. TrigFitting (real input) only.
 */

class TrigFitting
{
    void *buffer;
    kiss_fftr_cfg cfg;

public:
    TrigFitting();

    ~TrigFitting();
};


#endif //CYGNOIDES_DECISION_TRIG_FITTING_H
