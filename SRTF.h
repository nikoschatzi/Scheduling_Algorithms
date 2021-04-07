#ifndef MAIN_C_SRTF_H
#define MAIN_C_SRTF_H

int stored_processes[40];
int stored_processes_remaining_time[40];
int stored_processes_time_arrived[40];
int stored_priorities[40];
int FCFS_Gannt[20];
int entered_cpu[5];
int exit_cpu[5];
int loaded_in_cpu[5];
signed int time;

int CPU(int procces);
bool is_empty(bool change_state);
void queue(int process,int remaining_time,int priority,int time_arrived);

#endif //MAIN_C_SRTF_H

void SRTF()
{
    static int process_to_cpu = 0;
    static int previous_process_to_cpu = 0;
    static int previous_process_priority = 0;
    static int previous_process_time_arrived = 0;
    static int cpu_time_left = 100;
    static int save_best_place = 0;
    static int in_cpu_remaining_time = 100;
    int best_remaining_time;
    int n;

    best_remaining_time = 100;

    if(in_cpu_remaining_time==0 && time<20)
    {
        in_cpu_remaining_time = 100;
    }

    for(n=0; n<40; n++)
    {
        if(stored_processes[n]!=0)
        {
            if(stored_processes_remaining_time[n]<best_remaining_time && stored_processes_remaining_time[n]<in_cpu_remaining_time)
            {
                best_remaining_time = stored_processes_remaining_time[n];
                process_to_cpu = stored_processes[n];
                save_best_place = n;
            }

        }
    }

    if(previous_process_to_cpu!=process_to_cpu && cpu_time_left!=0)
    {
        queue(previous_process_to_cpu,cpu_time_left,previous_process_priority,previous_process_time_arrived);
    }

    if(previous_process_to_cpu!=process_to_cpu)
    {
        in_cpu_remaining_time = stored_processes_remaining_time[save_best_place];
    }

    if(in_cpu_remaining_time!=0)
    {
        CPU(process_to_cpu);
        in_cpu_remaining_time -= 1;
    }

    else
    {
        printf("    finished!");;
    }

    previous_process_to_cpu = process_to_cpu;
    previous_process_priority = stored_priorities[save_best_place];
    previous_process_time_arrived = stored_processes_time_arrived[save_best_place];

    stored_processes[save_best_place] = 0;
    stored_processes_time_arrived[save_best_place] = 0;
    stored_processes_remaining_time[save_best_place] = 0;
    stored_priorities[save_best_place] = 0;

    cpu_time_left = in_cpu_remaining_time;
}
