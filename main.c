#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

//include header files
#include "FCFS.h" //1
#include "PP.h"   //2
#include "SRTF.h" //3
#include "RR.h"   //4

//select algorithm, 1:FCFS, 2:PP, 3:SRFT, 4:RR
int CHOOSE_ALGORITHM = 1;

int pulse_level_old = 0;
int pulse_level_new = 0;
signed int time = -1;
bool rising_clock = false;

int stored_processes[40] = {0};
int stored_processes_remaining_time[40] = {0};
int stored_processes_time_arrived[40] = {0};
int stored_priorities[40] = {0};
int FCFS_Gannt[20] = {0};
int entered_cpu[5] = {100,100,100,100,100};
int exit_cpu[5] = {0};
int loaded_in_cpu[5] = {0};
int wait_in_cpu[5] = {0};

void clock()
{
    usleep(500000);

    if((pulse_level_old==0 && pulse_level_new==0) || (pulse_level_old==0 && pulse_level_new==1))
    {
        pulse_level_old = pulse_level_new;
        pulse_level_new = 1;
    }
    else if((pulse_level_old==1 && pulse_level_new==1) || (pulse_level_old==1 && pulse_level_new==0))
    {
        pulse_level_old = pulse_level_new;
        pulse_level_new = 0;
    }

    if((pulse_level_old==0 && pulse_level_new==1))
    {
        rising_clock = true;
        time += 1;
    }
    else
    {
        rising_clock = false;
    }
}

int CPU(int procces)
{
    static int previous_process = 0;
    static int in_cpu_time_counter = 0;

    static int time_counter = 0;

    printf("        P%d          ", procces);

    if(previous_process!=procces)
    {
        if(entered_cpu[procces-1]==100)
        {
            entered_cpu[procces - 1] = time_counter;
        }

        else
        {
            wait_in_cpu[procces-1] = time_counter - exit_cpu[procces-1];
        }

        loaded_in_cpu[procces-1] += 1;

        exit_cpu[previous_process - 1] = time_counter;

    }

    if(time_counter==19)
    {
        exit_cpu[procces-1] = time_counter + 1;
    }

    FCFS_Gannt[time_counter] = procces;
    previous_process = procces;

    time_counter += 1;

    return (procces*100);
}

bool is_empty(bool change_state)
{
    static bool flag = true;

    if(change_state==true)
    {
        flag = !flag;
    }

    else
    {
        return flag;
    }
}

void queue(int process,int remaining_time,int priority,int time_arrived)
{
    static int k = 0;

    stored_processes[k] = process;
    stored_processes_remaining_time[k] = remaining_time;
    stored_processes_time_arrived[k] = time_arrived;
    stored_priorities[k] = priority;

    k += 1;

}

void print_queue()
{
	int l;
	
    for(l=0; l<40 ; l++)
    {
        if(stored_processes[l]!=0)
        {
            printf("P%d        ", stored_processes[l]);
        }
    }
}

void Gannt_results()
{
	int a,j;
    printf("\nGannt chart");
    printf("\nTime:    ");
    for(a=0; a<20; a++)
    {
        if(a<=9)
        {
            printf("%d  ",a);
        }
        else
        {
            printf("%d ",a);
        }

    }
    printf("\n");
    printf("Process: ");
    for(j=0; j<20; j++)
    {
        printf("P%d ", FCFS_Gannt[j]);

    }
    printf("\n");
}

void time_results(int arrival_time[])
{
	int c;
	float mean_turnaround_time = 0;
	float mean_wait_time = 0;

    printf("\nProcesses:       P1  P2  P3  P4  P5\n");

    printf("wait time:     ");

    for(c=0; c<5; c++)
    {
        printf("  %d ",(entered_cpu[c]-arrival_time[c]+wait_in_cpu[c]));
        mean_wait_time += entered_cpu[c]-arrival_time[c]+wait_in_cpu[c];
    }
    printf("\nturnaround time:");

    for(c=0; c<5; c++)
    {
        printf(" %d  ", (exit_cpu[c] - arrival_time[c]));
        mean_turnaround_time += exit_cpu[c] - arrival_time[c];
    }

    printf("\ntimes loaded:    ");
    for(c=0; c<5; c++)
    {
        printf("%d   ",loaded_in_cpu[c]);
    }

    printf("\n\n");

    mean_wait_time = mean_wait_time/5;
    mean_turnaround_time = mean_turnaround_time/5;

    printf("mean_wait_time: %f\n",mean_wait_time);
    printf("mean_turnaround_time: %f\n",mean_turnaround_time);


}

int main() {

    int processes[] = {1,2,3,4,5};
    int arrival_time[] = {0,1,3,9,12};
    int total_time[] = {5,6,2,4,3};
    int priority[] = {3,5,2,1,4};
    int i,j;
	
    //quantum in ms for Round Robin
    int quantum = 4;

    if(CHOOSE_ALGORITHM==1)
    {
        printf("\n\nFirst Come First Served Scheduling Algorithm\n\n");
    }
    else if(CHOOSE_ALGORITHM==2)
    {
        printf("\n\nPriority Based Preemptive Scheduling Algorithm\n\n");
    }
    else if(CHOOSE_ALGORITHM==3)
    {
        printf("\n\nShortest Remaining Time First Scheduling Algorithm\n\n");
    }
    else if(CHOOSE_ALGORITHM==4)
    {
        printf("\n\nRound Robin Scheduling Algorithm\n\n");
    }

    for(i=0; i<81; i++)
    {
        clock();
        printf("Clock_Pulse  Rising_Edge");

        if(rising_clock==true)
        {
            printf("  Time  CPU_Process  Queue[1]  Queue[2]  Queue[3]  Queue[4]");
        }

        printf("\n  %d -> %d          %d",pulse_level_old,pulse_level_new,rising_clock);

        if(rising_clock==true)
        {
            printf("        %d",time);
        }

        for(j=0; j<5; j++)
        {
            if(rising_clock==true && time==arrival_time[j])
            {
                queue(processes[j],total_time[j],priority[j],time);
            }
        }

        if(rising_clock==true)
        {
            if(CHOOSE_ALGORITHM==1)
            {
                FCFS();
            }
            else if(CHOOSE_ALGORITHM==2)
            {
                PP();
            }
            else if(CHOOSE_ALGORITHM==3)
            {
                SRTF();
            }
            else if(CHOOSE_ALGORITHM==4)
            {
                RR(quantum);;
            }

            print_queue();
        }

        printf("\n");

    }

    Gannt_results();
    time_results(arrival_time);

    return 0;
}

