#ifndef MAIN_C_PP_H
#define MAIN_C_PP_H

//μεταβλητές και συναρτήσεις από το αρχείο main.c που απαιτούνται
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

//συνάρτηση υλοποίησης αλγορίθμου Priority based Preemptive
void PP()
{
    //μεταβλητή που αποθηκεύεται η διεργασία που θα σταλεί στην CPU
    static int process_to_cpu = 0;
    //μεταβλητές που αποθηκεύονται τα στοιχεία της προηγούμενης διεργασίας που έχει σταλεί στην CPU
    static int previous_process_to_cpu = 0;
    static int previous_process_priority = 0;
    static int previous_process_time_arrived = 0;
    //μεταβλητή που αποθηκεύεται ο υπολειπόμενος χρόνος
    static int cpu_time_left = 0;
    //μεταβλητή τύπου int που αποθηκεύται η θέση στην ουρά (έχει 40 θέσεις) που βρίσκεται η διαδικασία που θα μπει στην CPU
    static int save_best_place = 0;
    //μεταλητές για τις προτεραίοτητες
    static int in_cpu_priority = 0;
    int best_priority;
    //μεταβλητή για την for συνάρτηση
    int n;

    //αρχικοποίηση καλύτερης προτεραιότητας στο 0
    best_priority = 0;

    //αναζήτηση διαδικασίας με την μεγαλύτερη προτεραιότητα
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

            //αν δεν βρεθεί διαδικασία με μεγαλύτερη προτεραιότητα από αυτήν που είναι στην CPU αναθέτουμε ως καλύτερη προτεραιότητα αυτή της διαδιακασίας της CPU
            if(best_priority==0)
            {
                best_priority = in_cpu_priority;
            }

        }
    }

    //αν βρεθεί διαφορετική διαδιακασία και η προηγούμενη δεν έχει τελείωσει ξαναμπαίνει η προηγούμενη στην ουρά
    if(previous_process_to_cpu!=process_to_cpu && cpu_time_left!=0)
    {
        queue(previous_process_to_cpu,cpu_time_left,previous_process_priority,previous_process_time_arrived);
    }

    //αν βρεθεί διαφορετική διαδιακασία αλλάζουμε την υπολειπόμενο χρόνο
    if(previous_process_to_cpu!=process_to_cpu)
    {
        cpu_time_left = stored_processes_remaining_time[save_best_place];

        //η προτεραιότητα στην CPU είναι η καλύτερη προτεραιότητα που βρέθηκε
        in_cpu_priority = best_priority;
    }

    //αν δεν έχει τελειώσει ο χρόνος στέλνουμε την διεργασία και μειώνουμε τον χρόνο κατά 1
   if(cpu_time_left!=0)
   {
       CPU(process_to_cpu);
       cpu_time_left -= 1;
   }

   //με το τέλος τυπώνουμε finished
   else if(time==20)
   {
       printf("    finished!");;
   }

   //αν έχει τελειώσει μια διαδικασία μηδενίζουμε την προτεραιότητα που υπάρχει στην CPU
    if(cpu_time_left==0)
    {
        in_cpu_priority = 0;
    }

    //αποθήκευση προηγούμενης διεργασίας
    previous_process_to_cpu = process_to_cpu;
    previous_process_priority = in_cpu_priority;
    previous_process_time_arrived = stored_processes_time_arrived[save_best_place];

    //βγάζουμε την διαδικασία που στάλθηκε στην CPU από την ουρά
    stored_processes[save_best_place] = 0;
    stored_processes_time_arrived[save_best_place] = 0;
    stored_processes_remaining_time[save_best_place] = 0;
    stored_priorities[save_best_place] = 0;
}
