
#include "stdio.h"
#include "node.hpp"
#include "vm.hpp"



#include <iostream>


int main()
{
    



    // Inputs
    auto in1 = new InputNode();
    auto in2 = new InputNode();
    auto in3 = new InputNode();
    auto in4 = new InputNode();
    auto in5 = new InputNode();
    auto in6 = new InputNode();

    // Level 1
    auto n1 = new PlusNode();   // in1 + in2
    auto n2 = new TimesNode();  // in3 * in4
    auto n3 = new PlusNode();   // in5 + in6

    n1->leftChild = in1; n1->rightChild = in2;
    n2->leftChild = in3; n2->rightChild = in4;
    n3->leftChild = in5; n3->rightChild = in6;

    in1->parent = n1; in2->parent = n1;
    in3->parent = n2; in4->parent = n2;
    in5->parent = n3; in6->parent = n3;

    // Level 2
    auto n4 = new TimesNode();  // n1 * n2
    auto n5 = new PlusNode();   // n2 + n3

    n4->leftChild = n1; n4->rightChild = n2;
    n5->leftChild = n2; n5->rightChild = n3;

    n1->parent = n4;
    n2->parent = n4; // overwritten later intentionally (fanout case)

    // If you want true DAG fanout, do NOT overwrite parent; instead allow multiple parents.
    // For now we keep tree semantics, so rewire:

    n2->parent = n5;
    n3->parent = n5;

    // Level 3
    auto n6 = new TimesNode();  // n4 * n5
    n6->leftChild = n4;
    n6->rightChild = n5;

    n4->parent = n6;
    n5->parent = n6;

    // Level 4
    auto n7 = new PlusNode();   // n6 + n3
    n7->leftChild = n6;
    n7->rightChild = n3;

    n6->parent = n7;
    n3->parent = n7;

    // Level 5
    auto n8 = new TimesNode();  // n7 * n1
    n8->leftChild = n7;
    n8->rightChild = n1;

    n7->parent = n8;
    in1->parent = n8;

    // Level 6 (root)
    auto n9 = new PlusNode();   // n8 + n2
    n9->leftChild = n8;
    n9->rightChild = n2;

    n8->parent = n9;
    n2->parent = n9;

    // Output
    auto out = new OutputNode();
    out->Child = n9;
    n9->parent = out;
    VM vm;



    //vm.Compile({in1, in2, in3, in4, in5, in6});
    //vm.SetInputs({2,1,1, 1, 1, 1});
    //vm.PrintProgram();
    //printf("Result %d\n", vm.ExecuteProgram());



    Verifier verify({{2,1,1, 1, 1, 1}},  {6}, {in1, in2, in3, in4, in5, in6});
    verify.SetMaxInputFanout(1);
    if (verify.VerifyVM())
        printf("OKAY\n");
    else
        printf("NOT OKAY\n");    
    //if (verify.Verify(0))
    //{
    //    printf("Input Permutations checked %d\n", verify.input_perm_check);
    //    printf("OKAY\n");
    //} else
    //{
    //    printf("NOT OKAY\n");
    //}






}