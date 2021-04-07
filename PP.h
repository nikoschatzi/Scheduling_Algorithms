#ifndef MAIN_C_PP_H
#define MAIN_C_PP_H

//���������� ��� ����������� ��� �� ������ main.c ��� �����������
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

#endif //MAIN_C_PP_H

//��������� ���������� ���������� Priority based Preemptive
void PP()
{
    //��������� ��� ������������ � ��������� ��� �� ������ ���� CPU
    static int process_to_cpu = 0;
    //���������� ��� ������������� �� �������� ��� ������������ ���������� ��� ���� ������ ���� CPU
    static int previous_process_to_cpu = 0;
    static int previous_process_priority = 0;
    static int previous_process_time_arrived = 0;
    //��������� ��� ������������ � ������������� ������
    static int cpu_time_left = 0;
    //��������� ����� int ��� ����������� � ���� ���� ���� (���� 40 ������) ��� ��������� � ���������� ��� �� ���� ���� CPU
    static int save_best_place = 0;
    //��������� ��� ��� ��������������
    static int in_cpu_priority = 0;
    int best_priority;
    //��������� ��� ��� for ���������
    int n;

    //������������ ��������� �������������� ��� 0
    best_priority = 0;

    //��������� ����������� �� ��� ���������� �������������
    for(n=0; n<40; n++)
    {
        if(stored_processes[n]!=0)
        {
            if(stored_priorities[n]>best_priority && stored_priorities[n]>in_cpu_priority)
            {
                best_priority = stored_priorities[n];
                process_to_cpu = stored_processes[n];
                save_best_place = n;
            }

            //�� ��� ������ ���������� �� ���������� ������������� ��� ����� ��� ����� ���� CPU ���������� �� �������� ������������� ���� ��� ������������ ��� CPU
            if(best_priority==0)
            {
                best_priority = in_cpu_priority;
            }

        }
    }

    //�� ������ ����������� ����������� ��� � ����������� ��� ���� ��������� ����������� � ����������� ���� ����
    if(previous_process_to_cpu!=process_to_cpu && cpu_time_left!=0)
    {
        queue(previous_process_to_cpu,cpu_time_left,previous_process_priority,previous_process_time_arrived);
    }

    //�� ������ ����������� ����������� ��������� ��� ������������ �����
    if(previous_process_to_cpu!=process_to_cpu)
    {
        cpu_time_left = stored_processes_remaining_time[save_best_place];

        //� ������������� ���� CPU ����� � �������� ������������� ��� �������
        in_cpu_priority = best_priority;
    }

    //�� ��� ���� ��������� � ������ ��������� ��� ��������� ��� ��������� ��� ����� ���� 1
   if(cpu_time_left!=0)
   {
       CPU(process_to_cpu);
       cpu_time_left -= 1;
   }

   //�� �� ����� ��������� finished
   else if(time==20)
   {
       printf("    finished!");;
   }

   //�� ���� ��������� ��� ���������� ����������� ��� ������������� ��� ������� ���� CPU
    if(cpu_time_left==0)
    {
        in_cpu_priority = 0;
    }

    //���������� ������������ ����������
    previous_process_to_cpu = process_to_cpu;
    previous_process_priority = in_cpu_priority;
    previous_process_time_arrived = stored_processes_time_arrived[save_best_place];

    //�������� ��� ���������� ��� �������� ���� CPU ��� ��� ����
    stored_processes[save_best_place] = 0;
    stored_processes_time_arrived[save_best_place] = 0;
    stored_processes_remaining_time[save_best_place] = 0;
    stored_priorities[save_best_place] = 0;
}
