#include <iostream>
#include <typeinfo>
using namespace std;
/*
https://www.ibm.com/developerworks/community/blogs/12bb75c9-dfec-42f5-8b55-b669cc56ad76/entry/c__e6_a8_a1_e6_9d_bf__e7_a9_b6_e7_ab_9f_e4_bb_80_e4_b9_88_e6_98_af_e7_89_b9_e5_8c_96?lang=en
*/

struct t1{}; struct t2{}; struct t3{};

template <class A, int I> struct container{
   void callMe(){
      cout << "primary A: " << typeid(A).name() << " I: " << I << endl;
   }
};

template <int I1>  struct container<t3,I1>{
   void callMe(){
      cout << "partial specialization t3 and " << I1  << endl;
   }
};

template <> struct container<t3,99>{
   void callMe(){
      cout << "complete specialization t3, 99 " << endl;
   }
};


int main(void)
{
   container<t1,10> test1;
   test1.callMe();
   container<t3,99> test2;
   test2.callMe();
   container<t3,75> test3;
   test3.callMe();
   container<t3,25> test4;
   test4.callMe();
   return 0;
}
