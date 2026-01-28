
#include <iostream>
#include <vector>
#include <string>
#include <cctype>


using namespace std;

class CampusRoomAllocation {
    struct Room {
        int id;
        int capacity;
        string type;        // "LAB" or "CLASS"
        string floor;       // "BASEMENT", "GROUND", "F1", "F2", "F3", "F4"
        string department;  // Preferred department
        bool allocated;
        
        Room(int i, int c, string t, string f, string d) 
            : id(i), capacity(c), type(t), floor(f), department(d), allocated(false) {}
    };
    
    struct ClassRequest {
        string className;
        int students;
        string type;        // "LAB" or "CLASS"
        string department;  // Department name
        int priority;       // 1=Urgent, 2=Normal
        
        bool operator<(const ClassRequest& other) const {
            return priority > other.priority; // Reverse for max-heap
        }
    };
    
    // Department to floor mapping
    vector<pair<string, string>> deptFloors = {
        {"MECHANICAL", "BASEMENT"},
        {"ELECTRICAL", "GROUND"},
        {"AVIATION", "F1"},
        {"FINTECH", "F1"},
        {"BBA", "F1"},
        {"CS", "F2"},
        {"CYBER", "F2"},
        {"SE", "F3"},
        {"AI", "F3"},
        {"DS", "F3"}
    };
    
    // Rooms database - YOUR ACTUAL CAMPUS ROOMS
    vector<Room*> allRooms;
    vector<ClassRequest*> requests;
    
    string getFloorForDept(string dept) {
        for(auto& pair : deptFloors) {
            if(pair.first == dept) return pair.second;
        }
        return "ANY";
    }
    
    // Manual priority queue operations
    void pqPush(ClassRequest* req) {
        requests.push_back(req);
        int i = requests.size() - 1;
        while(i > 0 && requests[(i-1)/2]->priority > requests[i]->priority) {
            ClassRequest* temp = requests[(i-1)/2];
            requests[(i-1)/2] = requests[i];
            requests[i] = temp;
            i = (i-1)/2;
        }
    }
    
    ClassRequest* pqPop() {
        if(requests.empty()) return nullptr;
        
        ClassRequest* top = requests[0];
        requests[0] = requests.back();
        requests.pop_back();
        
        int i = 0, n = requests.size();
        while(true) {
            int left = 2*i + 1;
            int right = 2*i + 2;
            int smallest = i;
            
            if(left < n && requests[left]->priority < requests[smallest]->priority)
                smallest = left;
            if(right < n && requests[right]->priority < requests[smallest]->priority)
                smallest = right;
            
            if(smallest != i) {
                ClassRequest* temp = requests[i];
                requests[i] = requests[smallest];
                requests[smallest] = temp;
                i = smallest;
            } else break;
        }
        return top;
    }
    
public:
    CampusRoomAllocation() {
        // ============ INITIALIZE YOUR ACTUAL CAMPUS ROOMS ============
        
        // BASEMENT - Mechanical Dept
        allRooms.push_back(new Room(1, 50, "LAB", "BASEMENT", "MECHANICAL"));
        allRooms.push_back(new Room(2, 60, "LAB", "BASEMENT", "MECHANICAL"));
        
        // GROUND FLOOR - Electrical Dept
        allRooms.push_back(new Room(11, 50, "CLASS", "GROUND", "ELECTRICAL"));
        allRooms.push_back(new Room(12, 55, "CLASS", "GROUND", "ELECTRICAL"));
        allRooms.push_back(new Room(13, 60, "LAB", "GROUND", "ELECTRICAL"));
        
        // FLOOR 1 - Aviation, FinTech, BBA
        allRooms.push_back(new Room(21, 50, "CLASS", "F1", "AVIATION"));
        allRooms.push_back(new Room(22, 55, "CLASS", "F1", "FINTECH"));
        allRooms.push_back(new Room(23, 50, "CLASS", "F1", "BBA"));
        allRooms.push_back(new Room(24, 60, "CLASS", "F1", "ANY")); // General
        allRooms.push_back(new Room(75, 120, "CLASS", "F1", "ANY")); // Room 75 - Large hall
        
        // FLOOR 2 - CS/Cyber
        allRooms.push_back(new Room(31, 70, "CLASS", "F2", "CS")); // Room 31
        allRooms.push_back(new Room(32, 50, "CLASS", "F2", "CS"));
        allRooms.push_back(new Room(33, 60, "LAB", "F2", "CS"));
        allRooms.push_back(new Room(34, 55, "CLASS", "F2", "CYBER"));
        allRooms.push_back(new Room(35, 60, "LAB", "F2", "CYBER"));
        
        // FLOOR 3 - SE/AI/DS
        allRooms.push_back(new Room(41, 50, "CLASS", "F3", "SE"));
        allRooms.push_back(new Room(42, 55, "CLASS", "F3", "AI"));
        allRooms.push_back(new Room(43, 50, "CLASS", "F3", "DS"));
        allRooms.push_back(new Room(44, 60, "LAB", "F3", "ANY"));
        
        // FLOOR 4 - General/Labs
        allRooms.push_back(new Room(51, 50, "CLASS", "F4", "ANY"));
        allRooms.push_back(new Room(52, 60, "LAB", "F4", "ANY"));
        
        cout << "✓ Campus Room Allocation System Ready\n";
        cout << "  Loaded rooms across all floors\n";
    }
    
    void addClassRequest() {
        ClassRequest* req = new ClassRequest();
        
        cout << "\n--- ADD CLASS REQUEST ---\n";
        cout << "Class Name: "; 
        getline(cin, req->className);
        
        cout << "Department (MECHANICAL/ELECTRICAL/AVIATION/FINTECH/BBA/CS/CYBER/SE/AI/DS): ";
        getline(cin, req->department);
        
        // Convert to uppercase for consistency
        for(char& c : req->department) c = toupper(c);
        
        cout << "Number of Students: "; 
        cin >> req->students;
        
        cout << "Type (LAB/CLASS): "; 
        cin >> req->type;
        
        cout << "Priority (1=Urgent, 2=Normal): "; 
        cin >> req->priority;
        
        cin.ignore();
        pqPush(req);
        
        cout << "✓ Request added for " << req->department << " department\n";
    }
    
    Room* findBestRoom(ClassRequest* req) {
        Room* bestRoom = nullptr;
        string preferredFloor = getFloorForDept(req->department);
        
        // First pass: Try to match department floor exactly
        for(auto room : allRooms) {
            if(!room->allocated && 
               room->type == req->type &&
               room->capacity >= req->students) {
                
                // Check floor preference
                bool floorMatch = (room->floor == preferredFloor) || 
                                 (room->department == req->department) ||
                                 (room->department == "ANY");
                
                if(floorMatch) {
                    if(!bestRoom || room->capacity < bestRoom->capacity) {
                        bestRoom = room;
                    }
                }
            }
        }
        
        // Second pass: If no exact match, try any available room
        if(!bestRoom) {
            for(auto room : allRooms) {
                if(!room->allocated && 
                   room->type == req->type &&
                   room->capacity >= req->students) {
                    
                    if(!bestRoom || room->capacity < bestRoom->capacity) {
                        bestRoom = room;
                    }
                }
            }
        }
        
        if(bestRoom) {
            bestRoom->allocated = true;
        }
        
        return bestRoom;
    }
    
    void processAllRequests() {
        cout << "\n══════════════════════════════════════\n";
        cout << "      PROCESSING CLASS REQUESTS\n";
        cout << "══════════════════════════════════════\n";
        
        if(requests.empty()) {
            cout << "No pending requests\n";
            return;
        }
        
        int processed = 0, rejected = 0;
        while(!requests.empty()) {
            ClassRequest* req = pqPop();
            Room* room = findBestRoom(req);
            
            if(room) {
                cout << "✓ " << req->department << " - " << req->className;
                cout << " (" << req->students << " students) → ";
                cout << "Room " << room->id << " [" << room->floor << " Floor] ";
                cout << "(" << room->capacity << " seats, " << room->type << ")\n";
                processed++;
            } else {
                cout << "✗ " << req->department << " - " << req->className;
                cout << " REJECTED - No suitable " << req->type << " room";
                cout << " for " << req->students << " students\n";
                rejected++;
            }
            delete req;
        }
        
        cout << "══════════════════════════════════════\n";
        cout << "Results: " << processed << " allocated, " << rejected << " rejected\n";
        cout << "══════════════════════════════════════\n";
    }
    
    void showAvailableRooms() {
        cout << "\n══════════════════════════════════════\n";
        cout << "      AVAILABLE ROOMS BY FLOOR\n";
        cout << "══════════════════════════════════════\n";
        
        string currentFloor = "";
        for(auto room : allRooms) {
            if(!room->allocated) {
                if(room->floor != currentFloor) {
                    currentFloor = room->floor;
                    cout << "\n📍 " << currentFloor << " FLOOR:\n";
                }
                cout << "  Room " << room->id << ": " << room->capacity << " seats ";
                cout << "(" << room->type << ") - " << room->department << "\n";
            }
        }
        cout << "══════════════════════════════════════\n";
    }
    
    void resetAllocations() {
        for(auto room : allRooms) {
            room->allocated = false;
        }
        cout << "✓ All room allocations reset\n";
    }
    
    void runRoomSystem() {
        cout << "\n🏛️  CAMPUS ROOM ALLOCATION SYSTEM\n";
        
        while(true) {
            cout << "\n1. Add Class Request\n2. Process All Requests\n";
            cout << "3. View Available Rooms\n4. Reset Allocations\n";
            cout << "5. Back to Main Menu\nChoice: ";
            
            int choice; 
            cin >> choice; 
            cin.ignore();
            
            if(choice == 1) {
                addClassRequest();
            }
            else if(choice == 2) {
                processAllRequests();
            }
            else if(choice == 3) {
                showAvailableRooms();
            }
            else if(choice == 4) {
                resetAllocations();
            }
            else if(choice == 5) {
                cout << "Returning to main menu...\n";
                break;
            }
            else {
                cout << "Invalid choice! Please enter 1-5.\n";
            }
        }
    }
};