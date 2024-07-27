#ifndef FLOOR_H_
#define FLOOR_H_

#include <vector>

#include "room.h"

using namespace std;

class Floor {
   public:
    vector<Room> floor;
    void insert(Room room);
};

void Floor::insert(Room room) {
    for (int i = 0; i < floor.size(); i++) {
        if (floor[i].u == room.u) {
            floor[i].score++;
            return;
        }
    }
    floor.push_back(room);
}

#endif
