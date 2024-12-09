#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "Config.h"
#include "Output.h"

unsigned long long* outPtrs[OUTPTR_MAXCOUNT];

void registerOutputPtr(unsigned long long* ptr, int id) {
  if (id < OUTPTR_MAXCOUNT) {
    outPtrs[id] = ptr;
  }
}

// Note that seed program may try to fetch an output with invalid (unintialized)
// output ID. Therefore, if the output pointer is NULL, fall back to the default
// integer value, zero.
unsigned int fetchOutput(int id) {

  if (id < OUTPTR_MAXCOUNT) {
    if (outPtrs[id]) {
      return *(outPtrs[id]);
    }
    else {
      return 0;
    }
  }
  else {
    return 0;
  }
}
