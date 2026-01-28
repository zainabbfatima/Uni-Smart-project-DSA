#include <iostream>
#include <vector>
#include <string>


using namespace std;

class AirUniNavigation {
private:
    struct Node {
        int id;
        string code;
        string name;
        string type;
        string floor;
        Node(int i, string c, string n, string t, string f)
            : id(i), code(c), name(n), type(t), floor(f) {}
    };

    struct Edge {
        int from;
        int to;
        int distance;
        string routeName;
        Edge(int f, int t, int d, string r)
            : from(f), to(t), distance(d), routeName(r) {}
    };

    struct HeapNode {
        int nodeId;
        int distance;
        bool operator<(const HeapNode& other) const {
            return distance < other.distance;
        }
    };

    vector<Node> nodes;
    vector<Edge> edges;
    vector<vector<int>> adjacency;
    const int INF = 1000000000;

    class MinHeap {
    private:
        vector<HeapNode> heap;
        
        void heapifyUp(int index) {
            while (index > 0) {
                int parent = (index - 1) / 2;
                if (heap[index].distance < heap[parent].distance) {
                    HeapNode temp = heap[index];
                    heap[index] = heap[parent];
                    heap[parent] = temp;
                    index = parent;
                } else {
                    break;
                }
            }
        }
        
        void heapifyDown(int index) {
            int size = heap.size();
            while (true) {
                int left = 2 * index + 1;
                int right = 2 * index + 2;
                int smallest = index;
                
                if (left < size && heap[left].distance < heap[smallest].distance)
                    smallest = left;
                if (right < size && heap[right].distance < heap[smallest].distance)
                    smallest = right;
                    
                if (smallest != index) {
                    HeapNode temp = heap[index];
                    heap[index] = heap[smallest];
                    heap[smallest] = temp;
                    index = smallest;
                } else {
                    break;
                }
            }
        }
        
    public:
        void push(HeapNode node) {
            heap.push_back(node);
            heapifyUp(heap.size() - 1);
        }
        
        HeapNode pop() {
            if (heap.empty()) return {-1, -1};
            HeapNode root = heap[0];
            heap[0] = heap.back();
            heap.pop_back();
            if (!heap.empty()) heapifyDown(0);
            return root;
        }
        
        bool empty() { return heap.empty(); }
    };

    int findNodeId(string code) {
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i].code == code) return i;
        }
        return -1;
    }

    void addEdge(string fromCode, string toCode, int distance, string routeName) {
        int fromId = findNodeId(fromCode);
        int toId = findNodeId(toCode);
        
        if (fromId == -1 || toId == -1) {
            cout << "Error: Location not found!\n";
            return;
        }
        
        edges.push_back(Edge(fromId, toId, distance, routeName));
        int edgeIndex = edges.size() - 1;
        
        if (adjacency.size() <= fromId) adjacency.resize(fromId + 1);
        if (adjacency.size() <= toId) adjacency.resize(toId + 1);
        
        adjacency[fromId].push_back(edgeIndex);
        adjacency[toId].push_back(edgeIndex);
    }

    void initializeCampusData() {
        // ============ OUTDOOR NODES ============
        nodes.push_back(Node(0, "ACADEMIC_BLOCK", "Academic Block", "BUILDING", "OUTDOOR"));
        nodes.push_back(Node(1, "HOSTEL_BLOCK", "Hostel Block", "BUILDING", "OUTDOOR"));
        nodes.push_back(Node(2, "PARKING", "Parking Area", "AREA", "OUTDOOR"));
        nodes.push_back(Node(3, "MAIN_GATE", "Main Gate", "GATE", "OUTDOOR"));
        nodes.push_back(Node(4, "CANTEEN_1", "Mini Canteen 1", "CAFE", "OUTDOOR"));
        nodes.push_back(Node(5, "CANTEEN_2", "Mini Canteen 2", "CAFE", "OUTDOOR"));
        nodes.push_back(Node(6, "SPOT_CAFE", "Spot Cafe", "CAFE", "OUTDOOR"));
        nodes.push_back(Node(40, "SPORTS_COMPLEX", "Sports Complex", "BUILDING", "OUTDOOR"));

        // ============ BASEMENT ============
        nodes.push_back(Node(7, "B_CAFETERIA", "Basement Cafeteria", "ROOM", "BASEMENT"));
        nodes.push_back(Node(8, "B_GYM", "Gym", "ROOM", "BASEMENT"));
        nodes.push_back(Node(9, "B_MECH_DEPT", "Mechanical Dept", "ROOM", "BASEMENT"));
        nodes.push_back(Node(10, "B_MECH_LAB", "Mechanical Lab", "LAB", "BASEMENT"));

        // ============ GROUND FLOOR ============
        nodes.push_back(Node(11, "G_RECEPTION", "Reception", "ROOM", "GROUND"));
        nodes.push_back(Node(12, "G_ELEC_DEPT", "Electrical Dept", "ROOM", "GROUND"));
        nodes.push_back(Node(13, "G_ELEC_LABS", "Electrical Labs", "LAB", "GROUND"));
        nodes.push_back(Node(14, "G_DIRECTOR", "Director Office", "ROOM", "GROUND"));
        nodes.push_back(Node(15, "G_COMMON_ROOM", "Common Room", "ROOM", "GROUND"));
        nodes.push_back(Node(16, "G_FACULTY", "Faculty Offices", "ROOM", "GROUND"));

        // ============ FLOOR 1 ============
        nodes.push_back(Node(17, "F1_AVIATION", "Aviation Classes", "CLASS", "F1"));
        nodes.push_back(Node(18, "F1_FINTECH", "FinTech Classes", "CLASS", "F1"));
        nodes.push_back(Node(19, "F1_BBA", "BBA Classes", "CLASS", "F1"));
        nodes.push_back(Node(20, "F1_LIBRARY", "Library", "ROOM", "F1"));
        nodes.push_back(Node(21, "F1_ADMIN", "Admin Office", "ROOM", "F1"));
        nodes.push_back(Node(22, "F1_STUDENT_AFFAIRS", "Student Affairs Office", "ROOM", "F1"));
        nodes.push_back(Node(23, "F1_ROOM_75", "Hall Room 75", "HALL", "F1"));
        nodes.push_back(Node(24, "F1_FACULTY", "Faculty Offices", "ROOM", "F1"));

        // ============ FLOOR 2 ============
        nodes.push_back(Node(25, "F2_CS_CLASS", "CS Classes", "CLASS", "F2"));
        nodes.push_back(Node(26, "F2_CYBER_CLASS", "Cyber Classes", "CLASS", "F2"));
        nodes.push_back(Node(27, "F2_CS_LAB", "CS Labs", "LAB", "F2"));
        nodes.push_back(Node(28, "F2_HOD_CS", "HOD CS Office", "ROOM", "F2"));
        nodes.push_back(Node(29, "F2_FACULTY", "Faculty Offices", "ROOM", "F2"));

        // ============ FLOOR 3 ============
        nodes.push_back(Node(30, "F3_SE", "SE Classes", "CLASS", "F3"));
        nodes.push_back(Node(31, "F3_AI", "AI Classes", "CLASS", "F3"));
        nodes.push_back(Node(32, "F3_DS", "Data Science Classes", "CLASS", "F3"));
        nodes.push_back(Node(33, "F3_LABS", "Labs", "LAB", "F3"));
        nodes.push_back(Node(34, "F3_COMP_LABS", "Computer Labs", "LAB", "F3"));
        nodes.push_back(Node(35, "F3_FACULTY", "Faculty Offices", "ROOM", "F3"));

        // ============ FLOOR 4 ============
        nodes.push_back(Node(36, "F4_FYP_LAB", "FYP Labs", "LAB", "F4"));
        nodes.push_back(Node(37, "F4_COMMON_LABS", "Common Labs", "LAB", "F4"));
        nodes.push_back(Node(38, "F4_BIC", "Business Incubation", "ROOM", "F4"));
        nodes.push_back(Node(39, "F4_COMP_LABS", "Computer Labs", "LAB", "F4"));

        cout << "✓ Loaded " << nodes.size() << " locations\n";

        // ============ OUTDOOR CONNECTIONS ============
        addEdge("ACADEMIC_BLOCK", "MAIN_GATE", 100, "Main Entrance");
        addEdge("MAIN_GATE", "PARKING", 460, "Main Gate Path");
        addEdge("ACADEMIC_BLOCK", "PARKING", 400, "Back Entrance");
        addEdge("ACADEMIC_BLOCK", "HOSTEL_BLOCK", 400, "Hostel Path");
        addEdge("HOSTEL_BLOCK", "PARKING", 400, "Via Hostel");
        addEdge("PARKING", "CANTEEN_1", 30, "Parking Entrance");
        addEdge("CANTEEN_1", "CANTEEN_2", 150, "Between Canteens");
        addEdge("CANTEEN_1", "SPOT_CAFE", 150, "To Spot Cafe");
        addEdge("CANTEEN_2", "SPOT_CAFE", 10, "Adjacent");
        
        // Sports Complex connections (5 min walk = ~300 meters)
        addEdge("HOSTEL_BLOCK", "SPORTS_COMPLEX", 300, "Hostel to Sports");
        addEdge("ACADEMIC_BLOCK", "SPORTS_COMPLEX", 700, "Academic to Sports");
        addEdge("PARKING", "SPORTS_COMPLEX", 350, "Parking to Sports");
        
        // Outdoor to Indoor connections
        addEdge("ACADEMIC_BLOCK", "G_RECEPTION", 10, "Main Building Entrance");
        addEdge("MAIN_GATE", "G_RECEPTION", 110, "Direct Path to Reception");
        addEdge("PARKING", "G_RECEPTION", 450, "Parking to Reception");
        addEdge("HOSTEL_BLOCK", "G_RECEPTION", 420, "Hostel to Reception");
        addEdge("SPORTS_COMPLEX", "G_RECEPTION", 720, "Sports to Reception");

        // ============ INDOOR CONNECTIONS ============
        // Vertical connections
        addEdge("G_RECEPTION", "B_CAFETERIA", 20, "Stairs Down");
        addEdge("G_RECEPTION", "F1_AVIATION", 20, "Stairs Up");
        addEdge("F1_AVIATION", "F2_CS_CLASS", 20, "Stairs Up");
        addEdge("F2_CS_CLASS", "F3_SE", 20, "Stairs Up");
        addEdge("F3_SE", "F4_FYP_LAB", 20, "Stairs Up");

        // Ground floor connections
        addEdge("G_RECEPTION", "G_ELEC_DEPT", 30, "Main Corridor");
        addEdge("G_ELEC_DEPT", "G_ELEC_LABS", 15, "Adjacent");
        addEdge("G_RECEPTION", "G_DIRECTOR", 40, "Admin Wing");
        addEdge("G_RECEPTION", "G_COMMON_ROOM", 50, "East Wing");
        addEdge("G_RECEPTION", "G_FACULTY", 35, "Faculty Wing");

        // Floor 1 connections
        addEdge("F1_AVIATION", "F1_FINTECH", 25, "North Corridor");
        addEdge("F1_FINTECH", "F1_BBA", 25, "Corridor");
        addEdge("F1_BBA", "F1_LIBRARY", 30, "Library Wing");
        addEdge("F1_LIBRARY", "F1_ADMIN", 20, "Admin Section");
        addEdge("F1_ADMIN", "F1_STUDENT_AFFAIRS", 15, "Student Affairs");
        addEdge("F1_STUDENT_AFFAIRS", "F1_ROOM_75", 25, "To Hall Room 75");
        addEdge("F1_ROOM_75", "F1_FACULTY", 20, "Faculty Offices");
        addEdge("F1_FACULTY", "F1_AVIATION", 40, "Circular Corridor");

        // Floor 2 connections
        addEdge("F2_CS_CLASS", "F2_CYBER_CLASS", 20, "CS Corridor");
        addEdge("F2_CS_CLASS", "F2_CS_LAB", 15, "To Labs");
        addEdge("F2_CYBER_CLASS", "F2_HOD_CS", 25, "HOD Office");
        addEdge("F2_HOD_CS", "F2_FACULTY", 10, "Faculty Section");
        addEdge("F2_FACULTY", "F2_CS_CLASS", 30, "Faculty Corridor");

        // Floor 3 connections
        addEdge("F3_SE", "F3_AI", 20, "Tech Corridor");
        addEdge("F3_AI", "F3_DS", 20, "Data Science Wing");
        addEdge("F3_DS", "F3_LABS", 15, "To Labs");
        addEdge("F3_LABS", "F3_COMP_LABS", 10, "Computer Labs");
        addEdge("F3_COMP_LABS", "F3_FACULTY", 15, "Faculty Offices");
        addEdge("F3_FACULTY", "F3_SE", 25, "Faculty Wing");

        // Floor 4 connections
        addEdge("F4_FYP_LAB", "F4_COMMON_LABS", 20, "Lab Corridor");
        addEdge("F4_COMMON_LABS", "F4_BIC", 25, "To BIC");
        addEdge("F4_BIC", "F4_COMP_LABS", 30, "To Computer Labs");
        addEdge("F4_COMP_LABS", "F4_FYP_LAB", 35, "Circular Path");

        // Basement connections
        addEdge("B_CAFETERIA", "B_GYM", 40, "Cafeteria to Gym");
        addEdge("B_GYM", "B_MECH_DEPT", 30, "Gym to Mech Dept");
        addEdge("B_MECH_DEPT", "B_MECH_LAB", 10, "Dept to Lab");

        cout << "✓ Created " << edges.size() << " paths\n";
    }

    void displayPath(int startId, int endId, vector<int>& distance, vector<int>& parent) {
        cout << "\n══════════════════════════════════════════\n";
        cout << " NAVIGATION: " << nodes[startId].name;
        cout << " → " << nodes[endId].name << "\n";
        cout << "══════════════════════════════════════════\n";

        if (distance[endId] == INF) {
            cout << "✗ No path exists!\n";
            return;
        }

        cout << "✅ Total Distance: " << distance[endId] << " meters\n";
        cout << "📍 Path: ";
        
        vector<int> path;
        for (int v = endId; v != -1; v = parent[v]) {
            path.push_back(v);
        }

        for (int i = path.size() - 1; i >= 0; i--) {
            cout << nodes[path[i]].name;
            if (i > 0) cout << " → ";
        }
        
        cout << "\n\n📋 Step-by-step:\n";
        int cumulative = 0;
        
        for (int i = path.size() - 1; i > 0; i--) {
            int from = path[i];
            int to = path[i - 1];
            int segmentDist = 0;
            
            for (int edgeIndex : adjacency[from]) {
                Edge edge = edges[edgeIndex];
                if ((edge.from == from && edge.to == to) ||
                    (edge.from == to && edge.to == from)) {
                    segmentDist = edge.distance;
                    break;
                }
            }
            
            cumulative += segmentDist;
            cout << " " << (path.size() - i) << ". "
                 << nodes[from].name << " → "
                 << nodes[to].name << " ("
                 << segmentDist << "m, total: " << cumulative << "m)\n";
        }
        
        cout << "══════════════════════════════════════════\n";
    }

public:
    AirUniNavigation() {
        adjacency.clear();
        initializeCampusData();
    }

    void findShortestPath(string startCode, string endCode) {
        int startId = findNodeId(startCode);
        int endId = findNodeId(endCode);
        
        if (startId == -1 || endId == -1) {
            cout << "Error: Location not found!\n";
            return;
        }

        int n = nodes.size();
        vector<int> distance(n, INF);
        vector<int> parent(n, -1);
        vector<bool> visited(n, false);
        MinHeap minHeap;

        distance[startId] = 0;
        minHeap.push({startId, 0});

        while (!minHeap.empty()) {
            HeapNode current = minHeap.pop();
            int u = current.nodeId;

            if (visited[u]) continue;
            visited[u] = true;

            if (u == endId) break;

            for (int edgeIndex : adjacency[u]) {
                Edge edge = edges[edgeIndex];
                int v = (edge.from == u) ? edge.to : edge.from;
                int weight = edge.distance;

                if (!visited[v] && distance[u] + weight < distance[v]) {
                    distance[v] = distance[u] + weight;
                    parent[v] = u;
                    minHeap.push({v, distance[v]});
                }
            }
        }

        displayPath(startId, endId, distance, parent);
    }

    void exploreFrom(string startCode) {
        int startId = findNodeId(startCode);
        if (startId == -1) {
            cout << "Location not found!\n";
            return;
        }

        cout << "\n══════════════════════════════════════════\n";
        cout << " EXPLORING FROM: " << nodes[startId].name << "\n";
        cout << "══════════════════════════════════════════\n";

        vector<bool> visited(nodes.size(), false);
        vector<int> queue;
        int front = 0;

        visited[startId] = true;
        queue.push_back(startId);

        cout << "Reachable locations:\n";
        int count = 0;

        while (front < queue.size()) {
            int current = queue[front];
            front++;
            
            cout << " " << ++count << ". " << nodes[current].name;
            if (nodes[current].floor != "OUTDOOR") {
                cout << " (" << nodes[current].floor << ")";
            }
            cout << "\n";

            for (int edgeIndex : adjacency[current]) {
                Edge edge = edges[edgeIndex];
                int neighbor = (edge.from == current) ? edge.to : edge.from;
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    queue.push_back(neighbor);
                }
            }
        }

        if (count == 0) {
            cout << " No reachable locations found.\n";
        }
        
        cout << "══════════════════════════════════════════\n";
    }

    void displayCampusMap() {
        cout << "\n══════════════════════════════════════════\n";
        cout << " AIR UNIVERSITY CAMPUS MAP\n";
        cout << "══════════════════════════════════════════\n\n";
        
        cout << "🏫 OUTDOOR CAMPUS:\n";
        cout << " [Academic Block] ←→ [Hostel Block] (400m)\n";
        cout << " ↓ ↓\n";
        cout << " [Main Gate] ←→ [Parking Area] ←→ [Canteen 1]\n";
        cout << " ↓ ↓\n";
        cout << " [Canteen 2] ←→ [Spot Cafe]\n";
        cout << " [Sports Complex] ←→ [Hostel Block] (300m)\n\n";
        
        cout << "🏢 ACADEMIC BLOCK FLOORS:\n";
        cout << " └─ Basement: Cafeteria, Gym, Mechanical Dept & Lab\n";
        cout << " └─ Ground: Reception, Electrical Dept, Director, Common Room, Faculty\n";
        cout << " └─ Floor 1: Aviation, FinTech, BBA, Library, Admin\n";
        cout << "    Student Affairs, Room 75, Faculty Offices\n";
        cout << " └─ Floor 2: CS Classes, Cyber Security, CS Labs\n";
        cout << "    HOD Office, Faculty Offices\n";
        cout << " └─ Floor 3: SE, AI, Data Science, Labs\n";
        cout << "    Computer Labs, Faculty Offices\n";
        cout << " └─ Floor 4: FYP Labs, Common Labs, BIC, Computer Labs\n";
        cout << "══════════════════════════════════════════\n";
    }

    void listAllLocations() {
        cout << "\n══════════════════════════════════════════\n";
        cout << " ALL LOCATIONS (" << nodes.size() << " total)\n";
        cout << "══════════════════════════════════════════\n";
        
        string currentFloor = "";
        for (int i = 0; i < nodes.size(); i++) {
            if (nodes[i].floor != currentFloor) {
                currentFloor = nodes[i].floor;
                cout << "\n📍 " << currentFloor << ":\n";
            }
            cout << " " << nodes[i].code << " - " << nodes[i].name;
            cout << " (" << nodes[i].type << ")\n";
        }
        
        cout << "══════════════════════════════════════════\n";
    }

    void runNavigation() {
        int navChoice;
        string start, end;
        
        cout << "\n🚀 Navigation System Started!\n";
        
        do {
            cout << "\n══════════════════════════════════════════\n";
            cout << " AIR UNIVERSITY NAVIGATION SYSTEM\n";
            cout << "══════════════════════════════════════════\n";
            cout << "1. Find Shortest Path\n";
            cout << "2. Explore From Location\n";
            cout << "3. View Campus Map\n";
            cout << "4. List All Locations\n";
            cout << "5. Back to Main Menu\n";
            cout << "══════════════════════════════════════════\n";
            cout << "Enter choice: ";
            cin >> navChoice;
            cin.ignore();
            
            switch (navChoice) {
                case 1:
                    cout << "\nEnter START location code: ";
                    getline(cin, start);
                    cout << "Enter END location code: ";
                    getline(cin, end);
                    findShortestPath(start, end);
                    break;
                case 2:
                    cout << "\nEnter location code to explore from: ";
                    getline(cin, start);
                    exploreFrom(start);
                    break;
                case 3:
                    displayCampusMap();
                    break;
                case 4:
                    listAllLocations();
                    break;
                case 5:
                    cout << "Returning to main menu...\n";
                    break;
                default:
                    cout << "Invalid choice! Please enter 1-5.\n";
            }
        } while (navChoice != 5);
    }
};