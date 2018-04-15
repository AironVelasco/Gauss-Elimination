#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

class matrix
{
  public:
    int row, col, size, bcol;
    double *data;
    matrix(int, int);
};

matrix::matrix(int a, int b)
{
  row = a;
  col = b;
  bcol = b+1;
  size = row*bcol;
  data = new double[size];
}

void displayMat(matrix a)
{
  int k = 0;
  do
  {
	cout << a.data[k] << " ";
	k++;
	if (k % a.bcol == 0)
	  cout << endl;
  }  
  while (k<a.size);      
}

void eliminate(matrix a, int c)
{
  cout << "Eliminating...\n";
  double f; //pivot factor
  int pivot = c*a.bcol+c;
  for (int i=1; i<=2; i++)
  {
    f=-a.data[i*a.bcol]/a.data[pivot];
    a.data[i*a.bcol]=0;
    cout << "oc: " << a.data[i*a.bcol] << endl;
    cout << "Factor: " << f << endl;
    for (int j=1; j<a.bcol ;j++)
    {
      cout << "Changing " << a.data[i*a.bcol+j] << endl;
      cout << "Above: " << a.data[j] << endl << endl;
      a.data[i*a.bcol+j]+=f*a.data[j];
    }
    displayMat(a);
    cout << endl;
  }
  displayMat(a);
  return;
}

void setPivot(matrix a, int c)
{
  //c is counter 0 column
  double dtmp;
  int dpivot = c*a.bcol+c;
  int itmp = dpivot;
  double pivot = a.data[dpivot];
  cout << "Pivoting: " << pivot << endl;
  for(int i=dpivot, j=c; j<a.row; i+=a.bcol, j++)
  {
    cout << "Iteration " << j << endl;
    cout << a.data[i] << endl;
    if (pivot < a.data[i])
    {
      pivot = a.data[i];
      itmp = i;
    }
  }
  int k;
  if (dpivot == itmp)
  {
    cout << "No Pivot Needed\n";
    return;
  }
  else
  {
    for (int j=0, k = dpivot; j<a.bcol; j++, k++)
    {
      dtmp = a.data[k]; //storing next row
      a.data[k] = a.data[itmp+j]; //equate next row to pivot row
      a.data[itmp+j] = dtmp; //equate pivot row to next row
      cout << endl;
    }
  }
  displayMat(a);
  return;
}


matrix inputFile(int argc, char *argv[])
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
  displayMat(matA);
  cout << endl;
  
  mattest.close();
  return matA;
}

int main(int argc, char *argv[])
{
  //Opens input file and stores the matrix
  matrix matA = inputFile(argc, argv);
  for (int i=0; i<matA.row; i++)
  {
    cout << "ITERATION " << i << endl;
    setPivot(matA,i);
    eliminate(matA,i);
  }
  system("PAUSE");
  return EXIT_SUCCESS;
}
