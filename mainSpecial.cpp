#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

double *data;
double *var;
int prevComp, delRow;
int row, col, size;
int choice=0;

void displayMat()
{
  if (choice == 0)
  {
    int k = 0;
    do
    {
    cout << data[k] << " \t";
    k++;
    if (k % col == 0) //new line when k is col
      cout << endl;
    }
    while (k<size);
    cout << endl;
  }
}

bool noSol(int colPos, int countCol)
{
  for(int i=colPos; i<size+1; i=i+col)
  {
    if (data[i] == 0 && countCol < col)
    {
      countCol++;
      if (noSol(i+1, countCol) == true)
      {
        return true;
      }
      else
      {
        countCol = 0;
        if (i % col != 0)
        {
          return false;
        }
      }
    }
    else if (data[i-1] == 0 && countCol == col)
    {
      countCol = 0;
      return true;
    }
    else if (data[i] != 0 && i % col != 0)
    {
      if (countCol+1 == col)
      {
        return true;
      }
      else
      {
        return false;
      }
    }
  }
  return false;
}

bool multSol()
{
  if (row != col)
    return true;
  return false;
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
  if (choice == 0)
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
  if (choice == 0)
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
  if (pivot == 0) //sets pivot to nonzero if it's zero originally
  {
    pivot = spivot;
    itmp = dtmp;
  }
  //compares old and new addresses
  if (dpivot == itmp)
  {
    if (choice == 0)
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
  displayMat();
  return;
}

void removeRow(int rowNum)
{
  copy(data + rowNum*col+col, data + size, data + rowNum*col);
  row--;
  size = size - col;
  displayMat();
}

bool checkZeroRows(int colPos, int countCol)
{
  for(int i=colPos; i<size+1; i=i+col)
  {
    //cout << "i: " << i << endl;
    if (data[i] == 0 && countCol < col)
    {
      countCol++;
      //cout << " data[i]: " << data[i] << " colCount: " << countCol << endl;
      if (checkZeroRows(i+1, countCol) == true)
      {
        if (i % col == 0)
        {
          delRow = i / col;
          //cout << "cP: " << colPos << " i: " << i << endl;
          //cout << "Delete Row: " << delRow +1 << endl;
        }
        return true;
      }
      else
      {
        countCol = 0;
        if (i % col != 0)
          return false;
      }
    }
    else if (data[i-1] == 0 && countCol == col)
    {
      countCol = 0;
      return true;
    }
    else if (data[i] != 0 && i % col != 0)
    {
      return false;
    }
  }
  return false;
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
  data = new double[size+1];
  var = new double[row]; //solution array
  for(int i=0; i<row; i++) //initialized to zero, explained why in solve()
    var[i]=0;
  for (int i=0;i<size;i++)
	mattest >> data[i];

	//Special Cases
  //Multiples
  // No Solution (Row < Col)
    //Check Zero Columns
  delRow = -1;
  while (checkZeroRows(0,0) == true)
  {
    cout << "Removing a Row " << delRow+1 << " due to zero elements in Row" << endl;
    removeRow(delRow);
    delRow = -1;
  }

  cout << "Your Matrix is as follows: " <<endl;
  displayMat();

  mattest.close();
  return;
}

void save()
{
  string filename;
  cout <<"Please enter the filename: ";
  cin >> filename;
  filename.append(".txt");
  ofstream saveFile (filename.c_str());
  saveFile <<"For the given matrix: " <<endl;
  int k = 0;
  do
  {
    saveFile << data[k] << " \t";
    k++;
    if (k % col == 0) //new line when k is col
    {
      saveFile << endl;
    }

  }
  while (k<size);
  saveFile << endl;
  saveFile << "The Solution of Such Matrix is as Follows:" <<endl;
  for(int i=0; i<row; i++)
  {
    saveFile << "x" << i << " = " << var[i] << endl;
  }
}

int main(int argc, char *argv[])
{
  int choice2;
  //Opens input file and stores the matrix
  inputFile(argc, argv);
  do
  {
    cout <<"Do you want to show the process? (0 = Yes, 1 = No) : ";
    cin >>choice;
  }
  while (choice >1 || choice <0);
  for (int i=0; i<row-1; i++)
  {
    setPivot(i);
    eliminate(i);
    while (checkZeroRows(0,0) == true)
    {
      cout << "Removing Row " << delRow+1 << " due to zero elements in Row" << endl;
      removeRow(delRow);
      delRow = -1;
    }
    if (noSol(0,0) == true)
    {
      cout << "No Solution." << endl;
      return EXIT_SUCCESS;
    }
  }
  if (multSol() == true)
  {
    cout << "Multiple Solution." << endl;
    return EXIT_SUCCESS;
  }
  solve();
  output();
  do
  {
    cout <<"Do you want to save results? (0 = Yes, 1 = No) : ";
    cin >>choice2;
  }
  while (choice >1 || choice <0);
  if (choice2==0)
  {
    save();
  }

  delete [] data;
  delete [] var;
  cout << "Save Successful.";
  system("PAUSE");
  return EXIT_SUCCESS;
}
