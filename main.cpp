#include <cmath>
#include <conio.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>

using namespace std;

struct FileNotFoundException : runtime_error {
    explicit FileNotFoundException(const string & path)
            : runtime_error("File '" + path + "' does not found: can't be reading or writing") {}
};
struct MatrixNotFoundException : runtime_error {
    explicit MatrixNotFoundException(const string & arg)
            : runtime_error("Matrix not found exception: " + arg) {}
};
struct NotMatrixException : runtime_error {
    explicit NotMatrixException(const string & arg)
            : runtime_error("Not matrix exception: " + arg) {}
};
struct NotSymmetricalMatrixException : runtime_error {
    explicit NotSymmetricalMatrixException()
            : runtime_error("Not symmetrical matrix exception") {}
};
struct MatrixNumberException : runtime_error {
    explicit MatrixNumberException()
            : runtime_error("Matrix number exception: out of range") {}
};
struct MatrixMultiplyingException : runtime_error {
    explicit MatrixMultiplyingException(const string & arg)
            : runtime_error("Matrix multiplying exception: " + arg) {}
};


void pressAnyKeyForContinue() noexcept {
    cout << "Press any key for continue..." << flush;
    getch();
}

vector<vector<int>> toMatrix(const vector<vector<int>> & tapeMatrixDiagonalStorage) {
    int n = int(tapeMatrixDiagonalStorage.size());
    int m = int(tapeMatrixDiagonalStorage[0].size());
    vector<vector<int>> matrix(n, vector<int> (n, 0));
    for (int j = m - 1; j >= 0; --j)
        for (int i = m - 1 - j; i < n; ++i)
            matrix[i][i - (m - 1 - j)] = matrix[i - (m - 1 - j)][i] = tapeMatrixDiagonalStorage[i][j];
    return matrix;
}

int tapeCount(const vector<vector<int>> & tapeMatrix) {
    int n = int(tapeMatrix.size());
    for (int i = n - 1; i >= 0; --i)
        for (int j = 0; j < n - i; ++j)
            if (tapeMatrix[i][j] != 0)
                return abs(i - j) + 1;
    return 1;
}

string tapeMatrixToString(const vector<vector<int>> & tapeMatrix) {
    int n = int(tapeMatrix.size());
    vector<int> columnsSizes(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            columnsSizes[j] = max(columnsSizes[j], int(to_string(tapeMatrix[i][j]).size()));
    stringstream result;
    int m = tapeCount(tapeMatrix);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j)
            result << setw(columnsSizes[j])
                   << (i - j <= m - 1 && j - i <= m - 1 ? to_string(tapeMatrix[i][j]) : " ")
                   << (j == n - 1 ? "" : " ");
        result << (i == n - 1 ? "" : "\n");
    }
    return result.str();
}

vector<string> split(const string & s, char delim) {
    std::stringstream ss(s);
    std::string item;
    vector<string> result;
    while(std::getline(ss, item, delim)) {
        result.push_back(item);
    }
    return result;
}

string tapeMatrixDiagonalStorageToString(const vector<vector<int>> & tapeMatrixDiagonalStorage) {
    int n = int(tapeMatrixDiagonalStorage.size());
    int m = int(tapeMatrixDiagonalStorage[0].size());
    vector<int> columnsSizes(m);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < tapeMatrixDiagonalStorage[i].size(); ++j)
            columnsSizes[j] = max(columnsSizes[j], int(to_string(tapeMatrixDiagonalStorage[i][j]).size()));
    stringstream result;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < tapeMatrixDiagonalStorage[i].size(); ++j)
            result << setw(columnsSizes[j])
                   << (i + j < m - 1 ? " " : to_string(tapeMatrixDiagonalStorage[i][j]))
                   << (j == tapeMatrixDiagonalStorage[i].size() - 1 ? "" : " ");
        result << (i == n - 1 ? "" : "\n");
    }
    return result.str();
}

string toString(const vector<vector<int>> & matrix) {
    int n = int(matrix.size());
    vector<int> columnsSizes(n);
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < matrix[i].size(); ++j)
            columnsSizes[j] = max(columnsSizes[j], int(to_string(matrix[i][j]).size()));
    stringstream result;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < matrix[i].size(); ++j)
            result << setw(columnsSizes[j]) << matrix[i][j] << (j == matrix[i].size() - 1 ? "" : " ");
        result << (i == n - 1 ? "" : "\n");
    }
    return result.str();
}

void writeMatrix(const string & path, const vector<vector<int>> & matrix) {
    ofstream fout(path);
    if (!fout.is_open())
        throw FileNotFoundException(path);
    fout << toString(matrix);
    fout.close();
}

vector<vector<int>> readMatrix(const string & path) {
    ifstream fin(path);
    if (!fin)
        throw FileNotFoundException(path);
    vector<vector<int>> result;
    string stringBuffer;
    int intBuffer;
    while (getline(fin, stringBuffer)) {
        istringstream ss(stringBuffer);
        vector<int> line;
        while (ss >> intBuffer) {
            line.push_back(intBuffer);
        }
        if (!line.empty())
            result.push_back(line);
    }
    if (result.empty())
        throw MatrixNotFoundException("no numbers found");
    return result;
}

bool isSymmetrical(const vector<vector<int>> & matrix) {
    int n = int(matrix.size());
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            if (matrix[i][j] != matrix[j][i])
                return false;
    return true;
}

vector<vector<int>> toTapeMatrixDiagonalStorage(const vector<vector<int>> & matrix) {
    if (!isSymmetrical(matrix))
        throw NotSymmetricalMatrixException();
    int n = int(matrix.size());
    int m = tapeCount(matrix);
    vector<vector<int>> result(n, vector<int>(m, 0));
    for (int j = m - 1; j >= 0; --j)
        for (int i = m - 1 - j; i < n; ++i)
            result[i][j] = matrix[i][i - (m - 1 - j)];
    return result;
}

string getCurrentDateTime(const string& s) {
    time_t now = time(nullptr);
    struct tm timeStruct{};
    char  buf[80];
    timeStruct = *localtime(&now);
    if(s == "now")
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &timeStruct);
    else if(s == "date")
        strftime(buf, sizeof(buf), "%Y-%m-%d", &timeStruct);
    return buf;
}

void log(const char * filePath, const string& logMsg) {
    string now = getCurrentDateTime("now");
    ofstream ofs(filePath, std::ios_base::out | std::ios_base::app );
    if (!ofs.is_open())
        ofs = ofstream("errors.txt", std::ios_base::out | std::ios_base::app);
    ofs << now << '\t' << logMsg << '\n';
    ofs.close();
}

vector<vector<int>> multiply(const vector<vector<int>> & first, const vector<vector<int>> & second) {
    try {
        if (first[0].size() != second.size())
            throw MatrixMultiplyingException(
                    string("first matrix size: ") + to_string(first.size()) + "x" + to_string(first[0].size()) +
                    string("second matrix size: ") + to_string(second.size()) + "x" + to_string(second[0].size())
            );
        int l = int(first[0].size());
        int n = int(first.size());
        int m = int(second.size());
        vector<vector<int>> result(n, vector<int>(m, 0));
        for (int i = 0; i < n; ++i)
            for (int j = 0; j < m; ++j)
                for (int k = 0; k < l; ++k)
                    result[i][j] += first[i][k] * second[k][j];
        return result;
    } catch (...) {
        throw MatrixMultiplyingException("matrices must be rectangular");
    }
}

int main(int argc, char * argv[]) {
    if (argc != 2) {
        cerr << "[Utility format]: SparseMatrixManager errors.txt";
        return 1;
    }

    vector<pair<string, vector<vector<int>>>> tapeMatricesDiagonalStorages;
    while (true) {
        try {
            system("cls");
            cout << "1) list of matrices\n"
                 << "2) read matrix from file\n"
                 << "3) read diagonal storage for tape matrix from file\n"
                 << "4) multiply matrices\n"
                 << "0) exit\n"
                 << "Select item >" << flush;
            key_reading:
            char key = char(getch());
            if (key == '0')
                break;
            if (key == '1') { // list of matrices
                if (tapeMatricesDiagonalStorages.empty()) {
                    system("cls");
                    cout << "List of matrices is empty\n" << flush;
                    pressAnyKeyForContinue();
                } else {
                    while (true) {
                        if (tapeMatricesDiagonalStorages.empty())
                            break;
                        system("cls");
                        cout << "Matrices:\n";
                        int n = int(tapeMatricesDiagonalStorages.size());
                        for (int i = 0; i < n; ++i)
                            cout << setw(int(to_string(n).size())) << i + 1 << ") "
                                 << tapeMatricesDiagonalStorages[i].first << "\n";
                        cout << setw(int(to_string(0).size())) << 0 << ") back\n"
                             << "Enter matrix number or 0 for backing: " << flush;
                        int matrixNumber;
                        cin >> matrixNumber;
                        if (matrixNumber == 0)
                            break;
                        else if (matrixNumber < 0 || matrixNumber > n)
                            continue;
                        --matrixNumber;
                        string & storageName = tapeMatricesDiagonalStorages[matrixNumber].first;
                        auto tapeMatrixDiagonalStorage = tapeMatricesDiagonalStorages[matrixNumber].second;
                        while (true) {
                            try {
                                system("cls");
                                cout << "Name: " << storageName << "\n";
                                string matrixPrefix = "tape matrix: ", storagePrefix = "storage: ";
                                auto tapeMatrix = toMatrix(tapeMatrixDiagonalStorage);
                                auto tapeMatrixLines = split(tapeMatrixToString(tapeMatrix), '\n');
                                auto storageLines = split(
                                        tapeMatrixDiagonalStorageToString(tapeMatrixDiagonalStorage),
                                        '\n');
                                cout << matrixPrefix << tapeMatrixLines[0] << " "
                                     << storagePrefix << storageLines[0] << '\n';
                                n = int(tapeMatrixLines.size());
                                for (int i = 1; i < n; ++i)
                                    cout << setw(int(matrixPrefix.size())) << " " << tapeMatrixLines[i]
                                         << " " << setw(int(storagePrefix.size())) << " " << storageLines[i] << '\n';
                                cout << "\nItems:\n"
                                     << "1) write tape matrix to file\n"
                                     << "2) write storage to file\n"
                                     << "3) rename\n"
                                     << "4) remove\n"
                                     << "0) back\n"
                                     << "Select item >" << flush;
                                second_key_reading:
                                int item = getch();
                                if (item == '0') // back
                                    break;
                                if (item == '1') { // write tape matrix to file
                                    system("cls");
                                    cout << "Enter filename for the tape matrix writing: " << flush;
                                    string filename;
                                    cin >> filename;
                                    writeMatrix(filename, tapeMatrix);
                                } else if (item == '2') {// write storage to file
                                    system("cls");
                                    cout << "Enter filename for the tape matrix diagonal storage writing: " << flush;
                                    string filename;
                                    cin >> filename;
                                    writeMatrix(filename, tapeMatrixDiagonalStorage);
                                } else if (item == '3') {
                                    system("cls");
                                    cout << "Old matrix name: " << storageName << "\n"
                                         << "Enter new matrix name: " << flush;
                                    string newMatrixName;
                                    cin >> storageName;
                                } else if (item == '4') {// remove
                                    tapeMatricesDiagonalStorages.erase(next(tapeMatricesDiagonalStorages.begin(), matrixNumber));
                                    break;
                                } else
                                    goto second_key_reading;
                            } catch (runtime_error &exc) {
                                log(argv[1], exc.what());
                                cout << exc.what() << endl;
                                pressAnyKeyForContinue();
                            }
                        }
                    }
                }
            } else if (key == '2') { // read matrix from file
                system("cls");
                cout << "Enter filename with matrix: " << flush;
                string path;
                cin >> path;
                tapeMatricesDiagonalStorages.emplace_back(path, toTapeMatrixDiagonalStorage(readMatrix(path)));
            } else if (key == '3') { // read diagonal storage for tape matrix from file
                system("cls");
                cout << "Enter filename with diagonal storage of tape matrix: " << flush;
                string path;
                cin >> path;
                tapeMatricesDiagonalStorages.emplace_back(path, readMatrix(path));
            } else if (key == '4') { // multiply matrices
                system("cls");
                if (tapeMatricesDiagonalStorages.empty()) {
                    cout << "List of matrices is empty";
                    pressAnyKeyForContinue();
                } else {
                    cout << "Matrices:\n";
                    int n = int(tapeMatricesDiagonalStorages.size());
                    for (int i = 0; i < n; ++i)
                        cout << setw(int(to_string(n).size())) << i + 1 << ") "
                             << tapeMatricesDiagonalStorages[i].first << "\n";
                    cout << "Enter two matrices number for multiplying: " << flush;
                    int firstMatrixNumber, secondMatrixNumber;
                    cin >> firstMatrixNumber >> secondMatrixNumber;
                    if (firstMatrixNumber < 0 || firstMatrixNumber > n || secondMatrixNumber < 0 || secondMatrixNumber > n)
                        throw MatrixNumberException();
                    --firstMatrixNumber;
                    --secondMatrixNumber;
                    auto & firstMatrixData = tapeMatricesDiagonalStorages[firstMatrixNumber];
                    auto & secondMatrixData = tapeMatricesDiagonalStorages[secondMatrixNumber];
                    auto & firstStorage = firstMatrixData.second;
                    auto & secondStorage = secondMatrixData.second;
                    auto firstMatrix = toMatrix(firstStorage);
                    auto secondMatrix = toMatrix(secondStorage);
                    auto newMatrix = multiply(firstMatrix, secondMatrix);
                    string newMatrixName = firstMatrixData.first + "_multiply_" + secondMatrixData.first;
                    tapeMatricesDiagonalStorages.emplace_back(newMatrixName, toTapeMatrixDiagonalStorage(newMatrix));
                    cout << "New matrix name: " << newMatrixName << endl;
                    pressAnyKeyForContinue();
                }
            } else
                goto key_reading;
        } catch (runtime_error & exc) {
            log(argv[1], exc.what());
            cout << exc.what() << endl;
            pressAnyKeyForContinue();
        }
    }
    return 0;
}
