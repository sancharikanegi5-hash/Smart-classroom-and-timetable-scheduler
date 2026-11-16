#ifndef BACKTRACKING_H
#define BACKTRACKING_H
struct UndoItem {
    int sectionIndices[20];
    int sectionIndicesCount;
    int day;
    int slot;
    int duration;

    UndoItem() {
        sectionIndicesCount = 0;
        day = -1;
        slot = -1;
        duration = 1;
    }
    UndoItem(int sec, int d, int s, int dur) {
        sectionIndicesCount = 1;
        sectionIndices[0] = sec;
        day = d;
        slot = s;
        duration = dur;
    }
    UndoItem(const int secs[], int secCount, int d, int s, int dur) {
        sectionIndicesCount = secCount;
        for (int i = 0; i < secCount && i < 20; i++) {
            sectionIndices[i] = secs[i];
        }
        day = d;
        slot = s;
        duration = dur;
    }
};

#endif
