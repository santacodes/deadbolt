#include <string>
#include <vector>
#ifndef FILEBACKEND 
#define FILEBACKEND 

class filebackend {
    public:
        filebackend(std::vector<std::string>*);
        void create();
        void append();
        void del();
    private:
        std::vector<std::string> *functions; 
};


#endif