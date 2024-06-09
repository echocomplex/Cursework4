#ifndef MS_UNIVERSITY
#define MS_UNIVERSITY


#include "List.h"
#include <sstream>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdio>
#include <map>
#include "Student.h"
#include <ctime>


class University {
private:
    List <Student> database;
public:
    University () {}
    University (const University& unit);
    University (const std::string filename);
    University& operator= (const University& unit);
    friend std::ostream& operator<< (std::ostream& os, University& unit);
    ~University () {}
    void toFile (const std::string filename);
    void append (const Student unit);
    void remove (const unsigned int index);
    unsigned int find (const Student unit);
    Student get (const unsigned int index);
    bool isExists (const Student unit);
    List <std::pair<Student, unsigned short>> sort (const unsigned short session);
    static void encrypt (const std::string filenameIn, const std::string filenameOut);
    static void decrypt (const std::string filenameIn, const std::string filenameOut);
};

#endif // MS_UNIVERSITY