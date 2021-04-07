#ifndef MAIN_C_RR_H
#define MAIN_C_RR_H

int stored_processes[40];
int stored_processes_remaining_time[40];
int stored_processes_time_arrived[40];
int FCFS_Gannt[20];
int entered_cpu[5];
int exit_cpu[5];
int loaded_in_cpu[5];
signed int time;

int CPU(int procces);

#endif //MAIN_C_RR_H

void RR(int quantum)
{
    static int process_to_cpu = 0;
    static int previous_process_to_cpu = 0;
    static int previous_process_priority = 0;
    static int previous_process_time_arrived = 0;
    static int cpu_time_left = 0;
    static int in_cpu_previous_time = 0;
    static int save_best_place = 0;
    static int quantum_timer = 4;
    int arrival_time;
    int n;

    if(quantum_timer==quantum || cpu_time_left==0)
    {
        arrival_time = 100;

        for (n = 0; n < 40; n++) {
            if (stored_processes[n] != 0)
            {
                if (stored_processes_time_arrived[n] < arrival_time) {

                    arrival_time = stored_processes_time_arrived[n];
                    process_to_cpu = stored_processes[n];
                    cpu_time_left = stored_processes_remaining_time[n];
                    save_best_place = n;

                }

            }
        }
        quantum_timer = 0;
    }

    if(previous_process_to_cpu!=process_to_cpu && in_cpu_previous_time!=0)
    {
        queue(previous_process_to_cpu,in_cpu_previous_time,previous_process_priority,time);
    }

    if(previous_process_to_cpu!=process_to_cpu)
    {
        cpu_time_left = stored_processes_remaining_time[save_best_place];
    }

        if(cpu_time_left!=0)
        {
            CPU(process_to_cpu);
            cpu_time_left -= 1;
        }

        else
        {
            printf("    finished!");
        }

    quantum_timer += 1;

    previous_process_to_cpu = process_to_cpu;
    previous_process_priority = stored_priorities[save_best_place];
    previous_process_time_arrived = stored_processes_time_arrived[save_best_place];

    in_cpu_previous_time = cpu_time_left;

    stored_processes[save_best_place] = 0;
    stored_processes_time_arrived[save_best_place] = 0;
    stored_processes_remaining_time[save_best_place] = 0;
    stored_priorities[save_best_place] = 0;

}

