#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

const int ALPHABET_SIZE = 26;

struct TrieNode {
    TrieNode* children[ALPHABET_SIZE];
    bool is_end_of_word;

    TrieNode() : is_end_of_word(false) {
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            children[i] = nullptr;
        }
    }
};

class CommandTrie {
private:
    TrieNode* root;

    static int charToIndex(char c) {
        return tolower(c) - 'a';
    }

    void destroy(TrieNode* node) {
        if (!node) return;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            destroy(node->children[i]);
        }
        delete node;
    }

public:
    CommandTrie() {
        root = new TrieNode();
    }

    // Inserts a robot command into the Trie in O(L) time
    void insert(const string& word) {
        TrieNode* curr = root;
        for (char c : word) {
            if (!isalpha(c)) continue;
            int idx = charToIndex(c);
            if (!curr->children[idx]) {
                curr->children[idx] = new TrieNode();
            }
            curr = curr->children[idx];
        }
        curr->is_end_of_word = true;
    }

    // Returns true if exact command exists: O(L) time
    [[nodiscard]] bool search(const string& word) const {
        TrieNode* curr = root;
        for (char c : word) {
            if (!isalpha(c)) continue;
            int idx = charToIndex(c);
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return curr && curr->is_end_of_word;
    }

    // Returns true if there is any command starting with the given prefix: O(P) time
    [[nodiscard]] bool startsWith(const string& prefix) const {
        TrieNode* curr = root;
        for (char c : prefix) {
            if (!isalpha(c)) continue;
            int idx = charToIndex(c);
            if (!curr->children[idx]) return false;
            curr = curr->children[idx];
        }
        return curr != nullptr;
    }

    ~CommandTrie() {
        destroy(root);
    }
};

int main() {
    cout << "--- DAY 40: TRIE PREFIX TREE COMMAND INDEXER (DSA) ---" << endl << endl;

    CommandTrie cmdTree;

    // Load autonomous rover command lexicon
    vector<string> commands = {
        "navigate", "navigateto", "halt", "hardstop", "scan", "status", "spin"
    };

    for (const auto& cmd : commands) {
        cmdTree.insert(cmd);
    }
    cout << "Loaded " << commands.size() << " autonomous telemetry commands into Trie.\n\n";

    // Test Exact Search: O(L)
    vector<string> search_queries = {"navigate", "nav", "hardstop", "reverse"};
    cout << "--- EXACT COMMAND SEARCH ---\n";
    for (const auto& q : search_queries) {
        cout << "Query '" << q << "' ➔ "
             << (cmdTree.search(q) ? "✅ VALID COMMAND" : "❌ UNRECOGNIZED") << "\n";
    }

    // Test Prefix Matching: O(Prefix_Length)
    vector<string> prefix_queries = {"nav", "ha", "sp", "arm"};
    cout << "\n--- PREFIX / AUTOCOMPLETE CHECK ---\n";
    for (const auto& p : prefix_queries) {
        cout << "Prefix '" << p << "*' ➔ "
             << (cmdTree.startsWith(p) ? "🟢 MATCH FOUND (Can Autocomplete)" : "⚪ NO MATCH") << "\n";
    }

    return 0;
}