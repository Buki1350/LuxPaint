#pragma once
#include <ctime>

typedef float seconds;

class Timer final {
  time_t time_1 = 0, time_2 = 0;
  seconds lastDifference = 0;
  
  public:
  void newCheckPoint() {
    time( &time_2 );
    time_1 = time_2;
    lastDifference = time_2 - time_1;
  }

  seconds getLastTimeDiff() {
    return lastDifference;
  }
};