#include <iostream>
#include <typeinfo>
using namespace std;

struct t1{}; struct t2{}; struct t3{};

template <class A, int I> struct container{
   void callMe(){
      cout << "primary A: " << typeid(A).name() << " I: " << I << endl;
   }
};

template <> struct container<t3,99>{
   void callMe(){
      cout << "complete specialization t3, 99" << endl;
   }
};

int main(void)
{
   container<t1,10> test1;
   test1.callMe();
   container<t3,99> test2;
   test2.callMe();
   return 0;
}
