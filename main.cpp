#include <iostream>
using namespace std;

// Include all your .cpp files directly
#include "navigation.cpp"
#include "exam.cpp"
#include "room.cpp"
#include "complaint.cpp"
#include "event.cpp"

void displayMainMenu() {
    cout << "\n══════════════════════════════════════════\n";
    cout << "   AIR UNIVERSITY CAMPUS MANAGEMENT SYSTEM\n";
    cout << "══════════════════════════════════════════\n";
   cout << "1. 🗺️  Navigation System\n";
    cout << "2. 🎓 Exam Seating Plan\n";
    cout << "3. 🏛️  Campus Room Allocation\n";
    cout << "4. 📢 Complaint Handling System\n";
    cout << "5. 🎪 Event Registration System\n";
    cout << "6. 🚪 Exit\n";
    cout << "══════════════════════════════════════════\n";
    cout << "Enter your choice (1-6): ";
}

int main() {
    cout << "🚀 Welcome to Air University Campus Management System!\n";
    cout << "📍 Kamra Campus - Integrated Management Platform\n\n";
    
    int choice;
    
    do {
        displayMainMenu();
        cin >> choice;
        cin.ignore();
        
        switch (choice) {
            case 1: {
                AirUniNavigation nav;
                nav.runNavigation();
                break;
            }
            case 2: {
                ExamSeating exam;
                exam.runExamSystem();
                break;
            }
            case 3: {
                CampusRoomAllocation room;
                room.runRoomSystem();
                break;
            }
            case 4: {
                ComplaintSystem complaint;
                complaint.runComplaintSystem();
                break;
            }
            case 5: {
                EventRegistration event;
                event.runEventSystem();
                break;
            }
            case 6: {
                cout << "\n🙏 Thank you for using Air University Campus Management System!\n";
                cout << "👋 Goodbye!\n";
                break;
            }
            default:
                cout << "❌ Invalid choice! Please enter 1-6.\n";
        }
    } while (choice != 6);
    
    return 0;
}