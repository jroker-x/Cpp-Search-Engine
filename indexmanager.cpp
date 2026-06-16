#include <iostream>
#include <memory>
#include <utility>
#include <sstream>
#include <vector>
#include <filesystem>
#include <fstream>
#include <algorithm> 
#include <unordered_map>
#include <unordered_set>
 

std::string toLower(std::string tolower);
std::string normalizeWord(std::string normalize);

struct Document
{
    std::string path;
    std::string content;
};


class Searchengine{

    private:
        std::vector<Document> documents;  
        std::unordered_map<std::string,std::unordered_map<const Document*,int>> index;
        std::unordered_map<std::string, int> globalWordCounts;

    
    public:
    
    void buildindex(){

        index.clear();
        for (const auto& doc :documents)
        {

            std::stringstream ss(doc.content);
            std::string word;
            while (ss >> word)
            {
                word = normalizeWord(word);
                if (!word.empty())
                {
                    index[word][&doc]++;
                    globalWordCounts[word]++;
                }
                
              
            }
    
        }
        std::cout << "INDEXED WORDS: " << index.size() << '\n';
    }
    void loadDirectory(const std::string& path){

        for (auto const& entry : std::filesystem::directory_iterator(path))
        {
            std::filesystem::path filePath = entry.path();
            std::string filename = filePath.filename().string();
                if (filePath.extension() == ".txt")
                    {
            
                        std::ifstream infile(filePath);

                            if (infile.is_open())
                            {

                                Document input;
                                input.path = filePath.string();
                                std::string line;
                                
                                while (std::getline(infile,line))
                                {
                                    input.content += line + "\n";
                                }
                                input.content = toLower(input.content);
                                documents.push_back(input);
                            }
                    }
        }

    };

    std::unordered_map<const Document*,int> search(const std::string& word) const{
        auto lowercase = toLower(word);
        std::stringstream ss(lowercase);
        std::string currentword;
        ss >> currentword;
        auto it = index.find(currentword);
        
        if (it == index.end())
        {
            std::cout << "NO RESULTS FOUND!\n";
            return {};

        }

            auto results = it->second;
           
            while (ss >> currentword)
            {
                std::unordered_map<const Document*,int> intersection;
                auto it = index.find(currentword);
                if (it == index.end())
                {
                std::cout << "NO RESULTS FOUND!\n";
                return {};
                }
                for (auto const& doc : results)
                {
                    auto found = it->second.find(doc.first);
                    if (found!= it->second.end())
                    {
                        intersection[doc.first] = doc.second + found->second;
                    }
                    
                }
                results = std::move(intersection);
            }

        return results;
        
    }

    void printTopWords(int count){

        std::vector<std::pair<std::string,int>> topwords;

        for (const auto& pair : globalWordCounts)
        {
            topwords.push_back(pair);
        }
        
        std::sort(topwords.begin(),topwords.end(),[] (const auto& a, const auto& b){
            return a.second > b.second;
        });
        
        auto limit = std::min(count,static_cast<int>(topwords.size()));
        for (int i = 0; i < limit; i++)
        {
            std::cout << topwords[i].first << " : " << topwords[i].second << '\n';
        }
        
        


    }
};

int main(){

    Searchengine uno;
    std::string directory;
    std::cout << "PLEASE ENTER THE DIRECTORY: \n";
    std::getline(std::cin,directory); 
    uno.loadDirectory(directory);
    uno.buildindex();
    uno.printTopWords(5);
    std::string query;
    while (true)
    {
        std::cout << "SEARCH: ";
        std::getline(std::cin, query);
        auto result = uno.search(query);
        for (auto const& re : result)
        {
            std::cout << re.first->path <<" : " << re.second << '\n';
        }
    }
    
}

std::string toLower(std::string tolower) {
         std::transform(tolower.begin(), tolower.end(),tolower.begin(),[](char c){
            return std::tolower(c);
        });
        
        return tolower;
    }

std::string normalizeWord(std::string normalize) {
    normalize = toLower(normalize);
    normalize.erase(
        std::remove_if(normalize.begin(),normalize.end(),[](char c){
        return !std::isalpha(c);
        }),
        normalize.end()
        
    );   
        return normalize;
    }
