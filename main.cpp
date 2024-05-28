#include <iostream>
#include <algorithm>
#include <optional>
#include <vector>
#include <random>
using namespace std;

template <typename T, typename E, int L>
class OpenAddressing {
public:
    OpenAddressing(T empty, T deleted) {
        hashTableSize = L;
        numElements = 0;
        emptyKey = empty;
        deletedKey = deleted;

        hashTable = new pair<T, E>[L];
        for (int i = 0; i < L; ++i) {
            hashTable[i].first = emptyKey;
        }

        permutationArray = new vector<int>(L - 1); random_device random;
        iota(permutationArray->begin(), permutationArray->end(), 1);
        shuffle(permutationArray->begin(), permutationArray->end(), mt19937(random()));
    }

    void insert(T key, E value) {
        if (numElements < hashTableSize && !find(key).has_value()) {
            int hashPosition = hashFunction(key);
            int newHashPosition = hashPosition;

            if (hashTable[hashPosition].first != emptyKey && hashTable[hashPosition].first != deletedKey) {
                for (int i = 0; i < hashTableSize - 1 && hashTable[newHashPosition].first != emptyKey && hashTable[newHashPosition].first != deletedKey; ++i) {
                    newHashPosition = (hashPosition + permutationArray->at(i)) % hashTableSize;
                }
                hashPosition = newHashPosition;
            }

            hashTable[hashPosition].first = key; hashTable[hashPosition].second = value;
            numElements++;
        }
    }

    optional<E> remove(T key) {
        int hashPosition = hashFunction(key);
        int newHashPosition = hashPosition;

        for (int i = 0; i < hashTableSize - 1 && hashTable[newHashPosition].first != emptyKey; ++i) {
            if (hashTable[newHashPosition].first == key) {
                E tempValue = hashTable[newHashPosition].second;
                hashTable[newHashPosition].first = deletedKey;
                numElements--;
                return tempValue;
            }
            newHashPosition = (hashPosition + permutationArray->at(i)) % hashTableSize;
        }

        return nullopt;
    }

    optional<E> find(T key) {
        int hashPosition = hashFunction(key);
        int newHashPosition = hashPosition;

        for (int i = 0; i < hashTableSize - 1 && hashTable[newHashPosition].first != emptyKey; ++i) {
            if (hashTable[newHashPosition].first == key) {
                return hashTable[newHashPosition].second;
            }
            newHashPosition = (hashPosition + permutationArray->at(i)) % hashTableSize;
        }

        return nullopt;
    }

    int getNumElements() {
        return numElements;
    }

    vector<int>* getPermutationArray() {
        return permutationArray;
    }

    void print() {
        for (int i = 0; i < hashTableSize; ++i) {
            cout << "OpenAddressing[" << i << "] =";
            if (hashTable[i].first != emptyKey && hashTable[i].first != deletedKey) {
                cout << " {" << hashTable[i].first << ", " << hashTable[i].second << "}";
            }
            cout << "\n";
        }
    }

    void printPermutationArray() {
        for (int i = 0; i < hashTableSize - 1; ++i) {
            cout << "permutationArray[" << i << "] = " << permutationArray->at(i) << "\n";
        }
    }

    void clear() {
        numElements = 0;
        for (int i = 0; i < L; ++i) {
            hashTable[i].first = emptyKey;
        }
    }

private:
    int hashTableSize;
    int numElements;
    T emptyKey;
    T deletedKey;
    pair<T, E>* hashTable;

    vector<int>* permutationArray;

    int hashFunction(T key) {
        int modulus = key % hashTableSize;
        return modulus < 0 ? modulus + hashTableSize : modulus;
    }
};

int main() {
    return 0;
}
