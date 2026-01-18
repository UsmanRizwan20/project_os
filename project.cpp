#include <iostream>
using namespace std;

#define MAX 50
#define INF 1000000000

struct Process {
    int pid, arrival, burst;
    int remaining, completion, waiting, turnaround;
};

/* ---------- Utility ---------- */
void printTimes(Process p[], int n) {
    double wt = 0, tat = 0;
    cout << "\nPID\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n";
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].pid << "\t" << p[i].arrival << "\t" << p[i].burst << "\t"
             << p[i].completion << "\t\t" << p[i].waiting << "\t" << p[i].turnaround << "\n";
        wt += p[i].waiting;
        tat += p[i].turnaround;
    }
    cout << "Average Waiting Time    : " << wt / n << "\n";
    cout << "Average Turnaround Time : " << tat / n << "\n";
}

/* ---------- FCFS ---------- */
void FCFS(Process p[], int n) {
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].arrival > p[j].arrival) { Process t = p[i]; p[i] = p[j]; p[j] = t; }

    int time = 0;
    cout << "\n--- FCFS Gantt Chart ---\n";

    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival) { cout << "| IDLE (" << time << "-" << p[i].arrival << ") "; time = p[i].arrival; }
        cout << "| P" << p[i].pid << " (" << time << "-";
        p[i].waiting = time - p[i].arrival;
        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = time - p[i].arrival;
        cout << time << ") ";
    }
    cout << "|\n";
    printTimes(p, n);
}

/* ---------- SJF ---------- */
void SJF(Process p[], int n) {
    bool done[MAX] = {0};
    int time = 0, finished = 0;
    cout << "\n--- SJF Gantt Chart ---\n";

    while (finished < n) {
        int idx = -1;
        for (int i = 0; i < n; i++)
            if (!done[i] && p[i].arrival <= time)
                if (idx == -1 || p[i].burst < p[idx].burst) idx = i;

        if (idx == -1) {
            int next = INF;
            for (int i = 0; i < n; i++) if (!done[i] && p[i].arrival < next) next = p[i].arrival;
            cout << "| IDLE (" << time << "-" << next << ") ";
            time = next; continue;
        }

        cout << "| P" << p[idx].pid << " (" << time << "-";
        p[idx].waiting = time - p[idx].arrival;
        time += p[idx].burst;
        p[idx].completion = time;
        p[idx].turnaround = time - p[idx].arrival;
        done[idx] = true; finished++;
        cout << time << ") ";
    }
    cout << "|\n";
    printTimes(p, n);
}

/* ---------- Round Robin ---------- */
void RoundRobin(Process p[], int n, int qtm) {
    int q[MAX], front = 0, rear = 0, time, completed = 0, nextArr = 0;

    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].arrival > p[j].arrival) { Process t = p[i]; p[i] = p[j]; p[j] = t; }

    cout << "\n--- Round Robin Gantt Chart ---\n";
    time = p[0].arrival;

    while (completed < n) {
        while (nextArr < n && p[nextArr].arrival <= time) q[rear++] = nextArr++;

        if (front == rear) { time = p[nextArr].arrival; continue; }

        int idx = q[front++];
        cout << "| P" << p[idx].pid << " (" << time << "-";

        int exec = (p[idx].remaining < qtm) ? p[idx].remaining : qtm;
        p[idx].remaining -= exec;
        time += exec;

        cout << time << ") ";

        while (nextArr < n && p[nextArr].arrival <= time) q[rear++] = nextArr++;

        if (p[idx].remaining > 0) q[rear++] = idx;
        else {
            p[idx].completion = time;
            p[idx].turnaround = time - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            completed++;
        }
    }
    cout << "|\n";
    printTimes(p, n);
}

/* ---------- SRTF ---------- */
void SRTF(Process p[], int n) {
    int time = 0, done = 0, last = -1;
    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    cout << "\n--- SRTF Gantt Chart ---\n";

    while (done < n) {
        int idx = -1;
        for (int i = 0; i < n; i++)
            if (p[i].arrival <= time && p[i].remaining > 0)
                if (idx == -1 || p[i].remaining < p[idx].remaining) idx = i;

        if (idx == -1) { time++; continue; }

        if (last != idx) { cout << "| P" << p[idx].pid << " (" << time << "-"; last = idx; }

        p[idx].remaining--; time++;

        if (p[idx].remaining == 0) {
            cout << time << ") ";
            p[idx].completion = time;
            p[idx].turnaround = time - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            done++; last = -1;
        }
    }
    cout << "|\n";
    printTimes(p, n);
}

/* ---------- Main ---------- */
int main() {
    int n, qtm;
    Process p[MAX];

    cout << "Enter number of processes: ";
    cin >> n;

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        cout << "Arrival Time for P" << i + 1 << ": "; cin >> p[i].arrival;
        cout << "Burst Time for P" << i + 1 << ": "; cin >> p[i].burst;
    }

    cout << "\n1. FCFS\n2. SJF\n3. Round Robin\n4. SRTF\nChoice: ";
    int ch; cin >> ch;

    if (ch == 3) { cout << "Enter Time Quantum: "; cin >> qtm; }

    if (ch == 1) FCFS(p, n);
    else if (ch == 2) SJF(p, n);
    else if (ch == 3) RoundRobin(p, n, qtm);
    else if (ch == 4) SRTF(p, n);
    else cout << "Invalid choice\n";

    return 0;
}