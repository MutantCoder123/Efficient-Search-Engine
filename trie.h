#ifndef TRIE_H
#define TRIE_H
#include<vector>
#include<string>
#include<cstring>
#include "arena.h"

struct trieNode {
    trieNode* children[26];
    std::vector<int> docID;
    bool isEnd;
};

class invertedIdx {
    private:
        Arena* arena;
        trieNode* root;

        trieNode* createNode() {
            trieNode* node = (trieNode*)arena->alloc(sizeof(trieNode));
            // Fast clear memory
            std::memset(node->children, 0, sizeof(node->children));
            new (&node->docID) std::vector<int>();
            node->isEnd = false;
            return node; 
        }

    public:
        invertedIdx(Arena* mempool) {
            arena = mempool;
            root = createNode();
        }

        void add(const std::string& word, int docID) {
            trieNode* ptr = root;
            for(char c : word) {
                int idx = c - 'a';
                if(idx<0 || idx>25) continue; // Skip weird chars
                
                if(!ptr->children[idx]) {
                    ptr->children[idx] = createNode();
                }
                ptr = ptr->children[idx];
            }
            ptr->isEnd = true;
            // Prevent duplicate docIDs for same word
            if(ptr->docID.empty() || ptr->docID.back()!=docID) {
                ptr->docID.push_back(docID);
            }
        }

        std::vector<int> search(const std::string& word) {
            trieNode* ptr = root;
            for(char c : word) {
                int idx = c - 'a';
                if(!ptr->children[idx]) return {};
                ptr = ptr->children[idx];
            }
            if(ptr->isEnd) return ptr->docID;
            return {};
        }
};
#endif