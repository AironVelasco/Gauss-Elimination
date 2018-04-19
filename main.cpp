#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

double *data;
double *var;
int row, col, size;

void displayMat()
{
  int k = 0;
  do
  {
	cout << data[k] << " ";
	k++;
	if (k % col == 0) //new line when k is col
	  cout << endl;
  }
  while (k<size);
  cout << endl;
}

void output()
{
  for(int i=0; i<row; i++)
    cout << "x" << i << " = " << var[i] << endl;
  return;
}

void solve()
{
  cout << "Solving...\n";
  double d; //denominator
  double t[row]; //numerator terms
  for(int i=1; i<=row; i++)
  {
    t[0] = -data[(size-1)-((i-1)*col)];
    d = data[(size-1)-((i-1)*col)-i];
    /*var was initialized to zero so that
    it's null when it's not needed in the terms*/
    /*var will be given a value for the
    next iterations where it's needed*/
    for(int j=1; j<row; j++)
      t[j] = var[row-j]*data[(size-1)-((i-1)*col)-j];
    for(int j=0; j<row; j++)
      var[row-i] -= t[j];
    var[row-i] /= d;
  }
}

void eliminate(int c)
{
  cout << "Eliminating...\n";
  double f; //pivot factor
  int pivot = c*col+c; //pivot's array address
  for (int i=1; (i+c)<row; i++)
  {
    f=-data[(c+i)*col+c]/data[pivot];
    data[(i+c)*col+c]=0; //sets column pivot terms to zero
    for (int j=1+c; j<col ;j++)
      data[(c+i)*col+j]+=f*data[j+c*col]; //adjusts each term accordingly
  }
  displayMat();
  return;
}

void setPivot(int c)
{
  cout << "Pivoting...\n";
  double dtmp; //data temp, stores data or terms
  int dpivot = c*col+c; //stores pivot's array address
  int spivot; //safety pivot for when original pivot is zero
  int itmp = dpivot; //stores it again for comparison later
  double pivot = data[dpivot]; //pivot
  //searches for highest number by comparing to pivot
  for(int i=dpivot, j=c; j<row; i+=col, j++)
  {
    if (data[i] != 0) //stores nonzero pivot column
    {
      spivot = data[i];
      dtmp = i;
    }
    if (pivot < data[i])
    {
      pivot = data[i]; //sets new pivot
      itmp = i; //stores new pivot's array address
    }
  }
  cout << "Test\n";
  if (pivot == 0) //sets pivot to nonzero if it's zero originally
  {
    pivot = spivot;
    itmp = dtmp;
  }
  cout << "Test\n";
  //compares old and new addresses
  if (dpivot == itmp)
  {
    cout << "No Pivot Needed\n\n";
    return;
  }
  else
    for (int j=0, k = dpivot; k<col; j++, k++)
    {
      dtmp = data[k]; //storing old pivot row
      data[k] = data[itmp+j]; //equate old pivot row to new pivot row
      data[itmp+j] = dtmp; //equate new pivot row to old pivot row
    }
  cout << "Test\n";
  displayMat();
  return;
}


void inputFile(int argc, char *argv[])
{
  ifstream mattest; //matrix test
  string filename;

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

  mattest >> row;
  mattest >> col;
  size = row*col;
  data = new double[size];
  var = new double[row]; //solution array
  for(int i=0; i<row; i++) //initialized to zero, explained why in solve()
    var[i]=0;
  for (int i=0;i<size;i++)
	mattest >> data[i];
  displayMat();

  mattest.close();
  return;
}

int main(int argc, char *argv[])
{
  //Opens input file and stores the matrix
  inputFile(argc, argv);
  for (int i=0; i<row-1; i++)
  {
    setPivot(i);
    eliminate(i);
  }
  solve();
  output();

  delete [] data;
  delete [] var;
  system("PAUSE");
  return EXIT_SUCCESS;
}
