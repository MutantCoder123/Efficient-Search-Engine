#ifndef KMP_H
#define KMP_H
#include<vector>
#include<string>
#include<algorithm>
#include<cctype> 

// quick helper to lower case a string
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

std::string getSnippet(std::string s, std::string p) {
    // use lowercase copies for matching
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
        // compare the lowercase versions
        if(sLow[i]==pLow[j]) {
            j++;
        }
        else {
            if(j!=0) {
                j = lps[j-1];
                i--;
            }
        }
        if(j==k) {
            shift = (i-j) + 1;
            break; // found it
        }
    }

    if(shift!=-1) {
        // grab the substring from the ORIGINAL 's' to keep formatting
        int start = std::max(0, shift-30);
        int end = std::min(n, shift+k+50);
        return "..." + s.substr(start, end-start) + "...";
    }
    return "";
}
#endif