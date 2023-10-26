#include "types.h"
#include "stat.h"
#include "user.h"
#include "pstat.h"


/*
** this file added by JXP220032 for generating graph, 
** we use a python script to extract the data from text 
*/


void spin()
{
    int i = 0;
    int j = 0;
    int k = 0;
    for (i = 0; i < 500; ++i)
    {
        for (j = 0; j < 400000; ++j)
        {
            k = j % 10;
            k = k + 1;
        }
    }
}


/*
** creates a child process with 60 tickets to collec the stats and returns the pid,
*/ 
int collectStatsProcess()
{
    int child_pid = fork();
    if (child_pid == 0)
    {
        //setting 50% tickets so that this gets a chance to run.
        settickets(60);
        int k = 20;
        while (k--)
        {
            struct pstat ps;
            getpinfo(&ps); // Retrieve process statistics
            int i;
            for (i = 0; i < NPROC; i++)
            {
                if (ps.inuse[i] == 1)
                {
                    printf(1, "PID: %d, Tickets: %d, Ticks: %d\n", ps.pid[i], ps.tickets[i], ps.ticks[i]);
                }
            }
            printf(1, "#############################################\n");
            sleep(200); // Sleep for 200 ms
        }
        exit(); // Exit the child process
    }
    return child_pid;
}

int main()
{
    int num_children = 3;
    int num_iterations = 10;
    int i, j;
    int pid = collectStatsProcess();
    for (i = 0; i < num_children; i++)
    {
        int ch_pid = fork();
        if (ch_pid == 0)
        {
            // Child process
            int tickets = (i + 1) * 10;
            settickets(tickets);
            for (j = 0; j < num_iterations; j++)
            {
                spin();
            }
            exit();
        }
    }
    // Wait for child processes to finish
    sleep(30000);
    for (i = 0; i < num_children; i++)
    {
        wait();
    }
    kill(pid);
    wait();
    exit();
    return 0;
}
/* end of file*/
