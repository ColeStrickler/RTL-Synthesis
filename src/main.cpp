
#include "stdio.h"
#include "node.hpp"




#include <iostream>


int main()
{
    



    auto in1 = new InputNode();
    auto in2 = new InputNode();
    auto in3 = new InputNode();
    auto plus = new PlusNode();
    auto times = new TimesNode();
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

    Verifier verify({{3,2, 4}, {2,2,4}},  {20,16}, {in1, in2, in3});
    verify.SetMaxInputFanout(4);
    if (verify.Verify(0))
    {
        printf("OKAY\n");
    } else
    {
        printf("NOT OKAY\n");
    }






}