#include <iostream>
#include <loki/Typelist.h>
#include <mix/TwinArray.h>

using namespace std;
using namespace Loki;
using namespace TL;
using namespace Mix;

TwinArray<int, 16> ta;

int main()
{
  cout << "Test start." << endl;
  
  cout << "-----------------------------------" << endl;

  ta.push_back(2);
  ta.push_back(5);

  for( auto && e : ta )
    cout << e << ", ";
  cout << endl;
  
  cout << "-----------------------------------" << endl;
  
  cout << "Test end." << endl;

  return 0;
}
