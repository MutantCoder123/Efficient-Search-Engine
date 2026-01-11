/**
 * @file main.cpp
 * @author Indranil Saha
 * @brief Efficient Search Engine using Trie and KMP Algorithm.
 * @version 1.1 (UI Update)
 * @date 11/01/2026
 */
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include<chrono>
#include<cmath>
#include<iomanip>
#include "arena.h"
#include "trie.h"
#include "kmp.h"

using namespace std;

// --- UI / Formatting Constants ---
const string RESET  = "\033[0m";
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE   = "\033[34m";
const string MAGENTA= "\033[35m";
const string CYAN   = "\033[36m";
const string BOLD   = "\033[1m";
const string DIM    = "\033[2m";

void printHeader() {
    cout << "\n" << BOLD << CYAN 
         << "========================================\n"
         << "|--------SEARCH ENGINE v1.1 (CLI)------|\n"
         << "========================================" << RESET << "\n";
}

void printSeparator() {
    cout << DIM << "----------------------------------------" << RESET << "\n";
}

struct Document {
    int id;
    string name;
    string content;
    int num_words;
};

// Helper to read text files
string readFile(const string& path) {
    ifstream file(path);
    if(!file.is_open()) return "";
    stringstream buf;
    buf << file.rdbuf();
    return buf.str();
}

// Assign TF*IDF score to documents where the word is found and sort them based on score
vector<int> ranking(vector<pair<int,double>> &v, vector<Document> &corpus) {
    if (v.empty() || corpus.empty()) return {};
    double doc_feq = (double)v.size();
    double num_words;
    double TF;
    double feq;
    double IDF;
    double score;
    for(auto &x : v) {
        // Calculating score for each docID
        feq = x.second;
        num_words = corpus[x.first-1].num_words;
        TF = feq/num_words;
        IDF = (double)log10((double)corpus.size()/doc_feq);
        score = TF*IDF;
        x.second = score;
    }
    sort(v.begin(), v.end(), [](auto &a, auto &b) { return a.second > b.second; });
    
    // Only considering the top 10 results
    int cnt = min(10,(int)v.size());
    vector<int> sorted(cnt);
    for(int i=0; i<cnt; i++) {
        sorted[i] = v[i].first;
    }
    return sorted;
}

int main() {
    printHeader();

    // 1. System init
    size_t arenaSize = 50 * 1024 * 1024;
    Arena myArena(arenaSize);
    cout << YELLOW << "[SYS]--- " << RESET << "Arena Ready: " << BOLD << (arenaSize/1024/1024) << " MB" << RESET << "\n";

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

    cout << YELLOW << "[SYS]--- " << RESET << "Reading files...\n";
    int docId = 1;
    for(const auto& f : files) {
        string c = readFile(f);
        if(c.empty()) {
            cout << RED << "!!! [WARN] Skipped " << f << RESET << "\n";
            continue;
        }
        corpus.push_back({docId, f, c, 0});
        docId++;
    }

    // 3. Indexing into prefix tree
    auto startIdx = chrono::high_resolution_clock::now();
    for(auto& doc : corpus) {
        string word = "";
        int cnt = 0;
        for(char x : doc.content) {
            if(isalnum(x)) {
                word += tolower(x);
            }
            else {
                if(!word.empty()) {
                    index.add(word, doc.id);
                    word = "";
                    cnt++;
                }
            }
        }
        if(!word.empty()) {
            index.add(word, doc.id);
            cnt++;
        }
        doc.num_words = cnt;    // Store the word count in a document
    }
    auto stopIdx = chrono::high_resolution_clock::now();
    auto durIdx = chrono::duration_cast<chrono::microseconds>(stopIdx - startIdx);

    cout << YELLOW << "[SYS]--- " << RESET << "Indexing done in " << BOLD << durIdx.count() << " us (microsecond)" << RESET << "\n";
    cout << YELLOW << "[SYS]--- " << RESET << "RAM used: " << BOLD << myArena.getUsed() << " bytes" << RESET << "\n";
    printSeparator();

    // 4. Search loop
    while(true) {
        cout << "\n" << GREEN << "->> Enter query " << RESET << "(or '" << RED << "exit" << RESET << "' to quit): ";
        string q;
        cin >> q;
        if(q == "exit") break;

        auto start = chrono::high_resolution_clock::now();
        vector<pair<int,double>> res = index.search(q);
        auto stop = chrono::high_resolution_clock::now();
        auto dur = chrono::duration_cast<chrono::microseconds>(stop - start);

        if(res.empty()) {
            cout << RED << " --> X -- No match found." << RESET << "\n";
        }
        else {
            // Sorting the documents based on TF*IDF score
            vector<int> ranked = ranking(res, corpus); 
            
            cout << "\n" << BOLD << "   Found " << res.size() << " results in " << dur.count() << " us (microsecond)" << RESET << "\n";
            printSeparator();
            
            // Loop through ranked results
            // Note: 'res' was sorted in ranking(), so res[i] corresponds to ranked[i]
            for(int i = 0; i < ranked.size(); i++) {
                int id = ranked[i];
                double score = res[i].second;

                // Get details
                string name = corpus[id-1].name;
                string text = corpus[id-1].content;
                string snip = getSnippet(text, q);

                // Beautiful Output Block
                cout << " " << BOLD << i+1 << ". " << CYAN << name << RESET 
                     << "  " << MAGENTA << "[Score: " << fixed << setprecision(5) << score << "]" << RESET << "\n";
                
                if(!snip.empty()) {
                    cout << DIM << "    \"..." << snip << "...\"" << RESET << "\n";
                }
                cout << "\n";
            }
            printSeparator();
        }
    }
    
    cout << "\n" << BOLD << CYAN << "Goodbye!" << RESET << "\n";
    return 0;
}