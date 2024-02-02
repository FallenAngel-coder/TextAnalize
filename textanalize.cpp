#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <chrono>
#include <Windows.h>
#define DEBUG
using namespace std;
string GetText(const char* path) {
    ifstream in(path);
    string data;
    data.assign((istreambuf_iterator<char>(in.rdbuf())), istreambuf_iterator<char>());
    return data;
}
void ClearText(string& data) {
    for (size_t i = 0; i < data.length(); i++) {
        data[i] = tolower(data[i]);
        if ((data[i] < 'a' or data[i] > 'z') and data[i] != ' ') {
            data.erase(i, 1);
            i--;
        }
    }
}
void ToMultiMap(string& data, multimap<string, int>& a) {
    char* tmp = _strdup(data.data());
    char* tmp2 = strtok(tmp, " ");
    int i = 0;

    while (tmp2 != NULL) {
        a.insert({ string(tmp2), i });
        i++;
        tmp2 = strtok(NULL, " ");
    }
    delete[] tmp;
    delete[] tmp2;
}
void BanList(multimap<string, int>& a, const char* path) {
    ifstream in(path);
    string data1;
    multimap<string, int> banlist;
    multimap<string, int> temp;
    data1.assign((istreambuf_iterator<char>(in.rdbuf())), istreambuf_iterator<char>());
    ToMultiMap(data1, banlist);
    for (auto it: a)
        if (!banlist.count(it.first))
            temp.insert(it);
    a = temp;
}
void ToMap(multimap<string, int>& a, map<string, int>& b) {
    for (auto it : a)
        b.insert(it);
}
void SaveResult(multimap<string, int>& a, map<string, int>& b, const char* path, const char* path1) {
    bool flag = false;
    FILE* streamr, *streamm;
    fopen_s(&streamr, path, "wt");
    fopen_s(&streamm, path1, "wt");
    printf("%Unique words: %f%%\n", float(b.size()) / float(a.size()) * 100);
    if (streamr != 0 and streamm != 0) {
        fputs("Unique words: ", streamr);
        fprintf(streamr, "%.5f", float(b.size()) / float(a.size()) * 100);
        fputs("%\n", streamr);
        for (auto it : b) {
            printf("%s: %f%%\n", it.first.data(), float(a.count(it.first)) / float(a.size()) * 100);
            fputs(it.first.data(), streamr);
            fputs(": ", streamr);
            fprintf(streamr, "%.5f", float(a.count(it.first)) / float(a.size()) * 100);
            fputs("%\n", streamr);
            if (float(a.count(it.first)) / float(a.size()) * 100 > 10.0) {
                fputs(it.first.data(), streamm);
                fputs(": ", streamm);
                fprintf(streamm, "%.5f", float(a.count(it.first)) / float(a.size()) * 100);
                fputs("%\n", streamm);
            }
        }
        fclose(streamm);
        string data = GetText(path1);
        if (data.empty())
            cout << "No words that have > 10% usage\n";
        else
            cout << "Most used: \n" << data;

    }
}
int main()
{
#ifdef DEBUG
    auto begin = chrono::steady_clock::now();
#endif // DEBUG
    string data = GetText("D:/file.txt");
    ClearText(data);
    multimap<string, int> mult;
    map<string, int> m;
    ToMultiMap(data, mult);
    BanList(mult, "D:/banlist.txt");
    ToMap(mult, m);
    SaveResult(mult, m, "D:/result.txt", "D:/result1.txt");
#ifdef DEBUG
    auto end = chrono::steady_clock::now();
    auto elapsed_ms = chrono::duration_cast<chrono::milliseconds>(end - begin);
    cout << "\nThe time: " << elapsed_ms.count() << " ms\n";
#endif // DEBUG
}