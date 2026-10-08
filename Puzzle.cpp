#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <utility>

using namespace std;

void findSolution(
    vector<pair<string, bool>>& input,
    const vector<vector<int>>& byStart,
    int current,
    vector<int>& path,
    vector<int>& bestPath)
{
    static unsigned long long calls = 0;
    ++calls;

    if (path.size() > bestPath.size())
        bestPath = path;

    if (calls % 1'000'000 == 0)
    {
        cerr << "Calls: " << calls
             << " | Depth: " << path.size()
             << " | Best: " << bestPath.size() << '\n';
    }

    int end = stoi(input[current].first.substr(4, 2));

    for (int i : byStart[end])
    {
        if (input[i].second)
            continue;

        input[i].second = true;
        path.push_back(i);

        findSolution(input, byStart, i, path, bestPath);

        path.pop_back();
        input[i].second = false;
    }
}

string print(const string& number)
{
    return "(" + number.substr(0, 2) + ")"
         + number.substr(2, 2)
         + "(" + number.substr(4, 2) + ")";
}

int main()
{
    vector<pair<string, bool>> numbers;
    vector<vector<int>> byStart(100);
    vector<int> path;
    vector<int> solution;

    ifstream read("source.txt");

    if (!read.is_open())
    {
        cerr << "Cannot open source.txt\n";
        return 1;
    }

    string number;

    while (read >> number)
    {
        if (number.size() != 6 ||
            number.find_first_not_of("0123456789") != string::npos)
        {
            cerr << "Invalid piece: " << number << '\n';
            return 1;
        }

        int index = static_cast<int>(numbers.size());
        int start = stoi(number.substr(0, 2));

        numbers.push_back({number, false});
        byStart[start].push_back(index);
    }

    cout << "Read: " << numbers.size() << '\n';

    for (int start = 0; start < static_cast<int>(numbers.size()); ++start)
    {
        cerr << "Start: " << start << '\n';

        path.clear();
        path.push_back(start);
        numbers[start].second = true;

        findSolution(numbers, byStart, start, path, solution);

        numbers[start].second = false;
    }

    cout << "BEST COMBINATION (size: "
         << solution.size() << "):\n";

    for (int index : solution)
        cout << print(numbers[index].first) << '\n';

    return 0;
}