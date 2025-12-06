#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here
bool schedHelper(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    std::vector<size_t>& shiftsUsed,
    size_t day,
    size_t slot
);

// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    // Add your code below

    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();

    //no workers needed per day
    if(dailyNeed == 0){
        sched.assign(numDays, std::vector<Worker_T>());
        return true;
    }

    //can't schedule more workers per day than exist
    if(dailyNeed > numWorkers){
        return false;
    }

    //initialize schedule: numDays rows with dailyNeed slots
    sched.assign(numDays, std::vector<Worker_T>(dailyNeed, INVALID_ID));

    //track how many shifts each worker has been assigned
    std::vector<size_t> shiftsUsed(numWorkers, 0);

    //start at day 0, slot 0
    return schedHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, 0, 0);
}

bool schedHelper(const AvailabilityMatrix& avail,
                 const size_t dailyNeed,
                 const size_t maxShifts,
                 DailySchedule& sched,
                 std::vector<size_t>& shiftsUsed,
                 size_t day,
                 size_t slot)
{
    size_t numDays = avail.size();
    size_t numWorkers = avail[0].size();

    //all days filled
    if(day == numDays){
        return true;
    }

    //all slots filled for current day, 
    //move on to next day and reset slot to 0
    if(slot == dailyNeed){
        return schedHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day + 1, 0);
    }

    //try assigning each worker to day
    for(size_t worker = 0; worker < numWorkers; worker++){
        bool canUse = true;

        //check if worker is available
        if(!avail[day][worker]){
            canUse = false;
        }

        //check worker doesn't exceed maxShifts
        if(canUse && shiftsUsed[worker] >= maxShifts){
            canUse = false;
        }

        //make sure not to assign same worker twice on one day
        if(canUse){
            if(std::find(sched[day].begin(), sched[day].begin() + slot, (Worker_T)worker)
               != sched[day].begin() + slot)
            {
                canUse = false;
            }
        }

        //if any check failed, skip worker
        if(canUse){
            sched[day][slot] = (Worker_T)worker;
            shiftsUsed[worker]++;

            //fill next slot on same day
            if(schedHelper(avail, dailyNeed, maxShifts, sched, shiftsUsed, day, slot + 1)){
                //full valid schedule found
                return true;
            }

            //try another worker
            shiftsUsed[worker]--;
            sched[day][slot] = INVALID_ID;
        }
    }
    //no worker could be placed at (day, slot)
    return false;
}
