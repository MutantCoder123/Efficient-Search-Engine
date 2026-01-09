/**
 * @file main.cpp
 * @author Indranil Saha
 * @brief Efficient Search Engine using Trie and KMP Algorithm.
 * @version 1.0
 * @date 2026-01-09
 * * Features:
 * - Custom Memory Arena (No malloc/free overhead)
 * - Boolean Search with Prefix Matching
 * - Context-aware snippet generation
 */
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<chrono>
#include "arena.h"
#include "trie.h"
#include "kmp.h"

using namespace std;

struct Document {
    int id;
    string name;
    string content;
};

// Helper to read text files
string readFile(const string& path) {
    ifstream file(path);
    if(!file.is_open()) return "";
    stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

int main() {
    cout<<"--- SEARCH ENGINE v1 ---\n";

    // 1. System init
    // 50mb arena for real files
    size_t arenaSize = 50 * 1024 * 1024;
    Arena myArena(arenaSize);
    cout<<"[SYS] Arena Ready: "<<(arenaSize/1024/1024)<<" MB\n";

    invertedIdx index(&myArena);
    vector<Document> corpus;

    // 2. Load data
    vector<string> files = {
        "data/AI_taking_works.txt", 
        "data/boy_and_wolf.txt",
        "data/linux_guide.txt",  
        "data/news.txt",
        "data/sherlock.txt", 
        "data/tortoise.txt"
    };

    cout<<"[SYS] Reading files...\n";
    int docId = 1;
    for(const auto& f : files) {
        string c = readFile(f);
        if(c.empty()) {
            cout<<"[WARN] Skipped "<<f<<"\n";
            continue;
        }
        corpus.push_back({docId, f, c});
        docId++;
    }

    // 3. Indexing
    auto startIdx = chrono::high_resolution_clock::now();
    for(const auto& doc : corpus) {
        string word = "";
        for(char x : doc.content) {
            if(isalnum(x)) {
                word += tolower(x);
            }
            else {
                if(!word.empty()) {
                    index.add(word, doc.id);
                    word = "";
                }
            }
        }
        if(!word.empty()) index.add(word, doc.id);
    }
    auto stopIdx = chrono::high_resolution_clock::now();
    auto durIdx = chrono::duration_cast<chrono::microseconds>(stopIdx - startIdx);

    cout<<"[SYS] Indexing done in "<<durIdx.count()<<" us\n";
    cout<<"[SYS] Ram used: "<<myArena.getUsed()<<" bytes\n";

    // 4. Search loop
    while(true) {
        cout<<"\nEnter query (exit to quit): ";
        string q;
        cin>>q;
        if(q=="exit") break;

        auto start = chrono::high_resolution_clock::now();
        vector<int> res = index.search(q);
        auto stop = chrono::high_resolution_clock::now();
        auto dur = chrono::duration_cast<chrono::microseconds>(stop - start);

        if(res.empty()) {
            cout<<"No match found.\n";
        }
        else {
            cout<<"Found in "<<res.size()<<" docs ("<<dur.count()<<" us):\n";
            int last = -1;
            for(int id : res) {
                if(id==last) continue;
                last = id;

                // Get details
                string name = ""; 
                string text = "";
                for(auto& d : corpus) {
                    if(d.id==id) {
                        name = d.name;
                        text = d.content;
                        break;
                    }
                }
                cout<<" -> [Doc "<<id<<"] "<<name<<"\n";
                string snip = getSnippet(text, q);
                if(!snip.empty()) cout<<"    Snippet: "<<snip<<"\n";
            }
        }
    }
    return 0;
}