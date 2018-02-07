#include <iostream>
#include <vector>
#include <fstream>
#include <limits>

std::vector<int> keys;
std::vector<std::vector<unsigned> > roots;
std::vector<int> fakeKeys;

std::pair<std::vector<double>, std::vector<double>> readData(const std::string& fileName) {
    std::ifstream ifstream(fileName);
    unsigned n;
    ifstream >> n;
    keys.resize(n+1);
    for (int i = 0; i < n; i++) {
        ifstream >> keys[i+1];
    }
    std::vector<double> p(n+1, 0.0);
    for (int i = 0; i < n; i++) {
        ifstream >> p[i+1];
    }
    fakeKeys.resize(n+1);
    for (int i = 0; i <= n; i++) {
        ifstream >> fakeKeys[i];
    }
    std::vector<double> q(n+1, 0.0);
    for (int i = 0; i <= n; i++) {
        ifstream >> q[i];
    }
    ifstream.close();
    return {p, q};
}

std::pair<std::vector<std::vector<double> >, std::vector<std::vector<unsigned> > >
optimalBST(const std::pair<std::vector<double>, std::vector<double>>& data) {
    auto p = data.first;
    auto q = data.second;
    unsigned long n = p.size() - 1;
    std::vector<std::vector<double> > e(n+2, std::vector<double>(n+1, 0.0));
    std::vector<std::vector<double> > w(n+2, std::vector<double>(n+1, 0.0));
    std::vector<std::vector<unsigned> > root(n+1, std::vector<unsigned>(n+1, 0));
    for (int i = 1; i <= n+1; i++) {
        e[i][i-1] = q[i-1];
        w[i][i-1] = q[i-1];
    }
    for (int l = 1; l <= n; l++) {
        for (int i = 1; i <= n-l+1; i++) {
            int j = i+l-1;
            e[i][j] = std::numeric_limits<double>::max();
            w[i][j] = w[i][j-1] + p[j] + q[j];
            for (int r = i; r <= j; r++) {
                double t = e[i][r-1] + e[r+1][j] + w[i][j];
                if (t < e[i][j]) {
                    e[i][j] = t;
                    root[i][j] = r;
                }
            }
        }
    }
    return {e, root};
}

struct node {
    int value;
    node *l, *r;
    node(int value) {
        this->value = value;
    }
};

node* buildTree(unsigned i, unsigned j) {
    if (i > j) {
        int fakeRootKey = fakeKeys[j];
        return new node(fakeRootKey);
    }
    int index = roots[i][j];
    int rootKey = keys[index];
    auto root = new node(rootKey);
    root->l = buildTree(i, index-1);
    root->r = buildTree(index+1, j);
    return root;
}

int main() {

    std::cout << "Enter the filename: ";
    std::string fileName;
    std::cin >> fileName;
    auto result = optimalBST(readData(fileName));
    roots = result.second;
    unsigned n = roots.size() - 1;
    auto root = buildTree(1, n);
    return 0;
}
