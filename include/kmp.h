#ifndef KMP_H
#define KMP_H
#include<vector>
#include<string>
#include<algorithm>
#include<cctype> 

// Quick helper to lower case a string
std::string toLowerStr(std::string s) {
    for(auto &c : s) c = tolower(c);
    return s;
}

// O(k) lps calculation
void makeLps(std::string &pt, std::vector<int> &lps, int k) {
    int len = 0;
    int i = 1;
    lps[0] = 0;

    while(i<k) {
        if(pt[i]==pt[len]) {
            len++;
            lps[i] = len;
            i++;
        }
        else {
            if(len!=0) {
                len = lps[len-1]; // fallback
            }
            else {
                lps[i] = 0;
                i++;
            }
        }
    }
}
// KMP ALGORITHM:
// Efficiently searches for the query inside the document text.
// Unlike naive search (O(N*M)), KMP uses a pre-computed table
// to skip unnecessary comparisons, achieving O(N+M) complexity.
std::string getSnippet(std::string s, std::string p) {
    // Use lowercase copies for matching
    std::string sLow = toLowerStr(s);
    std::string pLow = toLowerStr(p);

    int n = sLow.size();
    int k = pLow.size();
    if(k==0) return "";

    std::vector<int> lps(k, 0);
    makeLps(pLow, lps, k);

    int j = 0;
    int shift = -1;

    for(int i=0; i<n; i++) {
        // Compare the lowercase versions
        if(sLow[i]==pLow[j]) {
            j++;
        }
        else {
            if(j!=0) {
                j = lps[j-1];
                i--;
            }
        }
        if (j == k) {
        if ( ((i == n - 1) || !isalnum(s[i + 1])) && 
                ((i - j < 0)  || !isalnum(s[i - j])) ) {
             
            shift = (i - j) + 1;
            break; // found it
        }
        j = lps[j-1]; // Reset to keep searching if not a whole word
    }
    }

    if(shift!=-1) {
        // Grab the substring from the ORIGINAL 's' to keep formatting
        int start = std::max(0, shift-30);
        int end = std::min(n, shift+k+50);
        while(start>0 && s[start]!=' ')     start--;
        while(end<n && s[end]!=' ')     end++;
        return "..." + s.substr(start, end-start) + "...";
    }
    return "";
}
#endif