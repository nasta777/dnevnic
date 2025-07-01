#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

class DiaryManager {
private:
    map<string, vector<int>> subjects;
    const string filename = "diary.txt";

    void printSubject(const string& subject) const {
        auto it = subjects.find(subject);
        if (it != subjects.end()) {
            cout << subject << ": ";
            for (int grade : it->second) {
                cout << grade << " ";
            }
            cout << "\n";
        }
    }

public:
    void addSubject() {
        string subject;
        cout << "Enter subject name: ";
        getline(cin, subject);

        if (subjects.find(subject) == subjects.end()) {
            subjects[subject] = vector<int>();
            cout << "Subject '" << subject << "' added.\n";
        }
        else {
            cout << "Subject already exists!\n";
        }
    }

    void addGrade() {
        if (subjects.empty()) {
            cout << "No subjects available. Add a subject first.\n";
            return;
        }

        string subject;
        cout << "Enter subject name: ";
        getline(cin, subject);

        auto it = subjects.find(subject);
        if (it == subjects.end()) {
            cout << "Subject not found!\n";
            return;
        }

        int grade;
        cout << "Enter grade (2-5): ";
        cin >> grade;
        cin.ignore();

        if (grade < 2 || grade > 5 ) {
            cout << "Invalid grade! Must be between 2 and 5.\n";
            return;
        }

        it->second.push_back(grade);
        cout << "Grade " << grade << " added to " << subject << ".\n";
    }

    void viewAll() const {
        if (subjects.empty()) {
            cout << "No subjects available.\n";
            return;
        }

        for (const auto& entry : subjects) {
            printSubject(entry.first);
        }
    }

    void showSubjectAverage() const {
        if (subjects.empty()) {
            cout << "No subjects available.\n";
            return;
        }

        string subject;
        cout << "Enter subject name: ";
        getline(cin, subject);

        auto it = subjects.find(subject);
        if (it == subjects.end()) {
            cout << "Subject not found!\n";
            return;
        }

        if (it->second.empty()) {
            cout << "No grades for this subject.\n";
            return;
        }

        double sum = 0;
        for (int grade : it->second) {
            sum += grade;
        }
        double average = sum / it->second.size();

        cout << fixed << setprecision(2);
        cout << "Average for " << subject << ": " << average << "\n";
    }

    void showOverallAverage() const {
        if (subjects.empty()) {
            cout << "No subjects available.\n";
            return;
        }

        double totalSum = 0;
        int totalCount = 0;

        for (const auto& entry : subjects) {
            for (int grade : entry.second) {
                totalSum += grade;
                totalCount++;
            }
        }

        if (totalCount == 0) {
            cout << "No grades available.\n";
            return;
        }

        double average = totalSum / totalCount;
        cout << fixed << setprecision(2);
        cout << "Overall average: " << average << "\n";
    }

    void saveToFile() const {
        ofstream file(filename);
        if (!file) {
            cerr << "Error saving data!\n";
            return;
        }

        for (const auto& entry : subjects) {
            file << entry.first << "|";
            for (size_t i = 0; i < entry.second.size(); ++i) {
                file << entry.second[i];
                if (i < entry.second.size() - 1) file << ",";
            }
            file << "\n";
        }

        cout << "Data saved successfully.\n";
    }

    void loadFromFile() {
        ifstream file(filename);
        if (!file) return;

        subjects.clear();
        string line;

        while (getline(file, line)) {
            size_t pos = line.find('|');
            if (pos == string::npos) continue;

            string subject = line.substr(0, pos);
            string gradesStr = line.substr(pos + 1);

            vector<int> grades;
            stringstream ss(gradesStr);
            string grade;

            while (getline(ss, grade, ',')) {
                try {
                    grades.push_back(stoi(grade));
                }
                catch (...) {
                    // Skip invalid grades
                }
            }

            if (!grades.empty()) {
                subjects[subject] = grades;
            }
        }
    }
};

int main() {
    DiaryManager diary;
    diary.loadFromFile();

    int choice;
    do {
        cout << "\nSchool Diary Menu:\n";
        cout << "1. Add subject\n";
        cout << "2. Add grade\n";
        cout << "3. View all subjects and grades\n";
        cout << "4. Calculate subject average\n";
        cout << "5. Calculate overall average\n";
        cout << "6. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1: diary.addSubject(); break;
        case 2: diary.addGrade(); break;
        case 3: diary.viewAll(); break;
        case 4: diary.showSubjectAverage(); break;
        case 5: diary.showOverallAverage(); break;
        case 6: diary.saveToFile(); break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 6);

    return 0;
}
