#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

class Room {
public:
    string name;
    string description;
    map<string, string> exits; // direction -> room name
    vector<string> items;
    bool locked;
    string requiredItemToUnlock;

    Room() : locked(false) {}
    Room(string n, string d) : name(n), description(d), locked(false) {}
};

class Game {
private:
    map<string, Room> rooms;
    string currentRoom;
    vector<string> inventory;
    bool gameOver;

public:
    Game() : gameOver(false) {
        setupWorld();
        currentRoom = "entrance";
    }

    void setupWorld() {
        Room entrance("entrance", "You stand at the entrance of a dark castle. A cold wind blows.");
        entrance.exits["north"] = "hall";
        entrance.items.push_back("torch");

        Room hall("hall", "A grand hall with dusty portraits. You hear echoes.");
        hall.exits["south"] = "entrance";
        hall.exits["east"] = "library";
        hall.exits["west"] = "armory";

        Room library("library", "Shelves of ancient books line the walls.");
        library.exits["west"] = "hall";
        library.items.push_back("key");

        Room armory("armory", "Rusty weapons hang on the walls. A locked door blocks the north path.");
        armory.exits["east"] = "hall";
        armory.exits["north"] = "treasure_room";
        armory.locked = true;
        armory.requiredItemToUnlock = "key";

        Room treasureRoom("treasure_room", "Gold and jewels sparkle everywhere. You found the treasure!");
        treasureRoom.items.push_back("treasure");

        rooms["entrance"] = entrance;
        rooms["hall"] = hall;
        rooms["library"] = library;
        rooms["armory"] = armory;
        rooms["treasure_room"] = treasureRoom;
    }

    bool hasItem(const string& item) {
        return find(inventory.begin(), inventory.end(), item) != inventory.end();
    }

    void showRoom() {
        Room& room = rooms[currentRoom];
        cout << "\n--- " << room.name << " ---\n";
        cout << room.description << endl;

        if (!room.items.empty()) {
            cout << "Items here: ";
            for (const auto& item : room.items) cout << item << " ";
            cout << endl;
        }

        cout << "Exits: ";
        for (const auto& [dir, dest] : room.exits) {
            cout << dir << " ";
        }
        cout << endl;
    }

    void move(const string& direction) {
        Room& room = rooms[currentRoom];
        if (room.exits.find(direction) == room.exits.end()) {
            cout << "You can't go that way.\n";
            return;
        }

        string destName = room.exits[direction];
        Room& dest = rooms[destName];

        if (dest.locked) {
            if (hasItem(dest.requiredItemToUnlock)) {
                cout << "You use the " << dest.requiredItemToUnlock << " to unlock the door.\n";
                dest.locked = false;
            } else {
                cout << "The door is locked. You need something to unlock it.\n";
                return;
            }
        }

        currentRoom = destName;
        if (currentRoom == "treasure_room") {
            cout << "\nCongratulations! You found the treasure and won the game!\n";
            gameOver = true;
        }
    }

    void take(const string& item) {
        Room& room = rooms[currentRoom];
        auto it = find(room.items.begin(), room.items.end(), item);
        if (it != room.items.end()) {
            inventory.push_back(item);
            room.items.erase(it);
            cout << "You picked up: " << item << endl;
        } else {
            cout << "That item isn't here.\n";
        }
    }

    void showInventory() {
        if (inventory.empty()) {
            cout << "Your inventory is empty.\n";
            return;
        }
        cout << "Inventory: ";
        for (const auto& item : inventory) cout << item << " ";
        cout << endl;
    }

    void showHelp() {
        cout << "\nCommands:\n";
        cout << "  go <direction>  - move (e.g., go north)\n";
        cout << "  take <item>     - pick up an item\n";
        cout << "  inventory       - show your items\n";
        cout << "  look            - show room description\n";
        cout << "  help            - show this help\n";
        cout << "  quit            - exit the game\n";
    }

    bool isGameOver() { return gameOver; }

    void processCommand(const string& command) {
        if (command.substr(0, 3) == "go ") {
            move(command.substr(3));
        } else if (command.substr(0, 5) == "take ") {
            take(command.substr(5));
        } else if (command == "inventory") {
            showInventory();
        } else if (command == "look") {
            showRoom();
        } else if (command == "help") {
            showHelp();
        } else if (command == "quit") {
            gameOver = true;
            cout << "Thanks for playing!\n";
        } else {
            cout << "Unknown command. Type 'help' for a list of commands.\n";
        }
    }
};

int main() {
    Game game;
    string command;

    cout << "===== Castle Adventure =====\n";
    game.showHelp();
    game.showRoom();

    while (!game.isGameOver()) {
        cout << "\n> ";
        getline(cin, command);
        game.processCommand(command);
    }

    return 0;
}
