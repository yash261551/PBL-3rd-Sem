#ifndef COACH_MANAGEMENT_H
#define COACH_MANAGEMENT_H

#define MAX_COACHES 5

#define AC_FIRST 0
#define AC_TWO_TIER 1
#define AC_THREE_TIER 2
#define SLEEPER 3
#define CHAIR_CAR 4

void initializeCoaches(void);

void displayCoaches(void);

void displayCoachAvailability(void);

float getCoachPrice(int coachType);

const char *getCoachName(int coachType);

int allocateSeatInCoach(int coachType);

void releaseSeatInCoach(int coachType, int seatNo);

#endif