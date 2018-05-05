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
  int flag=1;
  a = dabs(a);
  
  double temp = a;
  /*
  int itemp = a;
  double idtemp = itemp;
  
  if (idtemp==temp)
    while (a>1)
      a/=10;
  */
  
  while (a>1) //transform to form 0.xxx
  {
    a /= 10;
    flag++;
  };
  int a1 = pow10(a,n+1);
  int a2 = pow10(a,n);
  if ((a1%a2) >= 5)
  {
    if (temp >= 0)
      a2++;
    else 
      a2--;
  }
  for(int i=1; i<=n; i++)
    a /= 10;
  while(flag>1)
  {
    a *= 10;
    flag--;
  }
  if (temp >= 0)
    return a;
  else
    return -a;
}

int main(int argc, char *argv[])
{
    double a = 120.1234;
    int n = 2;
    cout << a << endl;
    cout << "sigfig: " << n << endl;
    cout << sigfig(a,n) << endl;
    cout << "Press the enter key to continue ...";
    cin.get();
    return EXIT_SUCCESS;
}
