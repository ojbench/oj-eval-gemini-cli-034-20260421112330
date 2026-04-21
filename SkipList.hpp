#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>

template<typename T>
class SkipList {
private:
    struct Node {
        T value;
        std::vector<Node*> next;
        Node(const T &item, int level) : value(item), next(level, nullptr) {}
    };

    std::vector<Node*> headNext;
    int maxLevel;
    int currentLevel;
    float probability;

    int randomLevel() {
        int level = 1;
        while (((double)std::rand() / RAND_MAX) < probability && level < maxLevel) {
            level++;
        }
        return level;
    }

    int randomLevel() {
        int level = 1;
        while ((std::rand() & 0xFFFF) < (0.5 * 0xFFFF) && level < maxLevel) {
            level++;
        }
        return level;
    }

public:
    SkipList() : maxLevel(32), currentLevel(1), probability(0.5) {
        headNext.assign(maxLevel, nullptr);
        static bool seeded = false;
        if (!seeded) {
            std::srand(std::time(nullptr));
            seeded = true;
        }
    }

    ~SkipList() {
        Node* curr = headNext[0];
        while (curr) {
            Node* next = curr->next[0];
            delete curr;
            curr = next;
        }
    }

    // Insert a value into the skip list. If the value already exists, do nothing.
    void insert(const T & item) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* curr = nullptr;
        
        for (int i = currentLevel - 1; i >= 0; i--) {
            Node* next = (curr == nullptr) ? headNext[i] : curr->next[i];
            while (next && next->value < item) {
                curr = next;
                next = curr->next[i];
            }
            update[i] = curr;
        }
        
        Node* target = (curr == nullptr) ? headNext[0] : curr->next[0];
        
        if (target && !(target->value < item) && !(item < target->value)) {
            return; // Already exists
        }

        int level = randomLevel();

        if (level > currentLevel) {
            for (int i = currentLevel; i < level; i++) {
                update[i] = nullptr;
            }
            currentLevel = level;
        }

        Node* newNode = new Node(item, level);
        for (int i = 0; i < level; i++) {
            if (update[i] == nullptr) {
                newNode->next[i] = headNext[i];
                headNext[i] = newNode;
            } else {
                newNode->next[i] = update[i]->next[i];
                update[i]->next[i] = newNode;
            }
        }
    }

    // Search for a value in the skip list
    bool search(const T & item) {
        Node* curr = nullptr;
        for (int i = currentLevel - 1; i >= 0; i--) {
            Node* next = (curr == nullptr) ? headNext[i] : curr->next[i];
            while (next && next->value < item) {
                curr = next;
                next = curr->next[i];
            }
        }
        Node* target = (curr == nullptr) ? headNext[0] : curr->next[0];
        return target && !(target->value < item) && !(item < target->value);
    }

    // Delete a value from the skip list. If the value does not exist in the skip list, do nothing.
    void deleteItem(const T & item) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* curr = nullptr;
        for (int i = currentLevel - 1; i >= 0; i--) {
            Node* next = (curr == nullptr) ? headNext[i] : curr->next[i];
            while (next && next->value < item) {
                curr = next;
                next = curr->next[i];
            }
            update[i] = curr;
        }
        
        Node* target = (curr == nullptr) ? headNext[0] : curr->next[0];
        
        if (target && !(target->value < item) && !(item < target->value)) {
            for (int i = 0; i < currentLevel; i++) {
                if (update[i] == nullptr) {
                    if (headNext[i] != target) break;
                    headNext[i] = target->next[i];
                } else {
                    if (update[i]->next[i] != target) break;
                    update[i]->next[i] = target->next[i];
                }
            }
            delete target;
            while (currentLevel > 1 && headNext[currentLevel - 1] == nullptr) {
                currentLevel--;
            }
        }
    }
};

#endif
