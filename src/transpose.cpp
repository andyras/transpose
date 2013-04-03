#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

/* Counts the number of lines in a file. */
int countRows(const char * fileName) {
 int n = 0;	// number of lines
 std::string line;
 std::ifstream in(fileName);
 while (std::getline(in, line)) {
  n++;
 }
 if (n == 0 ) {
  std::cerr << "WARNING [" << __FUNCTION__ << "]: input file " << fileName << " is empty.\n";
 }

 std::cout << n << " rows in input.\n";

 return n;
}

/* Counts the number of columns in a file (from first line) */
int countCols(const char * fileName) {
 int n = 0;		// number of columns
 std::string line;	// string for first line
 std::string junk;	// string for first line

 // open input stream
 std::ifstream inputFile(fileName);

 // get first line
 std::getline(inputFile, line);
 std::stringstream is(line);
 while (is >> junk) {
  n++;
 }

 if (n == 0) {
  std::cerr << "ERROR: no columns in input\n";
  exit(1);
 }

 std::cout << n << " columns in input.\n";

 return n;
}

void transpose(const char * fileName) {
 int rows = countRows(fileName);
 int cols = countCols(fileName);
 double * mat = new double [rows*cols];	// matrix of values
 /*
 std::string line;
 std::stringstream is;
 std::string val;	// numerical value read in

 // open input stream
 std::ifstream inputFile(fileName);

 // read each row
 for (int ii = 0; ii < rows; ii++) {
  std::getline(inputFile, line);
  // put line into stringstream
  is << line;
  // extract each column
  for (int jj = 0; jj < rows; jj++) {
   is >> val;
   std::cout << val;
   //std::cout << is;
  }
  std::cout << "\n";
 }
 */
 // read input
 FILE * inputFile;
 inputFile = fopen(fileName, "r");

 // check for file exists
 if (inputFile != NULL) {
  // loop over rows
  for (int ii = 0; ii < rows; ii++) {
   // loop over columns
   for (int jj = 0; jj < rows; jj++) {
    fscanf(inputFile, "%lf", &mat[ii*cols + jj]);
    std::cout << mat[ii*cols + jj];
   }
   std::cout << "\n";
  }
 }
 else {
  std::cerr << "ERROR [" << __FUNCTION__ << "]: " << fileName << " does not exist.\n";
  exit(1);
 }
 
 fclose(inputFile);

 return;
}

int main (int argc, char ** argv) {
 // get file name
 char * inputFile = argv[1];
 std::cout << "File name is " << inputFile << "\n";

 if (inputFile == NULL) {
  std::cerr << "Bogus!  You did not provide a file name.\n";
  return 1;
 }

 transpose(inputFile);

 return 0;
}
