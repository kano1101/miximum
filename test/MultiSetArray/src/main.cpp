#include <iostream>
#include <loki/Typelist.h>
#include <mix/MultiSetArray.h>

using namespace std;
using namespace Loki;
using namespace TL;
using namespace Mix;

MultiSetArray<int, 16> msa;

int main()
{
  cout << "Test start." << endl;
  
  cout << "-----------------------------------" << endl;

  msa.insert(5);
  msa.insert(2);

  for( auto && e : msa )
    cout << e << ", ";
  cout << endl;
  
  cout << "-----------------------------------" << endl;
  
  cout << "Test end." << endl;

  return 0;
}
