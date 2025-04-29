// Rushikesh Palodkar - palodkar24612657
// Project 1 - Operating Systems
// This project creates a hash table in a binary file and searches names.
// Took me a while to get lseek working properly. Debugging was tricky, but I got it!


#include <fcntl.h>     // open, O_CREAT, etc.
#include <unistd.h>    // read, write, lseek, close
#include <cstring>     // memset, strncpy, strtok
#include <iostream>    // for input output
#include <fstream>     // ifstream for employees.txt
using namespace std;

const int BUCKET_SIZE = 200;
const int NAME_SIZE = 16;

struct Employee {
    char name[NAME_SIZE];
    int salary;
};
// I use 31 here because it's a prime number and I saw it's good for hashing.
// Not 100% sure why, but it seems to work well for strings.
int hashFunction(const char* name) {
    int hash = 0;
    for (int i = 0; name[i] != '\0'; ++i) {
        hash = (hash * 31 + name[i]) % BUCKET_SIZE; // funcation takes name and turn into 0 to 199
    }
    return hash;
}

int main(int argc, char* argv[]) { // accepting command line arguments we need it in next phases which is search phase
        // Create or truncate the binary file (do this ONCE)
	int fd = open("employee_table.bin", O_CREAT | O_RDWR | O_TRUNC, 0666);
	if (fd < 0) {
    	cerr << "Error creating binary file.\n";
    		return 1;
	}

	// Fill file with 200 empty Employee records
	Employee empty;
	memset(&empty, 0, sizeof(Employee));
	for (int i = 0; i < BUCKET_SIZE; ++i) {
    		write(fd, &empty, sizeof(Employee));
	}
	//open employees.txt file for reading employee records
	ifstream infile("employees.txt");
	string line;


	// going line by line in the file employees.txt to get all data
    while (getline(infile, line)) { 
        size_t comma = line.find(',');
        if (comma == string::npos) continue; // this skips the line of there is no comma

        string name = line.substr(0, comma); // this gets the name position
        int salary = stoi(line.substr(comma + 1)); // this gets the salary after the comma till end
No cape code as 	
// if (salary < 10000 || salary > 250000) continue; // skip invalid salaries
	// This creates an employee and stores the employee and its salary in Employee struct
        Employee emp;
        strncpy(emp.name, name.c_str(), NAME_SIZE); // copy name and salary
        emp.salary = salary; // stores employee salary as salary from struct to varuable

        int index = hashFunction(emp.name); // Uses harsh function to convert the employee name into number 0-199 this tells us where in the binary file to store this employee
        int originalIndex = index; // stores the orignal index

        // Linear probing for collision resolution
	// in this we are checking is this spot already taken? if yes i will keep looking for the next one , line moving down the row in table -- linear probing
        for (int i = 0; i < BUCKET_SIZE; ++i) { // bucket size is 200 so we check just in case if our complete fill is full
            off_t offset = index * sizeof(Employee);
        // This part confused me at first — lseek moves the file pointer, like jumping
        // to the correct bucket. I had to look this up in a few places.
            lseek(fd, offset, SEEK_SET); // asks os to move my file pointer to this spot

            Employee existing;
            read(fd, &existing, sizeof(Employee)); // checking whats at this location

            if (existing.name[0] == '\0') { // check if its Empty spot
                lseek(fd, offset, SEEK_SET);
                write(fd, &emp, sizeof(Employee)); // write the employee data to this line
                break; // done
            }

            // Collision – try next
            index = (index + 1) % BUCKET_SIZE; // if its taken wrap around to 0 if you go past 199
            if (index == originalIndex) { // If this condition is true this means that i have tried all the spot in file and there is no spot empty to store that emapoyee -- file is full
                cerr << "Hash table full, cannot insert.\n"; 
                break;
            }
        }
    }

   
        // Reopen the file for reading
    close(fd);
// close the file after writting
// I did testing here as this was the first part where we add all the employee at there respective places in hash map
// Phases 2

// we open the file to search the employee in our hash table 
    fd = open("employee_table.bin", O_RDONLY);// open in read only mode
    if (fd < 0) { // if file openn failed
        cerr << "Error opening binary file for reading.\n";
        return 1;
    }

    // Handle command-line search names
    // Looping through the command line name
    for (int arg = 1; arg < argc;  ++arg) {
    // start at arg = 1 because the argv[0] is the program name
        const char* target = argv[arg]; // same the current seach name in traget
        int index = hashFunction(target); // hash to get the get the starting index
        int originalIndex = index; // save to orignal index so you can check for full loop
        bool found = false; // start with found flase

        for (int i = 0; i < BUCKET_SIZE; ++i) { // same as before linear probing
            off_t offset = index * sizeof(Employee);
            lseek(fd, offset, SEEK_SET); // move to right location in the binary file

            Employee emp;
            read(fd, &emp, sizeof(Employee)); // read the employee from that slot

            if (emp.name[0] == '\0') { // if its emapy stop
                break; // Empty = not found
            }

            if (strncmp(emp.name, target, NAME_SIZE) == 0) { // if it matches we found and print name salaray and index
                cout << "Name = " << emp.name
                     << "\tSalary = " << emp.salary
                     << "\tIndex = " << index << endl;
                found = true;
                break;
            }

            index = (index + 1) % BUCKET_SIZE; //try next sopt and write around to 0
            if (index == originalIndex) break; // if we loop to same spot we stop to privent infinity loop
        }

        if (!found) { // if not found we print not found
            cout << "Name = " << target
                 << "\tSalary = Not found"
                 << "\tIndex = Not found" << endl;
        }
    }

    close(fd);
    return 0;
}
