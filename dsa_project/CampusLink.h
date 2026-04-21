#pragma once

#include <iostream>
#include <string>
#include <climits>

using namespace std;

// ==========================================
// DATA STRUCTURES & MODULES
// ==========================================

// Forward Declarations
struct HashNode;
struct BuildingNode;
struct AVLNode;
struct MessageNode;
struct ConversationNode;

// --- MODULE D: COMPLAINTS (QUEUE) ---
struct ComplaintNode {
    int ticketID;
    string raisedBy;
    string building;
    string roomID;
    string description;
    string timeStamp;
    ComplaintNode* next;

    ComplaintNode();
};

class ComplaintQueue {
public:
    ComplaintNode* front;
    ComplaintNode* rear;
    int counter;

    ComplaintQueue();
    ~ComplaintQueue();

    void enqueue(string user, string bldg, string room, string desc, string time);
    void dequeue();
    void display();
    void removeComplaintsByBuilding(string bName);
};

// --- MODULE E: MESSAGING (STACK) ---
struct MessageNode {
    string fromUser;
    string toUser;
    string messageText;
    string timestamp;
    MessageNode* next;

    MessageNode();
};

class MessageStack {
public:
    MessageNode* top;

    MessageStack();
    ~MessageStack();

    void push(string from, string to, string text, string time);
    MessageNode* pop();
    MessageNode* peek();
    void display();
};

struct ConversationNode {
    string friendName;
    MessageStack* stack;
    ConversationNode* next;

    ConversationNode(string f);
    ~ConversationNode();
};

// --- MODULE C: ROOMS (AVL TREE) ---
struct AVLNode {
    string roomID;
    string type;
    int floor;
    bool isReserved;
    string reservedBy;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(string id, string t, int f);
};

class RoomAVL {
private:
    int height(AVLNode* N);
    int maxVal(int a, int b);
    int getBalance(AVLNode* N);
    AVLNode* rightRotate(AVLNode* y);
    AVLNode* leftRotate(AVLNode* x);
    AVLNode* insertNode(AVLNode* node, string id, string type, int floor);
    AVLNode* minValueNode(AVLNode* node);
    AVLNode* deleteNode(AVLNode* root, string id);
    int countLeavesUtil(AVLNode* node);
    int countInternalNodesUtil(AVLNode* node);
    void inorder(AVLNode* root);
    void preorder(AVLNode* root);
    void postorder(AVLNode* root);
    void clear(AVLNode* node);

public:
    AVLNode* root;

    RoomAVL();
    ~RoomAVL();

    void insert(string id, string type, int floor);
    void remove(string id);
    AVLNode* search(AVLNode* r, string id);
    int reserveRoom(string roomID, string userName);
    int countLeaves();
    int countInternalNodes();
    void cancelReservationsByUser(AVLNode* r, string username);
    void searchByType(AVLNode* r, string type);
    void printInorder();
    void printPreorder();
    void printPostorder();
};

// --- MODULE A: PEOPLE DIRECTORY (HASH TABLE) ---
struct HashNode {
    string username;
    string password;
    string role;
    string department;
    string email;
    string lastBookedRoom;
    ConversationNode* conversations;
    HashNode* next;

    HashNode(string u, string p, string r, string d, string e);
};

class PeopleDirectory {
private:
    HashNode** table;
    int capacity;
    int size;

    int hashFunction(string key);
    void resize();

public:
    PeopleDirectory();
    ~PeopleDirectory();

    void registerUser(string u, string p, string r, string d, string e);
    HashNode* getUser(string u);
    void deleteUser(string u);
    void updateUser(string u, string p, string r, string d, string e);
    void displayAll();

    // Messaging Integration
    ConversationNode* getConversation(string u, string friendName);
    void sendMessage(string fromUser, string toUser, string text);
    MessageNode* viewLatestMessage(string user, string friendName);
    MessageNode* popLatestMessage(string user, string friendName);
    void displayConversation(string user1, string user2);
};

// --- MODULE B: CAMPUS MAP (GRAPH) ---
struct EdgeNode {
    string dest;
    int weight;
    string type; // road, walkway
    EdgeNode* next;

    EdgeNode(string d, int w, string t);
};

struct BuildingNode {
    string name;
    EdgeNode* edges;
    RoomAVL* rooms;
    BuildingNode* next;

    bool visited;
    int minDist;
    BuildingNode* parent;

    BuildingNode(string n);
};

class CampusGraph {
private:
    BuildingNode* head;
    void DFSHelper(BuildingNode* u);
    void printPath(BuildingNode* b);

public:
    CampusGraph();
    ~CampusGraph();

    BuildingNode* getBuilding(string name);
    void addBuilding(string name);
    void removeBuilding(string name);
    void addPath(string src, string dest, int w, string type);
    void removePath(string src, string dest);
    void BFS(string startBuildingName);
    void DFS(string start);
    void shortestPath(string src, string dest);
    void PrimsAlgorithm();
    BuildingNode* getHead();
};

// --- MODULE F: SYSTEM INTEGRATION ---
class UniversitySystem {
public:
    PeopleDirectory people;
    CampusGraph campus;
    ComplaintQueue complaints;
    HashNode* currentUser;

    UniversitySystem();
    void seedData();
    void deleteUserIntegrated(string username);
    void deleteBuildingIntegrated(string bName);
    void reserveRoomLogic(string bName, string rID);

    // Menus
    void menuUser();
    void menuMap();
    void menuRooms();
    void menuComplaints();
    void menuMessaging();
    void mainMenu();
};
