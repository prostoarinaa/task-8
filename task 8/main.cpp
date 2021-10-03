/*Задача#8:  Напиши еще многопоточную программу для подсчёта кол-ва слов в тексте. Текст возьми из любого источника, но что бы было его много в итоге ... 50кб к примеру в одном файле.
 Программа должна мониторить файлы с текстом в папке, т.е. если появится новый файл то и для него начать подсчет.
 Подсчет для каждого файла должен быть в отдельном трэде для вариантов:
Общее кол-во слов (один трэд)
3 определенных слова на выбор (еще 3 трэда).
Вычитываем текст из файла кусками.
Вывести рез-т в косоль просто после того как подсчет завершен.
Вид Вывода, в <> подставляюся конкретные числа и строки, остальное захардкодить с помощью #define:
<thread_id> || <file_name> || Start_time: <HH:MM:SS>  || Finish_time: <HH:MM:SS>  || Common: <value> | <Word1>: <value> |  <Word2>: <value> | <Word3>: <value>*/

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include <thread>
#include <fstream>
#include <filesystem>
#include <algorithm>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;
using namespace std::__fs::filesystem;
mutex m;
unsigned long long CountWordsInStr (unsigned long long &countOfWords, string str) {
    for ( auto i = 0;i<str.size();i++ ) {
 
            if (str[i] == ' ') {
                countOfWords++;
            }
    }
    return countOfWords;
}

unsigned long long CountSpesialWordInStr (unsigned long long &countOfSpesialWords, string str, string word) {

    string s = "";
    str += ' ';
    vector<char> del = {' ','.',',','-','?','!', ';', ':', ')', '(', '"', '\n'};
    for (auto i =0;i< str.size();i++) {
        if (find(del.cbegin(), del.cend(), str[i])==del.cend()) {
            s += str[i];
        }
        else {
            if (s == word) {
                countOfSpesialWords++;
                s = "";
        }
            else {
                s = "";
            }
        }
    }
    return countOfSpesialWords;
}

void tread(string filename){
    ifstream file;
    thread::id id;
    file.open(filename);
    string str = "", word1 = "Annotation", word2 = "Катя", word3 = "гроза";
    unsigned long long countOfWords = 0;
    unsigned long long countOfSpesialWords1 = 0,
                       countOfSpesialWords2 = 0,
                       countOfSpesialWords3 = 0;
    while(1)
    {
        getline(file, str, '\n');
        thread thread_array[4];
        thread_array[0] = thread(CountWordsInStr, ref(countOfWords), str);
        thread_array[1] = thread(CountSpesialWordInStr, ref(countOfSpesialWords1), str, word1);
        thread_array[2] = thread(CountSpesialWordInStr, ref(countOfSpesialWords2), str, word2);
        thread_array[3] = thread(CountSpesialWordInStr, ref(countOfSpesialWords3), str, word3);
        for (int i = 0; i < 4; i++) {
               if (thread_array[i].joinable()) {
                   id = thread_array[i].get_id();
                   thread_array[i].join();
               }
//                   if (i != 0) {
//                       if ( i==1)
//                           cout << "Thread with id " << id << " finished. With result "<<countOfSpesialWords1<<"\n";
//                       else if (i==2)
//                           cout << "Thread with id " << id << " finished. With result "<<countOfSpesialWords2<<"\n";
//                       else
//                           cout << "Thread with id " << id << " finished. With result "<<countOfSpesialWords3<<"\n";
//                   }
//                   else {
//                       cout << "Thread with id " << id << " finished. With result "<<countOfWords<<"\n";
//                   }
               
        }
        if (file.eof()) break;
    }
    cout <<"countOfSpesialWords1= "<<countOfSpesialWords1<< endl;
    cout <<"countOfSpesialWords2= "<<countOfSpesialWords2<< endl;
    cout <<"countOfSpesialWords3= "<<countOfSpesialWords3<< endl;
    cout <<"countOfWords= "<<countOfWords<< endl;
    file.close();
}

int main() {
    vector <string> vectorOfFiles;
    string path = "/Users/pk/Desktop/OI/TASKS/task 8/task 8/books"; // whatever the path is
    while(true) {
        cout << "---------NEW ITERATION----------"<< endl;
        for (const auto & entry : directory_iterator(path)) {
            if (find(vectorOfFiles.cbegin(), vectorOfFiles.cend(), entry.path())==vectorOfFiles.cend()) {
                vectorOfFiles.push_back(entry.path());
            }
            else continue;
        }
    for(auto i = 0; i< vectorOfFiles.size();i++) {
        cout << vectorOfFiles[i]<< endl;
    }
    for(auto i = 0; i< vectorOfFiles.size();i++) {
    auto start = std::chrono::steady_clock::now();
        tread(vectorOfFiles[i]);
    auto finish = steady_clock::now();
    cout << "A= " << duration_cast<milliseconds>(finish-start).count()<<endl;
    }
        this_thread::sleep_for(chrono::milliseconds(10000));
    }
    
    return 0;
}
