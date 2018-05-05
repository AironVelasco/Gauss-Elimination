#include <cstdlib>
#include <iostream>
#include <fstream>
#include <math.h>

using namespace std;

double *data;
double *ogData;
double *var;
bool inpFile;
int prevComp, delRow;
int row, col, size, ogSize;
int choice=0;

void displayMat() //This program displays the matrix entered by user.
{
  if (choice == 0)
  {
    int elemno = 0;
    do
    {
    cout << data[elemno] << " \t";
    elemno++;
    if (elemno % col == 0) //new line when k reaches the number "col"
      cout << endl;
    }
    while (elemno < size);
    cout << endl;
  }
}

bool noSol(int colPos, int countCol) //Checks if Matrix has No Sol'n
{ //ADD COMMENTS HERE
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

bool multSol() //Checks if Matrix Has Multiple Sol'n
{ //This happens when the #of rows doesnt match the #of columns-1
  if (row  != col-1)
  {
    return true;
  }
  else
  {
    return false;
  }
}

void output() //Outputs the solution to the matrix
{
  for(int i=0; i<row; i++)
    cout << "x" << i << " = " << var[i] << endl;
  return;
}

void solve() //Uses back substitution to get the answers to the matrix
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
    {
      t[j] = var[row-j]*data[(size-1)-((i-1)*col)-j];
    }
    for(int j=0; j<row; j++)
    {
      var[row-i] -= t[j];
    }
    var[row-i] /= d;
  }
}

void eliminate(int c) //Performs The Gauss method of elimination
{
  /* This function eliminates the elements under the pivot*/
  if (choice == 0)
  {
    cout << "Eliminating...\n";
  }
  double f; //pivot factor
  int pivot = c*col+c; //pivot's array address
  for (int i=1; (i+c)<row; i++)
  {
    f=-data[(c+i)*col+c]/data[pivot];
    data[(i+c)*col+c]=0;
    //sets terms in the same column as the pivot to zero
    for (int j=1+c; j<col ;j++)
    {
      data[(c+i)*col+j]+=f*data[j+c*col];
      //adjusts each term accordingly by subtracting to the data
    }
  }
  displayMat(); //Shows the user the adjusted matrix
  return;
}

void setPivot(int c) //This function rearranges the pivot
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
    {
      cout << "No Pivot Needed\n\n";
    }

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

void removeRow(int rowNum) // if a whole row contains 0,it is removed
{
  //data from the next columns are moved
  copy(data + rowNum*col+col, data + size, data + rowNum*col);
  row--;  //the size changes as a result
  size = size - col;
  displayMat();
}

bool checkZeroRows(int colPos, int countCol)
{ //checks if the elements of a given row are all zeroes
  for(int i=colPos; i<size+1; i=i+col)
  {
    if (data[i] == 0 && countCol < col)
    {
      countCol++;
      if (checkZeroRows(i+1, countCol) == true)
      {
        if (i % col == 0)
        {
          delRow = i / col;
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

void inputFile(int argc, char *argv[]) //Loads file
{
  ifstream mattest; //matrix test filename
  string filename;
  inpFile = false;
  if (argc < 2)
  {
    cout <<"Please enter the filename: ";
    cin >> filename;
    mattest.open(filename.c_str());
  }
  else
  {
    inpFile = true;
    mattest.open(argv[1]);
  }

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
  ogSize = size;
  ogData = new double[ogSize+1];
  var = new double[row]; //solution array
  for(int i=0; i<row; i++)
  {//initialized to zero, explained why in solve()
    var[i]=0;
  }
  for (int i=0;i<size;i++)
  {
   mattest >> data[i];
   ogData[i] = data[i]; //makes a duplicate for save file
  }
  delRow = -1;
  while (checkZeroRows(0,0) == true)
  {
    cout << "Removing a Row " << delRow+1;
    cout << " due to zero elements in Row" << endl;
    removeRow(delRow);
    delRow = -1;
  }

  cout << "Your Matrix is as follows: " <<endl;
  displayMat();

  mattest.close();
  return;
}

double dabs(double a) //absolute value function for a double
{
  if (a < 0)
    return -a;
  else
    return a;
}

int pow10(double a, int n) //multiplies num by 10^n
{
  for(int i=1; i<=n; i++)
    a *= 10;
  return a;
}

double sigfig(double a, int n) //rounds number to specified amt of sf
{
  int flaga = 1;
  int flagb = 1;
  int b1,b2;
  double temp = a;
  a = dabs(a);

  //set to form x.xxx
  while(a>1) //counts # of digits before the decimal point
  {
    a/=10;
    flaga++;
  };
  while(a<1) //counts the # of digits after the decimal point
  {
    a*=10;
    flagb++;
  };
  b1 = pow10(a,n);
  b2 = pow10(a,n-1);

  if((b1%b2)>=5)
  {
    if (temp>=0)
      b2++;
    else
      b2--;
  }

  a=b2;

  for(int i=1; i<n; i++)
    a /= 10;

  while(flaga>1)
  {
    a*=10;
    flaga--;
  };
  while(flagb>1)
  {
    a/=10;
    flagb--;
  };

  if (temp>=0)
    return a;
  else
    return -a;
}

void save() //saves file
{
  int sf;
  string filename;
  cout <<"Please enter the filename: ";
  cin >> filename;
  cout <<"How many significant figures should it include?: ";
  cin >> sf;
  filename.append(".txt");
  ofstream saveFile (filename.c_str());
  saveFile <<"For the given matrix: " <<endl;
  int k = 0;
  do
  { //original matrix entered by user
    saveFile << ogData[k] << " \t";
    k++;
    if (k % col == 0)
    {
      saveFile << endl;
    }

  }
  while (k<ogSize);
  saveFile << endl;
  saveFile << "The Solution of Such Matrix is as Follows:" <<endl;
  for(int i=0; i<row; i++)
  { //solution to each variable in the matrix
    saveFile << "x" << i << " = " << sigfig(var[i],sf) << endl;
  }
}

int main(int argc, char *argv[])
{
  int choice2;
  //Opens input file and stores the matrix
  inputFile(argc, argv);
  if (inpFile == false)
  {
    do
    {
      cout <<"Do you want to show the process? (0 = Yes, 1 = No) : ";
      cin >>choice;
    }
    while (choice >1 || choice <0);
  }
  else //as per the specs, if file specified is values don't show soln
    choice = 1;

  for (int i=0; i<row-1; i++)
  {
    setPivot(i);
    eliminate(i);
    while (checkZeroRows(0,0) == true)
    {
      cout << "Removing Row ";
      cout << delRow+1 << " due to zero elements in Row" << endl;
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
  while (choice2 >1 || choice2 <0);
  if (choice2==0)
  {
    save();
  }

  //frees up space
  delete [] data;
  delete [] var;
  cout << "Save Successful." <<endl;
  system("PAUSE");
  return EXIT_SUCCESS;
}
