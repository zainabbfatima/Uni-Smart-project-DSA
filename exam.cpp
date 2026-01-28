
#include <iostream>
#include <vector>
#include <string>


using namespace std;

class ExamSeating {
    // Singly Linked List Node
    struct SNode {
        string rollNo;
        int seat;
        SNode* next;
        SNode(string r, int s) : rollNo(r), seat(s), next(nullptr) {}
    };
    
    // Doubly Linked List Node for reorganization
    struct DNode {
        string rollNo;
        DNode* prev;
        DNode* next;
        DNode(string r) : rollNo(r), prev(nullptr), next(nullptr) {}
    };
    
    // BST Node for sorted seats
    struct BSTNode {
        int seat;
        string rollNo;
        BSTNode* left;
        BSTNode* right;
        BSTNode(int s, string r) : seat(s), rollNo(r), left(nullptr), right(nullptr) {}
    };
    
    // Hash Table entry
    struct HashEntry {
        string rollNo;
        int seat;
        HashEntry* next;
        HashEntry(string r, int s) : rollNo(r), seat(s), next(nullptr) {}
    };
    
    SNode* studentList;
    DNode* seatCircle;
    BSTNode* bstRoot;
    vector<HashEntry*> hashTable;
    int hashSize, totalSeats;
    
    int hashFunc(string roll) {
        int h = 0;
        for(char c : roll) h = (h * 31 + c) % hashSize;
        return h;
    }
    
    void insertBST(BSTNode*& root, int seat, string roll) {
        if(!root) { root = new BSTNode(seat, roll); return; }
        if(seat < root->seat) insertBST(root->left, seat, roll);
        else insertBST(root->right, seat, roll);
    }
    
    void inorderBST(BSTNode* root) {
        if(root) {
            inorderBST(root->left);
            cout << "Seat " << root->seat << ": " << root->rollNo << endl;
            inorderBST(root->right);
        }
    }
    
public:
    ExamSeating() : studentList(nullptr), seatCircle(nullptr), bstRoot(nullptr), 
                   hashSize(50), totalSeats(0) {
        hashTable.resize(hashSize, nullptr);
        cout << "✓ Exam seating system ready\n";
    }
    
    void addStudent(string rollNo) {
        int seat = ++totalSeats;
        
        // Add to singly linked list (at front)
        SNode* newStudent = new SNode(rollNo, seat);
        newStudent->next = studentList;
        studentList = newStudent;
        
        // Add to circular doubly linked list
        DNode* newSeat = new DNode(rollNo);
        if(!seatCircle) {
            seatCircle = newSeat;
            seatCircle->next = seatCircle->prev = seatCircle;
        } else {
            newSeat->next = seatCircle;
            newSeat->prev = seatCircle->prev;
            seatCircle->prev->next = newSeat;
            seatCircle->prev = newSeat;
        }
        
        // Add to hash table
        int idx = hashFunc(rollNo);
        HashEntry* newEntry = new HashEntry(rollNo, seat);
        newEntry->next = hashTable[idx];
        hashTable[idx] = newEntry;
        
        // Add to BST
        insertBST(bstRoot, seat, rollNo);
        
        cout << "✓ Student " << rollNo << " → Seat " << seat << endl;
    }
    
    void rotateSeats(int steps) {
        if(!seatCircle) { 
            cout << "No students!\n"; 
            return; 
        }
        
        for(int i = 0; i < steps; i++) 
            seatCircle = seatCircle->next;
        
        cout << "✓ Seats rotated by " << steps << " positions\n";
        cout << "First seat now: " << seatCircle->rollNo << endl;
    }
    
    int findSeat(string rollNo) {
        int idx = hashFunc(rollNo);
        HashEntry* curr = hashTable[idx];
        
        while(curr) {
            if(curr->rollNo == rollNo) return curr->seat;
            curr = curr->next;
        }
        return -1;
    }
    
    void displaySeating() {
        cout << "\n══════════════════════════════\n";
        cout << "      EXAM SEATING PLAN\n";
        cout << "══════════════════════════════\n";
        
        if(!bstRoot) { 
            cout << "No students registered\n"; 
            return; 
        }
        inorderBST(bstRoot);
        cout << "══════════════════════════════\n";
    }
    
    void displayCircularList() {
        if(!seatCircle) { 
            cout << "No students!\n"; 
            return; 
        }
        
        cout << "\nCircular seating order:\n";
        DNode* curr = seatCircle;
        do {
            cout << curr->rollNo << " ";
            curr = curr->next;
        } while(curr != seatCircle);
        cout << endl;
    }
    
    void runExamSystem() {
        cout << "\n🎓 EXAM SEATING SYSTEM\n";
        
        while(true) {
            cout << "\n1. Add Student\n2. Display Plan\n3. Find Seat\n";
            cout << "4. Rotate Seats\n5. Circular View\n6. Back to Main Menu\nChoice: ";
            
            int choice; 
            cin >> choice; 
            cin.ignore();
            
            if(choice == 1) {
                string roll;
                cout << "Roll Number: "; 
                getline(cin, roll);
                addStudent(roll);
            }
            else if(choice == 2) {
                displaySeating();
            }
            else if(choice == 3) {
                string roll;
                cout << "Roll to find: "; 
                getline(cin, roll);
                int seat = findSeat(roll);
                if(seat != -1) 
                    cout << "Seat: " << seat << endl;
                else 
                    cout << "Student not found!\n";
            }
            else if(choice == 4) {
                int steps;
                cout << "Rotate by: "; 
                cin >> steps;
                rotateSeats(steps);
            }
            else if(choice == 5) {
                displayCircularList();
            }
            else if(choice == 6) {
                cout << "Returning to main menu...\n";
                break;
            }
            else {
                cout << "Invalid choice! Please enter 1-6.\n";
            }
        }
    }
};