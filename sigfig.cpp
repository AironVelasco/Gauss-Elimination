#include <cstdlib>
#include <iostream>

using namespace std;

double dabs(double a)
{
  if (a < 0)
    return -a;
  else
    return a;
}

int pow10(double a, int n)
{
  for(int i=1; i<=n; i++)
    a *= 10;
  return a;
}

double sigfig(double a, int n)
{
  int flaga = 1;
  int flagb = 1;
  int b1,b2;
  double temp = a;
  a = dabs(a);
  
  //set to form x.xxx
  while(a>1)
  {
    a/=10;
    flaga++;
  };
  while(a<1)
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

int main(int argc, char *argv[])
{
    double a = 515.55;
    int n = 4;
    cout << a << endl;
    cout << "sigfig: " << n << endl;
    cout << sigfig(a,n) << endl;
    cout << "Press the enter key to continue ...";
    cin.get();
    return EXIT_SUCCESS;
}
