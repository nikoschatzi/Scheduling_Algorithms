#ifndef MAIN_C_SRTF_H
#define MAIN_C_SRTF_H

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

#endif //MAIN_C_SRTF_H


//συνάρτηση υλοποίησης αλγορίθμου Shortest Remaining Time First
void SRTF()
{
    //μεταβλητή που αποθηκεύεται η διεργασία που θα σταλεί στην CPU
    static int process_to_cpu = 0;
    //μεταβλητές που αποθηκεύονται τα στοιχεία της προηγούμενης διεργασίας που έχει σταλεί στην CPU
    static int previous_process_to_cpu = 0;
    static int previous_process_priority = 0;
    static int previous_process_time_arrived = 0;
    //μεταβλητή που αποθηκεύεται ο υπολειπόμενος χρόνος, αρχικοποίηση σε μεγάλη τυχαία τιμή (το 100)
    static int cpu_time_left = 100;
    //μεταβλητή τύπου int που αποθηκεύται η θέση στην ουρά (έχει 40 θέσεις) που βρίσκεται η διαδικασία που θα μπει στην CPU
    static int save_best_place = 0;
    //extra μεταβλητή για αποθήκευση χρόνου που απαιτείται για την υλοποίηση
    static int in_cpu_remaining_time = 100;
    //μεταβλητή για καλύτερο υπολειπόμενο χρόνο
    int best_remaining_time;
    //μεταβλητή για την for συνάρτηση
    int n;

    //αρχικοποίηση της μεταβλητής σε μια μεγάλη τυχαία τιμή (το 100)
    best_remaining_time = 100;

    //εύρεση διεργασίας με τον μικρότερο υπολειπόμενο χρόνο
    if(in_cpu_remaining_time==0 && time<20)
    {
        in_cpu_remaining_time = 100;
    }

    for(n=0; n<40; n++)
    {
        if(stored_processes[n]!=0)
        {
            //αν μια διεργασία έχει τον μικρότερο υπολειπόμενο χρόνο από όσες είναι στην ουρά και από αυτήν που είναι στην CPU είναι η καλύτερη σύμφωνα με τον αλγόριθμο
            if(stored_processes_remaining_time[n]<best_remaining_time && stored_processes_remaining_time[n]<in_cpu_remaining_time)
            {
                best_remaining_time = stored_processes_remaining_time[n];
                process_to_cpu = stored_processes[n];
                save_best_place = n;
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
        in_cpu_remaining_time = stored_processes_remaining_time[save_best_place];
    }

    //αν δεν έχει τελειώσει ο χρόνος στέλνουμε την διεργασία και μειώνουμε τον χρόνο κατά 1
    if(in_cpu_remaining_time!=0)
    {
        CPU(process_to_cpu);
        in_cpu_remaining_time -= 1;
    }

    //με το τέλος τυπώνουμε finished
    else
    {
        printf("    finished!");;
    }

    //αποθήκευση προηγούμενης διεργασίας
    previous_process_to_cpu = process_to_cpu;
    previous_process_priority = stored_priorities[save_best_place];
    previous_process_time_arrived = stored_processes_time_arrived[save_best_place];

    //βγάζουμε την διαδικασία που στάλθηκε στην CPU από την ουρά
    stored_processes[save_best_place] = 0;
    stored_processes_time_arrived[save_best_place] = 0;
    stored_processes_remaining_time[save_best_place] = 0;
    stored_priorities[save_best_place] = 0;

    //εξισώνουμε τους δύο χρόνους
    cpu_time_left = in_cpu_remaining_time;
}
