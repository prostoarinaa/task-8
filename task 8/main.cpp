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

using namespace std;
using namespace std::__fs::filesystem;

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
    vector<char> del = {' ','.',',','-','?','!', ';', ':', ')', '(', '"'};
    for (auto i =0;i< str.size();i++) {
        
        if (find(del.cbegin(), del.cend(), str[i])==del.cend()) {
            s += str[i];
        }
        else if (s == word) {
            countOfSpesialWords++;
            s = "";
        }
        else {
            s = "";
        }
    }
    return countOfSpesialWords;
}

void tread1(string filename){
    ifstream file;
    thread::id id;
    file.open(filename);
    string str = "", s;
    unsigned long long countOfWords = 0;
    unsigned long long countOfSpesialWords = 0;
    thread thread_array[4];
  //  thread_array[0] = thread(CountWordsInStr, countOfWords, str);
    for (int i = 1; i < 4; i++) {
            thread_array[i] = thread(CountSpesialWordInStr, countOfSpesialWords,ref(countOfWords), str);
    }
    for (int i = 1; i < 4; i++) {
           if (thread_array[i].joinable()) {
               id = thread_array[i].get_id();
               thread_array[i].join();
               cout << "Thread with id " << id << " finished. With result "<<countOfSpesialWords<<"\n";
           }
    }
  //  thread th(CountWordsInStr,ref(countOfWords), str);
   // int i =0;
    while(1)
    {
        getline(file, str, '\n');
      //  i++;
       // CountWordsInStr(countOfWords, str);
     //   CountSpesialWordInStr(countOfSpesialWords, str, "Катя");
        if (file.eof()) break;
    }
    cout <<"countOfSpesialWords= "<<countOfSpesialWords<< endl;
    cout <<"countOfWords= "<<countOfWords<< endl;
    file.close();
  //  th.join();
}

int main() {
    vector <string> vectorOfFiles;
    string path = "/Users/pk/Desktop/OI/TASKS/task 8/task 8"; // whatever the path is
    for (const auto & entry : directory_iterator(path))
        vectorOfFiles.push_back(entry.path());
    for(auto i = 0; i< vectorOfFiles.size();i++) {
        cout << vectorOfFiles[i]<< endl;
    }
    //for(auto i = 0; i< vectorOfFiles.size();i++) {
        tread1(vectorOfFiles[0]);
   // }
    return 0;
}
