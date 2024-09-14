#include<bits/stdc++.h>
using namespace std;

class BankersAlgorithm {
private:
    int numProcesses;
    int numResources;
    vector<vector<int>> maximum;
    vector<vector<int>> allocation;
    vector<vector<int>> need;
    vector<int> available;
    vector<vector<bool>> request;
    vector<vector<bool>> waitForGraph;

    void displayMatrices() {
        cout << "\nAvailable Resources: ";
        for (int i = 0; i < numResources; i++) {
            cout << available[i] << " ";
        }
        cout << "\n";

        cout << "Maximum Matrix:\n";
        for (const auto& row : maximum) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << "\n";
        }

        cout << "Allocation Matrix:\n";
        for (const auto& row : allocation) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << "\n";
        }

        cout << "Need Matrix:\n";
        for (const auto& row : need) {
            for (int val : row) {
                cout << val << " ";
            }
            cout << "\n";
        }

        cout << "Request Matrix:\n";
        for (const auto& row : request) {
            for (bool val : row) {
                cout << val << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }

    bool isSafe(vector<int>& safeSequence) {
        vector<int> work = available;
        vector<bool> finish(numProcesses, false);
        safeSequence.clear();

        cout << "\nChecking system safety:\n";
        displayMatrices();

        for (int count = 0; count < numProcesses; count++) {
            bool found = false;
            for (int p = 0; p < numProcesses; p++) {
                if (!finish[p]) {
                    bool canProceed = true;
                    for (int j = 0; j < numResources; j++) {
                        if (need[p][j] > work[j]) {
                            canProceed = false;
                            break;
                        }
                    }
                    if (canProceed) {
                        for (int k = 0; k < numResources; k++) {
                            work[k] += allocation[p][k];
                        }
                        safeSequence.push_back(p);
                        finish[p] = true;
                        found = true;
                        cout << "Process P" << p << " has been allocated. Current Available: ";
                        for (int m = 0; m < numResources; m++) {
                            cout << work[m] << " ";
                        }
                        cout << "\n";
                        
                        displayMatrices();

                        break;
                    }
                }
            }
            if (!found) {
                return false; 
            }
        }
        return true; 
    }

    void dfs(int v, vector<bool>& visited, vector<bool>& recStack, bool& hasCycle) {
        if (recStack[v]) {
            hasCycle = true;
            return;
        }
        if (visited[v]) return;

        visited[v] = true;
        recStack[v] = true;

        for (int u = 0; u < numProcesses; u++) {
            if (waitForGraph[v][u]) {
                dfs(u, visited, recStack, hasCycle);
            }
        }
        recStack[v] = false;
    }

    bool detectDeadlock() {
        vector<bool> visited(numProcesses, false);
        vector<bool> recStack(numProcesses, false);
        bool hasCycle = false;

        for (int i = 0; i < numProcesses; i++) {
            if (!visited[i]) {
                dfs(i, visited, recStack, hasCycle);
                if (hasCycle) {
                    return true;
                }
            }
        }
        return false;
    }

    void handleDeadlock() {
        cout << "\nDeadlock detected. Terminating a process...\n";

        int terminatedProcess = -1;

        for (int i = 0; i < numProcesses; i++) {
            vector<int> originalAvailable = available;
            vector<vector<int>> originalAllocation = allocation;
            vector<vector<int>> originalNeed = need;
            vector<vector<int>> originalMaximum = maximum;
            vector<vector<bool>> originalRequest = request;

            for (int j = 0; j < numResources; j++) {
                available[j] += allocation[i][j];
            }

            maximum.erase(maximum.begin() + i);
            allocation.erase(allocation.begin() + i);
            need.erase(need.begin() + i);
            request.erase(request.begin() + i);
            for (auto& row : request) {
                row.erase(row.begin() + i);
            }
            numProcesses--;

            waitForGraph = vector<vector<bool>>(numProcesses, vector<bool>(numProcesses, false));
            for (int p = 0; p < numProcesses; p++) {
                for (int q = 0; q < numProcesses; q++) {
                    if (need[p][q] > 0) {
                        waitForGraph[p][q] = true;
                    }
                }
            }

            vector<int> safeSequence;
            if (isSafe(safeSequence)) {
                terminatedProcess = i;  
                cout << "\nSystem is now in a SAFE state after terminating process P" << i << ".\n";
                cout << "Safe sequence is: ";
                for (int j : safeSequence) {
                    cout << "P" << j << " ";
                }
                cout << "\n";

                addNewProcess();

                waitForGraph = vector<vector<bool>>(numProcesses, vector<bool>(numProcesses, false));
                for (int p = 0; p < numProcesses; p++) {
                    for (int q = 0; q < numProcesses; q++) {
                        if (need[p][q] > 0) {
                            waitForGraph[p][q] = true;
                        }
                    }
                }

                vector<int> newSafeSequence;
                if (isSafe(newSafeSequence)) {
                    cout << "System is SAFE after adding a new process.\n";
                    cout << "Safe sequence is: ";
                    for (int j : newSafeSequence) {
                        cout << "P" << j << " ";
                    }
                    cout << "\n";
                } else {
                    cout << "System is still UNSAFE after adding a new process.\n";
                }
                return;
            } else {
                available = originalAvailable;
                allocation = originalAllocation;
                need = originalNeed;
                maximum = originalMaximum;
                request = originalRequest;
                numProcesses++;
                maximum.insert(maximum.begin() + i, vector<int>(numResources, 0));
                allocation.insert(allocation.begin() + i, vector<int>(numResources, 0));
                need.insert(need.begin() + i, vector<int>(numResources, 0));
                request.insert(request.begin() + i, vector<bool>(numResources, false));
                for (auto& row : request) {
                    row.insert(row.begin() + i, false);
                }
                waitForGraph.insert(waitForGraph.begin() + i, vector<bool>(numProcesses, false));
                for (auto& row : waitForGraph) {
                    row.insert(row.begin() + i, false);
                }
            }
        }

        if (terminatedProcess == -1) {
            cout << "\nCould not find a safe state after terminating any process.\n";
        }
    }

    void addNewProcess() {
        cout << "\nAdding a new process:\n";
        numProcesses++;

        maximum.resize(numProcesses, vector<int>(numResources));
        allocation.resize(numProcesses, vector<int>(numResources));
        need.resize(numProcesses, vector<int>(numResources));
        request.resize(numProcesses, vector<bool>(numResources, false));
        waitForGraph.resize(numProcesses, vector<bool>(numProcesses, false));

        cout << "New Maximum Matrix for P" << numProcesses - 1 << ":\n";
        for (int j = 0; j < numResources; j++) {
            maximum[numProcesses - 1][j] = rand() % 10;
            cout << maximum[numProcesses - 1][j] << " ";
        }
        cout << "\n";

        cout << "New Allocation Matrix for P" << numProcesses - 1 << ":\n";
        for (int j = 0; j < numResources; j++) {
            allocation[numProcesses - 1][j] = rand() % (maximum[numProcesses - 1][j] + 1);
            cout << allocation[numProcesses - 1][j] << " ";
        }
        cout << "\n";

        cout << "New Need Matrix for P" << numProcesses - 1 << ":\n";
        for (int j = 0; j < numResources; j++) {
            need[numProcesses - 1][j] = maximum[numProcesses - 1][j] - allocation[numProcesses - 1][j];
            cout << need[numProcesses - 1][j] << " ";
        }
        cout << "\n";

        for (int i = 0; i < numProcesses - 1; i++) {
            if (need[i][numProcesses - 1] > 0) {
                waitForGraph[i][numProcesses - 1] = true;
            }
        }
    }

public:
    BankersAlgorithm(int p, int r)
        : numProcesses(p), numResources(r) {
        maximum.resize(numProcesses, vector<int>(numResources));
        allocation.resize(numProcesses, vector<int>(numResources));
        need.resize(numProcesses, vector<int>(numResources));
        available.resize(numResources);
        request.resize(numProcesses, vector<bool>(numResources, false));
        waitForGraph.resize(numProcesses, vector<bool>(numProcesses, false));
    }

    void generateRandomMatrices() {
        srand(time(0));

        cout << "Available Resources:\n";
        for (int i = 0; i < numResources; i++) {
            available[i] = rand() % 10;
            cout << available[i] << " ";
        }
        cout << "\n";

        cout << "Maximum Matrix:\n";
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                maximum[i][j] = rand() % 10;
                cout << maximum[i][j] << " ";
            }
            cout << "\n";
        }

        cout << "Allocation Matrix:\n";
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                allocation[i][j] = rand() % (maximum[i][j] + 1);
                cout << allocation[i][j] << " ";
            }
            cout << "\n";
        }

        cout << "Request Matrix:\n";
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                request[i][j] = rand() % 2;
                cout << request[i][j] << " ";
            }
            cout << "\n";
        }

        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                need[i][j] = maximum[i][j] - allocation[i][j];
            }
        }
        waitForGraph = vector<vector<bool>>(numProcesses, vector<bool>(numProcesses, false));
        for (int p = 0; p < numProcesses; p++) {
            for (int q = 0; q < numProcesses; q++) {
                if (request[p][q] && need[q][p] > 0) {
                    waitForGraph[p][q] = true;
                }
            }
        }
    }

    void runSafetyCheck() {
        vector<int> safeSequence;
        if (isSafe(safeSequence)) {
            cout << "\nSystem is in a SAFE state.\n";
            cout << "Safe sequence is: ";
            for (int i : safeSequence) {
                cout << "P" << i << " ";
            }
            cout << "\n";
        } else {
            cout << "\nSystem is in an UNSAFE state!\n";
            if (detectDeadlock()) {
                handleDeadlock();
            } else {
                cout << "No deadlock detected, but system is still unsafe.\n";
            }
        }
    }
};

int main() {
    int numProcesses, numResources;
    cout << "Enter the number of processes: ";
    cin >> numProcesses;
    cout << "Enter the number of resources: ";
    cin >> numResources;

    BankersAlgorithm ba(numProcesses, numResources);

    ba.generateRandomMatrices();
    ba.runSafetyCheck();

    return 0;
}
