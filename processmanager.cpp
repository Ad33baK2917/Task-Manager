#include <iostream>
#include <windows.h>
#include <tlhelp32.h>

using namespace std;

void displayProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        cout << "Failed to take a snapshot of the processes." << endl;
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32)) {
        cout << "Failed to retrieve the first process." << endl;
        CloseHandle(hProcessSnap);
        return;
    }

    do {
        wcout << L"PID: " << pe32.th32ProcessID << L"\tProcess Name: " << pe32.szExeFile << endl;
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
}

void killProcess(DWORD pid) {
    HANDLE hProcess = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
    if (hProcess == NULL) {
        cout << "Failed to open process with PID: " << pid << endl;
        return;
    }

    if (!TerminateProcess(hProcess, 0)) {
        cout << "Failed to terminate process with PID: " << pid << endl;
    } else {
        cout << "Process " << pid << " has been terminated." << endl;
    }

    CloseHandle(hProcess);
}

int main() {
    int choice;
    DWORD pid;

    while (true) {
        cout << "\nWindows Task Manager\n";
        cout << "1. Display Running Processes\n";
        cout << "2. Kill Process by PID\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                displayProcesses();
                break;
            case 2:
                cout << "Enter PID to kill: ";
                cin >> pid;
                killProcess(pid);
                break;
            case 3:
                cout << "Exiting...\n";
                return 0;
            default:
                cout << "Invalid choice, please try again.\n";
        }
    }

    return 0;
}
