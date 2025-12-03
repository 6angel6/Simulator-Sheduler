#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_PROCESSES 100

// Structure to log each execution step for the Gantt Chart
typedef struct {
    int pid;
    int start;
    int end;
} ExecutionBlock;

typedef struct {
    int pid;
    int at;             // Arrival Time
    int bt;             // Burst Time
    int priority;       // Priority (lower number = higher priority)
    int ct;             // Completion Time
    int tat;            // Turnaround Time
    int wt;             // Waiting Time
    int rt;             // Remaining Time
} Process;


// Function to print the final metrics table
void print_table(Process p[], int n) {
    int total_wt = 0, total_tat = 0;

    printf("\n--------------------------------------------------------------------------------\n");
    printf("| PID | Priority | Arrival | Burst | Completion | Turnaround | Waiting |\n");
    printf("--------------------------------------------------------------------------------\n");

    for(int i = 0; i < n; i++) {

        total_wt += p[i].wt;
        total_tat += p[i].tat;
        printf("| P%d  |    %d     |    %d    |   %d   |     %d      |     %d      |    %d    |\n",
               p[i].pid, p[i].priority, p[i].at, p[i].bt, p[i].ct, p[i].tat, p[i].wt);
    }
    printf("--------------------------------------------------------------------------------\n");
    printf("Average Turnaround Time: %.2f\n", (float)total_tat / n);
    printf("Average Waiting Time: %.2f\n", (float)total_wt / n);
    printf("--------------------------------------------------------------------------------\n\n");
}

// Function to print the execution order in a console-style Gantt Chart
void print_gantt_chart(ExecutionBlock timeline[], int count) {
    printf("\n--- Gantt Chart (Execution Order) ---\n");

    for(int i = 0; i < count; i++) {
        printf("+------");
    }
    printf("+\n");

    for(int i = 0; i < count; i++) {
        if (timeline[i].pid == 0) {
            printf("| IDLE ");
        } else {
            printf("| P%d   ", timeline[i].pid);
        }
    }
    printf("|\n");

    for(int i = 0; i < count; i++) {
        printf("+------");
    }
    printf("+\n");

    printf("%d", timeline[0].start);
    for(int i = 0; i < count; i++) {
        int spaces = 6;
        int end_len = 0;
        int temp = timeline[i].end;
        if (temp == 0) end_len = 1;
        while(temp > 0) {
            temp /= 10;
            end_len++;
        }

        spaces = 6 - (end_len - 1);
        for(int j=0; j<spaces; j++) printf(" ");

        printf("%d", timeline[i].end);
    }
    printf("\n");
}


// Utility function to sort processes by Arrival Time (AT)
void sort_by_arrival(Process p[], int n) {
    for(int i=0; i<n-1; i++) {
        for(int j=0; j<n-i-1; j++) {
            if(p[j].at > p[j+1].at) {
                Process temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
}


// 1. First-Come-First-Served (FCFS)
void fcfs(Process p[], int n) {
    printf("\n--- FCFS Scheduling ---\n");
    sort_by_arrival(p, n);

    ExecutionBlock timeline[MAX_PROCESSES + 1];
    int timeline_count = 0;
    int current_time = 0;

    for(int i=0; i<n; i++) {
        if(current_time < p[i].at) {

            timeline[timeline_count++] = (ExecutionBlock){0, current_time, p[i].at};
            current_time = p[i].at;
        }


        int start = current_time;
        p[i].ct = current_time + p[i].bt;
        current_time = p[i].ct;

        timeline[timeline_count++] = (ExecutionBlock){p[i].pid, start, p[i].ct};

        p[i].tat = p[i].ct - p[i].at;
        p[i].wt = p[i].tat - p[i].bt;
    }

    print_gantt_chart(timeline, timeline_count);
    print_table(p, n);
}

// 2. Shortest Job First (SJF) - Non-Preemptive
void sjf(Process p[], int n) {
    printf("\n--- SJF (Non-Preemptive) ---\n");
    int completed = 0, current_time = 0;
    int is_completed[MAX_PROCESSES] = {0};

    for(int i=0; i<n; i++) p[i].rt = p[i].bt;

    ExecutionBlock timeline[MAX_PROCESSES * 2];
    int timeline_count = 0;

    while(completed != n) {
        int idx = -1;
        int min_bt = 100000;

        for(int i=0; i<n; i++) {
            if(p[i].at <= current_time && is_completed[i] == 0) {
                if(p[i].bt < min_bt) {
                    min_bt = p[i].bt;
                    idx = i;
                }
                if(p[i].bt == min_bt) {
                    if(idx == -1 || p[i].at < p[idx].at) {
                        idx = i;
                    }
                }
            }
        }

        if(idx != -1) {
            int start = current_time;
            p[idx].ct = current_time + p[idx].bt;
            int end = p[idx].ct;

            timeline[timeline_count++] = (ExecutionBlock){p[idx].pid, start, end};

            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;

            is_completed[idx] = 1;
            completed++;
            current_time = end;
        } else {
            // IDLE period
            if (timeline_count > 0 && timeline[timeline_count-1].pid == 0) {
                timeline[timeline_count-1].end++;
            } else {
                timeline[timeline_count++] = (ExecutionBlock){0, current_time, current_time + 1}; 
            }
            current_time++;
        }
    }
    print_gantt_chart(timeline, timeline_count);
    print_table(p, n);
}

// 3. Priority Scheduling (Non-Preemptive)
void priority_scheduling(Process p[], int n) {
    printf("\n--- Priority Scheduling (Non-Preemptive) ---\n");
    int completed = 0, current_time = 0;
    int is_completed[MAX_PROCESSES] = {0};

    ExecutionBlock timeline[MAX_PROCESSES * 2];
    int timeline_count = 0;

    while(completed != n) {
        int idx = -1;
        int highest_priority = 100000;

        for(int i=0; i<n; i++) {
            if(p[i].at <= current_time && is_completed[i] == 0) {
                if(p[i].priority < highest_priority) {
                    highest_priority = p[i].priority;
                    idx = i;
                }
                if(p[i].priority == highest_priority) {
                    if(idx == -1 || p[i].at < p[idx].at) {
                        idx = i;
                    }
                }
            }
        }

        if(idx != -1) {
            int start = current_time;
            p[idx].ct = current_time + p[idx].bt;
            int end = p[idx].ct;


            timeline[timeline_count++] = (ExecutionBlock){p[idx].pid, start, end};

            p[idx].tat = p[idx].ct - p[idx].at;
            p[idx].wt = p[idx].tat - p[idx].bt;

            is_completed[idx] = 1;
            completed++;
            current_time = end;
        } else {

            if (timeline_count > 0 && timeline[timeline_count-1].pid == 0) {
                timeline[timeline_count-1].end++;
            } else {
                timeline[timeline_count++] = (ExecutionBlock){0, current_time, current_time + 1}; 
            }
            current_time++;
        }
    }
    print_gantt_chart(timeline, timeline_count);
    print_table(p, n);
}

// 4. Round Robin (RR)
void round_robin(Process p[], int n, int quantum) {
    printf("\n--- Round Robin (Quantum: %d) ---\n", quantum);
    sort_by_arrival(p, n);

    int remaining_burst[MAX_PROCESSES];
    for(int i=0; i<n; i++) remaining_burst[i] = p[i].bt;

    ExecutionBlock timeline[MAX_PROCESSES * 100];
    int timeline_count = 0;

    int current_time = 0;
    int done = 0;


    while(done < n) {
        int work_done_this_cycle = 0;

        for(int i=0; i<n; i++) {
            if(remaining_burst[i] > 0 && p[i].at <= current_time) {
                work_done_this_cycle = 1;

                int exec_time = (remaining_burst[i] > quantum) ? quantum : remaining_burst[i];

                int start = current_time;
                current_time += exec_time;
                remaining_burst[i] -= exec_time;

                timeline[timeline_count++] = (ExecutionBlock){p[i].pid, start, current_time};

                if(remaining_burst[i] == 0) {
                    p[i].ct = current_time;
                    p[i].tat = p[i].ct - p[i].at;
                    p[i].wt = p[i].tat - p[i].bt;
                    done++;
                }
            }
        }

        if(work_done_this_cycle == 0) {
            if (timeline_count > 0 && timeline[timeline_count-1].pid == 0) {
                timeline[timeline_count-1].end++;
            } else {
                timeline[timeline_count++] = (ExecutionBlock){0, current_time, current_time + 1};
            }
            current_time++;
        }
    }
    print_gantt_chart(timeline, timeline_count);
    print_table(p, n);
}


int main() {
    Process processes[MAX_PROCESSES];
    int n, choice, quantum;

    printf("OS Simulator Project: CPU Scheduling\n");
    printf("Enter number of processes: ");
    scanf("%d", &n);

    for(int i=0; i<n; i++) {
        processes[i].pid = i + 1;
        printf("Process %d [Arrival Time, Burst Time, Priority]: ", i+1);

        scanf("%d %d %d", &processes[i].at, &processes[i].bt, &processes[i].priority);
    }

    while(1) {
        printf("\nSelect Algorithm:\n");
        printf("1. FCFS\n");
        printf("2. SJF (Non-Preemptive)\n");
        printf("3. Priority (Non-Preemptive)\n");
        printf("4. Round Robin\n");
        printf("5. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        Process temp_p[MAX_PROCESSES];
        for(int i=0; i<n; i++) temp_p[i] = processes[i];

        switch(choice) {
            case 1: fcfs(temp_p, n); break;
            case 2: sjf(temp_p, n); break;
            case 3: priority_scheduling(temp_p, n); break;
            case 4:
                printf("Enter Time Quantum: ");
                scanf("%d", &quantum);
                round_robin(temp_p, n, quantum);
                break;
            case 5: exit(0);
            default: printf("Invalid choice!\n");
        }
    }
    return 0;
}
