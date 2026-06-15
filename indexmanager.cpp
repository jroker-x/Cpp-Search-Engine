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
        
        std::unordered_map<std::string,std::unordered_set<const Document*>> index;

    
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
                    index[word].insert(&doc);
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

    std::unordered_set<const Document*> search(const std::string& word) const{
        auto lowercase = toLower(word);
        std::stringstream ss(lowercase);
        std::string currentword;
        ss >> currentword;
        auto it = index.find(currentword);
        
        if (it == index.end())
        {
            std::cout << "NO RESULTS FOUND!\n";
            return std::unordered_set<const Document*>();

        }
       
        
            auto results = it->second;
           
            
            while (ss >> currentword)
            {
                std::unordered_set<const Document*> intersection;
                auto it = index.find(currentword);
                if (it == index.end())
                {
                std::cout << "NO RESULTS FOUND!\n";
                return std::unordered_set<const Document*>();
                }
                for (auto const& doc : results)
                {
                    if (it->second.find(doc) != it->second.end())
                    {
                        intersection.insert(doc);
                    }
                    
                }
                results = std::move(intersection);
            }
           
        

        return results;
        
    }
};


int main(){

    Searchengine uno;
    uno.loadDirectory("C:\\Users\\hakke\\OneDrive\\Desktop\\cpp test files");
    uno.buildindex();
    std::string query;
    while (true)
    {
        std::cout << "SEARCH: ";
        std::getline(std::cin, query);
        auto result = uno.search(query);
        for (auto const& re : result)
        {
            std::cout << re->path << '\n';
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