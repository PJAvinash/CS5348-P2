#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"

// added by JXP220032 for generating graph
void spin()
{
    int i = 0;
    int j = 0;
    int k = 0;
    for (i = 0; i < 50; ++i)
    {
        for (j = 0; j < 400000; ++j)
        {
            k = j % 10;
            k = k + 1;
        }
    }
}

void collectStatsThread(void* arg)
{
    int k =20;
    while (k--)
    {
        struct pstat ps;
        getpinfo(&ps);  // Retrieve process statistics
        int i;
        for (i = 0; i < NPROC; i++)
        {
            if (ps.inuse[i] == 1)
            {
                printf(1, "PID: %d, Tickets: %d, Ticks: %d\n", ps.pid[i], ps.tickets[i], ps.ticks[i]);
            }
        }
        sleep(500);  // Sleep for 500 ms
    }
}

int main()
{
    int num_children = 3;
    int num_iterations = 10;
    int i, j;
    thread_create(collectStatsThread, 0);
    for (i = 0; i < num_children; i++)
    {
        if (fork() == 0)
        {
            // Child process
            int tickets = (i+1)*10;
            settickets(tickets);
            for (j = 0; j < num_iterations; j++)
            {
                spin();
            }
            exit();
        }
    }
    // Wait for child processes to finish
    for (i = 0; i < num_children; i++)
    {
        wait();
    }
    exit();
}
