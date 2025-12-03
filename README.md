# 💻 Operating System CPU Scheduling Simulator

This project implements a simulator for four fundamental CPU scheduling algorithms. The project strictly adheres to the requirement that the core logic must be implemented in **C**, while the visualization and user interface are provided by a self-contained **HTML/CSS/JS** file.

### 230183 Axmedov Ruslan

### 1. Algorithms Implemented (C & JavaScript)

| Algorithm | Type | Description |
| :--- | :--- | :--- |
| **1. First-Come-First-Served (FCFS)** | Non-Preemptive | Processes are executed in the order of their arrival. |
| **2. Shortest Job First (SJF)** | Non-Preemptive | The process with the smallest **Burst Time** is executed next. |
| **3. Priority Scheduling** | Non-Preemptive | The process with the highest priority (lowest number) is executed next. |
| **4. Round Robin (RR)** | Preemptive | Processes are given a small time slice (Quantum) to run, after which they are moved to the end of the queue. |

### 2. Visualization (index.html)

The web interface provides a step-by-step, time-based animation showing the life cycle of each process:
* **Real-time Clock:** A timer tracks the Current Time ($t=0, t=1, ...$).
* **Waiting Queue:** Visual representation of processes that have arrived but are waiting for the CPU.
* **CPU (Running):** Shows the currently executing process.
* **Completed Processes:** Shows processes that have finished their execution.
* **Live Gantt Chart:** The chart is built dynamically, second-by-second, illustrating the CPU utilization.
* **Final Metrics:** Calculates and displays **Average Waiting Time (WT)** and **Average Turnaround Time (TAT)**.

## 🚀 Instructions to Run

### Part A: Running the C Backend (Core Logic)

1.  **Compile:** Open a terminal or command prompt in the project directory.
    ```bash
    gcc scheduler.c -o scheduler
    ```
2.  **Run:** Execute the compiled program.
    ```bash
    ./scheduler
    ```
3.  **Use:** The program will ask for the number of processes and their parameters (**AT, BT, Priority**). You will then be prompted to select an algorithm (1-4) to run and display the calculated metrics (WT/TAT) in the console.

### Part B: Running the HTML Frontend (Visualization)

1.  **Open:** Locate the **`index.html`** file in your file explorer.
2.  **Execute:** Double-click the file. It will automatically open in your default web browser (Chrome, Firefox, etc.).
3.  **Interact:**
    * Input **Arrival Time, Burst Time,** and **Priority** for your processes and click **`+ Add new process`**.
    * Select an **Algorithm** from the dropdown (and specify a **Time Quantum** for RR).
    * Click **`▶ Start Simulation`** to view the animated execution.

## 💡 Core OS Concepts

The simulator relies on the following process metrics, which must be provided by the user:

| Term | Full Name | Definition |
| :--- | :--- | :--- |
| **AT** | **Arrival Time** | The time at which the process enters the Ready Queue. |
| **BT** | **Burst Time** | The total CPU time required by the process to complete its execution. |
| **Priority** | Priority Level | The importance level of the process (Lower number = Higher Priority). |
| **TAT** | **Turnaround Time** | The total time spent by a process in the system (Completion Time - Arrival Time). |
| **WT** | **Waiting Time** | The total time a process spends waiting in the Ready Queue (TAT - Burst Time). |


<img width="1598" height="678" alt="image" src="https://github.com/user-attachments/assets/746818b6-df85-4265-86f5-e8831b801815" />

