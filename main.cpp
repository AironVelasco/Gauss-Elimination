//CE 30 Project 3: Matrix Solver Using Gaussian Elimination Method
//Julius Balgos, Sylvester Cunanan, Airon Velasco
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
int solflag=0;

void displayMat() //This program displays the matrix entered by user.
{
  if (choice == 0)
  {
    int elemno = 0;
    do
    {
      cout << data[elemno] << " \t";
      elemno++;
      if (elemno % col == 0) //new line after "col" # of elements
      {
        cout << endl;
      }
    }
    while (elemno < size);
    cout << endl;
  }
}

bool noSol(int colPos, int countCol) //Checks if Matrix has No Sol'n
{ 
  //the function utilizes recursion to check the elements in each row
  /*the recursion is triggered when a zero element is found in the 
  first column*/
  //and also when the succeding element in the row is also a zero.
  /*The recursion ends when it has reached the final element in the 
  row.*/
  for(int i=colPos; i<size+1; i=i+col)
  {
    if (data[i] == 0 && countCol < col) // Checks if the element is 0.
    {
      countCol++;
      /*calls the function again with the position of the next 
      element in the row*/
      if (noSol(i+1, countCol) == true)
      {
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
      //if the last value in the row is a non-zero while the 
      //other elements are zero, then the matrix has no solution 
      //and the function would return true.
      if (countCol+1 == col)
        return true;
      else
        return false;
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
  {
    cout << "x" << i << " = " << var[i] << endl;
  }
  return;
}

void solve() //Uses back substitution to get the answers to the matrix
{
  if (choice == 0)
  {
    cout << "Solving...\n";
  }
  double d; //denominator
  double t[row]; //numerator terms
  for(int i=1; i<=row; i++) //goes thru each row starting with last
  {
    /*size-1 indicates we get the data
    with reference from the last term*/
    t[0] = -data[(size-1)-((i-1)*col)]; //right-side coefficient
    d = data[(size-1)-((i-1)*col)-i]; //main diagonal coefficient
    /*var was initialized to zero so that
    it's null when it's not needed in the terms*/
    /*var will be given a value for the
    next iterations where it's needed*/
    for(int j=1; j<row; j++) //goes thru each col of i row
    {
      /*multiplies the previous solved vars with
      their coefficients in i row*/
      t[j] = var[row-j]*data[(size-1)-((i-1)*col)-j];
    }
    for(int j=0; j<row; j++)
      var[row-i] -= t[j]; //subtracts each terms
    var[row-i] /= d; //divides by the var's coefficient
  }
}

void eliminate(int c) //Performs The Gauss method of elimination
{
  /*c here tells which diagonal pivot we are located as the reference
  /* This function eliminates the elements under the pivot*/
  if (choice == 0)
  {
    cout << "Eliminating...\n";
  }
  double f; //pivot factor
  int pivot = c*col+c; //pivot's array address
  /*goes thru each row where c limits the number of iterations
  as the pivot goes to the next row*/
  for (int i=1; (i+c)<row; i++)
  {
    /*divides the negative of the data under pivot's column 
    by the pivot; this is the factor for making the pivot column zero
    where Af + B = 0 => f = -B/A*/
    f = -data[(c+i)*col+c]/data[pivot];
    data[(i+c)*col+c] = 0; //sets pivot column to zero
    for (int j=1+c; j<col ;j++) //goes thru each col of i row
    {
      /*given the form Af + B = Anext, the row terms
      are adjusted accordingly with the pivot factor
      where A is the term above the term being adjusted
      which is B*/
      data[(c+i)*col+j]+=f*data[j+c*col];
    }
  }
  displayMat(); //Shows the user the adjusted matrix
  return;
}

void setPivot(int c) //This function rearranges the pivot
{
  if (choice == 0)
  {
    cout << "Pivoting...\n";
  }
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
    if (choice == 0) //if the same, no pivot needed
    {
      cout << "No Pivot Needed\n\n";
    }
    return;
  }
  else
  {
    /*starts with dpivot "the old pivot" location until col is reached.
    itmp is the new pivot location where j increments as the code
    goes thru the col of the pivot row*/
    for (int j=0, k = dpivot; k<col; j++, k++)
    {
      dtmp = data[k]; //storing old pivot row
      data[k] = data[itmp+j]; //equate old pivot row to new pivot row
      data[itmp+j] = dtmp; //equate new pivot row to old pivot row
    }
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
  //As with the noSol function, this function also utilizes recursion 
  //to check the elements in each row. The recursion is triggered 
  //when a zero element is found in the first column and also when 
  //the succeeding element in the row is also a zero. 
  //Recursion ends when it has reached the final element in the row.
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
  {
    return -a;
  }
  else
  {
    return a;
  }
}

int pow10(double a, int n) //multiplies num by 10^n
{
  for(int i=1; i<=n; i++)
  {
    a *= 10;
  }
  return a;
}

double sigfig(double a, int n) //rounds number to specified amt of sf
{
  int flaga = 1; //flag for a<1 doubles
  int flagb = 1; //flag for a>1 doubles
  int b1,b2; //used to compare rounding digit and its lower digit
  double temp = a;
  a = dabs(a);

  //set to form x.xxx
  while(a>1) //sets double like 12.3 to 1.23
  {
    a/=10;
    flaga++;
  };
  while(a<1) //sets double like 0.123 to 1.23
  {
    a*=10;
    flagb++;
  };
  b1 = pow10(a,n); //acquires the significant figure last digit
  b2 = pow10(a,n-1); //acquires the digit to the right of that

  /*does Half Round Up Method. For example
  1.4->1, 1.5->2, 1.6->2, -1.4->-1, -1.5->-1, -1.6->-2*/
  if((b1%b2)>=5 && temp>=0)  
  {
    b2++;
  }
  else if ((b1%b2)>5 && temp<0)
  {
    b2++;
  }
  
  a = b2; //stores int to double to apply decimals

  for(int i=1; i<n; i++) //reverts back by n (sigfig) times
  {
    a /= 10;
  }
  //reverts further more from x.xxx form
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

  if (temp>=0) //returns with the original's sign
  {
    return a;
  }
  else
  {
    return -a;
  }
}

void save() //saves file
{
  int sf;
  string filename;
  cout <<"Please enter the filename: ";
  cin >> filename;
  if (solflag ==0)
  {
    cout <<"How many significant figures should it include?: ";
    cin >> sf;
  }
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
  if (solflag==0)
  {
    saveFile << "The solution to the matrix is as follows:" <<endl;
    for(int i=0; i<row; i++)
    { //solution to each variable in the matrix
      saveFile << "x" << i << " = " << sigfig(var[i],sf) << endl;
    }
    saveFile << "Solutions are saved in ";
    saveFile << sf <<" significant figures."<< endl;
  }
  else if (solflag==1)
  {
    saveFile << "There is no solution to this matrix." <<endl;
  }
  else if (solflag==2)
  {
    saveFile << "There are multiple solution to this matrix." <<endl;
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
  {
    choice = 1;
  }

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
      cout << "No Solution can be found." << endl;
      solflag=1;
    }
  }
  if (multSol() == true)
  {
    cout << "Multiple Solutions found." << endl;
    solflag=2;
  }
  if (solflag == 0)
  {
    solve();
    output();
  }
  do
  {
    cout <<"Do you want to save results? (0 = Yes, 1 = No) : ";
    cin >>choice2;
  }
  while (choice2 >1 || choice2 <0);
  if (choice2==0)
  {
    save();
    cout << "Save Successful." <<endl;
  }

  //frees up space
  delete [] data;
  delete [] var;
  system("PAUSE");
  return EXIT_SUCCESS;
}
