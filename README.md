# CampusLink: Integrated University Management System

CampusLink is a comprehensive C++ application designed to manage university operations through the strategic implementation of advanced Data Structures and Algorithms (DSA). This project serves as a practical demonstration of complex data management, pathfinding, and hierarchical organization.

## 🚀 Key Features

- **User Authentication**: Secure registration and login system with role-based access (Admin/Student).
- **Interactive Campus Map**: Graphical representation of the campus with pathfinding capabilities.
- **Room Scheduling**: Efficient management of room bookings and availability across different buildings.
- **Complaint Management**: A systematic queue-based approach to raising and processing campus-wide complaints.
- **Messaging System**: Intra-campus communication with conversation history between users.

## 🛠️ Data Structures & Algorithms Implementation

This project showcases the following core DSA concepts:

| Module | Data Structure | Purpose |
| :--- | :--- | :--- |
| **People Directory** | **Hash Table** | Provides $O(1)$ average time complexity for user lookup and authentication. Includes dynamic resizing. |
| **Campus Map** | **Graph (Adjacency List)** | Model of the campus layout. Implements **Dijkstra's Algorithm** for shortest paths and **Prim's Algorithm** for Minimum Spanning Trees (MST). |
| **Room Management** | **AVL Tree** | Maintains a self-balancing binary search tree of rooms for $O(\log n)$ search, insertion, and deletion. |
| **Complaints** | **Queue** | Handles complaints in a First-In-First-Out (FIFO) manner. |
| **Messaging** | **Stack** | Stores message history for each conversation, prioritising the most recent communication. |
| **Navigation** | **BFS & DFS** | Implements Breadth-First and Depth-First search for graph traversal and connectivity checks. |

## 📁 Project Structure

The project has been refactored into a professional 3-file architecture:

- `CampusLink.h`: Contains all class declarations, structures, and function prototypes.
- `CampusLink.cpp`: Contains the logic and implementation for all modules.
- `main.cpp`: The entry point with the user interface and menu system.

## 🛠️ Setup & Execution

### Prerequisites
- A C++ compiler (MSVC, GCC, or Clang)
- Git (for repository management)

### Compilation (Standard CLI)
```bash
g++ -o CampusLink main.cpp CampusLink.cpp
```

### Visual Studio
Simply open the provided `.vcxproj` file in Visual Studio, and the project will be configured automatically.

## 📝 Usage

Upon running the application, you will be presented with a menu-driven interface:
1. **Auth**: Manage users and log in.
2. **Map**: Explore the campus, find shortest paths, or analyze the map structure.
3. **Rooms**: Manage room availability and bookings within buildings.
4. **Complaints**: Submit and process campus maintenance requests.
5. **Messaging**: Communicate with other registered users.

## 📜 License
This project is licensed under the MIT License.
