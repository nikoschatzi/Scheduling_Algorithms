#ifndef MAIN_C_RR_H
#define MAIN_C_RR_H

//μεταβλητές και συναρτήσεις από το αρχείο main.c που απαιτούνται
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


//συνάρτηση υλοποίησης αλγορίθμου Round Robin
void RR(int quantum)
{
    //μεταβλητή που αποθηκεύεται η διεργασία που θα σταλεί στην CPU
    static int process_to_cpu = 0;
    //μεταβλητές που αποθηκεύονται τα στοιχεία της προηγούμενης διεργασίας που έχει σταλεί στην CPU
    static int previous_process_to_cpu = 0;
    static int previous_process_priority = 0;
    static int previous_process_time_arrived = 0;
    //μεταβλητή που αποθηκεύεται ο υπολειπόμενος χρόνος, αρχικοποίηση στο 0
    static int cpu_time_left = 0;
    //μεταβλητή που αποθηκεύεται ο υπολειπόμενος χρόνος της προηγούμενης διαδικασία (για την περίπτωση που βγει από την CPU και πρέπει να σταλεί στην ουρά), αρχικοποίηση στο 0
    static int in_cpu_previous_time = 0;
    //μεταβλητή τύπου int που αποθηκεύται η θέση στην ουρά (έχει 40 θέσεις) που βρίσκεται η διαδικασία που θα μπει στην CPU
    static int save_best_place = 0;
    //timer που είναι αρχικοποιημένος με το κβάντο χρόνου του αλγορίθμου
    static int quantum_timer = 4;
    //μεταβλητή που αποθηκεύεται ο χρόνος άφιξης της διαδικασίας
    int arrival_time;
    //μεταβλητή για την for συνάρτηση
    int n;

    //αν ο χρόνος γίνει ίσος με το κβάντο χρόνο ή αν τελειώσει η τρέχουσα διεργασία γίνεται έλεγχος
    if(quantum_timer==quantum || cpu_time_left==0)
    {
        arrival_time = 100;

        for (n = 0; n < 40; n++) {
            if (stored_processes[n] != 0)
            {
                //συνθήκη για επιλογή διαδικασία που θα μπεί στην CPU
                if (stored_processes_time_arrived[n] < arrival_time) {

                    arrival_time = stored_processes_time_arrived[n];
                    process_to_cpu = stored_processes[n];
                    cpu_time_left = stored_processes_remaining_time[n];
                    save_best_place = n;

                }

            }
        }
        //μηδενισμός του timer
        quantum_timer = 0;
    }

    //αν βρεθεί διαφορετική διαδιακασία και η προηγούμενη δεν έχει τελείωσει ξαναμπαίνει η προηγούμενη στην ουρά
    if(previous_process_to_cpu!=process_to_cpu && in_cpu_previous_time!=0)
    {
        queue(previous_process_to_cpu,in_cpu_previous_time,previous_process_priority,time);
    }

    //αν βρεθεί διαφορετική διαδιακασία αλλάζουμε την υπολειπόμενο χρόνο
    if(previous_process_to_cpu!=process_to_cpu)
    {
        cpu_time_left = stored_processes_remaining_time[save_best_place];
    }

        //αν δεν έχει τελειώσει ο χρόνος στέλνουμε την διεργασία και μειώνουμε τον χρόνο κατά 1
        if(cpu_time_left!=0)
        {
            CPU(process_to_cpu);
            cpu_time_left -= 1;
        }

        //στο τέλος τυπώνουμε finished
        else
        {
            printf("    finished!");
        }

    //μειώση του timer κατά 1
    quantum_timer += 1;

    //αποθήκευση προηγούμενης διεργασίας
    previous_process_to_cpu = process_to_cpu;
    previous_process_priority = stored_priorities[save_best_place];
    previous_process_time_arrived = stored_processes_time_arrived[save_best_place];

    //εξισώνουμε τους δύο χρόνους
    in_cpu_previous_time = cpu_time_left;

    //βγάζουμε την διαδικασία που στάλθηκε στην CPU από την ουρά
    stored_processes[save_best_place] = 0;
    stored_processes_time_arrived[save_best_place] = 0;
    stored_processes_remaining_time[save_best_place] = 0;
    stored_priorities[save_best_place] = 0;

}

