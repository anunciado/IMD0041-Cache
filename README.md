# Cache

Cache memory is a small type of volatile computer memory that provides high-speed data access to a processor and stores frequently used computer programs, applications, and data. It is the fastest memory in a computer, and is usually integrated into the motherboard and is embedded directly into the processor or mainstream random access memory (RAM). Cache memory provides faster data storage and access by storing program instances and data accessed routinely by the processor. So when a processor requests data that already has an instance in the cache, it does not have to go to the main memory or the hard disk to fetch the data. The cache simulator will be the implementation of a tool that simulates the execution of a cache.

In this context, the project receives entries to initialize the size of the blocks / lines in words, the number of blocks in the main memory and the number of lines in the cache memory. The simulator will receive as input, in sequence, the size of the block (in number of words), the number of blocks of the main memory, the number of rows in the cache, the mapping (1 - Direct, where each block can be placed in a 2 - Fully Associative, where each block can be placed in any position in the cache; 3 - Partially Associative, where each block can be placed in a restricted set of positions in the cache.), the number of sets (case (1 - Random, where the block to be substituted is randomly chosen; 2 - FIFO, where the oldest block is replaced (1 - Random, where the block to be replaced is randomly selected; even though it has been recently used), 3 - LFU, where the least used block is replaced, 4 - LRU, where it is replaced the block that has not been used for a long time.), through these inputs the program will simulate the execution of the requi addresses of the main memory contained in the cache memory, changing it when necessary.

For this implementation, we used the C ++ language with the use of four non-standard libraries of the language, being: the string, where the "Read" and "Show" commands will be read as strings and if it is "Read", the string in order to find the address desired by the user; the vector where vectors of vectors have been declared, where each line behaves like a block / line of memory and the elements between them will be the words contained in that block; the fstream, where the file "config.txt" will be read, which holds the entries for program initialization; and ctime, where through the clock the machine will get a number within a range of values, giving an idea of ​​randomness, thus being used for the random substitution policy.

In order to execute the program, the "config.txt" file must be changed in the way that the user chooses, in order to initialize the program features, there will soon be initialization of blocks / lines in words, number of blocks in memory and the number of rows in the cache memory, where all words contained in the cache are initialized to -1, thus characterizing that they are empty.
    
### Prerequisites

You will need to install the modules below to run the program: 
* [GCC 8.1 or greater](http://gcc.gnu.org/)

### Running

To run this software, we gonna compile in the terminal:
1. Enter the folder and compile all the .cpp files with the following command:
```
$ make
```
2. Enter the bin folder and run the following command:
```
$ ./lab2
```
3. From this it only interacts with the system.

The program will begin its execution, where for reading it will have to be typed "Read x", where x is the memory address that the data access was requested, if it is said to be HIT line y, being ya line of the cache where the address is memory. Already to show all the words contained in the cache and main memory will have to be typed "Show". Finally, while prompting for instructions, it will continue running the program until the user presses Ctrl + D.

## Built With

* [Atom](https://atom.io/) - A code editor used

## Authors
### Developers: 
* **Luís Eduardo Anunciado Silva ([cruxiu@ufrn.edu.br](mailto:cruxiu@ufrn.edu.br))** 
### Project Advisor: 
* **Gustavo Girao Barreto Da Silva ([girao@imd.ufrn.br](mailto:girao@imd.ufrn.br))** 

See also the list of [contributors](https://github.com/cruxiu/IMD0041-Cache/contributors) who participated in this project.

## License

This project is licensed under the GPL 3.0 - see the [LICENSE](LICENSE) file for details
