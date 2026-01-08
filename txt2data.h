#ifndef HDC_TXT2DATA_H
#define HDC_TXT2DATA_H

#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_set>
#include <vector>

//i'm not happy with this code, it was purely a test.
inline std::vector<std::vector<std::string>> dataset(const std::string &FILEPATH,const int &min_sentence_length=5) { //returns dataset in the form of: vector<sentences:vector<words:string>>
    std::vector<std::string> filenames;
    for (const auto &entry:std::filesystem::recursive_directory_iterator(FILEPATH)) {
        if (!entry.is_directory()) {
            filenames.emplace_back(entry.path().string());
        }
    }
    std::string data;
    for (const auto &file:filenames) {
        std::ifstream stream(file);
        std::string line;
        std::string filecontent;
        if (!stream.is_open()){throw std::runtime_error("Unable to open file: "+file);}
        std::string prev;
        while (std::getline(stream,line)) {
            if (std::isalnum(prev.back())&&std::isalnum(line.front())) {
                filecontent+=line;
            }
            else {
                filecontent+=line+' ';
            }
            prev=line;
        }
        stream.close();
        data+=filecontent;
    }
    for (int i=0;i<static_cast<int>(data.size());i++) {
        char &c=data[i];
        //lowercase
        c=std::tolower(c);
        //sentence end
        if (c=='?'||c=='!'){c='.';}
        //word fragmentation
        else if (!(std::isalnum(c)||c=='.'||c=='\'')){c=' ';}
    }
    std::vector<std::vector<std::string>> sentences;
    std::vector<std::string> sentence;
    std::string word;
    for (int i=0;i<static_cast<int>(data.size());i++) {
        char &c=data[i];
        if (!word.empty()&&(c==' '||c=='.')) { //theres a word to store
            //preprocess word here
            //remove 1 letter words that arent "I" or "A"
            if (word.size()==1&&word[0]!='i'&&word[0]!='a') {
                word.clear();
                continue;
            }
            //remove periods
            while (word.back()=='.'){word.pop_back();}
            //remove "'" in words and words with numbers in them
            bool cont=false;
            for (int j=0;j<static_cast<int>(word.size());j++) {
                if (word[j]=='\''){word.erase(word.begin()+j);}
                if (std::isdigit(word[j])) {
                    word.clear();
                    cont=true;
                    break;
                }
            }
            if (cont){continue;}
            sentence.emplace_back(word);
            word.clear();
        }
        if (i>0&&data[i-1]=='.') { //theres a sentence to store
            //preprocess sentence here
            if (static_cast<int>(sentence.size())<min_sentence_length) {
                sentence.clear();
                continue;
            }
            sentences.emplace_back(sentence);
            sentence.clear();
        }
        //build word
        if (c!=' '&&c!='.') {
            word+=c;
        }
    }
    return sentences;
}







#endif //HDC_TXT2DATA_H