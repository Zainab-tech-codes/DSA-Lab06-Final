#include <iostream>
#include <string>
#include <utility>
using namespace std;

// ========================================
// Book Node
// ========================================
struct BookNode
{
    int bookID;
    string title;
    BookNode* next;
};

// ========================================
// Circular Linked List Class
// head points to the last node (pivot)
// head->next points to the first node
// ========================================
class CircularLinkedList
{
private:
    BookNode* head;
    int count;

    // Get node at an index
    BookNode* GetNodeAt(int index)
    {
        if (head == nullptr)
            return nullptr;

        BookNode* current = head->next;

        for (int i = 0; i < index; i++)
            current = current->next;

        return current;
    }

    // Swap the data of two nodes
    void SwapData(BookNode* a, BookNode* b)
    {
        swap(a->bookID, b->bookID);
        swap(a->title, b->title);
    }

    // Normalize a circular index to 0..count-1
    int NormalizeIndex(int index)
    {
        if (count == 0) return 0;
        return ((index % count) + count) % count;
    }

public:
    // Constructor
    CircularLinkedList()
    {
        head = nullptr;
        count = 0;
    }

    // Check whether the list is empty
    bool IsEmpty()
    {
        return count == 0;
    }

    bool IsNotEmpty()
    {
        return count > 0;
    }

    // ====================================
    // Insert at node next to head (front)
    // ====================================
    void Insert(BookNode* book)
    {
        if (book == nullptr)
            return;

        if (head == nullptr)
        {
            head = book;
            book->next = book;
        }
        else
        {
            book->next = head->next;
            head->next = book;
        }

        count++;
    }

    // ====================================
    // Remove node next to head (front)
    // ====================================
    BookNode* Remove()
    {
        if (IsEmpty())
            return nullptr;

        BookNode* first = head->next;

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

    // ====================================
    // Insert at an index
    // ====================================
    void InsertAt(BookNode* book, int index)
    {
        if (book == nullptr)
            return;

        if (count == 0)
        {
            head = book;
            book->next = book;
            count++;
            return;
        }

        // Negative indexes wrap around the circle
        if (index < 0)
            index = NormalizeIndex(index);

        // Index greater than count wraps around
        if (index > count)
            index %= count;

        // Insert at the beginning
        if (index == 0)
        {
            Insert(book);
            return;
        }

        // Insert at the end
        if (index == count)
        {
            book->next = head->next;
            head->next = book;
            head = book;
            count++;
            return;
        }

        // Insert in the middle
        BookNode* previous = GetNodeAt(index - 1);

        book->next = previous->next;
        previous->next = book;

        count++;
    }

    // ====================================
    // Remove at an index
    // ====================================
    BookNode* RemoveAt(int index)
    {
        if (IsEmpty())
            return nullptr;

        index = NormalizeIndex(index);

        if (index == 0)
            return Remove();

        BookNode* previous = GetNodeAt(index - 1);
        BookNode* target = previous->next;

        previous->next = target->next;

        // Update head if removing the last node
        if (target == head)
            head = previous;

        target->next = nullptr;
        count--;

        return target;
    }

    // ====================================
    // Swap data at two indexes
    // ====================================
    void Swap(int index1, int index2)
    {
        if (IsEmpty())
            return;

        index1 = NormalizeIndex(index1);
        index2 = NormalizeIndex(index2);

        if (index1 == index2)
            return;

        BookNode* first = GetNodeAt(index1);
        BookNode* second = GetNodeAt(index2);

        SwapData(first, second);
    }

    // ====================================
    // Sort by book ID (ascending)
    // ====================================
    void Sort()
    {
        if (count < 2)
            return;

        BookNode* current = head->next;

        for (int i = 0; i < count - 1; i++)
        {
            BookNode* nextNode = current->next;

            for (int j = i + 1; j < count; j++)
            {
                if (current->bookID > nextNode->bookID)
                    SwapData(current, nextNode);

                nextNode = nextNode->next;
            }

            current = current->next;
        }
    }

    // ====================================
    // Reverse the circular linked list
    // ====================================
    void Reverse()
    {
        if (count < 2)
            return;

        BookNode* oldTail = head;
        BookNode* oldFirst = head->next;

        BookNode* previous = oldTail;
        BookNode* current = oldFirst;

        while (current != oldTail)
        {
            BookNode* nextNode = current->next;

            current->next = previous;

            previous = current;
            current = nextNode;
        }

        oldTail->next = previous;

        // Old first becomes the new last node
        head = oldFirst;
    }

    // ====================================
    // Print exactly count nodes
    // ====================================
    void Print()
    {
        if (IsEmpty())
        {
            cout << "No books in the collection.\n";
            return;
        }

        BookNode* current = head->next;

        cout << "\nIndex\tBook ID\tTitle\n";
        cout << "-----------------------------------\n";

        for (int i = 0; i < count; i++)
        {
            cout << i << "\t"
                 << current->bookID << "\t"
                 << current->title << endl;

            current = current->next;
        }

        cout << "Total books: " << count << endl;
    }

    // ====================================
    // Move to Next Book (circular traversal demo)
    // ====================================
    void MoveToNext()
    {
        if (IsEmpty())
        {
            cout << "Collection is empty. Nothing to move.\n";
            return;
        }

        // Rotate: make the current first book the new last book
        head = head->next;

        cout << "Moved to next book. New first book is: "
             << head->next->title << " (ID: " << head->next->bookID << ")\n";
    }

    // ====================================
    // Check Collection Status
    // ====================================
    void CheckStatus()
    {
        if (IsEmpty())
        {
            cout << "Collection is EMPTY.\n";
        }
        else
        {
            cout << "Collection is NOT empty.\n";
            cout << "Total books: " << count << endl;
            cout << "First book : " << head->next->title
                 << " (ID: " << head->next->bookID << ")\n";
            cout << "Last book  : " << head->title
                 << " (ID: " << head->bookID << ")\n";
        }
    }

    // ====================================
    // Copy constructor
    // ====================================
    CircularLinkedList(const CircularLinkedList& other)
    {
        head = nullptr;
        count = 0;

        if (other.head == nullptr)
            return;

        BookNode* current = other.head->next;

        for (int i = 0; i < other.count; i++)
        {
            BookNode* copy = new BookNode{
                current->bookID,
                current->title,
                nullptr
            };

            InsertAt(copy, count);
            current = current->next;
        }
    }

    // ====================================
    // Assignment operator
    // ====================================
    CircularLinkedList& operator=(const CircularLinkedList& other)
    {
        if (this == &other)
            return *this;

        while (IsNotEmpty())
            delete Remove();

        if (other.head == nullptr)
            return *this;

        BookNode* current = other.head->next;

        for (int i = 0; i < other.count; i++)
        {
            BookNode* copy = new BookNode{
                current->bookID,
                current->title,
                nullptr
            };

            InsertAt(copy, count);
            current = current->next;
        }

        return *this;
    }

    // ====================================
    // Destructor
    // ====================================
    ~CircularLinkedList()
    {
        while (IsNotEmpty())
            delete Remove();
    }
};

// ========================================
// Create a Book Node
// ========================================
BookNode* CreateBook(int id, string title)
{
    BookNode* book = new BookNode;

    book->bookID = id;
    book->title = title;
    book->next = nullptr;

    return book;
}

// ========================================
// Helper: Get book details from user
// ========================================
BookNode* GetBookFromUser()
{
    int id;
    string title;

    cout << "Enter Book ID: ";
    cin >> id;
    cin.ignore();   // clear newline

    cout << "Enter Book Title: ";
    getline(cin, title);

    return CreateBook(id, title);
}

// ========================================
// Menu Display
// ========================================
void ShowMenu()
{
    cout << "\n========================================\n";
    cout << "  CIRCULAR LIBRARY BOOK DISPLAY SYSTEM\n";
    cout << "========================================\n";
    cout << "1.  Display All Books\n";
    cout << "2.  Insert Book at Head\n";
    cout << "3.  Remove Book from Head\n";
    cout << "4.  Insert Book at Index\n";
    cout << "5.  Remove Book at Index\n";
    cout << "6.  Swap Books\n";
    cout << "7.  Sort Books by ID\n";
    cout << "8.  Reverse Book Collection\n";
    cout << "9.  Move to Next Book\n";
    cout << "10. Check Collection Status\n";
    cout << "11. Exit\n";
    cout << "========================================\n";
    cout << "Enter your choice: ";
}

// ========================================
// Main: Menu-Driven Application
// ========================================
int main()
{
    CircularLinkedList library;
    int choice;

    // Optional: add a few starter books so the list is not empty at start
    library.InsertAt(CreateBook(103, "Operating Systems"), 0);
    library.InsertAt(CreateBook(101, "Data Structures"), 1);
    library.InsertAt(CreateBook(105, "Computer Networks"), 2);
    library.InsertAt(CreateBook(102, "Database Systems"), 3);
    library.InsertAt(CreateBook(104, "Software Engineering"), 4);

    do
    {
        ShowMenu();
        cin >> choice;

        switch (choice)
        {
        case 1: // Display All Books
        {
            cout << "\n--- Current Circular Collection ---";
            library.Print();
            break;
        }

        case 2: // Insert Book at Head
        {
            BookNode* book = GetBookFromUser();
            library.Insert(book);
            cout << "Book inserted at head (next to pivot).\n";
            break;
        }

        case 3: // Remove Book from Head
        {
            BookNode* removed = library.Remove();
            if (removed != nullptr)
            {
                cout << "Removed from head: " << removed->title
                     << " (ID: " << removed->bookID << ")\n";
                delete removed;
            }
            else
            {
                cout << "Collection is empty. Nothing to remove.\n";
            }
            break;
        }

        case 4: // Insert Book at Index
        {
            int index;
            cout << "Enter index to insert at: ";
            cin >> index;

            BookNode* book = GetBookFromUser();
            library.InsertAt(book, index);
            cout << "Book inserted at index " << index << ".\n";
            break;
        }

        case 5: // Remove Book at Index
        {
            int index;
            cout << "Enter index to remove: ";
            cin >> index;

            BookNode* removed = library.RemoveAt(index);
            if (removed != nullptr)
            {
                cout << "Removed: " << removed->title
                     << " (ID: " << removed->bookID << ")\n";
                delete removed;
            }
            else
            {
                cout << "Collection is empty or invalid index.\n";
            }
            break;
        }

        case 6: // Swap Books
        {
            int i1, i2;
            cout << "Enter first index: ";
            cin >> i1;
            cout << "Enter second index: ";
            cin >> i2;

            library.Swap(i1, i2);
            cout << "Books at indexes " << i1 << " and " << i2 << " swapped.\n";
            break;
        }

        case 7: // Sort Books by ID
        {
            library.Sort();
            cout << "Collection sorted by Book ID (ascending).\n";
            break;
        }

        case 8: // Reverse Book Collection
        {
            library.Reverse();
            cout << "Collection reversed.\n";
            break;
        }

        case 9: // Move to Next Book
        {
            library.MoveToNext();
            break;
        }

        case 10: // Check Collection Status
        {
            library.CheckStatus();
            break;
        }

        case 11: // Exit
        {
            cout << "Exiting program. Goodbye!\n";
            break;
        }

        default:
            cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 11);

    return 0;
}
