#include <stdio.h>
#include <stdbool.h>

#ifndef MAIN_C_FCFS_H
#define MAIN_C_FCFS_H

//μεταβλητές και συναρτήσεις από το αρχείο main.c που απαιτούνται
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

//συνάρτηση υλοποίησης αλγορίθμου First Come First Served
void FCFS()
{
    //μεταβλητή τύπου int που αποθηκεύται η διαδικασία που θα μπεί στην CPU
    static int process_to_cpu = 0;
    //μεταβλητή τύπου int που αποθηκεύται ο υπολειπόμενος χρόνος της διαδικασία που θα μείνει στην CPU
    static int cpu_time_left = 0;
    //μεταβλητή τύπου int που αποθηκεύται η θέση στην ουρά (έχει 40 θέσεις) που βρίσκεται η διαδικασία που θα μπει στην CPU
    static int save_best_place = 0;
    //μεταβλητή που αποθηκεύεται η τιμή που μας επιστρέφει η συνάρτηση is_empty και βλέπουμε αν η CPU είναι άδεια ή όχι
    int cpu_empty;
    //μεταβλητή που αποθηκεύεται ο χρόνος άφιξης της διαδικασίας
    int arrival_time;
    //μεταβλητή για την for συνάρτηση
    int n;

    //έλεγχος αν η CPU είναι άδεια
    cpu_empty = is_empty(false);

    //αν η CPU είναι άδεια τότε ψάχνουμε ποια διαδικασία έφτασε πιο νωρίς για να σταλεί στην CPU
    if(cpu_empty==true)
    {
        //αρχικοποίηση της μεταβλητής σε μια μεγάλη τυχαία τιμή (το 100)
        arrival_time = 100;

        for(n=0; n<40; n++)
        {
            if(stored_processes[n]!=0)
            {
                //αν βρεθέι διαδικασία που έφτασε πιο νωρίς τότε θα πρέπει να σταλεί αυτή στην CPU
                if(stored_processes_time_arrived[n]<arrival_time)
                {
                    arrival_time = stored_processes_time_arrived[n];
                    process_to_cpu = stored_processes[n];
                    cpu_time_left = stored_processes_remaining_time[n];
                    save_best_place = n;
                }

            }
        }

        //διαγραφή της διαδικασία που θα σταλεί στην CPU από την ουρά
        stored_processes[save_best_place] = 0;
        stored_processes_time_arrived[save_best_place] = 0;
        stored_processes_remaining_time[save_best_place] = 0;

        //αν ο χρόνος που απέμεινε για την διαδικασία δεν είναι 0 την στέλνουμε στην CPU
        if(cpu_time_left!=0)
        {
            CPU(process_to_cpu);
        }

        //όταν τελειώσουν όλες οι διεργασίες τυπώνουμε finished
        else
        {
            printf("    finished!");
        }

        //μείωση του υπολειπόμενου χρόνου κατά 1
        cpu_time_left -= 1;

        //αλλαγή κατάστασης CPU σε γεμάτη
        if(cpu_time_left!=0)
        {
            is_empty(true);
        }

    }

    //αν η CPU δεν είναι άδεια στέλνουμε πάλι την διαδικασία που στείλαμε και πριν και μειώνουμε τον χρόνο κατά 1
    else
    {
        CPU(process_to_cpu);
        cpu_time_left -= 1;

        //αν ο υπολειπόμενος χρόνος είναι 0 κάνουμε την κατάσταση της CPU άδεια
        if(cpu_time_left==0)
        {
            is_empty(true);
        }
    }

}

