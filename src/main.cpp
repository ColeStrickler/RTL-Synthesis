
#include "stdio.h"
#include "node.hpp"




#include <iostream>


int main()
{
    



    auto in1 = new InputNode();
    auto in2 = new InputNode();
    auto in3 = new InputNode();
    auto plus = new TimesNode();
    auto times = new PlusNode();
    auto out = new OutputNode();
    in1->parent = plus;
    in2->parent = plus;
    in3->parent = times;


    plus->leftChild = in1;
    plus->rightChild = in2;
    times->leftChild = in3;
    times->rightChild = plus;
    out->Child = times;
    plus->parent = times;
    times->parent = out;

    Verifier verify({{3,3, 3}, {3,4,6}, {2,1,1}, {0,1,0}},  {12,18,3,0}, {in1, in2, in3});
    verify.SetMaxInputFanout(4);
    if (verify.Verify(0))
    {
        printf("Input Permutations checked %d\n", verify.input_perm_check);
        printf("OKAY\n");
    } else
    {
        printf("NOT OKAY\n");
    }






}