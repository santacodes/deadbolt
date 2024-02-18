#include <string>
#include <vector>
#include <map>
#ifndef FILEBACKEND 
#define FILEBACKEND 

class filebackend {
    public:
        filebackend(std::vector<std::string>*);

    private:
        
        void create();
        void append();
        void del();
        std::vector<std::string> *functions; 

        
};


#endif