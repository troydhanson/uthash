// Windows does not have unix diff so this is a simple replacement
#include <iostream>
#include <fstream>
using namespace std;
int main(int argc, char *argv[] ) {
  int rc=-1;
  if (argc != 3) {
      cout << "usage: " << argv[0] << " file1 file2\n";
      return -1;
  }
  char *file1 = argv[1];
  char *file2 = argv[2];
  ifstream is1(file1, ios::in);
  ifstream is2(file2, ios::in);
  if (is1.fail()) {cerr << "failed to open " << file1 << "\n"; goto done;}
  if (is2.fail()) {cerr << "failed to open " << file2 << "\n"; goto done;}
  char d1[256], d2[256];
  do {
    is1.read(d1,sizeof(d1));
    is2.read(d2,sizeof(d2));
    if ((is1.gcount() != is2.gcount()) || memcmp(d1,d2,is1.gcount())) {
        cout << file1 << " and " << file2 << " differ\n";
        goto done;
    }
  } while (!is1.eof() && !is2.eof());

  rc=0;

 done:
  is1.close();
  is2.close();
  return rc;
}

