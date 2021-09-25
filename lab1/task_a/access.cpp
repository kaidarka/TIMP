#include <direct.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <windows.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

vector<string> info;
string path = "template.tbl";

void getFile();
string getUserName();
bool isFile(const std::string& file);
void lockFiles();
void unlockFiles();
void addFiles();
void deleteFiles();
void changeFiles();
bool authorization();
void choice();
void init();

int main() {
    // Проверка иницализации
    if (isFile(path)) {
        // Папка уже иницализирована
        getFile();
        cout << "Enter password: ";
        if (authorization()) {
            // Пароль верный
            choice();
        } else {
            // Пароль неверный, выход из программы
            cout << "Wrong password";
            return 0;
        }
    } else {
        // Инициализация
        init();
        choice();
    }
    cout << "Good day";
    return 0;
}

// Получение файла в вектор строк
void getFile() {
    ifstream fin;
    string buff;
    fin.open(path);
    for (int i = 0; i < 3; i++) {
        getline(fin, buff);
    }
    fin >> buff;
    fin >> buff;
    while (buff != ".TE") {
        info.push_back(buff);
        fin >> buff;
        fin >> buff;
    }
    fin.close();
}

// Получение имени пользователя, чтобы позже вписать его в команду
string getUserName() {
    char path[MAX_PATH], *p = NULL;
    GetEnvironmentVariable((LPCTSTR) "USERPROFILE", (LPSTR)path,
                           (DWORD)MAX_PATH);
    p = &path[strlen(path) - 1];
    for (; *p != '\\'; --p)
        ;
    ++p;

    // Возвращаем строку в нужном нам виде
    string uName = " /P " + string(p);
    return uName;
}

// Проверка на инициализацию
bool isFile(const std::string& file) {
    struct stat buf;
    return (stat(file.c_str(), &buf) == 0);
}

// Блокировка файлов (N - отключение доступа)
void lockFiles() {
    // Получаем имя пользователя из функции выше
    string uName = getUserName();

    // Получаем текущую дирректорию
    char currentDir[FILENAME_MAX];
    _getcwd(currentDir, sizeof(currentDir));

    // Собираем строку в нужную нам команду
    string fPartCmd = "cacls " + string(currentDir);
    // Для каждого файла выполняем команду, поэтому перебираем массив файлов и подставляем каждый
    for (int i = 1; i < info.size(); i++) {
        if (!isFile(info[i])) {
            ofstream createFile;
            createFile.open(info[i]);
            createFile.close();
        }
        string finalCmd = fPartCmd + "\\" + info[i] + uName + ":N";
        // Приводим к нужному нам типу
        const char* c = finalCmd.c_str();
        cout << finalCmd << endl;

        // Выполнение команды
        system(c);
    }
    cout << "Files has been locked" << endl;
}

// Разблокировка файлов, процесс аналогичный блокировке, за исключением флагов.
// (F - полный доступ)
void unlockFiles() {
    // Получаем имя пользователя из функции выше
    string uName = getUserName();

    // Получаем текущую дирректорию
    char currentDir[FILENAME_MAX];
    _getcwd(currentDir, sizeof(currentDir));

    // Собираем строку в нужную нам команду
    string fPartCmd = "cacls " + string(currentDir);
    // Для каждого файла выполняем команду, поэтому перебираем массив файлов и подставляем каждый
    for (int i = 1; i < info.size(); i++) {
        if (!isFile(info[i])) {
            ofstream createFile;
            createFile.open(info[i]);
            createFile.close();
        }
        string finalCmd = fPartCmd + "\\" + info[i] + uName + ":F";
        // Приводим к нужному нам типу
        const char* c = finalCmd.c_str();
        // Выполнение команды
        system(c);
    }
    cout << "Files has been unlocked" << endl;
}

void addFile() {
    //Добавлем файл в вектор, после чего пересобираем template
    cout << "Enter name of file: ";
    string input;
    cin >> input;
    info.push_back(input);
    ofstream add;
    add.open(path);
    add << ".TS\n tab( );\n c l.\n";
    for (int i = 0; i < info.size(); i++) {
        add << i + 1 << " " << info[i] << "\n";
    }
    add << ".TE";
    add.close();
    cout << endl;
    changeFiles();
}

void deleteFile() {
    //Аналогично прошлому удаляем файл из вектора, после чего пересобираем template
    int a;
    cout << "What file do you want to delete(number): ";
    cin >> a;
    if (a > 0 && a < info.size()){
        auto iter = info.cbegin();
        info.erase(iter + a);
        ofstream removeFile;
        removeFile.open(path);
        removeFile << ".TS\n tab( );\n c l.\n";
        for (int i = 0; i < info.size(); i++) {
            removeFile << i + 1 << " " << info[i] << "\n";
        }
        removeFile << ".TE";
        removeFile.close();
    }
    
    cout << endl;
    changeFiles();
}

void changeFiles() {
    // Выбор действия с файлами
    int a;
    cout << "Your files:" << endl;
    for (int i = 1; i < info.size(); i++) {
        cout << i << " " << info[i] << endl;
    }
    cout << "What do you want:" << endl
         << "[1] Add file" << endl
         << "[2] Delete file" << endl
         << "[3-default] Back" << endl;
    cin >> a;
    cout << endl;
    if (a == 1) {
        addFile();
    } else if (a == 2) {
        deleteFile();
    } else {
        choice();
    }
}

// Авторизация, тут все просто
bool authorization() {
    string input;
    getline(cin, input);
    hash<string> hasher;
    input = hasher(input);
    if (input == info[0]) {
        return true;
    } else {
        return false;
    }
}

// Выбор нужного действия
void choice() {
    int choiceInput;
    cout << "What do you want?" << endl
         << "[1] Lock files" << endl
         << "[2] Unlock files" << endl
         << "[3] Change template.tbl" << endl
         << "[4-default] Exit" << endl;
    cin >> choiceInput;

    // Применяем выбор и вызываем соответствеющие функции, после чего
    // заканчиваем выполнение программы
    if (choiceInput == 1) {
        lockFiles();
    } else if (choiceInput == 2) {
        unlockFiles();
    } else if (choiceInput == 3) {
        changeFiles();
    }
}

//Иницализация
void init() {
    //Создание файла
    ofstream initFile;
    initFile.open("template.tbl");
    //Ввод пароля
    string pass;
    cout << "Enter password for access: ";
    getline(cin, pass);
    //Хэширование пароля
    hash<string> hasher;
    pass = hasher(pass);
    //Добавление пароля в templte
    info.push_back(pass);
    initFile << ".TS\n tab( );\n c l.\n1 " << pass << "\n.TE";
    initFile.close();
}
