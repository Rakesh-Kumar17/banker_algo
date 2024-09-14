#include <iostream>
#include <vector>
#include <cstdlib> 
#include <ctime>    

class BankersAlgorithm {
private:
    int numProcesses;
    int numResources;
    std::vector<std::vector<int>> maximum;
    std::vector<std::vector<int>> allocation;
    std::vector<std::vector<int>> need;
    std::vector<int> available;

    void displayMatrices() {
        // std::cout << "Available Resources: ";
        // for (int i = 0; i < numResources; i++) {
        //     std::cout << available[i] << " ";
        // }
        std::cout << "\n";

        std::cout << "Maximum Matrix:\n";
        for (const auto& row : maximum) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }

        std::cout << "Allocation Matrix:\n";
        for (const auto& row : allocation) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }

        std::cout << "Need Matrix:\n";
        for (const auto& row : need) {
            for (int val : row) {
                std::cout << val << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }
    bool isSafe(std::vector<int>& safeSequence) {
        std::vector<int> work = available;
        std::vector<bool> finish(numProcesses, false);
        safeSequence.clear();

        std::cout << "\nInitial state:\n";
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
                        std::cout << "Process P" << p << " has been allocated. Current Available: ";
                        for (int m = 0; m < numResources; m++) {
                            std::cout << work[m] << " ";
                        }
                        std::cout << "\n";
                        
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

public:
    BankersAlgorithm(int p, int r)
        : numProcesses(p), numResources(r) {
        maximum.resize(numProcesses, std::vector<int>(numResources));
        allocation.resize(numProcesses, std::vector<int>(numResources));
        need.resize(numProcesses, std::vector<int>(numResources));
        available.resize(numResources);
    }

    void generateRandomMatrices() {
      
        std::srand(std::time(0));

       
        std::cout << "Available Resources:\n";
        for (int i = 0; i < numResources; i++) {
            available[i] = std::rand() % 10;
            std::cout << available[i] << " ";
        }
        std::cout << "\n";

        std::cout << "Maximum Matrix:\n";
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                maximum[i][j] = std::rand() % 10;
                std::cout << maximum[i][j] << " ";
            }
            std::cout << "\n";
        }

        std::cout << "Allocation Matrix:\n";
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                allocation[i][j] = std::rand() % (maximum[i][j] + 1); 
                std::cout << allocation[i][j] << " ";
            }
            std::cout << "\n";
        }

       
        for (int i = 0; i < numProcesses; i++) {
            for (int j = 0; j < numResources; j++) {
                need[i][j] = maximum[i][j] - allocation[i][j];
            }
        }
    }

    void runSafetyCheck() {
        std::vector<int> safeSequence;
        if (isSafe(safeSequence)) {
            std::cout << "\nSystem is in a SAFE state.\n";
            std::cout << "Safe sequence is: ";
            for (int i : safeSequence) {
                std::cout << "P" << i << " ";
            }
            std::cout << "\n";
        } else {
            std::cout << "\nSystem is in an UNSAFE state!\n";
        }
    }
};

int main() {
    int numProcesses, numResources;
    std::cout << "Enter the number of processes: ";
    std::cin >> numProcesses;
    std::cout << "Enter the number of resources: ";
    std::cin >> numResources;

    BankersAlgorithm ba(numProcesses, numResources);

    ba.generateRandomMatrices();
    ba.runSafetyCheck();

    return 0;
}