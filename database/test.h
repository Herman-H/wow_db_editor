#include "dbtmp.h"
#include <cstdio>

template <typename T>
struct make_fail{ typedef typename T::falalalalalail fails; };

void test_tuple()
{
    dbtmp::tuple<int,int,float,double> t0(0,1,2.0f,3.0);
    printf("Tuple initialized to <0,1,2,3>:\n");
    printf("Index 0 = %d\n", dbtmp::get<0>(t0));
    printf("Index 1 = %d\n", dbtmp::get<1>(t0));
    printf("Index 2 = %f\n", dbtmp::get<2>(t0));
    printf("Index 3 = %f\n", dbtmp::get<3>(t0));
}

void test_sized_cstring()
{
    dbtmp::sized_cstring s("Hej!");
    printf("String is: %s\n", s.data);
    printf("Size is: %d\n", s.size);

    dbtmp::sized_cstring str[] = { {"Hej!"}, {"Hejda"}, {"Lol"} };
    printf("String[0] is: %s\n", str[0].data);
    printf("and its size is: %d\n", str[0].size);
    printf("String[1] is: %s\n", str[1].data);
    printf("and its size is: %d\n", str[1].size);
    printf("String[2] is: %s\n", str[2].data);
    printf("and its size is: %d\n", str[2].size);
    printf("Total size of strings is: %d\n", str[0].size + str[1].size + str[2].size - 3);
}
