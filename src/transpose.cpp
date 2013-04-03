#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>

//#define DEBUG_TRANSPOSE

struct Parameters {
 bool dryRun;
 bool verbose;
 std::string outputExt;
 bool printHelp;
};

/* Counts the number of lines in a file. */
int countRows(const char * fileName, Parameters p) {
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

 if (p.verbose) {
  std::cout << n << " rows in input.\n";
 }

 return n;
}

/* Counts the number of columns in a file (from first line) */
int countCols(const char * fileName, Parameters p) {
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

 if (p.verbose) {
  std::cout << n << " columns in input.\n";
 }

 return n;
}

void transpose(const char * fileName, Parameters p) {
 int rows = countRows(fileName, p);
 int cols = countCols(fileName, p);
 double * mat = new double [rows*cols];	// matrix of values

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

 // find position of last period and insert extension
 size_t pos = outputFile.rfind(".");
 if (pos == std::string::npos) {
  outputFile.append(p.outputExt.c_str());
 }
 else {
  outputFile.insert(pos, p.outputExt.c_str());
 }
#ifdef DEBUG_TRANSPOSE
 std::cout << "output file name is " << outputFile << ".\n";
#endif

 FILE * output;
 if (!p.dryRun) {
  output = fopen(outputFile.c_str(), "w");
 }

 // loop over rows (transpose of original columns)
 for (int ii = 0; ii < cols; ii++) {
  // loop over columns (transpose of original rows)
  for (int jj = 0; jj < rows; jj++) {
   if (jj == 0) {
#ifdef DEBUG_TRANSPOSE
    std::cout << mat[jj*cols + ii];
#endif
    if (!p.dryRun) {
     fprintf(output, "%-.10g", mat[jj*cols + ii]);
    }
   }
   else {
#ifdef DEBUG_TRANSPOSE
    std::cout << " " << mat[jj*cols + ii];
#endif
    if (!p.dryRun) {
     fprintf(output, " %-.10g", mat[jj*cols + ii]);
    }
   }
  }
#ifdef DEBUG_TRANSPOSE
  std::cout << "\n";
#endif
  if (!p.dryRun) {
   fprintf(output, "\n");
  }
 }

 if (!p.dryRun) {
  fclose(output);
 }
 
 return;
}

int main (int argc, char ** argv) {
 char c;	// parameter for parsing arguments

 Parameters p;
 p.dryRun = false;
 p.verbose = false;
 p.outputExt = "TR";
 p.printHelp = false;

 /* process command line options */
 while ((c = getopt(argc, argv, "dho:v")) != -1) {
  switch (c) {
   case 'd':
    if (p.verbose) {
     std::cout << "Perform dry run (no output).\n";
    }
    p.dryRun = true;
    break;
   case 'h':
    if (p.verbose) {
     std::cout << "Print help.\n";
    }
    p.printHelp = true;
    break;
   case 'v':
    std::cout << "Print verbose information.\n";
    p.verbose = true;
    break;
   case 'o':
    p.outputExt = optarg;
    if (p.verbose) {
     std::cout << "Specifying output filename pre-extension: " << p.outputExt << ".\n";
    }
    break;
   case '?':
    if (optopt == 'o') {
     fprintf(stderr, "Option -%c requires a filename extension argument.\n", optopt);
    }
    else if (isprint(optopt)) {
     fprintf(stderr, "Unknown option -%c.\n", optopt);
    }
    else {
     fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
    }
    return 1;
   default:
    continue;
  }
 }

 if (p.printHelp) {
  std::cout << "\n"
            << "transpose: a utility for transposing 2D matrices.\n"
            << "Usage: transpose [-d] [-h] [-o] ext [-v] inputfile\n"
	    << "\n"
	    << "-d: Dry run (create no output).\n"
	    << "-h: Print this help message.\n"
	    << "-o: Specify a filename extension. The default is to append 'TR' to the\n"
	    << "    filename, e.g. input.dat --> inputTR.dat.\n"
	    << "-v: Print verbose information about what the program is doing.\n"
	    << "\n"
	    << "transpose will try to be smart about not accepting inputs where the matrix\n"
	    << "is not rectangular, i.e. containing any line where the number of records is\n"
	    << "not the same as in the first line.\n"
	    << "\n";
  return 0;
 }

 // get file name
 if (argc == optind) {
  std::cerr << "BOGUS!  You did not provide a file name.\n";
  return 1;
 }
 char * inputFile = argv[optind];
#ifdef DEBUG_TRANSPOSE
 std::cout << "Input file name is " << inputFile << "\n";
#endif

 transpose(inputFile, p);

 return 0;
}
