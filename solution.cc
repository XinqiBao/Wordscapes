/**
 *
 * A solution for Wordscapes, an app in appstore game
 *
 */

#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <vector>
using namespace std;

class lset {
   private:
    class letter {
       public:
        bool word;
        lset* next;
        letter() : word(false), next(nullptr){};
    };
    letter* ltr[26];

   public:
    lset() {
        for (int i = 0; i < 26; i++) ltr[i] = new letter;
    }
    ~lset() {
        for (int i = 0; i < 26; i++) {
            if (ltr[i]->next != nullptr) delete ltr[i]->next;
            delete ltr[i];
        }
    }

    void insert(const char* w) {
        int index;
        if (w[0] >= 97 && w[0] <= 122)
            index = w[0] - 97;
        else if (w[0] >= 65 && w[0] <= 90)
            index = w[0] - 65;
        else
            return;
        if (w[1] == '\0')
            ltr[index]->word = true;
        else {
            if (ltr[index]->next == nullptr) ltr[index]->next = new lset;
            ltr[index]->next->insert(w + 1);
        }
    }
    bool find(const char* w) {
        int index;
        if (w[0] >= 97 && w[0] <= 122)
            index = w[0] - 97;
        else if (w[0] >= 65 && w[0] <= 90)
            index = w[0] - 65;
        else
            return false;
        if (w[1] == '\0')
            return ltr[index]->word;
        else if (ltr[index]->next == nullptr)
            return false;
        else
            return ltr[index]->next->find(w + 1);
        return false;
    }
    bool prefix(const char* w) {
        int index;
        if (w[0] >= 97 && w[0] <= 122)
            index = w[0] - 97;
        else if (w[0] >= 65 && w[0] <= 90)
            index = w[0] - 65;
        else
            return false;
        if (ltr[index]->next == nullptr) return false;
        if (w[1] == '\0')
            return true;
        else
            return ltr[index]->next->prefix(w + 1);
        return false;
    }
};

void loadDictionary(lset* dic, string filename) {
    cout << "Loading dictionary..." << endl;
    ifstream f(filename);
    string str;
    while (f >> str) {
        dic->insert(&str[0]);
    }
    cout << "Load completed!" << endl;
    f.close();
}

class scape {
   private:
    lset& dic;
    int numLetters;
    vector<char> letters;
    int findWords() {
        cout << "Find words: blank as '*' || back new scape: 0 \n->";
        string str;
        cin >> str;
        if (str[0] == '0') return 1;
        vector<char> word;
        vector<bool> lstat(letters.size(), false);
        for (int i = 0; i < str.length(); i++) {
            if (str[i] == '*')
                word.push_back('*');
            else {
                char ch;
                if (str[i] >= 'a' && str[i] <= 'z')
                    ch = str[i];
                else if (str[i] >= 'A' && str[i] <= 'Z')
                    ch = str[i] - 'A' + 'a';
                for (int k = 0; k < letters.size(); k++) {
                    if (letters[k] == ch && lstat[k] == false) {
                        lstat[k] = true;
                        word.push_back(ch);
                        break;
                    }
                }
            }
        }
        int sizeW = word.size();
        set<string> ans;
        finding(word, lstat, 0, sizeW, "", ans);
        for (auto& w : ans) cout << w << endl;
        return 0;
    }
    void finding(vector<char> w, vector<bool> lst, int pos, int ws, string str,
                 set<string>& ans) {
        if (pos == ws) {
            if (dic.find(&(str)[0]))
                // cout << str << endl;
                ans.insert(str);
            return;
        }
        if (pos != 0 && !dic.prefix(&(str + '\0')[0])) return;
        if (w[pos] == '*') {
            for (int i = 0; i < lst.size(); i++) {
                if (lst[i] == false) {
                    string st = str + letters[i];  // cout <<'-'<<st<<endl;
                    vector<bool> lst_(lst);
                    lst_[i] = true;
                    finding(w, lst_, pos + 1, ws, st, ans);
                }
            }
        } else {
            str += w[pos];
            finding(w, lst, pos + 1, ws, str, ans);
        }
    }

   public:
    scape(lset& dic) : dic(dic) {
        cout << "add letters || back new scape: 0 \n->";
        string str;
        cin >> str;
        if (str[0] == '0') return;
        for (int i = 0; i < str.length(); i++) {
            if (str[i] >= 'a' && str[i] <= 'z')
                letters.push_back(str[i]);
            else if (str[i] >= 'A' && str[i] <= 'Z')
                letters.push_back(str[i] - 'A' + 'a');
        }
        numLetters = letters.size();
        while (findWords() == 0)
            ;
    }
};

int main() {
    lset dic;
    loadDictionary(&dic, "words_alpha.txt");

    cout << "Dictionary loaded!" << endl;

    while (1) {
        cout << "Command: New scape: 1 || exit: 0 \n->";
        int cmd;
        cin >> cmd;
        if (cmd == 1) {
            scape sc(dic);
        } else if (cmd == 0)
            break;
    }
    cout << "Releasing memory for dictionary..." << endl;
    return 0;
}
