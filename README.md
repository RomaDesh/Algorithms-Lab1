You need to implement the quicksort algorithm in C/C++. The data to be sorted is represented as pairs (k, v), where k is a non-negative integer and v is a string of arbitrary length. The data must be ordered by increasing values of k.

Requirements:

1. The program must be implemented as a single source file lab1.cpp.
2. The program must be compiled by the MS Visual C++ 2017 compiler in strict compliance with the ISO C++ 14 standard on the Windows x64 platform without using any third-party libraries or tools.
3. The use of STL, Boost and other algorithmic libraries is not allowed.
4. The program should be a console application, the interaction with which is carried out exclusively by passing command line arguments in accordance with the following pattern:

> lab1 InputFile OutputFile 

Here InputFile is the name of the file containing the source data, OutputFile is the name of the file containing the result of the work.
5. The source data file should be a text file, on the first line of which there is a non-negative integer N equal to the number of pairs to be sorted, and on the following N lines - the pairs to be sorted themselves, with k and v values separated by one space. An example of a source data file:

4
3 bla
2 bla bla
1 blabla
0 blablabla

6. The file containing the result of the work has the same format as the source data file. The file should contain pairs (k, v) sorted in ascending order. An example of a result file:

4
0 blablabla
1 blabla 
2 bla bla 
3 bla

7. In case of successful completion of work, the program should return an error code of 0.
8. In case of non-compliance with command line arguments to the above format, the program should terminate with an error code of 1.
9. In case of inability to read input or write output files, the program should terminate with an error code of 2.
10. In case of non-compliance with the source data file format described above, the program should terminate with an error code of 3.