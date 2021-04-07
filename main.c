#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>

//include των header αρχείων που έχουν τις συναρτήσεις υλοποίησης των αλγορίθμων
#include "FCFS.h" //1
#include "PP.h"   //2
#include "SRTF.h" //3
#include "RR.h"   //4

//επιλογή αλγορίθμου για προσομοίωση, 1:FCFS, 2:PP, 3:SRFT, 4:RR
int CHOOSE_ALGORITHM = 1;

//αρχικοποίηση global μεταβλητών που αφορούν το ρολόι
int pulse_level_old = 0;
int pulse_level_new = 0;
signed int time = -1;
bool rising_clock = false;

//αρχικοποίηση global μεταβλητών που αφορούν την αποθήκευση στοιχείων σε πίνακες
//επιλέγεται η ουρά να έχει 40 θέσεις αποθήκευσης
int stored_processes[40] = {0};
int stored_processes_remaining_time[40] = {0};
int stored_processes_time_arrived[40] = {0};
int stored_priorities[40] = {0};
//πίνακας για την απεικόνιση του Gannt διαγράμματος
int FCFS_Gannt[20] = {0};
//αρχικοποίηση πίνακα που αποθηκεύονται οι χρόνοι εισόδου των διεργασιών στην CPU
int entered_cpu[5] = {100,100,100,100,100};
//αρχικοποίηση πίνακα που αποθηκεύονται οι χρόνοι εξόδου των διεργασιών από την CPU
int exit_cpu[5] = {0};
//αρχικοποίηση πίνακα που αποθηκεύονται οι χρόνοι φόρτωσης των διεργασιών από την CPU
int loaded_in_cpu[5] = {0};
//αρχικοποίηση πίνακα που αποθηκεύονται οι χρόνοι αναμονής των διεργασιών στην CPU
int wait_in_cpu[5] = {0};

//συνάρτηση προσομοίωσης παλμών ρολογιού, σε rising edge αλλάζει το time
void clock()
{
    //εισαγωγή 0.5 seconds καθυστέρησης
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

    //σε rising edge αυξάνεται το time κατά ένα
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

//συνάρτηση προσομοίωσης λειτουργίας CPU, είναι τύπου int καθώς επιστρέφει το αποτέλεσμα ενός πολλαπλασιασμού
int CPU(int procces)
{
    //μεταβλητές τύπου static int για τον υπολογισμό wait time and turnaround time
    static int previous_process = 0;
    static int in_cpu_time_counter = 0;

    //μεταβλητές για την υλοποίηση του Gannt διαγράμματος και για τον υπολογισμό των χρόνων που απαιτούνται
    static int time_counter = 0;

    //εκτύπωση της διαδικασία που βρίσκεται στην CPU
    printf("        P%d          ", procces);

    //έλεγχος αν έχει μπει καινούργια διαδικασία στην CPU
    if(previous_process!=procces)
    {
        //ο πίνακας entered_cpu αρχικοποιείται σε μια μεγάλη τυχαία τιμή (το 100), αν η θέση του πίνακα που αντιστοιχεί σε μια συγκεκριμένη διαδικασία δεν έχει ξαναγραφτεί πρέπει να γίνει ανάθεση του χρόνου
        if(entered_cpu[procces-1]==100)
        {
            entered_cpu[procces - 1] = time_counter;
        }

        //ο χρόνος αναμονής είναι ίσος με τον τρέχον χρόνο μείον την χρονική στιγμή που μπήκε η διαδικασία
        else
        {
            wait_in_cpu[procces-1] = time_counter - exit_cpu[procces-1];
        }

        //πόσες φορές φορτώθηκε η διαδικασία στην CPU
        loaded_in_cpu[procces-1] += 1;

        //η χρονική στιγμή που βγήκε η διαδικασία από την CPU
        exit_cpu[previous_process - 1] = time_counter;

    }

    //αν ο χρόνος είναι ίσος με 19 ανάθεση του χρόνου εξόδου της τελευταίας διαδικασίας στο 19
    if(time_counter==19)
    {
        exit_cpu[procces-1] = time_counter + 1;
    }

    //πίνακας για αποθήκευση διαδικάσιων για την εμφάνιση του διαγράμματος Gannt
    FCFS_Gannt[time_counter] = procces;
    previous_process = procces;

    //αύξηση του time_counter κατά 1
    time_counter += 1;

    //πράξη πολλαπλασιασμού που κάνει η CPU
    return (procces*100);
}

//συνάρτηση τύπου bool που επιστρέφει αν η CPU είναι άδεια ή όχι
bool is_empty(bool change_state)
{
    //αρχικοποίηση κατάστασης CPU σε άδεια
    static bool flag = true;

    //αν το όρισμα της συνάρτησης είναι true αλλαγή κατάστασης CPU από άδεια σε γεμάτη ή το αντίστροφο
    if(change_state==true)
    {
        flag = !flag;
    }

    //διαφορετικά η συνάρτηση επιστρέφει την κατάσταση της CPU
    else
    {
        return flag;
    }
}

//συνάρτηση υλοποίησης της ready_queue, η συνάρτηση αυτή χρησιμοποιείτα από όλους τους αλγορίθμους. Όταν έρχεται ο χρόνος άφιξης μια διεργασίας εισέρχεται στην queue
void queue(int process,int remaining_time,int priority,int time_arrived)
{
    static int k = 0;

    stored_processes[k] = process;
    stored_processes_remaining_time[k] = remaining_time;
    stored_processes_time_arrived[k] = time_arrived;
    stored_priorities[k] = priority;

    k += 1;

}

//συνάρτηση για απεικόνιση των στοιχείων της queue
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

//συνάρτηση για απεικόνιση του Gannt διαγράμματος
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

//συνάρτηση για απεικόνιση των αποτελεσμάτων για τους χρόνους που ζητούνται
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

//συνάρτηση main
int main() {

    //αρχικοποίηση πινάκων διαδικασιών με βάση τον πίνακα της εκφώνησης
    int processes[] = {1,2,3,4,5};
    int arrival_time[] = {0,1,3,9,12};
    int total_time[] = {5,6,2,4,3};
    int priority[] = {3,5,2,1,4};
    int i,j;
    //ορισμός για το κβάντο χρόνου για τον αλγόριθμο Round Robin
    int quantum = 4;

    //ανάλογα με την επιλογή του αλγορίθμου εμφανίζεται το αντίστοιχο μήνυμα (επιλογή στην γραμμή 12 του αρχείου)
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

        //ανάλογα με την επιλογή του αλγορίθμου εμφανίζεται καλείται η αντίστοιχη συνάρτηση (επιλογή στην γραμμή 12 του αρχείου)
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

    //συναρτήσεις για απεικόνιση διαγράμματος Gannt και των χρόνων
    Gannt_results();
    time_results(arrival_time);

    return 0;
}

