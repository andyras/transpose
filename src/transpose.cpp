#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

//#define DEBUG_TRANSPOSE

/* Counts the number of lines in a file. */
int countRows(const char * fileName) {
 int n = 0;	// number of lines
 std::string line;
 std::ifstream in(fileName);
 while (std::getline(in, line)) {
  n++;
 }
 if (n == 0 ) {
  std::cerr << "ERROR [" << __FUNCTION__ << "]: input file " << fileName << " is empty or missing.\n";
  exit(1);
 }

#ifdef DEBUG_TRANSPOSE
 std::cout << n << " rows in input.\n";
#endif

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

#ifdef DEBUG_TRANSPOSE
 std::cout << n << " columns in input.\n";
#endif

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
 int flag;
 FILE * inputFile;
 inputFile = fopen(fileName, "r");
 char junk [1024];	// junk string for checking for extra cols
 double djunk;		// double for catching junk

 // check for file exists
 if (inputFile != NULL) {
  // loop over rows
  for (int ii = 0; ii < rows; ii++) {
   // loop over columns
   for (int jj = 0; jj < cols; jj++) {
    flag = fscanf(inputFile, "%lf", &mat[ii*cols + jj]);
    if ((flag == 0) || (flag == EOF)) {
     std::cerr << "\nERROR: not enough columns of input at row "
               << (ii+1) << ", column " << (jj+1) << ".\n";
     exit(1);
    }
#ifdef DEBUG_TRANSPOSE
    if (jj == 0) {
     std::cout << mat[ii*cols + jj];
    }
    else {
     std::cout << " " << mat[ii*cols + jj];
    }
#endif
   }
   // check for extra columns
   fgets(junk, 1024, inputFile);
   if (sscanf(junk, "%lf", &djunk) == 1) {
    std::cerr << "\nERROR: found extra columns on row " << (ii+1) << ".\n";
    exit(1);
   }
#ifdef DEBUG_TRANSPOSE
   std::cout << "\n";
#endif
  }
 }
 else {
  std::cerr << "ERROR [" << __FUNCTION__ << "]: " << fileName << " does not exist.\n";
  exit(1);
 }
 
 fclose(inputFile);

 // write output

 // create string for output file name
 std::string outputFile = fileName;
 std::string outputExt("TR");	// extension for output file name

 // find position of last period and insert extension
 size_t pos = outputFile.rfind(".");
 if (pos == std::string::npos) {
  outputFile.append(outputExt.c_str());
 }
 else {
  outputFile.insert(pos, outputExt.c_str());
 }
#ifdef DEBUG_TRANSPOSE
 std::cout << "output file name is " << outputFile << ".\n";
#endif

 FILE * output;
 output = fopen(outputFile.c_str(), "w");

 // loop over rows (transpose of original columns)
 for (int ii = 0; ii < cols; ii++) {
  // loop over columns (transpose of original rows)
  for (int jj = 0; jj < rows; jj++) {
   if (jj == 0) {
#ifdef DEBUG_TRANSPOSE
    std::cout << mat[jj*cols + ii];
#endif
    fprintf(output, "%-.10g", mat[jj*cols + ii]);
   }
   else {
#ifdef DEBUG_TRANSPOSE
    std::cout << " " << mat[jj*cols + ii];
#endif
    fprintf(output, " %-.10g", mat[jj*cols + ii]);
   }
  }
#ifdef DEBUG_TRANSPOSE
  std::cout << "\n";
#endif
  fprintf(output, "\n");
 }

 fclose(output);
 
 return;
}

int main (int argc, char ** argv) {
 // get file name
 char * inputFile = argv[1];
#ifdef DEBUG_TRANSPOSE
 std::cout << "Input file name is " << inputFile << "\n";
#endif

 if (inputFile == NULL) {
  std::cerr << "Bogus!  You did not provide a file name.\n";
  return 1;
 }

 transpose(inputFile);

 return 0;
}
