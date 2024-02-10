Real-Time Process Scheduler Simulator

This project invites you to explore and compare the performance of three popular CPU scheduling algorithms: Non-preemptive Highest Priority First (HPF), Shortest Remaining Time Next (SRTN), and Round Robin (RR).

Platform: Linux
Language: C

Project Overview:

The project is divided into four main modules:

    Process Generator: Simulates process creation and provides input to the scheduler as processes arrive.
    Clock: Emulates an integer time clock, keeping track of simulated time progression.
    Scheduler: The core of the project, implementing the chosen scheduling algorithm to manage running processes and their states.
    Process: Simulates individual processes executing CPU-bound tasks and interacting with the scheduler upon completion.

Algorithms Implemented:

    Non-preemptive Highest Priority First (HPF): Prioritizes processes based on their assigned priority values.
    Shortest Remaining Time Next (SRTN): Selects the process with the shortest remaining execution time next, minimizing overall wait times.
    Round Robin (RR): Allocates CPU time slices to processes in a predefined order, ensuring fairness and responsiveness.
