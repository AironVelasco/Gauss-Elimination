#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

class matrix
{
  public:
    int row, col, size;
    double *data;
    matrix(int, int);
};

matrix::matrix(int a, int b)
{
  row = a;
  col = b;
  size = row*col;
  data = new double[size];
}

void setPivot(matrix a, int c)
{
  //c is counter 0 column
  int i=0;
  int itmp;
  int ctmp=c;
  double dtmp;
  double pivot = a.data[0+c];
  cout << pivot << endl;
  while (i<a.size)
  {
    i+=a.col+c;
    c--;
    cout << a.data[i] << endl;
    if (pivot < a.data[i])
    {
      pivot = a.data[i];
      itmp = i;
    }
  };
  for (int j = ctmp; j<a.col; j++)
  {
    dtmp = a.data[j];
    a.data[j] = a.data[itmp+j];
    a.data[itmp+j] = dtmp;
  }
  cout << "Pivot: " << pivot << endl;
  int k=0;
  do
  {
	cout << a.data[k];
	k++;
	if (k % a.col == 0)
	  cout << endl;
  }  
  while (k<a.size);  
  return;
}


void inputFile(int argc, char *argv[])
{
  ifstream mattest;
  string filename;
  int irow, icol;
  
  if (argc < 2)
  {
    cout <<"Please enter the filename: ";
    cin >> filename;
    mattest.open(filename.c_str());
  }
  else
  {
    mattest.open(argv[1]);
  }

  while (!mattest)
  {
    cout << "Please enter the filename: ";
    cin >> filename;
    mattest.open(filename.c_str());
  }

  mattest >> irow;
  mattest >> icol;
  
  matrix matA(irow,icol);

  cout << "size: " << matA.size << endl;
  
  
  
  for (int i=0;i<matA.size;i++)
  {
	mattest >> matA.data[i];
  }
  int i = 0;
  do
  {
	cout << matA.data[i];
	i++;
	if (i % matA.col == 0)
	  cout << endl;
  }  
  while (i<matA.size);
  /*
  for (int i=0; i<matA.row; i++)
  {
    for (int j=0; j<matA.col; j++)
    {
      mattest >> matA.data[i][j];
    }
    cout << endl;
  }
  
  for (int i=0; i<matA.row; i++)
  {
    for (int j=0; j<matA.col; j++)
    {
      cout << matA.data[i][j];
    }
    cout << endl;
  }*/
  cout << endl;
  
  setPivot(matA, 0);
  
  mattest.close();
}

int main(int argc, char *argv[])
{
  //Opens input file
  inputFile(argc, argv);
  system("PAUSE");
  return EXIT_SUCCESS;
}
