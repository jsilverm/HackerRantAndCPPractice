#include <bits/stdc++.h>

using namespace std;

vector<string> split_string(string);


int activityNotifications(vector<int> expenditure, int d) {

    vector<int> dDayWindow(d);
    int notificationsSent = 0;
    int posOfMedian = d / 2;
    //cerr << "d=" << d << ",posOfMedian=" << posOfMedian << "\n";
    copy(cbegin(expenditure), cbegin(expenditure) + d, begin(dDayWindow));
    sort(begin(dDayWindow), end(dDayWindow));
    //for (const auto el : dDayWindow ) cout << "el: " << el << ",";
    //cerr << "dDayWindow[posOfMedian] + dDayWindow[posOfMedian-1]" <<          
       // dDayWindow[posOfMedian] << " + " << dDayWindow[posOfMedian-1] << "\n";
    double median = ((d & 1) == 1) ?    dDayWindow[posOfMedian] :
        (dDayWindow[posOfMedian] + dDayWindow[posOfMedian - 1]) / 2.0;
    if (expenditure[d] >= 2.0 * median) {
       // cerr << "median= " << median << ", 2*median= " << 2 * median << ",expenditure[" << d
       //   << "]=" << expenditure[d] << "\n";
        ++notificationsSent;
        //cerr << "First batch yielded a notification!\n";
    }
    // as new values come in, if the incoming happens to be equal to the one falling off the
    // end, we don't need to do anything, the contents of the window stays the same.
    // otherwise, we delete the single value falling off the end, and insert the new value
    // at the appropriate spot.  No sort is necessary.
    int trailingEnd = expenditure[0];
    // each time thru: i is the new value we are comparing to median.
    // i - 1 is the new value to compute the median
    // 
    for (int i = d + 1; i != expenditure.size(); ++i) {
        //cerr << "i=" << i << "posOfMedian=" << posOfMedian << "\n";
        // i represents day to compare to twice median of prior d days
        // updated value of prior d days gets adjusted in dDayWindow
        int valueToTest = *(cbegin(expenditure) + i);
        int newValue = *(cbegin(expenditure) + i - 1);
        if (trailingEnd != newValue) { // if happens to be the same, leave window alone!
            dDayWindow.erase(lower_bound(begin(dDayWindow), end(dDayWindow), trailingEnd)); // fell out of window
            dDayWindow.insert(upper_bound(cbegin(dDayWindow), cend(dDayWindow), newValue), newValue); // our new guy
        }
        median = ((d & 1) == 1) ?
            dDayWindow[posOfMedian] :
            ((dDayWindow[posOfMedian] + dDayWindow[posOfMedian - 1]) / 2.0);
        //cerr << "Comparing new value of: " << expenditure[i] << "versus " << 2* median
           // << "\n";
        if (valueToTest >= 2 * median) {
            ++notificationsSent;
          //cerr << "notification sent at i=" << i << "\n";
        }
        trailingEnd = *(cbegin(expenditure) + i - d);
    }
    //  if ( !isSorted(dDayWindow) ) cout << "dDayWindow not sorted properly!!\n";
    return notificationsSent;
}

int main()
{
    ofstream fout(getenv("OUTPUT_PATH"));

    string nd_temp;
    getline(cin, nd_temp);

    vector<string> nd = split_string(nd_temp);

    int n = stoi(nd[0]);

    int d = stoi(nd[1]);

    string expenditure_temp_temp;
    getline(cin, expenditure_temp_temp);

    vector<string> expenditure_temp = split_string(expenditure_temp_temp);

    vector<int> expenditure(n);

    for (int i = 0; i < n; i++) {
        int expenditure_item = stoi(expenditure_temp[i]);

        expenditure[i] = expenditure_item;
    }

    int result = activityNotifications(expenditure, d);

    fout << result << "\n";

    fout.close();

    return 0;
}

vector<string> split_string(string input_string) {
    string::iterator new_end = unique(input_string.begin(), input_string.end(), [] (const char &x, const char &y) {
        return x == y and x == ' ';
    });

    input_string.erase(new_end, input_string.end());

    while (input_string[input_string.length() - 1] == ' ') {
        input_string.pop_back();
    }

    vector<string> splits;
    char delimiter = ' ';

    size_t i = 0;
    size_t pos = input_string.find(delimiter);

    while (pos != string::npos) {
        splits.push_back(input_string.substr(i, pos - i));

        i = pos + 1;
        pos = input_string.find(delimiter, i);
    }

    splits.push_back(input_string.substr(i, min(pos, input_string.length()) - i + 1));

    return splits;
}