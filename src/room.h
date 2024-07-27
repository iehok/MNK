#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <set>

using namespace std;

class Room {
   public:
    set<pair<int, int>> u;
    int score;
    bool operator<(const Room& a) const { return score > a.score; };
    void echo();
};

void Room::echo() {
    for (decltype(u)::iterator itr = u.begin(); itr != u.end(); itr++) printf("%d(%d) ", (*itr).first, (*itr).second);
    printf("\n");
}

#endif
