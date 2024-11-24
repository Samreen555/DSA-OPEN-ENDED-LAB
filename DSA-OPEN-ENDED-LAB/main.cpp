#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#include <cstdio>

using namespace std;
namespace fs = std::experimental::filesystem;
struct Node {
    string playerName;
    int odiMatches, t20Matches, testMatches;
    Node* next;
    Node* previous;

    Node(const string& name, int odi = 0, int t20 = 0, int test = 0)
        : playerName(name), odiMatches(odi), t20Matches(t20), testMatches(test), next(nullptr), previous(nullptr) {}
};

class doubly_linked_list {
private:
    Node* head;
    Node* tail;

        void writeToFile(Node* node) {
        string fileName = node->playerName + ".txt";

        // Remove the old file (optional, but ensures consistency)
        remove(fileName.c_str());

        // Create a new file with updated data
        ofstream outFile(fileName);
        if (!outFile.is_open()) {
            cerr << "Error: Could not create file for player: " << node->playerName << endl;
            return;
        }

        outFile << "Player: " << node->playerName << endl;
        outFile << "ODI Matches: " << node->odiMatches << endl;
        outFile << "T20 Matches: " << node->t20Matches << endl;
        outFile << "Test Matches: " << node->testMatches << endl;
        outFile.close();
        cout << "File successfully updated for player: " << node->playerName << endl;
    }
    void readPlayerFromFile(const string& fileName) {
        cout << "Reading file: " << fileName << endl;

        ifstream inFile(fileName);
        if (!inFile.is_open()) {
            cerr << "Error: Could not read file: " << fileName << endl;
            return;
        }

        string line, name;
        int odi = 0, t20 = 0, test = 0;
        bool validFile = false;

        while (getline(inFile, line)) {
            //cout << "Processing line: " << line << endl; // Debug print

            if (line.find("Player:") == 0) {
                name = line.substr(line.find(":") + 2);
                validFile = true;
            }
            else if (line.find("ODI Matches:") == 0) {
                odi = stoi(line.substr(line.find(":") + 2));
            }
            else if (line.find("T20 Matches:") == 0) {
                t20 = stoi(line.substr(line.find(":") + 2));
            }
            else if (line.find("Test Matches:") == 0) {
                test = stoi(line.substr(line.find(":") + 2));
            }
        }

        if (validFile) {
            cout << "Adding player: " << name << " to the list." << endl;
            insert_player(name, odi, t20, test);
        }
        else {
            cerr << "Error: Invalid file format for " << fileName << endl;
        }

        inFile.close();
    }


public:
    doubly_linked_list() : head(nullptr), tail(nullptr) {}

    ~doubly_linked_list() {
        Node* current = head;
        while (current) {
            Node* nextNode = current->next;
            delete current;
            current = nextNode;
        }
    }

    void insert_player(const string& name, int odi = 0, int t20 = 0, int test = 0) {
        Node* newNode = new Node(name, odi, t20, test);
        if (!head) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            newNode->previous = tail;
            tail = newNode;
        }
        writeToFile(newNode); // Write player data to a file
        cout << "Player " << name << " added successfully." << endl;
                cout << "--------\n";

    }

    Node* find_player(const string& name) {
        Node* current = head;
        while (current) {
            if (current->playerName == name)
                return current;
            current = current->next;
        }
        return nullptr;
    }

    void update_player(const string& name, int odi = 0, int t20 = 0, int test = 0) {
        Node* player = find_player(name);
        if (player) {
            // Update player data
            player->odiMatches += odi;
            player->t20Matches += t20;
            player->testMatches += test;

            // Write updated data to the file
            writeToFile(player);

            cout << "Player " << name << " updated successfully." << endl;
        }
        else {
            cout << "Player " << name << " not found." << endl;
        }
    }

    void delete_player(const string& name) {
        Node* player = find_player(name);
        if (!player) {
            cout << "Player " << name << " not found." << endl;
            return;
        }

        if (player == head) {
            head = head->next;
            if (head)
                head->previous = nullptr;
        }
        else if (player == tail) {
            tail = tail->previous;
            if (tail)
                tail->next = nullptr;
        }
        else {
            player->previous->next = player->next;
            player->next->previous = player->previous;
        }

        if (remove((name + ".txt").c_str()) == 0) {
            cout << "File for player " << name << " deleted successfully." << endl;
        }
        else {
            cout << "Error deleting file for player " << name << "." << endl;
        }

        delete player;
        cout << "Player " << name << " deleted successfully." << endl;
    }

   void display_all_players() {
    Node* current = head;
    if (!current) {
        cout << "No players found." << endl;
        return;
    }

    while (current) {
        cout << "Player: " << current->playerName
            << ", ODI: " << current->odiMatches
            << ", T20: " << current->t20Matches
            << ", Test: " << current->testMatches << endl;
        current = current->next;
    }
}
};

int getValidatedInput(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear(); // Clear the error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard invalid input
            cout << "Invalid input. Please enter a valid number." << endl;
        }
        else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard any extra input
            return value; // Return the valid input
        }
    }
}

void displayMenu() {
    cout << "\n=== Cricket Team Management ===" << endl;
    cout << "1. Add Player" << endl;
    cout << "2. Update Player Matches" << endl;
    cout << "3. Delete Player" << endl;
    cout << "4. Display All Players" << endl;
    cout << "5. Exit" << endl;
}

int main() {
    doubly_linked_list team;
    int odi, t20, test;
    string name, choice;

    do {
        displayMenu();
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == "1") {
            cout << "Enter player name: ";
            cin.ignore();
            getline(cin, name);
            odi = getValidatedInput("Enter ODI matches: ");
            t20 = getValidatedInput("Enter T20 matches: ");
            test = getValidatedInput("Enter Test matches: ");
            team.insert_player(name, odi, t20, test);
        }
        else if (choice == "2") {
            cout << "Enter player name: ";
            cin.ignore();
            getline(cin, name);
            odi = getValidatedInput("Enter ODI matches to add: ");
            t20 = getValidatedInput("Enter T20 matches to add: ");
            test = getValidatedInput("Enter Test matches to add: ");
            team.update_player(name, odi, t20, test);
        }
        else if (choice == "3") {
            cout << "Enter player name: ";
            cin.ignore();
            getline(cin, name);
            team.delete_player(name);
        }
        else if (choice == "4") {
            team.loadExistingPlayers();
            team.display_all_players();
        }
        else if (choice == "5") {
            cout << "Exiting..." << endl;
            break;
        }
        else {
            cout << "Invalid choice. Please try again." << endl;
        }
    } while (true);

    return 0;
}
