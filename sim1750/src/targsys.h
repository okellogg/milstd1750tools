/* targsys.h  Target system used: change as needed.  */

/*
#define F9450
 */
/*
#define PACE
 */
/*
#define GVSC
 */

/*
#define MA31750
*/

#define MAS281

/******************* end of processor brand defs *******************/

/* Processor clock cycle in nanoseconds: */

/* The ERA MAS281 board has a 10MHz clock */
#define uP_CYCLE_IN_NS 100

/* Watchdog (GO Timer) period as measured in 10 microsecond units: */
#define GOTIMER_PERIOD_IN_10uSEC  25

