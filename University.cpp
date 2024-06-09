#include "University.h"

University::University (const University& unit) {
    this->database = unit.database;
}

University::University (const std::string filename) {
    std::ifstream file (filename, std::ios::in);
    if (file.is_open()) {
        std::string row;
        while (std::getline(file, row)) {
            std::istringstream iss(row);
            std::string word;
            Student unit;
            int count = 0;
            grade g;
            unsigned short session;
            while (iss >> word) {
                if (count == 0) { unit.setNumber(word); }
                else if (count == 1) { unit.setFirstName(word); }
                else if (count == 2) { unit.setMiddleName(word); }
                else if (count == 3) { unit.setLastName(word); }
                else if (count == 4) { unit.setYear(std::stoi(word)); }
                else if (count == 5) { unit.setBirthday(word); }
                else if (count == 6) { unit.setFaculty(word); }
                else if (count == 7) { unit.setDepartment(word); }
                else if (count == 8) { unit.setGroup(word); }
                else if (count == 9) { unit.setGender(word); }
                else if (count == 10) { session = std::stoi(word); }
                else if (count == 11) { g.subject = word; }
                else if (count == 12) { g.date = word; }
                else if (count == 13) { g.mark = std::stoi(word); }
                ++count;
            }
            if (this->database.isExists(unit)) {
                unsigned int index = this->database.find(unit);
                if (session > this->database[index].getNumberOfSessions()) {
                    for (int i = 0; i < (session - this->database[index].getNumberOfSessions()); ++i) {
                        this->database[index].createSession();
                    }
                }
                this->database[index].appendGradeToSession(session, g);
            }
            else {
                unit.createSession();
                unit.appendGradeToSession(1, g);
                this->database.append(unit);
            }
        }
    }
    else {
        throw std::runtime_error("File does not exist");
    }
    file.close();
}

University& University::operator= (const University& unit) {
    this->database = unit.database;
    return *this;
}

std::ostream& operator<< (std::ostream& os, University& unit) {
    std::string toOut;
    for (int i = 0; i < unit.database.length(); ++i) {
        Student guy = unit.database[i];
        List <List <grade>> sessions = guy.getSessions();
        for (int j = 0; j < sessions.length(); ++j) {
            for (int k = 0; k < sessions[j].length(); ++k) {
                std::string row = 
                    guy.getNumber() + " " + 
                    guy.getFirstName() + " " + 
                    guy.getMiddleName() + " " + 
                    guy.getLastName() + " " + 
                    std::to_string(guy.getYear()) + " " + 
                    guy.getBirthday() + " " + 
                    guy.getFaculty() + " " + 
                    guy.getDepartment() + " " + 
                    guy.getGroup() + " " + 
                    guy.getGender() + " " + 
                    sessions[j][k].subject + " " + 
                    sessions[j][k].date + " " + 
                    std::to_string(sessions[j][k].mark) + "\n";
                toOut += row;
            }
        }
    }
    os << toOut;
    return os;
}

void University::toFile (const std::string filename) {
    std::string toOut;
    for (int i = 0; i < this->database.length(); ++i) {
        Student guy = this->database[i];
        List <List <grade>> sessions = guy.getSessions();
        for (int j = 0; j < sessions.length(); ++j) {
            for (int k = 0; k < sessions[j].length(); ++k) {
                std::string row = 
                    guy.getNumber() + " " + 
                    guy.getFirstName() + " " + 
                    guy.getMiddleName() + " " + 
                    guy.getLastName() + " " + 
                    std::to_string(guy.getYear()) + " " + 
                    guy.getBirthday() + " " + 
                    guy.getFaculty() + " " + 
                    guy.getDepartment() + " " + 
                    guy.getGroup() + " " + 
                    guy.getGender() + " " + 
                    std::to_string(j + 1) + " " + 
                    sessions[j][k].subject + " " + 
                    sessions[j][k].date + " " + 
                    std::to_string(sessions[j][k].mark) + "\n";
                toOut += row;
            }
        }
    }
    std::ofstream file(filename.c_str(), std::ios::trunc);
    file << toOut;
    file.close();
}

void University::append (const Student unit) {
    this->database.append(unit);
}

void University::remove (const unsigned int index) {
    this->database.remove(index);
} 

unsigned int University::find (const Student unit) {
    return this->database.find(unit);
}

bool University::isExists (const Student unit) {
    return this->database.isExists(unit);
}

Student University::get (const unsigned int index) {
    return this->database[index];
}

List <std::pair<Student, unsigned short>> University::sort (const unsigned short session) {
    List <std::pair<Student, unsigned short>> result;
    for (int i = 0; i < this->database.length(); ++i) {
        if (this->database[i].getNumberOfSessions() < session - 1) {
            continue;
        }
        List <grade> grades = this->database[i].getGrades(session - 1);
        unsigned int count = 0;
        for (int j = 0; j < grades.length(); ++j) {
            if (grades[j].mark == 5) { ++count; }
        }
        result.append(std::make_pair(this->database[i], count));
    }
    if (result.length() > 0) {
        for (int i = 0; i < result.length() - 1; ++i) {
            for (int j = 0; j < result.length() - i - 1; ++j) {
                if (result[j].second < result[j + 1].second) {
                    std::pair<Student, unsigned short> temp = result[j];
                    result[j] = result[j + 1];
                    result[j + 1] = temp;
                }
            }
        }
    }
    return result;
}

void University::encrypt (const std::string filenameIn, const std::string filenameOut) {
    std::string key;
    char alphabet[63] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz1234567890";
    srand(time(NULL));
    for (int i = 0; i < 16; ++i) {
        key += alphabet[rand() % 62];
    }
    std::ofstream file("key.txt", std::ios::trunc);
    file << key;
    file.close();
    std::string commandAES = "openssl aes-128-cbc -salt -in " + filenameIn + " -out " + filenameOut + " -pass pass:" + key;
    std::system(commandAES.c_str());
    if (std::remove(filenameIn.c_str()) != 0) {
        std::cerr << "CANNOT DELETE >>> " + filenameIn << std::endl;
    }
    std::string commandRSA = "openssl rsautl -encrypt -inkey rsa.public -pubin -in key.txt -out key.txt.enc";
    std::system(commandRSA.c_str());
    if (std::remove("key.txt") != 0) {
        std::cerr << "CANNOT DELETE >>> key.txt" << std::endl;
    }
}

void University::decrypt (std::string filenameIn, std::string filenameOut) {
    std::string commandRSA = "openssl rsautl -decrypt -inkey rsa.private -in key.txt.enc -out key.txt";
    std::system(commandRSA.c_str());
    std::string key;
    std::ifstream file("key.txt", std::ios::in);
    file >> key;
    file.close();
    if (std::remove("key.txt.enc") != 0) {
        std::cerr << "CANNOT DELETE >>> key.txt.enc" << std::endl;
    }
    std::string commandAES = "openssl aes-128-cbc -d -in " + filenameIn + " -out " + filenameOut + " -pass pass:" + key;
    std::system(commandAES.c_str());
    if (std::remove(filenameIn.c_str()) != 0) {
        std::cerr << "CANNOT DELETE >>> " + filenameIn << std::endl;
    }
}