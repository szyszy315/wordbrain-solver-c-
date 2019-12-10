#include<iostream>
#include<string>
#include<fstream>
#include<vector>
#include <set>
using namespace std;

struct TrieNode {
  TrieNode *children[26];
  string word;

  TrieNode() : word("") {
    for (int i = 0; i < 26; i++) {
      children[i] = nullptr;
    }
  }
};

TrieNode *buildTrie(string filename1) {
  TrieNode *root = new TrieNode();
  string tmp;
  ifstream file;
  file.open(filename1);
  while (file.good()) {
    std::getline(file, tmp);
    string word = tmp;
    TrieNode *curr = root;
    for (int i = 0; i < word.length(); i++) {
      char c = word[i] - 'a';
      if (curr->children[c] == nullptr) {
        curr->children[c] = new TrieNode();
      }
      curr = curr->children[c];
    }
    curr->word = word;
  }
  file.close();
  return root;
}

void drop(vector<vector<char>> *board) {
  for (int i = board->size() - 1; i > 0; i--) {
    for (int j = board->size() - 1; j > -1; j--) {
      if (board ->at(i).at(j) == '#') {
        int t = i;
        while (t >= 0 && board->at(t).at(j) == '#') {
          t--;
        }
        if (t >= 0) {
          board->at(i).at(j) = board->at(t).at(j);
          board->at(t).at(j) = '#';
        }
      }
    }
  }
}


void dfs(set<vector<string>> *p_allres, vector<vector<char>> *board, \
  int i, int j, TrieNode *p, vector<string> result, vector<string>ast, int x, TrieNode *root) {
  char c = board->at(i)[j];
  if (c == '#' || !p->children[c - 'a']) return;
  vector<vector<char>> nboard = *board;
  vector<vector<char>> *newboard = &nboard;
  newboard->at(i)[j] = '#';
  TrieNode *pt;
  string tmp;
  pt = p->children[c - 'a'];
  p = p->children[c - 'a'];
  vector<string> newresult = result;
  if (p->word.size() == ast.at(x).size()) {
    string wo = p->word;
    int i = 0;
    while (i < ast.at(x).size() && ast.at(x).at(i) != '*' ) {
      if (ast.at(x).at(i) != wo.at(i)) return;
      i++;
    }
    newresult.push_back(p->word);
    if (newresult.size() == ast.size()) {
      p_allres -> insert(newresult);
      return;
    }
    tmp = p -> word;
    p->word = "";
    drop(newboard);
    // for (int i = 0; i < newboard.size(); i++) {
    //     for (int j = 0; j < newboard.size(); j++) {
    //         std::cout << newboard[i][j];
    //     }
    // }
    x++;
    p = root;
    for (int i = 0; i < newboard->size(); i++) {
      for (int j = 0; j < newboard->size(); j++) {
        dfs(p_allres, newboard, i, j, root, newresult, ast, x, root);
      }
    }
    pt -> word = tmp;
  } else {
    if (i > 0) dfs(p_allres, newboard, i - 1, j, p, newresult, ast, x, root);
    if (i < newboard->size() - 1) dfs(p_allres, newboard, i + 1, \
      j, p, newresult, ast, x, root);
    if (j > 0) dfs(p_allres, newboard, i, j - 1, p, newresult, ast, x, root);
    if (j < newboard->size() - 1) dfs(p_allres, newboard, i, j + 1, \
      p, newresult, ast, x, root);
    if (j > 0 && i > 0) dfs(p_allres, newboard, i - 1, j - 1, p, newresult, ast, x, root);
    if (i > 0 && j < newboard->size() - 1) dfs(p_allres, newboard, \
      i - 1, j + 1, p, newresult, ast, x, root);
    if (i < newboard->size() - 1 && j > 0) dfs(p_allres, newboard, i + 1, \
      j - 1, p, newresult, ast, x, root);
    if (i < newboard->size() - 1 && j < newboard[0].size() - 1) dfs(p_allres, \
      newboard, i + 1, j + 1, p, newresult, ast, x, root);
  }
  newboard->at(i)[j] = c;
}

vector<vector<string>> findWords(string filename1, string filename2) {
  string aster, n1, n2;
  vector<vector<string>> allres;
  TrieNode *root = buildTrie(filename1);
  TrieNode *root1 = buildTrie(filename2);
  while(cin >> n1) {
    set<vector<string>> someres;
    std::vector<string> v, ast;
    int n = n1.size();
    int len = n * n;
    v.push_back(n1);
    for (int i = 1; i < n; i++) {
      cin >> n2;
      v.push_back(n2);
    }
    while (len != 0) {
      cin >> aster;
      len -= aster.size();
      ast.push_back(aster);
    }
    int c1 = ast.size();
    vector<string> result;
    vector<vector<char>> board, board1;
    vector<char> emp;
    for (int i = 0; i < n; i++) {
      board.push_back(emp);
      for (int j = 0; j < n; j++) {
        board.at(i).push_back(v.at(i).at(j));
      }
    }
    board1 = board;
    int num = 0;
    int size = someres.size();
    for (int i = 0; i < board.size(); i++) {
      for (int j = 0; j < board.size(); j++) {
        dfs(&someres, &board, i, j, root, result, ast, 0, root);
      }
    }
    if (size == someres.size()) {
      for (int i = 0; i < board1.size(); i++) {
        for (int j = 0; j < board1.size(); j++) {
          dfs(&someres, &board1, i, j, root1, result, ast, 0, root1);
        }
      }
    }
    for (auto i:someres) {
      for (auto j:i){
        cout << j << ' ';
      } 
      cout << '\n';
    }
    cout <<'.' << '\n';
    for (auto i : someres) allres.push_back(i);
    allres.push_back({"."});
  }
  return allres;
}

int main(int argc, char **argv) {
  string filename1 = *(argv + 1);
  string filename2 = *(argv + 2);

  vector<vector<string>> res = findWords(filename1, filename2);
}
