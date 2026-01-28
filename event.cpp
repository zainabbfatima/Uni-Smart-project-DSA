
#include <iostream>
#include <vector>
#include <string>
#include <cctype>


using namespace std;

class EventRegistration {
    struct Participant {
        int id;
        string name;
        string rollNo;      // Student roll number (245111, 245113, etc.)
        string department;
        string eventType;   // "TECH", "CULTURAL", "SPORTS", "WORKSHOP"
        
        Participant(int i, string n, string r, string d, string e)
            : id(i), name(n), rollNo(r), department(d), eventType(e) {}
    };
    
    // BST Node for sorted storage (by ID)
    struct BSTNode {
        Participant* data;
        BSTNode* left;
        BSTNode* right;
        
        BSTNode(Participant* p) : data(p), left(nullptr), right(nullptr) {}
    };
    
    // Linked List Node for registration order
    struct ListNode {
        Participant* data;
        ListNode* next;
        
        ListNode(Participant* p) : data(p), next(nullptr) {}
    };
    
    BSTNode* bstRoot;
    ListNode* head;
    ListNode* tail;
    int nextId;
    int participantCount;
    
    // BST Operations
    BSTNode* insertBST(BSTNode* root, Participant* part) {
        if(!root) return new BSTNode(part);
        
        if(part->id < root->data->id)
            root->left = insertBST(root->left, part);
        else
            root->right = insertBST(root->right, part);
        
        return root;
    }
    
    BSTNode* searchBST(BSTNode* root, int id) {
        if(!root || root->data->id == id) return root;
        
        if(id < root->data->id)
            return searchBST(root->left, id);
        else
            return searchBST(root->right, id);
    }
    
    BSTNode* findMin(BSTNode* root) {
        while(root && root->left) root = root->left;
        return root;
    }
    
    BSTNode* deleteBST(BSTNode* root, int id) {
        if(!root) return root;
        
        if(id < root->data->id)
            root->left = deleteBST(root->left, id);
        else if(id > root->data->id)
            root->right = deleteBST(root->right, id);
        else {
            // Node found
            if(!root->left) {
                BSTNode* temp = root->right;
                delete root;
                return temp;
            }
            else if(!root->right) {
                BSTNode* temp = root->left;
                delete root;
                return temp;
            }
            
            // Node with two children
            BSTNode* temp = findMin(root->right);
            root->data = temp->data;
            root->right = deleteBST(root->right, temp->data->id);
        }
        return root;
    }
    
    void inorderBST(BSTNode* root) {
        if(root) {
            inorderBST(root->left);
            cout << "ID: " << root->data->id << " | " << root->data->name;
            cout << " | Roll: " << root->data->rollNo;
            cout << " | Dept: " << root->data->department;
            cout << " | Event: " << root->data->eventType << endl;
            inorderBST(root->right);
        }
    }
    
    // Linked List Operations
    void addToList(Participant* part) {
        ListNode* newNode = new ListNode(part);
        
        if(!head) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
    }
    
    void removeFromList(int id) {
        if(!head) return;
        
        if(head->data->id == id) {
            ListNode* temp = head;
            head = head->next;
            if(!head) tail = nullptr;
            delete temp;
            return;
        }
        
        ListNode* prev = head;
        ListNode* curr = head->next;
        
        while(curr) {
            if(curr->data->id == id) {
                prev->next = curr->next;
                if(!curr->next) tail = prev;
                delete curr;
                return;
            }
            prev = curr;
            curr = curr->next;
        }
    }
    
public:
    EventRegistration() : bstRoot(nullptr), head(nullptr), tail(nullptr), 
                         nextId(5001), participantCount(0) {
        cout << "✓ Event Registration System Ready\n";
    }
    
    void registerParticipant() {
        cout << "\n--- EVENT REGISTRATION ---\n";
        
        string name, roll, dept, eventType;
        cout << "Full Name: "; 
        getline(cin, name);
        
        cout << "Roll Number: "; 
        getline(cin, roll);
        
        cout << "Department: "; 
        getline(cin, dept);
        
        cout << "Event Type (TECH/CULTURAL/SPORTS/WORKSHOP): "; 
        getline(cin, eventType);
        
        // Convert to uppercase for consistency
        for(char& c : eventType) c = toupper(c);
        
        Participant* newPart = new Participant(nextId++, name, roll, dept, eventType);
        
        // Add to BST (for sorted access)
        bstRoot = insertBST(bstRoot, newPart);
        
        // Add to Linked List (for registration order)
        addToList(newPart);
        
        participantCount++;
        
        cout << "\n✓ Registration Successful!\n";
        cout << "  Registration ID: " << newPart->id << "\n";
        cout << "  Name: " << newPart->name << "\n";
        cout << "  Event: " << newPart->eventType << "\n";
    }
    
    void searchParticipant() {
        int id;
        cout << "Enter Registration ID to search: "; 
        cin >> id; 
        cin.ignore();
        
        BSTNode* found = searchBST(bstRoot, id);
        if(found) {
            Participant* p = found->data;
            cout << "\n✓ PARTICIPANT FOUND:\n";
            cout << "  ID: " << p->id << "\n";
            cout << "  Name: " << p->name << "\n";
            cout << "  Roll No: " << p->rollNo << "\n";
            cout << "  Department: " << p->department << "\n";
            cout << "  Event: " << p->eventType << "\n";
        } else {
            cout << "✗ Participant not found!\n";
        }
    }
    
    void cancelRegistration() {
        int id;
        cout << "Enter Registration ID to cancel: "; 
        cin >> id; 
        cin.ignore();
        
        BSTNode* found = searchBST(bstRoot, id);
        if(found) {
            // Remove from BST
            bstRoot = deleteBST(bstRoot, id);
            
            // Remove from Linked List
            removeFromList(id);
            
            participantCount--;
            cout << "✓ Registration #" << id << " cancelled successfully\n";
        } else {
            cout << "✗ Registration ID not found!\n";
        }
    }
    
    void displaySorted() {
        cout << "\n══════════════════════════════\n";
        cout << "  PARTICIPANTS (Sorted by ID)\n";
        cout << "══════════════════════════════\n";
        
        if(!bstRoot) {
            cout << "No participants registered.\n";
        } else {
            inorderBST(bstRoot);
        }
        cout << "Total: " << participantCount << " participants\n";
        cout << "══════════════════════════════\n";
    }
    
    void displayByRegistrationOrder() {
        cout << "\n══════════════════════════════\n";
        cout << "  REGISTRATION ORDER\n";
        cout << "══════════════════════════════\n";
        
        if(!head) {
            cout << "No participants registered.\n";
        } else {
            ListNode* curr = head;
            int count = 1;
            while(curr) {
                cout << count++ << ". ID: " << curr->data->id;
                cout << " | " << curr->data->name;
                cout << " (" << curr->data->rollNo << ")";
                cout << " | " << curr->data->eventType << " event\n";
                curr = curr->next;
            }
        }
        cout << "══════════════════════════════\n";
    }
    
    void showEventStatistics() {
        vector<string> events = {"TECH", "CULTURAL", "SPORTS", "WORKSHOP"};
        vector<int> counts(4, 0);
        
        // Count participants by event type
        ListNode* curr = head;
        while(curr) {
            string type = curr->data->eventType;
            for(int i = 0; i < 4; i++) {
                if(type == events[i]) {
                    counts[i]++;
                    break;
                }
            }
            curr = curr->next;
        }
        
        cout << "\n══════════════════════════════\n";
        cout << "      EVENT STATISTICS\n";
        cout << "══════════════════════════════\n";
        cout << "Total Participants: " << participantCount << "\n\n";
        
        for(int i = 0; i < 4; i++) {
            cout << events[i] << ": " << counts[i] << " participants";
            if(participantCount > 0) {
                int percent = (counts[i] * 100) / participantCount;
                cout << " (" << percent << "%)";
            }
            cout << "\n";
        }
        cout << "══════════════════════════════\n";
    }
    
    void runEventSystem() {
        cout << "\n🎪 EVENT REGISTRATION SYSTEM\n";
        
        while(true) {
            cout << "\n1. Register Participant\n2. Search Participant\n";
            cout << "3. Cancel Registration\n4. View All (Sorted by ID)\n";
            cout << "5. View Registration Order\n6. Event Statistics\n";
            cout << "7. Back to Main Menu\nChoice: ";
            
            int choice; 
            cin >> choice; 
            cin.ignore();
            
            if(choice == 1) {
                registerParticipant();
            }
            else if(choice == 2) {
                searchParticipant();
            }
            else if(choice == 3) {
                cancelRegistration();
            }
            else if(choice == 4) {
                displaySorted();
            }
            else if(choice == 5) {
                displayByRegistrationOrder();
            }
            else if(choice == 6) {
                showEventStatistics();
            }
            else if(choice == 7) {
                cout << "Returning to main menu...\n";
                break;
            }
            else {
                cout << "Invalid choice! Please enter 1-7.\n";
            }
        }
    }
};