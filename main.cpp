#include <stdlib.h>
#include <string>
#include <vector>

typedef student element;
 
struct student {
   student(unsigned age, std::string name);
   bool operator==(const student & other) const;
   long long hash() const;
   unsigned age_;
   std::string name_;
};


class HashStrat {

};

template <typename T, long long (*hashS)(element) = 0>
class Linkedhs {



    Linkedhs() {

    }


    Linkedhs(T ...Args) {
        
    }

private:
    long long hash(T e) {
        if hashS == 0 {
            return e.hash();
        } else {
            return hashS(e);
        }
    }

};