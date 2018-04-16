#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

class matrix
{
  public:
    int row, col, size, bcol;
    double *data;
    double *var;
    matrix(int, int);
};

matrix::matrix(int a, int b)
{
  row = a;
  col = b;
  bcol = b+1; //includes right-side
  size = row*bcol; //includes right-side
  data = new double[size];
  var = new double[row]; //solution array
  for(int i=0; i<row; i++) //initialized to zero, explained why in solve()
    var[i]=0;
}

void displayMat(matrix a)
{
  int k = 0;
  do
  {
	cout << a.data[k] << " ";
	k++;
	if (k % a.bcol == 0) //new line when k is bcol
	  cout << endl;
  }  
  while (k<a.size);
  cout << endl;
}

void output(matrix a)
{
  for(int i=0; i<a.row; i++)
    cout << "x" << i << " = " << a.var[i] << endl;
  return;
}

void solve(matrix a)
{
  cout << "Solving...\n";
  double d; //denominator
  double t[a.row]; //numerator terms
  for(int i=1; i<=a.row; i++)
  {
    t[0] = -a.data[(a.size-1)-((i-1)*a.bcol)];
    d = a.data[(a.size-1)-((i-1)*a.bcol)-i];
    /*var was initialized to zero so that
    it's null when it's not needed in the terms*/
    /*var will be given a value for the
    next iterations where it's needed*/
    for(int j=1; j<a.row; j++)
      t[j] = a.var[a.row-j]*a.data[(a.size-1)-((i-1)*a.bcol)-j];
    for(int j=0; j<a.row; j++)
      a.var[a.row-i] -= t[j];
    a.var[a.row-i] /= d;
  }
}

void eliminate(matrix a, int c)
{
  cout << "Eliminating...\n";
  double f; //pivot factor
  int pivot = c*a.bcol+c; //pivot's array address
  for (int i=1; (i+c)<a.row; i++)
  {
    f=-a.data[(c+i)*a.bcol+c]/a.data[pivot];
    a.data[(i+c)*a.bcol+c]=0; //sets column pivot terms to zero
    for (int j=1+c; j<a.bcol ;j++)
      a.data[(c+i)*a.bcol+j]+=f*a.data[j+c*a.bcol]; //adjusts each term accordingly
  }
  displayMat(a);
  return;
}

void setPivot(matrix a, int c)
{
  cout << "Pivoting...\n";
  double dtmp; //data temp, stores data or terms
  int dpivot = c*a.bcol+c; //stores pivot's array address
  int spivot = 0; //safety pivot for when original pivot is zero
  int itmp = dpivot; //stores it again for comparison later
  double pivot = a.data[dpivot]; //pivot
  //searches for highest number by comparing to pivot
  for(int i=dpivot, j=c; j<a.row; i+=a.bcol, j++)
  {
    if (a.data[i] != 0) //stores nonzero pivot column
    {
      spivot = a.data[i];
      dtmp = i;
    }
    if (pivot < a.data[i])
    {
      pivot = a.data[i]; //sets new pivot
      itmp = i; //stores new pivot's array address
    }
  }
  if (pivot == 0) //sets pivot to nonzero if it's zero originally
  {
    pivot = spivot;
    itmp = dtmp;
  }
  //compares old and new addresses
  if (dpivot == itmp)
  {
    cout << "No Pivot Needed\n\n";
    return;
  }
  else
  {
    for (int j=0, k = dpivot; j<a.bcol; j++, k++)
    {
      dtmp = a.data[k]; //storing old pivot row
      a.data[k] = a.data[itmp+j]; //equate old pivot row to new pivot row
      a.data[itmp+j] = dtmp; //equate new pivot row to old pivot row
    }
  }
  displayMat(a);
  return;
}


matrix inputFile(int argc, char *argv[])
{
  ifstream mattest; //matrix test
  string filename;
  int irow, icol;
  
  if (argc < 2)
  {
    cout <<"Please enter the filename: ";
    cin >> filename;
    mattest.open(filename.c_str());
  }
  else
    mattest.open(argv[1]);

  while (!mattest)
  {
    cout << "Please enter the filename: ";
    cin >> filename;
    mattest.open(filename.c_str());
  }

  mattest >> irow;
  mattest >> icol;
  
  matrix matA(irow,icol);
  
  for (int i=0;i<matA.size;i++)
	mattest >> matA.data[i];
  displayMat(matA);
  
  mattest.close();
  return matA;
}

int main(int argc, char *argv[])
{
  //Opens input file and stores the matrix
  matrix matA = inputFile(argc, argv);
  for (int i=0; i<matA.row-1; i++)
  {
    setPivot(matA,i);
    eliminate(matA,i);
  }
  solve(matA);
  output(matA);
  system("PAUSE");
  return EXIT_SUCCESS;
}
