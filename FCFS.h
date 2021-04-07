#include <stdio.h>
#include <stdbool.h>

#ifndef MAIN_C_FCFS_H
#define MAIN_C_FCFS_H

int stored_processes[40];
int stored_processes_remaining_time[40];
int stored_processes_time_arrived[40];
int FCFS_Gannt[20];
int entered_cpu[5];
int exit_cpu[5];
int loaded_in_cpu[5];

int CPU(int procces);
bool is_empty(bool change_state);

#endif //MAIN_C_FCFS_H

void FCFS()
{
    static int process_to_cpu = 0;
    static int cpu_time_left = 0;
    static int save_best_place = 0;
    int cpu_empty;
    int arrival_time;
    int n;

    cpu_empty = is_empty(false);

    if(cpu_empty==true)
    {
        arrival_time = 100;

        for(n=0; n<40; n++)
        {
            if(stored_processes[n]!=0)
            {
                if(stored_processes_time_arrived[n]<arrival_time)
                {
                    arrival_time = stored_processes_time_arrived[n];
                    process_to_cpu = stored_processes[n];
                    cpu_time_left = stored_processes_remaining_time[n];
                    save_best_place = n;
                }

            }
        }

        stored_processes[save_best_place] = 0;
        stored_processes_time_arrived[save_best_place] = 0;
        stored_processes_remaining_time[save_best_place] = 0;

        if(cpu_time_left!=0)
        {
            CPU(process_to_cpu);
        }

        else
        {
            printf("    finished!");
        }

        cpu_time_left -= 1;

        if(cpu_time_left!=0)
        {
            is_empty(true);
        }

    }

    else
    {
        CPU(process_to_cpu);
        cpu_time_left -= 1;

        if(cpu_time_left==0)
        {
            is_empty(true);
        }
    }

}

