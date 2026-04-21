#include "CampusLink.h"

// ==========================================
// COMPLAINTS MODULE
// ==========================================
ComplaintNode::ComplaintNode() {
    next = nullptr;
}

ComplaintQueue::ComplaintQueue() {
    front = rear = nullptr;
    counter = 1001;
}

ComplaintQueue::~ComplaintQueue() {
    while (front) {
        dequeue();
    }
}

void ComplaintQueue::enqueue(string user, string bldg, string room, string desc, string time) {
    ComplaintNode* newNode = new ComplaintNode();
    newNode->ticketID = counter++;
    newNode->raisedBy = user;
    newNode->building = bldg;
    newNode->roomID = room;
    newNode->description = desc;
    newNode->timeStamp = time;

    if (rear == nullptr) {
        front = rear = newNode;
    }
    else {
        rear->next = newNode;
        rear = newNode;
    }
    cout << "Complaint #" << newNode->ticketID << " submitted successfully.\n";
}

void ComplaintQueue::dequeue() {
    if (front == nullptr) return;
    ComplaintNode* temp = front;
    front = front->next;
    if (front == nullptr) rear = nullptr;
    delete temp;
}

void ComplaintQueue::display() {
    if (!front) {
        cout << "No pending complaints.\n";
        return;
    }
    ComplaintNode* curr = front;
    cout << "\n--- Pending Complaints ---\n";
    while (curr) {
        cout << "ID: " << curr->ticketID << " | " << curr->raisedBy
            << " | " << curr->building << " " << curr->roomID << "\n";
        curr = curr->next;
    }
}

void ComplaintQueue::removeComplaintsByBuilding(string bName) {
    while (front && front->building == bName) {
        ComplaintNode* temp = front;
        front = front->next;
        delete temp;
    }
    if (!front) {
        rear = nullptr;
        return;
    }
    ComplaintNode* curr = front;
    while (curr->next) {
        if (curr->next->building == bName) {
            ComplaintNode* temp = curr->next;
            curr->next = curr->next->next;
            if (temp == rear) rear = curr;
            delete temp;
        }
        else {
            curr = curr->next;
        }
    }
}

// ==========================================
// MESSAGING MODULE
// ==========================================
MessageNode::MessageNode() {
    next = nullptr;
}

MessageStack::MessageStack() {
    top = nullptr;
}

MessageStack::~MessageStack() {
    while (top) {
        MessageNode* temp = pop();
        delete temp;
    }
}

void MessageStack::push(string from, string to, string text, string time) {
    MessageNode* newNode = new MessageNode();
    newNode->fromUser = from;
    newNode->toUser = to;
    newNode->messageText = text;
    newNode->timestamp = time;
    newNode->next = top;
    top = newNode;
}

MessageNode* MessageStack::pop() {
    if (!top) return nullptr;
    MessageNode* temp = top;
    top = top->next;
    return temp;
}

MessageNode* MessageStack::peek() {
    return top;
}

void MessageStack::display() {
    if (!top) {
        cout << "No messages.\n";
        return;
    }
    MessageNode* curr = top;
    while (curr) {
        cout << "[" << curr->timestamp << "] " << curr->fromUser << ": " << curr->messageText << "\n";
        curr = curr->next;
    }
}

ConversationNode::ConversationNode(string f) {
    friendName = f;
    next = nullptr;
    stack = new MessageStack();
}

ConversationNode::~ConversationNode() {
    delete stack;
}

// ==========================================
// ROOMS MODULE (AVL TREE)
// ==========================================
AVLNode::AVLNode(string id, string t, int f) {
    roomID = id;
    type = t;
    floor = f;
    isReserved = false;
    reservedBy = "None";
    left = right = nullptr;
    height = 1;
}

RoomAVL::RoomAVL() {
    root = nullptr;
}

RoomAVL::~RoomAVL() {
    clear(root);
}

void RoomAVL::clear(AVLNode* node) {
    if (!node) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

int RoomAVL::height(AVLNode* N) {
    return N ? N->height : 0;
}

int RoomAVL::maxVal(int a, int b) {
    return (a > b) ? a : b;
}

int RoomAVL::getBalance(AVLNode* N) {
    return N ? height(N->left) - height(N->right) : 0;
}

AVLNode* RoomAVL::rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = maxVal(height(y->left), height(y->right)) + 1;
    x->height = maxVal(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* RoomAVL::leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = maxVal(height(x->left), height(x->right)) + 1;
    y->height = maxVal(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* RoomAVL::insertNode(AVLNode* node, string id, string type, int floor) {
    if (!node) return new AVLNode(id, type, floor);
    if (id < node->roomID) node->left = insertNode(node->left, id, type, floor);
    else if (id > node->roomID) node->right = insertNode(node->right, id, type, floor);
    else return node;

    node->height = 1 + maxVal(height(node->left), height(node->right));
    int balance = getBalance(node);

    if (balance > 1 && id < node->left->roomID) return rightRotate(node);
    if (balance < -1 && id > node->right->roomID) return leftRotate(node);
    if (balance > 1 && id > node->left->roomID) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    if (balance < -1 && id < node->right->roomID) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVLNode* RoomAVL::minValueNode(AVLNode* node) {
    AVLNode* current = node;
    while (current->left) current = current->left;
    return current;
}

AVLNode* RoomAVL::deleteNode(AVLNode* root, string id) {
    if (!root) return root;
    if (id < root->roomID) root->left = deleteNode(root->left, id);
    else if (id > root->roomID) root->right = deleteNode(root->right, id);
    else {
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                temp = root;
                root = nullptr;
            }
            else *root = *temp;
            delete temp;
        }
        else {
            AVLNode* temp = minValueNode(root->right);
            root->roomID = temp->roomID;
            root->type = temp->type;
            root->floor = temp->floor;
            root->isReserved = temp->isReserved;
            root->reservedBy = temp->reservedBy;
            root->right = deleteNode(root->right, temp->roomID);
        }
    }
    if (!root) return root;

    root->height = 1 + maxVal(height(root->left), height(root->right));
    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

void RoomAVL::insert(string id, string type, int floor) { root = insertNode(root, id, type, floor); }
void RoomAVL::remove(string id) { root = deleteNode(root, id); }

AVLNode* RoomAVL::search(AVLNode* r, string id) {
    if (!r || r->roomID == id) return r;
    return (id < r->roomID) ? search(r->left, id) : search(r->right, id);
}

int RoomAVL::reserveRoom(string roomID, string userName) {
    AVLNode* room = search(root, roomID);
    if (!room) return -1;
    if (room->isReserved) return 0;
    room->isReserved = true;
    room->reservedBy = userName;
    return 1;
}

int RoomAVL::countLeavesUtil(AVLNode* node) {
    if (!node) return 0;
    if (!node->left && !node->right) return 1;
    return countLeavesUtil(node->left) + countLeavesUtil(node->right);
}

int RoomAVL::countLeaves() { return countLeavesUtil(root); }

int RoomAVL::countInternalNodesUtil(AVLNode* node) {
    if (!node || (!node->left && !node->right)) return 0;
    return 1 + countInternalNodesUtil(node->left) + countInternalNodesUtil(node->right);
}

int RoomAVL::countInternalNodes() { return countInternalNodesUtil(root); }

void RoomAVL::cancelReservationsByUser(AVLNode* r, string username) {
    if (!r) return;
    cancelReservationsByUser(r->left, username);
    if (r->isReserved && r->reservedBy == username) {
        r->isReserved = false;
        r->reservedBy = "None";
        cout << "Auto-cancelled reservation for room " << r->roomID << "\n";
    }
    cancelReservationsByUser(r->right, username);
}

void RoomAVL::searchByType(AVLNode* r, string type) {
    if (!r) return;
    searchByType(r->left, type);
    if (r->type == type) cout << "Found: " << r->roomID << "\n";
    searchByType(r->right, type);
}

void RoomAVL::inorder(AVLNode* root) {
    if (root) {
        inorder(root->left);
        cout << "Room: " << root->roomID << " (" << root->type << ") - "
            << (root->isReserved ? "Reserved by " + root->reservedBy : "Available") << "\n";
        inorder(root->right);
    }
}
void RoomAVL::printInorder() { inorder(root); cout << endl; }
void RoomAVL::preorder(AVLNode* root) { if (root) { cout << root->roomID << " "; preorder(root->left); preorder(root->right); } }
void RoomAVL::printPreorder() { preorder(root); cout << endl; }
void RoomAVL::postorder(AVLNode* root) { if (root) { postorder(root->left); postorder(root->right); cout << root->roomID << " "; } }
void RoomAVL::printPostorder() { postorder(root); cout << endl; }

// ==========================================
// PEOPLE DIRECTORY MODULE
// ==========================================
HashNode::HashNode(string u, string p, string r, string d, string e) {
    username = u; password = p; role = r; department = d; email = e;
    lastBookedRoom = "None"; conversations = nullptr; next = nullptr;
}

PeopleDirectory::PeopleDirectory() {
    capacity = 10; size = 0;
    table = new HashNode * [capacity];
    for (int i = 0; i < capacity; i++) table[i] = nullptr;
}

PeopleDirectory::~PeopleDirectory() {
    for (int i = 0; i < capacity; i++) {
        HashNode* curr = table[i];
        while (curr) {
            HashNode* temp = curr;
            curr = curr->next;
            // Delete conversations
            ConversationNode* cCurr = temp->conversations;
            while (cCurr) {
                ConversationNode* cTemp = cCurr;
                cCurr = cCurr->next;
                delete cTemp;
            }
            delete temp;
        }
    }
    delete[] table;
}

int PeopleDirectory::hashFunction(string key) {
    unsigned long hash = 5381;
    for (char c : key) hash = ((hash << 5) + hash) + c;
    return hash % capacity;
}

void PeopleDirectory::resize() {
    int oldCap = capacity;
    HashNode** oldTable = table;
    capacity *= 2;
    table = new HashNode * [capacity];
    for (int i = 0; i < capacity; i++) table[i] = nullptr;
    size = 0;
    for (int i = 0; i < oldCap; i++) {
        HashNode* curr = oldTable[i];
        while (curr) {
            HashNode* nextNode = curr->next;
            int idx = hashFunction(curr->username);
            curr->next = table[idx];
            table[idx] = curr;
            size++;
            curr = nextNode;
        }
    }
    delete[] oldTable;
}

void PeopleDirectory::registerUser(string u, string p, string r, string d, string e) {
    if ((float)size / capacity >= 0.75) resize();
    int idx = hashFunction(u);
    HashNode* temp = table[idx];
    while (temp) {
        if (temp->username == u) { cout << "Error: Username taken.\n"; return; }
        temp = temp->next;
    }
    HashNode* newNode = new HashNode(u, p, r, d, e);
    newNode->next = table[idx];
    table[idx] = newNode;
    size++;
    cout << "User " << u << " registered.\n";
}

HashNode* PeopleDirectory::getUser(string u) {
    int idx = hashFunction(u);
    HashNode* temp = table[idx];
    while (temp) {
        if (temp->username == u) return temp;
        temp = temp->next;
    }
    return nullptr;
}

void PeopleDirectory::deleteUser(string u) {
    int idx = hashFunction(u);
    HashNode* curr = table[idx], * prev = nullptr;
    while (curr) {
        if (curr->username == u) {
            if (prev) prev->next = curr->next;
            else table[idx] = curr->next;
            
            // Delete conversations first
            ConversationNode* cCurr = curr->conversations;
            while (cCurr) {
                ConversationNode* cTemp = cCurr;
                cCurr = cCurr->next;
                delete cTemp;
            }
            delete curr;
            size--;
            cout << "User deleted.\n";
            return;
        }
        prev = curr; curr = curr->next;
    }
    cout << "User not found.\n";
}

void PeopleDirectory::updateUser(string u, string p, string r, string d, string e) {
    HashNode* user = getUser(u);
    if (!user) {
        cout << "User not found.\n";
        return;
    }
    user->password = p;
    user->role = r;
    user->department = d;
    user->email = e;
    cout << "Profile updated for " << u << ".\n";
}

void PeopleDirectory::displayAll() {
    for (int i = 0; i < capacity; i++) {
        if (table[i]) {
            cout << "Bucket " << i << ": ";
            HashNode* curr = table[i];
            while (curr) {
                cout << "[" << curr->username << "|" << curr->role << "] -> ";
                curr = curr->next;
            }
            cout << "NULL\n";
        }
    }
}

ConversationNode* PeopleDirectory::getConversation(string u, string friendName) {
    HashNode* user = getUser(u);
    if (!user) return nullptr;
    ConversationNode* curr = user->conversations;
    while (curr) {
        if (curr->friendName == friendName) return curr;
        curr = curr->next;
    }
    ConversationNode* newNode = new ConversationNode(friendName);
    newNode->next = user->conversations;
    user->conversations = newNode;
    return newNode;
}

void PeopleDirectory::sendMessage(string fromUser, string toUser, string text) {
    HashNode* sender = getUser(fromUser), * receiver = getUser(toUser);
    if (!sender || !receiver) { cout << "Error: User(s) not found.\n"; return; }
    getConversation(fromUser, toUser)->stack->push(fromUser, toUser, text, "Now");
    getConversation(toUser, fromUser)->stack->push(fromUser, toUser, text, "Now");
    cout << "Message sent from " << fromUser << " to " << toUser << ".\n";
}

MessageNode* PeopleDirectory::viewLatestMessage(string user, string friendName) {
    ConversationNode* convo = getConversation(user, friendName);
    return (convo && convo->stack->top) ? convo->stack->top : nullptr;
}

MessageNode* PeopleDirectory::popLatestMessage(string user, string friendName) {
    ConversationNode* convo = getConversation(user, friendName);
    return convo ? convo->stack->pop() : nullptr;
}

void PeopleDirectory::displayConversation(string user1, string user2) {
    if (!getUser(user1)) { cout << "User not found.\n"; return; }
    ConversationNode* convo = getConversation(user1, user2);
    if (!convo->stack->top) cout << "No conversation history with " << user2 << ".\n";
    else {
        cout << "\n--- Conversation between " << user1 << " and " << user2 << " ---\n";
        convo->stack->display();
    }
}

// ==========================================
// CAMPUS MAP MODULE (GRAPH)
// ==========================================
EdgeNode::EdgeNode(string d, int w, string t) {
    dest = d; weight = w; type = t; next = nullptr;
}

BuildingNode::BuildingNode(string n) {
    name = n; edges = nullptr; next = nullptr; visited = false;
    rooms = new RoomAVL(); minDist = INT_MAX; parent = nullptr;
}

CampusGraph::CampusGraph() { head = nullptr; }

CampusGraph::~CampusGraph() {
    while (head) {
        BuildingNode* temp = head;
        head = head->next;
        // Delete edges
        EdgeNode* eCurr = temp->edges;
        while (eCurr) {
            EdgeNode* eTemp = eCurr;
            eCurr = eCurr->next;
            delete eTemp;
        }
        delete temp->rooms;
        delete temp;
    }
}

BuildingNode* CampusGraph::getBuilding(string name) {
    BuildingNode* curr = head;
    while (curr) {
        if (curr->name == name) return curr;
        curr = curr->next;
    }
    return nullptr;
}

void CampusGraph::addBuilding(string name) {
    if (getBuilding(name)) { cout << "Building exists.\n"; return; }
    BuildingNode* newNode = new BuildingNode(name);
    newNode->next = head; head = newNode;
    cout << "Building " << name << " added.\n";
}

void CampusGraph::removeBuilding(string name) {
    BuildingNode* curr = head;
    while (curr) {
        EdgeNode* eCurr = curr->edges, * ePrev = nullptr;
        while (eCurr) {
            if (eCurr->dest == name) {
                if (ePrev) ePrev->next = eCurr->next;
                else curr->edges = eCurr->next;
                delete eCurr; eCurr = ePrev ? ePrev->next : curr->edges;
            }
            else { ePrev = eCurr; eCurr = eCurr->next; }
        }
        curr = curr->next;
    }
    curr = head; BuildingNode* prev = nullptr;
    while (curr) {
        if (curr->name == name) {
            if (prev) prev->next = curr->next;
            else head = curr->next;
            EdgeNode* e = curr->edges;
            while (e) { EdgeNode* t = e; e = e->next; delete t; }
            delete curr->rooms; delete curr;
            cout << "Building " << name << " removed.\n";
            return;
        }
        prev = curr; curr = curr->next;
    }
    cout << "Building not found.\n";
}

void CampusGraph::addPath(string src, string dest, int w, string type) {
    BuildingNode* s = getBuilding(src), * d = getBuilding(dest);
    if (!s || !d) { cout << "Invalid buildings.\n"; return; }
    EdgeNode* e1 = new EdgeNode(dest, w, type); e1->next = s->edges; s->edges = e1;
    EdgeNode* e2 = new EdgeNode(src, w, type); e2->next = d->edges; d->edges = e2;
    cout << "Path added.\n";
}

void CampusGraph::removePath(string src, string dest) {
    BuildingNode* s = getBuilding(src), * d = getBuilding(dest);
    if (!s || !d) { cout << "Invalid buildings.\n"; return; }
    
    auto removeEdge = [](BuildingNode* from, string to) {
        EdgeNode* curr = from->edges, * prev = nullptr;
        while (curr) {
            if (curr->dest == to) {
                if (prev) prev->next = curr->next;
                else from->edges = curr->next;
                delete curr; return true;
            }
            prev = curr; curr = curr->next;
        }
        return false;
    };
    if (removeEdge(s, dest) && removeEdge(d, src)) cout << "Path removed.\n";
    else cout << "Path not found.\n";
}

void CampusGraph::BFS(string startBuildingName) {
    BuildingNode* startNode = getBuilding(startBuildingName);
    if (!startNode) return;
    BuildingNode* tempNode = head;
    while (tempNode) { tempNode->visited = false; tempNode = tempNode->next; }
    
    struct QueueNode { BuildingNode* b; QueueNode* next; };
    QueueNode* qf = nullptr, * qr = nullptr;
    auto enq = [&](BuildingNode* n) {
        QueueNode* newNode = new QueueNode{ n, nullptr };
        if (!qr) qf = qr = newNode;
        else { qr->next = newNode; qr = newNode; }
    };
    auto deq = [&]() {
        if (!qf) return (BuildingNode*)nullptr;
        QueueNode* t = qf; BuildingNode* r = qf->b; qf = qf->next;
        if (!qf) qr = nullptr; delete t; return r;
    };

    enq(startNode); startNode->visited = true;
    cout << "BFS: ";
    while (qf) {
        BuildingNode* curr = deq(); cout << curr->name << " -> ";
        EdgeNode* e = curr->edges;
        while (e) {
            BuildingNode* n = getBuilding(e->dest);
            if (n && !n->visited) { n->visited = true; enq(n); }
            e = e->next;
        }
    }
    cout << "End\n";
}

void CampusGraph::DFS(string start) {
    BuildingNode* s = getBuilding(start); if (!s) return;
    BuildingNode* t = head; while (t) { t->visited = false; t = t->next; }
    cout << "DFS: "; DFSHelper(s); cout << "End\n";
}

void CampusGraph::DFSHelper(BuildingNode* u) {
    u->visited = true; cout << u->name << " -> ";
    EdgeNode* e = u->edges;
    while (e) {
        BuildingNode* v = getBuilding(e->dest);
        if (v && !v->visited) DFSHelper(v);
        e = e->next;
    }
}

void CampusGraph::shortestPath(string src, string dest) {
    BuildingNode* startNode = getBuilding(src), * endNode = getBuilding(dest);
    if (!startNode || !endNode) { cout << "Invalid nodes.\n"; return; }
    BuildingNode* curr = head; int count = 0;
    while (curr) { curr->minDist = INT_MAX; curr->visited = false; curr->parent = nullptr; curr = curr->next; count++; }
    startNode->minDist = 0;
    for (int i = 0; i < count; i++) {
        BuildingNode* u = nullptr; int minVal = INT_MAX;
        BuildingNode* scan = head;
        while (scan) { if (!scan->visited && scan->minDist < minVal) { minVal = scan->minDist; u = scan; } scan = scan->next; }
        if (!u || u->minDist == INT_MAX) break;
        u->visited = true; if (u == endNode) break;
        EdgeNode* e = u->edges;
        while (e) {
            BuildingNode* v = getBuilding(e->dest);
            if (v && !v->visited && u->minDist + e->weight < v->minDist) { v->minDist = u->minDist + e->weight; v->parent = u; }
            e = e->next;
        }
    }
    if (endNode->minDist == INT_MAX) cout << "No path found.\n";
    else { cout << "Shortest Path Cost: " << endNode->minDist << "\nPath: "; printPath(endNode); cout << "\n"; }
}

void CampusGraph::printPath(BuildingNode* b) { if (b) { printPath(b->parent); cout << b->name << " "; } }

void CampusGraph::PrimsAlgorithm() {
    if (!head) return;
    BuildingNode* curr = head; int count = 0;
    while (curr) { curr->minDist = INT_MAX; curr->visited = false; curr->parent = nullptr; curr = curr->next; count++; }
    head->minDist = 0;
    for (int i = 0; i < count; i++) {
        BuildingNode* u = nullptr; int minVal = INT_MAX;
        BuildingNode* scan = head;
        while (scan) { if (!scan->visited && scan->minDist < minVal) { minVal = scan->minDist; u = scan; } scan = scan->next; }
        if (!u) break;
        u->visited = true;
        EdgeNode* e = u->edges;
        while (e) {
            BuildingNode* v = getBuilding(e->dest);
            if (v && !v->visited && e->weight < v->minDist) { v->minDist = e->weight; v->parent = u; }
            e = e->next;
        }
    }
    cout << "Prim's Minimum Spanning Tree:\n";
    curr = head; int total = 0;
    while (curr) {
        if (curr->parent) { cout << curr->parent->name << " - " << curr->name << " : " << curr->minDist << "\n"; total += curr->minDist; }
        curr = curr->next;
    }
    cout << "Total MST Weight: " << total << "\n";
}

BuildingNode* CampusGraph::getHead() { return head; }

// ==========================================
// SYSTEM INTEGRATION MODULE
// ==========================================
UniversitySystem::UniversitySystem() { currentUser = nullptr; seedData(); }

void UniversitySystem::seedData() {
    people.registerUser("subhan", "123", "Admin", "CS-dept", "admin@uni.edu");
    people.registerUser("fatima", "123", "Student", "CS", "std@uni.edu");
    campus.addBuilding("CS_building"); campus.addBuilding("Library"); campus.addBuilding("Cafe");
    campus.addBuilding("Main-Gate"); campus.addBuilding("FSM_Building"); campus.addBuilding("SFC_building"); campus.addBuilding("Dhaba");
    campus.addPath("CS_building", "Library", 70, "Walkway"); campus.addPath("CS_building", "Cafe", 90, "Walkway");
    campus.addPath("CS_building", "Main-Gate", 150, "Walkway"); campus.addPath("CS_building", "FSM_Building", 10, "Walkway");
    campus.addPath("CS_building", "SFC_building", 30, "Walkway"); campus.addPath("CS_building", "Dhaba", 10, "Road");
    campus.addPath("Library", "Cafe", 20, "Walkway"); campus.addPath("Library", "Main-Gate", 200, "Walkway");
    campus.addPath("Library", "FSM_Building", 5, "Walkway"); campus.addPath("Library", "SFC_building", 60, "Walkway");
    campus.addPath("Library", "Dhaba", 250, "Road"); campus.addPath("Cafe", "Main-Gate", 150, "Walkway");
    campus.addPath("Cafe", "FSM_Building", 10, "Walkway"); campus.addPath("Cafe", "SFC_building", 30, "Walkway");
    campus.addPath("Cafe", "Dhaba", 10, "Road"); campus.addPath("Main-Gate", "FSM_Building", 230, "Walkway");
    campus.addPath("Main-Gate", "SFC_building", 120, "Walkway"); campus.addPath("Main-Gate", "Dhaba", 550, "Road");
    campus.addPath("FSM_Building", "SFC_building", 90, "Walkway"); campus.addPath("FSM_Building", "Dhaba", 190, "Road");
    campus.addPath("SFC_building", "Dhaba", 400, "Road");
    BuildingNode* cs = campus.getBuilding("CS_building");
    if (cs) { cs->rooms->insert("101", "Classroom", 1); cs->rooms->insert("102", "Lab", 1); }
}

void UniversitySystem::deleteUserIntegrated(string username) {
    BuildingNode* b = campus.getHead();
    while (b) { b->rooms->cancelReservationsByUser(b->rooms->root, username); b = b->next; }
    people.deleteUser(username);
}

void UniversitySystem::deleteBuildingIntegrated(string bName) {
    complaints.removeComplaintsByBuilding(bName);
    campus.removeBuilding(bName);
}

void UniversitySystem::reserveRoomLogic(string bName, string rID) {
    BuildingNode* b = campus.getBuilding(bName); if (!b) { cout << "Building not found.\n"; return; }
    int res = b->rooms->reserveRoom(rID, currentUser->username);
    if (res == -1) cout << "Room not found.\n";
    else if (res == 0) cout << "Room is already reserved.\n";
    else if (res == 1) {
        currentUser->lastBookedRoom = bName + "-" + rID;
        cout << "Success! Room booked.\n";
        people.sendMessage("System", currentUser->username, "You booked " + bName + " " + rID);
    }
}

void UniversitySystem::menuUser() {
    while (true) {
        cout << "\n=== User Directory ===\n1. Register\n2. Login\n3. Update Profile\n4. Delete\n5. Search\n6. Display All\n7. Back\n> ";
        int c; cin >> c; if (c == 7) return;
        switch (c) {
        case 1: { string u, p, r, d, e; cout << "User: "; cin >> u; cout << "Pass: "; cin >> p; cout << "Role: "; cin >> r; cout << "Dept: "; cin >> d; cout << "Email: "; cin >> e; people.registerUser(u, p, r, d, e); break; }
        case 2: { string u, p; cout << "User: "; cin >> u; cout << "Pass: "; cin >> p; HashNode* user = people.getUser(u); if (user && user->password == p) { currentUser = user; cout << "Logged in.\n"; } else cout << "Invalid.\n"; break; }
        case 3: { 
            if (!currentUser) cout << "Login first.\n";
            else { string p, r, d, e; cout << "New Pass: "; cin >> p; cout << "New Role: "; cin >> r; cout << "New Dept: "; cin >> d; cout << "New Email: "; cin >> e; people.updateUser(currentUser->username, p, r, d, e); }
            break; 
        }
        case 4: { string u; cout << "User: "; cin >> u; deleteUserIntegrated(u); break; }
        case 5: { string u; cout << "User: "; cin >> u; cout << (people.getUser(u) ? "Found.\n" : "Not found.\n"); break; }
        case 6: { people.displayAll(); break; }
        }
    }
}

void UniversitySystem::menuMap() {
    while (true) {
        cout << "\n=== Campus Map ===\n1. Add Bldg\n2. Rem Bldg\n3. Add Path\n4. Rem Path\n5. Dijkstra\n6. BFS\n7. DFS\n8. Prim's\n9. Back\n> ";
        int c; cin >> c; if (c == 9) return;
        switch (c) {
        case 1: { string n; cout << "Name: "; cin >> n; campus.addBuilding(n); break; }
        case 2: { string n; cout << "Name: "; cin >> n; deleteBuildingIntegrated(n); break; }
        case 3: { string s, d, t; int w; cout << "Src: "; cin >> s; cout << "Dest: "; cin >> d; cout << "Weight: "; cin >> w; cout << "Type: "; cin >> t; campus.addPath(s, d, w, t); break; }
        case 4: { string s, d; cout << "Src: "; cin >> s; cout << "Dest: "; cin >> d; campus.removePath(s, d); break; }
        case 5: { string s, d; cout << "Src: "; cin >> s; cout << "Dest: "; cin >> d; campus.shortestPath(s, d); break; }
        case 6: { string s; cout << "Start: "; cin >> s; campus.BFS(s); break; }
        case 7: { string s; cout << "Start: "; cin >> s; campus.DFS(s); break; }
        case 8: { campus.PrimsAlgorithm(); break; }
        }
    }
}

void UniversitySystem::menuRooms() {
    string bName; cout << "Building Name: "; cin >> bName;
    BuildingNode* b = campus.getBuilding(bName); if (!b) { cout << "Not found.\n"; return; }
    while (true) {
        cout << "\n=== Rooms (" << bName << ") ===\n1. Insert\n2. Delete\n3. Search\n4. Search Type\n5. Reserve\n6. Cancel\n7. Traversals\n8. Stats\n9. Back\n> ";
        int c; cin >> c; if (c == 9) return;
        switch (c) {
        case 1: { string r, t; int f; cout << "ID: "; cin >> r; cout << "Type: "; cin >> t; cout << "Floor: "; cin >> f; b->rooms->insert(r, t, f); break; }
        case 2: { string r; cout << "ID: "; cin >> r; b->rooms->remove(r); break; }
        case 3: { string r; cout << "ID: "; cin >> r; cout << (b->rooms->search(b->rooms->root, r) ? "Found.\n" : "Not found.\n"); break; }
        case 4: { string t; cout << "Type: "; cin >> t; b->rooms->searchByType(b->rooms->root, t); break; }
        case 5: { if (!currentUser) cout << "Login first.\n"; else { string r; cout << "Room ID: "; cin >> r; reserveRoomLogic(bName, r); } break; }
        case 6: { string r; cout << "Room ID: "; cin >> r; AVLNode* node = b->rooms->search(b->rooms->root, r); if (node && node->isReserved) { node->isReserved = false; node->reservedBy = "None"; cout << "Cancelled.\n"; } break; }
        case 7: { cout << "Inorder: "; b->rooms->printInorder(); break; }
        case 8: { cout << "Leaves: " << b->rooms->countLeaves() << "\nInternal: " << b->rooms->countInternalNodes() << "\n"; break; }
        }
    }
}

void UniversitySystem::menuComplaints() {
    if (!currentUser) { cout << "Login first.\n"; return; }
    while (true) {
        cout << "\n=== Complaints ===\n1. Submit\n2. Process\n3. Display\n4. Back\n> ";
        int c; cin >> c; if (c == 4) return;
        switch (c) {
        case 1: { string b, r, d; cout << "Bldg: "; cin >> b; cout << "Room: "; cin >> r; cin.ignore(); cout << "Desc: "; getline(cin, d); complaints.enqueue(currentUser->username, b, r, d, "Today"); break; }
        case 2: { complaints.dequeue(); cout << "Processed.\n"; break; }
        case 3: { complaints.display(); break; }
        }
    }
}

void UniversitySystem::menuMessaging() {
    if (!currentUser) { cout << "Login first.\n"; return; }
    while (true) {
        cout << "\n=== Messaging ===\n1. Send\n2. View Latest\n3. Pop Latest\n4. Display Conversation\n5. Back\n> ";
        int c; cin >> c; if (c == 5) return;
        switch (c) {
        case 1: { string t, m; cout << "To: "; cin >> t; cin.ignore(); cout << "Msg: "; getline(cin, m); people.sendMessage(currentUser->username, t, m); break; }
        case 2: { string f; cout << "Friend: "; cin >> f; MessageNode* m = people.viewLatestMessage(currentUser->username, f); if (m) cout << "Latest: " << m->messageText << endl; else cout << "None.\n"; break; }
        case 3: { string f; cout << "Friend: "; cin >> f; MessageNode* m = people.popLatestMessage(currentUser->username, f); if (m) { cout << "Deleted: " << m->messageText << endl; delete m; } else cout << "None.\n"; break; }
        case 4: { string f; cout << "Friend: "; cin >> f; people.displayConversation(currentUser->username, f); break; }
        }
    }
}

void UniversitySystem::mainMenu() {
    while (true) {
        cout << "\n================ MAIN MENU ================\n1. Auth\n2. Map\n3. Rooms\n4. Complaints\n5. Messaging\n6. Exit\n===========================================\n";
        cout << "User: " << (currentUser ? currentUser->username : "Guest") << "\n> ";
        int ch; if (!(cin >> ch)) { cin.clear(); cin.ignore(1000, '\n'); continue; }
        if (ch == 1) menuUser(); else if (ch == 2) menuMap(); else if (ch == 3) menuRooms(); else if (ch == 4) menuComplaints(); else if (ch == 5) menuMessaging(); else if (ch == 6) break; else cout << "Invalid.\n";
    }
}
