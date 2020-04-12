#include <iostream>
#include <map>
#include <loki/Typelist.h>
#include <mix/MultiMapArray.h>

using namespace std;
using namespace Loki;
using namespace TL;
using namespace Mix;

using Container = MultiMapArray<int, char, 4>;
// using Container = multimap<int, char>;
Container c;

int main()
{
  cout << "Test start." << endl;
  
  cout << "-----------------------------------" << endl;

  c.insert(std::make_pair(5, 'a'));
  c.insert(std::make_pair(2, 'b'));
  c.insert(std::make_pair(3, 'c'));
  c.insert(std::make_pair(7, 'd'));
  c.insert(std::make_pair(3, 'e'));

  for( auto & e : c )
    cout << e.second << ", ";
  cout << endl;
  
  cout << "-----------------------------------" << endl;
  
  cout << "Test end." << endl;

  return 0;
}
