#include "backtracking.h"
#include "../Timetable/timetable.h"

void Timetable::resizeUndoStack() {
    int newCap = undoCapacity * 2;
    UndoItem* newStack = new UndoItem[newCap];
    for (int i = 0; i <= top; i++) {
        newStack[i] = undoStack[i];
    }
    delete[] undoStack;
    undoStack = newStack;
    undoCapacity = newCap;
}
void Timetable::pushUndo(int secIdx, int day, int slot, int duration) {
    if (top >= undoCapacity - 1) {
        resizeUndoStack();
    }
    top++;
    undoStack[top] = UndoItem(secIdx, day, slot, duration);
}

void Timetable::pushCombinedUndo(const int secIndices[], int count, int day, int slot, int duration) {
    if (top >= undoCapacity - 1) {
        resizeUndoStack();
    }
    top++;
    undoStack[top] = UndoItem(secIndices, count, day, slot, duration);
}

void Timetable::undo() {
    if (top < 0) return;

    UndoItem item = undoStack[top--];
    int d = item.day;
    int s = item.slot;
    int dur = item.duration;

    for (int c = 0; c < item.sectionIndicesCount; c++) {
        int sec = item.sectionIndices[c];
        if (sec >= 0 && sec < sectionCount && d >= 0 && d < DAYS) {
            for (int k = 0; k < dur && (s + k) < SLOTS; k++) {
                sections[sec].grid[d][s + k] = SchedulingRequest();
                sections[sec].occupied[d][s + k] = false;
            }
        }
    }
}
bool Timetable::backtrackSchedule(int requestIndex) {
    if (requestIndex == totalRequests) {
        return true;
    }

    SchedulingRequest currentReq = requestList[requestIndex];
    int targetSecIndices[20];
    int targetSecCount = 0;
    int calculatedStrength = 0;

    if (currentReq.isCombined && currentReq.sectionNamesCount > 0) {
        for (int i = 0; i < currentReq.sectionNamesCount && targetSecCount < 20; i++) {
            int idx = getSectionIndex(currentReq.sectionNames[i]);
            if (idx == -1) {
                idx = addSection(currentReq.sectionNames[i]);
            }
            if (idx != -1) {
                targetSecIndices[targetSecCount++] = idx;
                calculatedStrength += sections[idx].studentStrength;
            }
        }
    } else {
        int idx = getSectionIndex(currentReq.sectionName);
        if (idx == -1) {
            idx = addSection(currentReq.sectionName);
        }
        if (idx != -1) {
            targetSecIndices[0] = idx;
            targetSecCount = 1;
            calculatedStrength = sections[idx].studentStrength;
        }
    }

    if (targetSecCount == 0) return false;
    int requiredCapacity = calculatedStrength;
    if (currentReq.studentStrength > requiredCapacity) {
        requiredCapacity = currentReq.studentStrength;
    }
    for (int pass = 0; pass < 2; pass++) {
        for (int day = 0; day < workingDays; day++) {
            if (pass == 0 && !currentReq.isLab) {
                bool alreadyOnDay = false;
                for (int s = 0; s < targetSecCount; s++) {
                    if (subjectAlreadyOnDay(targetSecIndices[s], day, currentReq.subjectCode)) {
                        alreadyOnDay = true;
                        break;
                    }
                }
                if (alreadyOnDay) continue;
            }

            for (int slot = 0; slot < periodsPerDay; slot++) {
                bool validSlot = true;
                for (int s = 0; s < targetSecCount; s++) {
                    if (!isValidSlotBasic(targetSecIndices[s], day, slot, currentReq.duration, currentReq.facultyName)) {
                        validSlot = false;
                        break;
                    }
                }
                if (!validSlot) continue;
                if (roomCount > 0) {
                    if (!currentReq.roomName.empty() && currentReq.roomName != "TBD") {
                        for (int r = 0; r < roomCount; r++) {
                            if (availableRooms[r].id == currentReq.roomName) {
                                if (isRoomSuitableAndFree(r, day, slot, currentReq.duration, currentReq.isLab, requiredCapacity)) {
                                    SchedulingRequest placedReq = currentReq;
                                    placedReq.roomName = availableRooms[r].id;
                                    placedReq.studentStrength = requiredCapacity;

                                    if (currentReq.isCombined) {
                                        addCombinedClass(targetSecIndices, targetSecCount, day, slot, placedReq);
                                    } else {
                                        addClass(targetSecIndices[0], day, slot, placedReq);
                                    }

                                    if (backtrackSchedule(requestIndex + 1)) return true;
                                    undo();
                                }
                                break;
                            }
                        }
                    } else {
                        for (int r = 0; r < roomCount; r++) {
                            if (isRoomSuitableAndFree(r, day, slot, currentReq.duration, currentReq.isLab, requiredCapacity)) {
                                SchedulingRequest placedReq = currentReq;
                                placedReq.roomName = availableRooms[r].id;
                                placedReq.studentStrength = requiredCapacity;

                                if (currentReq.isCombined) {
                                    addCombinedClass(targetSecIndices, targetSecCount, day, slot, placedReq);
                                } else {
                                    addClass(targetSecIndices[0], day, slot, placedReq);
                                }
                                if (backtrackSchedule(requestIndex + 1)) {
                                    return true;
                                }
                                undo();
                            }
                        }
                    }
                } else {
                    if (!currentReq.roomName.empty() && currentReq.roomName != "TBD") {
                        bool roomConflict = false;
                        for (int k = 0; k < currentReq.duration; k++) {
                            if (hasRoomConflict(day, slot + k, currentReq.roomName)) {
                                roomConflict = true;
                                break;
                            }
                        }
                        if (!roomConflict) {
                            if (currentReq.isCombined) {
                                addCombinedClass(targetSecIndices, targetSecCount, day, slot, currentReq);
                            } else {
                                addClass(targetSecIndices[0], day, slot, currentReq);
                            }
                            if (backtrackSchedule(requestIndex + 1)) return true;
                            undo();
                        }
                    }
                }
            }
        }
    }
    return false;
}
