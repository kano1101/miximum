#include <iostream>
#include <random>

#include <loki/Typelist.h>
#include <mix/Director.h>
#include <mix/mini/PerformanceTester.h>


int main()
{
  Mix::PerformanceTester<int> pt;
  for ( int j = 0; j < 5; ++j ) {
    pt.Start(j);
    for ( int i = 0; i < 1000000; ++i ) {
    }
    pt.Finish(j);
  }

  pt.Print();
      
  return 0;
}
