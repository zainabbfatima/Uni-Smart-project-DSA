
#include <iostream>
#include <vector>
#include <string>


using namespace std;

class ComplaintSystem {
    struct Complaint {
        int id;
        string studentName;
        string rollNo;      // Student roll number
        string department;
        string category;    // "ACADEMIC", "INFRASTRUCTURE", "ADMIN", "OTHER"
        string description;
        string status;      // "PENDING", "IN PROGRESS", "RESOLVED"
        
        Complaint(int i, string name, string roll, string dept, string cat, string desc)
            : id(i), studentName(name), rollNo(roll), department(dept), 
              category(cat), description(desc), status("PENDING") {}
    };
    
    // Circular Queue for complaints (FIFO)
    vector<Complaint*> complaintQueue;
    int front, rear, capacity;
    int nextId;
    
    // Stack for undo operations
    vector<Complaint*> undoStack;
    
    // Statistics
    int pendingCount, resolvedCount;
    
    bool isFull() { return (rear + 1) % capacity == front; }
    bool isEmpty() { return front == rear; }
    
public:
    ComplaintSystem(int cap = 50) : front(0), rear(0), capacity(cap), 
                                  nextId(1001), pendingCount(0), resolvedCount(0) {
        complaintQueue.resize(capacity, nullptr);
        cout << "✓ Complaint System Ready (Capacity: " << capacity << " complaints)\n";
    }
    
    void submitComplaint() {
        if(isFull()) {
            cout << "✗ Complaint queue is full! Cannot submit more.\n";
            return;
        }
        
        cout << "\n--- SUBMIT COMPLAINT ---\n";
        string name, roll, dept, category, desc;
        
        cout << "Your Name: "; 
        getline(cin, name);
        
        cout << "Roll Number: "; 
        getline(cin, roll);
        
        cout << "Department: "; 
        getline(cin, dept);
        
        cout << "Category (ACADEMIC/INFRASTRUCTURE/ADMIN/OTHER): "; 
        getline(cin, category);
        
        cout << "Describe your issue: "; 
        getline(cin, desc);
        
        Complaint* newComplaint = new Complaint(nextId++, name, roll, dept, category, desc);
        complaintQueue[rear] = newComplaint;
        rear = (rear + 1) % capacity;
        
        undoStack.push_back(newComplaint);
        pendingCount++;
        
        cout << "\n✓ Complaint #" << newComplaint->id << " submitted successfully!\n";
        cout << "  Status: " << newComplaint->status << "\n";
        cout << "  Position in queue: " << pendingCount << "\n";
    }
    
    void processNextComplaint() {
        if(isEmpty()) {
            cout << "No complaints to process.\n";
            return;
        }
        
        Complaint* complaint = complaintQueue[front];
        cout << "\n══════════════════════════════\n";
        cout << "   PROCESSING COMPLAINT #" << complaint->id << "\n";
        cout << "══════════════════════════════\n";
        
        cout << "Student: " << complaint->studentName << " (Roll: " << complaint->rollNo << ")\n";
        cout << "Department: " << complaint->department << "\n";
        cout << "Category: " << complaint->category << "\n";
        cout << "Issue: " << complaint->description << "\n";
        cout << "Submitted Status: " << complaint->status << "\n";
        
        cout << "\nUpdate Status to:\n";
        cout << "1. IN PROGRESS\n2. RESOLVED\n3. KEEP PENDING\nChoice: ";
        
        int choice; 
        cin >> choice; 
        cin.ignore();
        
        if(choice == 1) {
            complaint->status = "IN PROGRESS";
            cout << "✓ Status updated to IN PROGRESS\n";
        }
        else if(choice == 2) {
            complaint->status = "RESOLVED";
            cout << "✓ Complaint RESOLVED and closed\n";
            
            // Remove from queue
            complaintQueue[front] = nullptr;
            front = (front + 1) % capacity;
            pendingCount--;
            resolvedCount++;
            
            delete complaint;
            return;
        }
        
        // Move to next in queue (circular)
        complaintQueue[front] = nullptr;
        front = (front + 1) % capacity;
        
        // Move processed complaint to back of queue if not resolved
        if(complaint->status != "RESOLVED") {
            complaintQueue[rear] = complaint;
            rear = (rear + 1) % capacity;
        }
    }
    
    void undoLastComplaint() {
        if(undoStack.empty()) {
            cout << "No complaints to undo.\n";
            return;
        }
        
        Complaint* last = undoStack.back();
        undoStack.pop_back();
        
        // Find and remove from queue
        for(int i = front; i != rear; i = (i + 1) % capacity) {
            if(complaintQueue[i] && complaintQueue[i]->id == last->id) {
                cout << "✓ Complaint #" << last->id << " undone and removed\n";
                complaintQueue[i] = nullptr;
                pendingCount--;
                
                // Compact the queue
                vector<Complaint*> temp;
                for(int j = front; j != rear; j = (j + 1) % capacity) {
                    if(complaintQueue[j]) temp.push_back(complaintQueue[j]);
                }
                
                // Reset queue
                front = 0; 
                rear = 0;
                for(auto comp : temp) {
                    complaintQueue[rear] = comp;
                    rear = (rear + 1) % capacity;
                }
                
                delete last;
                return;
            }
        }
        cout << "✗ Could not find complaint to undo\n";
    }
    
    void showPendingComplaints() {
        if(isEmpty()) {
            cout << "No pending complaints.\n";
            return;
        }
        
        cout << "\n══════════════════════════════\n";
        cout << "   PENDING COMPLAINTS (" << pendingCount << ")\n";
        cout << "══════════════════════════════\n";
        
        int count = 1;
        for(int i = front; i != rear; i = (i + 1) % capacity) {
            if(complaintQueue[i]) {
                Complaint* comp = complaintQueue[i];
                cout << count++ << ". #" << comp->id << " - " << comp->category << "\n";
                cout << "   Student: " << comp->studentName << " (" << comp->rollNo << ")\n";
                cout << "   Issue: " << comp->description.substr(0, 50);
                if(comp->description.length() > 50) cout << "...";
                cout << "\n   Status: " << comp->status << "\n\n";
            }
        }
        cout << "══════════════════════════════\n";
    }
    
    void showStatistics() {
        cout << "\n══════════════════════════════\n";
        cout << "      SYSTEM STATISTICS\n";
        cout << "══════════════════════════════\n";
        cout << "Total Submitted: " << (pendingCount + resolvedCount) << "\n";
        cout << "Pending: " << pendingCount << "\n";
        cout << "Resolved: " << resolvedCount << "\n";
        cout << "Queue Space Used: " << pendingCount << "/" << capacity << "\n";
        
        if(!undoStack.empty()) {
            cout << "Last Action: Complaint #" << undoStack.back()->id << "\n";
        }
        cout << "══════════════════════════════\n";
    }
    
    void runComplaintSystem() {
        cout << "\n📢 COMPLAINT HANDLING SYSTEM\n";
        
        while(true) {
            cout << "\n1. Submit Complaint\n2. Process Next Complaint\n";
            cout << "3. View Pending Complaints\n4. Undo Last Complaint\n";
            cout << "5. Show Statistics\n6. Back to Main Menu\nChoice: ";
            
            int choice; 
            cin >> choice; 
            cin.ignore();
            
            if(choice == 1) {
                submitComplaint();
            }
            else if(choice == 2) {
                processNextComplaint();
            }
            else if(choice == 3) {
                showPendingComplaints();
            }
            else if(choice == 4) {
                undoLastComplaint();
            }
            else if(choice == 5) {
                showStatistics();
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