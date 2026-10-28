#include <iostream>
#include <string>
#include <utility>
using namespace std;

// ========================================
// Traffic Phase Node
// ========================================
struct TrafficPhase
{
    int phaseID;
    string direction;   // North, East, South, West, etc.
    string signal;      // Green, Yellow, Red
    int duration;       // seconds
    TrafficPhase* next;
};

// ========================================
// Circular Linked List for Traffic Phases
// head points to the last node (pivot)
// head->next points to the first phase
// ========================================
class CircularTrafficList
{
private:
    TrafficPhase* head;
    int count;

    TrafficPhase* GetNodeAt(int index)
    {
        if (head == nullptr)
            return nullptr;

        TrafficPhase* current = head->next;
        for (int i = 0; i < index; i++)
            current = current->next;
        return current;
    }

    void SwapData(TrafficPhase* a, TrafficPhase* b)
    {
        swap(a->phaseID, b->phaseID);
        swap(a->direction, b->direction);
        swap(a->signal, b->signal);
        swap(a->duration, b->duration);
    }

    int NormalizeIndex(int index)
    {
        if (count == 0) return 0;
        return ((index % count) + count) % count;
    }

public:
    CircularTrafficList()
    {
        head = nullptr;
        count = 0;
    }

    bool IsEmpty()
    {
        return count == 0;
    }

    bool IsNotEmpty()
    {
        return count > 0;
    }

    // Insert at front (next to head)
    void Insert(TrafficPhase* phase)
    {
        if (phase == nullptr)
            return;

        if (head == nullptr)
        {
            head = phase;
            phase->next = phase;
        }
        else
        {
            phase->next = head->next;
            head->next = phase;
        }
        count++;
    }

    // Remove from front (next to head)
    TrafficPhase* Remove()
    {
        if (IsEmpty())
            return nullptr;

        TrafficPhase* first = head->next;

        if (first == head)
        {
            head = nullptr;
        }
        else
        {
            head->next = first->next;
        }

        first->next = nullptr;
        count--;
        return first;
    }

    // Insert at any index
    void InsertAt(TrafficPhase* phase, int index)
    {
        if (phase == nullptr)
            return;

        if (count == 0)
        {
            head = phase;
            phase->next = phase;
            count++;
            return;
        }

        if (index < 0)
            index = NormalizeIndex(index);

        if (index > count)
            index %= count;

        if (index == 0)
        {
            Insert(phase);
            return;
        }

        if (index == count)
        {
            phase->next = head->next;
            head->next = phase;
            head = phase;
            count++;
            return;
        }

        TrafficPhase* previous = GetNodeAt(index - 1);
        phase->next = previous->next;
        previous->next = phase;
        count++;
    }

    // Remove at any index
    TrafficPhase* RemoveAt(int index)
    {
        if (IsEmpty())
            return nullptr;

        index = NormalizeIndex(index);

        if (index == 0)
            return Remove();

        TrafficPhase* previous = GetNodeAt(index - 1);
        TrafficPhase* target = previous->next;

        previous->next = target->next;

        if (target == head)
            head = previous;

        target->next = nullptr;
        count--;
        return target;
    }

    // Swap two phases
    void Swap(int index1, int index2)
    {
        if (IsEmpty())
            return;

        index1 = NormalizeIndex(index1);
        index2 = NormalizeIndex(index2);

        if (index1 == index2)
            return;

        TrafficPhase* first = GetNodeAt(index1);
        TrafficPhase* second = GetNodeAt(index2);
        SwapData(first, second);
    }

    // Sort by Phase ID (ascending)
    void Sort()
    {
        if (count < 2)
            return;

        TrafficPhase* current = head->next;

        for (int i = 0; i < count - 1; i++)
        {
            TrafficPhase* nextNode = current->next;

            for (int j = i + 1; j < count; j++)
            {
                if (current->phaseID > nextNode->phaseID)
                    SwapData(current, nextNode);

                nextNode = nextNode->next;
            }
            current = current->next;
        }
    }

    // Reverse the cycle order
    void Reverse()
    {
        if (count < 2)
            return;

        TrafficPhase* oldTail = head;
        TrafficPhase* oldFirst = head->next;

        TrafficPhase* previous = oldTail;
        TrafficPhase* current = oldFirst;

        while (current != oldTail)
        {
            TrafficPhase* nextNode = current->next;
            current->next = previous;
            previous = current;
            current = nextNode;
        }

        oldTail->next = previous;
        head = oldFirst;
    }

    // Print all phases (one full cycle)
    void Print()
    {
        if (IsEmpty())
        {
            cout << "No traffic phases configured.\n";
            return;
        }

        TrafficPhase* current = head->next;

        cout << "\nIndex\tID\tDirection\tSignal\tDuration(s)\n";
        cout << "-----------------------------------------------------\n";

        for (int i = 0; i < count; i++)
        {
            cout << i << "\t"
                 << current->phaseID << "\t"
                 << current->direction << "\t\t"
                 << current->signal << "\t"
                 << current->duration << endl;

            current = current->next;
        }

        cout << "Total phases: " << count << endl;
        cout << "(Last phase links back to first phase - Circular)\n";
    }

    // Move to next phase (simulate traffic light rotation)
    void MoveToNext()
    {
        if (IsEmpty())
        {
            cout << "No phases available.\n";
            return;
        }

        head = head->next;   // rotate the pivot

        cout << "\n>>> Now Active Phase: "
             << head->next->direction << " - "
             << head->next->signal
             << " (" << head->next->duration << " seconds)\n";
    }

    // Show current status
    void CheckStatus()
    {
        if (IsEmpty())
        {
            cout << "Traffic system is EMPTY. No phases configured.\n";
            return;
        }

        cout << "\n--- Traffic System Status ---\n";
        cout << "Total phases      : " << count << endl;
        cout << "Current first     : " << head->next->direction
             << " (" << head->next->signal << ")\n";
        cout << "Current last      : " << head->direction
             << " (" << head->signal << ")\n";
        cout << "System is Circular: Yes (last links to first)\n";
    }

    // Copy constructor
    CircularTrafficList(const CircularTrafficList& other)
    {
        head = nullptr;
        count = 0;

        if (other.head == nullptr)
            return;

        TrafficPhase* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            TrafficPhase* copy = new TrafficPhase{
                current->phaseID,
                current->direction,
                current->signal,
                current->duration,
                nullptr
            };
            InsertAt(copy, count);
            current = current->next;
        }
    }

    // Assignment operator
    CircularTrafficList& operator=(const CircularTrafficList& other)
    {
        if (this == &other)
            return *this;

        while (IsNotEmpty())
            delete Remove();

        if (other.head == nullptr)
            return *this;

        TrafficPhase* current = other.head->next;
        for (int i = 0; i < other.count; i++)
        {
            TrafficPhase* copy = new TrafficPhase{
                current->phaseID,
                current->direction,
                current->signal,
                current->duration,
                nullptr
            };
            InsertAt(copy, count);
            current = current->next;
        }
        return *this;
    }

    // Destructor
    ~CircularTrafficList()
    {
        while (IsNotEmpty())
            delete Remove();
    }
};

// ========================================
// Helper: Create a new phase
// ========================================
TrafficPhase* CreatePhase(int id, string direction, string signal, int duration)
{
    TrafficPhase* phase = new TrafficPhase;
    phase->phaseID = id;
    phase->direction = direction;
    phase->signal = signal;
    phase->duration = duration;
    phase->next = nullptr;
    return phase;
}

// ========================================
// Helper: Get phase details from user
// ========================================
TrafficPhase* GetPhaseFromUser()
{
    int id, duration;
    string direction, signal;

    cout << "Enter Phase ID: ";
    cin >> id;
    cin.ignore();

    cout << "Enter Direction (e.g. North, East, South, West): ";
    getline(cin, direction);

    cout << "Enter Signal (Green / Yellow / Red): ";
    getline(cin, signal);

    cout << "Enter Duration in seconds: ";
    cin >> duration;

    return CreatePhase(id, direction, signal, duration);
}

// ========================================
// Menu
// ========================================
void ShowMenu()
{
    cout << "\n================================================\n";
    cout << "     TRAFFIC MANAGEMENT SYSTEM (Circular List)\n";
    cout << "================================================\n";
    cout << "1.  Display All Traffic Phases\n";
    cout << "2.  Insert Phase at Head\n";
    cout << "3.  Remove Phase from Head\n";
    cout << "4.  Insert Phase at Index\n";
    cout << "5.  Remove Phase at Index\n";
    cout << "6.  Swap Two Phases\n";
    cout << "7.  Sort Phases by ID\n";
    cout << "8.  Reverse Phase Order\n";
    cout << "9.  Move to Next Phase (Rotate)\n";
    cout << "10. Check System Status\n";
    cout << "11. Exit\n";
    cout << "================================================\n";
    cout << "Enter your choice: ";
}

// ========================================
// Main Program
// ========================================
int main()
{
    CircularTrafficList traffic;

    // Initial 4-way intersection phases
    traffic.InsertAt(CreatePhase(1, "North", "Green", 30), 0);
    traffic.InsertAt(CreatePhase(2, "East",  "Green", 25), 1);
    traffic.InsertAt(CreatePhase(3, "South", "Green", 30), 2);
    traffic.InsertAt(CreatePhase(4, "West",  "Green", 25), 3);

    int choice;

    do
    {
        ShowMenu();
        cin >> choice;

        switch (choice)
        {
        case 1:
            cout << "\n--- Current Traffic Phases (One Full Cycle) ---";
            traffic.Print();
            break;

        case 2:
        {
            TrafficPhase* phase = GetPhaseFromUser();
            traffic.Insert(phase);
            cout << "Phase inserted at head.\n";
            break;
        }

        case 3:
        {
            TrafficPhase* removed = traffic.Remove();
            if (removed != nullptr)
            {
                cout << "Removed: " << removed->direction
                     << " (" << removed->signal << ")\n";
                delete removed;
            }
            else
            {
                cout << "System is empty.\n";
            }
            break;
        }

        case 4:
        {
            int index;
            cout << "Enter index: ";
            cin >> index;
            TrafficPhase* phase = GetPhaseFromUser();
            traffic.InsertAt(phase, index);
            cout << "Phase inserted at index " << index << ".\n";
            break;
        }

        case 5:
        {
            int index;
            cout << "Enter index to remove: ";
            cin >> index;
            TrafficPhase* removed = traffic.RemoveAt(index);
            if (removed != nullptr)
            {
                cout << "Removed: " << removed->direction
                     << " (" << removed->signal << ")\n";
                delete removed;
            }
            else
            {
                cout << "Nothing to remove.\n";
            }
            break;
        }

        case 6:
        {
            int i1, i2;
            cout << "Enter first index: ";
            cin >> i1;
            cout << "Enter second index: ";
            cin >> i2;
            traffic.Swap(i1, i2);
            cout << "Phases swapped.\n";
            break;
        }

        case 7:
            traffic.Sort();
            cout << "Phases sorted by ID.\n";
            break;

        case 8:
            traffic.Reverse();
            cout << "Phase order reversed.\n";
            break;

        case 9:
            traffic.MoveToNext();
            break;

        case 10:
            traffic.CheckStatus();
            break;

        case 11:
            cout << "Exiting Traffic Management System. Goodbye!\n";
            break;

        default:
            cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 11);

    return 0;
}