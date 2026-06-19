#include<iostream>
#include<vector>

size_t hash(const std::string& str);

template<typename k,typename v>
class Hashmap{
    private:
    std::vector<std::vector<std::pair<std::string,int>>> buckets;
    size_t entrycount = 0;

    public:
        Hashmap(size_t bucketcount = 10)
        : buckets(bucketcount){}

        void insert(const k& key,const v& value){
            
            size_t bucketindex = hash(key);
            bucketindex %= buckets.size();
           

            for (auto const& pair : buckets[bucketindex])    
            {
                if (pair.first == key)
                {
                    pair.second = value;
                    return;
                }
                
            }
            buckets[bucketindex].push_back({key,value});
            entrycount++;

            if (static_cast<double>(entrycount) / buckets.size() >= 1.0)
            {
                rehash();
            }
            


        }

        void Printbucket()const{
            for (int i = 0; i < buckets.size(); i++)
            {
                std::cout << "Bucket" << i << '\n';
                for (const auto& pair : buckets[i])
                {
                    std::cout << pair.first << " : " << pair.second << '\n';
                }
                std::cout << '\n';
            }
            
            
        }

        bool contains(const k& key){

            size_t bucketindex = hash(key);
            bucketindex %= buckets.size();
            for (const auto& pair : buckets[bucketindex])
            {
              if (pair.first == key)
              {
                return true;
              }
              
            }
            return false;
        }

        v* find(const k& key){

            size_t bucketindex = hash(key);
            bucketindex %= buckets.size();
            for (auto& pair : buckets[bucketindex])
            {
              if (pair.first == key)
              {
                return &pair.second;
              }
              
            }
            return nullptr;

        }
        
        v& operator[](const k& key){

            size_t bucketindex = hash(key);
            bucketindex %= buckets.size();
            for (auto& pair : buckets[bucketindex])
            {
              if (pair.first == key)
              {
                return pair.second;
              }
            }

            buckets[bucketindex].push_back({key,v{}});
            entrycount++;

            if (static_cast<double>(entrycount) / buckets.size() >= 1.0)
            {
                rehash();
            }

            return  *find(key);
        }

        void rehash(){

            std::vector<std::vector<std::pair<k,v>>> newBuckets(buckets.size() * 2);
            for (auto const& ob : buckets)     
            {
                for (auto const& pair : ob)
                {
                    size_t newIndex = hash(pair.first);
                    newIndex %= newBuckets.size();
                    newBuckets[newIndex].push_back(pair);
                   
                } 
            }
             buckets = std::move(newBuckets);
        }

        size_t size(){
            return entrycount;
        }

        size_t bucketCount()const{
            return buckets.size();
        }

        double loadFactor() const
        {
        return static_cast<double>(entrycount) / buckets.size();
        }  



}; 
 

int main(){

    Hashmap<std::string,int> map;
 
}

size_t hash(const std::string& str){

    size_t hash = 0;

    for (char c : str)
    {
       hash += c;
    }

    return hash;

}
