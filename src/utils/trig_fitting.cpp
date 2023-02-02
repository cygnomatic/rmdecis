//
// Created by catslashbin on 23-2-1.
//

#include "trig_fitting.h"

#include <cstdlib>
#include <vector>

#define BUFFER_MEM_SIZE (10 * 1024 * 1024)

TrigFitting::TrigFitting()
{
    buffer = malloc(BUFFER_MEM_SIZE); // 10 Mb
}

TrigFitting::~TrigFitting() {
    free(buffer);
}