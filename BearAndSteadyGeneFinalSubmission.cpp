#include <bits/stdc++.h>

using namespace std;
struct GeneCount {
    int a = 0;
    int t = 0;
    int c = 0;
    int g = 0;
};
struct WhichAreNeeded {
    bool A = false;
    bool G = false;
    bool T = false;
    bool C = false;
};
// "simplest" interface is taking substring and record describing what we are looking for
// *fastest* interface is probably two iterators on the full string.  Go for fast!
// returns count for each we need to count, 0 for any others we don't care about!
GeneCount takeStock(string::const_iterator start, string::const_iterator end, const GeneCount &gc, const WhichAreNeeded &weNeed) {
    int aCount = 0;
    int gCount = 0;
    int tCount = 0;
    int cCount = 0;

    for (auto it = start; it != end; ++it) {
        char c = *it;
        if (c == 'A') {
            ++aCount;
        }
        else if (c == 'G') {
            ++gCount;
        }
        else if (c == 'T') {
            ++tCount;
        }
        else if (c == 'C') {
            ++cCount;
        }

    }
    GeneCount stock;
    cout << "A.  Need: " << gc.a << "\tHave: " << aCount << "\n";
    cout << "T.  Need: " << gc.t << "\tHave: " << tCount << "\n";
    cout << "G.  Need: " << gc.g << "\tHave: " << gCount << "\n";
    cout << "C.  Need: " << gc.c << "\tHave: " << cCount << "\n";
    if (weNeed.A) stock.a = aCount;
    else stock.a = 0;
    if (weNeed.G) stock.g = gCount;
    else stock.g = 0;
    if (weNeed.T) stock.t = tCount;
    else stock.t = 0;
    if (weNeed.C) stock.c = cCount;
    else stock.c = 0;
    return stock;
}

int shortfall(const GeneCount &weveGot, const GeneCount &weNeed) {
    int total = 0;
    if (weNeed.a > weveGot.a) total += weNeed.a - weveGot.a;
    if (weNeed.g > weveGot.g) total += weNeed.g - weveGot.g;
    if (weNeed.t > weveGot.t) total += weNeed.t - weveGot.t;
    if (weNeed.c > weveGot.c) total += weNeed.c - weveGot.c;
    return total;
}
// Complete the steadyGene function below.
int steadyGene(string gene) {
    // precondition: they promise that len( gene ) == 4 * n
    // do one quick pass thru to calculate the base pair frequency
    unordered_map<char, int> freq;
    for (const auto c : gene) ++freq[c];
    int desired = gene.size() / 4;
    cout << "Desired # of each base: " << desired << "\n";
    auto nctA = desired - freq['A'];   // necessary change/adjustment to count of base 'A'
    auto nctT = desired - freq['T'];
    auto nctC = desired - freq['C'];
    auto nctG = desired - freq['G'];

    GeneCount target;         // constant for whole run
    WhichAreNeeded weNeed;   // constant for whole run, set below
    if (nctA < 0) { weNeed.A = true; target.a = -nctA; }
    if (nctT < 0) { weNeed.T = true; target.t = -nctT; }
    if (nctG < 0) { weNeed.G = true; target.g = -nctG; }
    if (nctC < 0) { weNeed.C = true; target.c = -nctC; }
    cout << "target.a:" << target.a << "\ttarget.t:" << target.t << "\ttarget.c" << target.c << "\ttarget.g:" << target.g << "\n";
    cout << "weNeed.A:" << weNeed.A << "\tweNeed.T:" << weNeed.T << "\tweNeed.C:" << weNeed.C
        << "\tweNeed.G:" << weNeed.G << "\n";
    int bestConceivableReplacementSize = target.a + target.t + target.g + target.c;
    cout << "bestConceivableReplacementSize: " << bestConceivableReplacementSize << "\n";
    if (0 == bestConceivableReplacementSize) return 0;
    int windowSize = bestConceivableReplacementSize;    // need shortest answer so be optimistic.
    do {
        cout << "Current window size: " << windowSize << "\n";
        GeneCount weveGot = takeStock(cbegin(gene), cbegin(gene) + windowSize, target, weNeed);
        int currentShortfall = shortfall(weveGot, target);
        if (0 == currentShortfall) return windowSize;
        int minShortfall = currentShortfall;
        char prev = *cbegin(gene);
        auto it = cbegin(gene) + windowSize;
        while (it < cend(gene)) {

            //cout << ".";
            if (prev == 'A' && weNeed.A) { --weveGot.a; } //cout << "Lost an a!\n"; 
            else if (prev == 'C' && weNeed.C) --weveGot.c;
            else if (prev == 'G' && weNeed.G) --weveGot.g;
            else if (prev == 'T' && weNeed.T) --weveGot.t;
            char head = *it; //cout << "head=" << head << "...";
            if (head == 'A' && weNeed.A) { ++weveGot.a; } //cout << "Found 1 a!\n";
            else if (head == 'C' && weNeed.C) ++weveGot.c;
            else if (head == 'G' && weNeed.G) ++weveGot.g;
            else if (head == 'T' && weNeed.T) ++weveGot.t;
            currentShortfall = shortfall(weveGot, target);
            if (currentShortfall < minShortfall) minShortfall = currentShortfall;
            if (0 == currentShortfall) { cout << "BINGO!!\n"; return windowSize; }
            prev = *(it - windowSize + 1);
            if (currentShortfall == bestConceivableReplacementSize) it += bestConceivableReplacementSize;
            else ++it;
        }
        // 
        windowSize += minShortfall;
        // if it has what we need return windowSize;
        // else roll from here to end - windowSize, update the rollingShortFall, and if
        // it ever becomes zero, return windowSize
        // keep track of min shortfall inside loop and add that to
        // the windowSize so we don't waste time with doomed windowSize(s).

    } while (true);
    // the above steps only need to happen once, our big while loop keeps examinging sliding
    // windows of increasingly large sizes until BINGO!

    // naive algorithm: currentSize = bestConceivableReplacementSize, found = false
    // while not found, search whole gene for substr( X, currentSize ) with targetCount
    // if found return currentSize
    // else ++currentSize;
    //bool matches( string::const_iterator start, string::const_iterator end, const GeneCount &gc, int &indexOfFirstUseful)
    // optimization of worst case times:  say we are already up to very long stretches and
    // still haven't found what we need.  Telling us that we can skip the whole string length
    // if it contains NONE of what we need will reduce the number of calls to the matching
    // function at the cost of some work inside it.  Telling us that we can skip some subset
    // of the size of the string can be done at slightly more cost
    // we will leave that as an optimization for now and just do a basic sliding window now.
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    int n;
    cin >> n;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string gene;
    getline(cin, gene);

    int result = steadyGene(gene);

    fout << result << "\n";

    fout.close();

    return 0;
}