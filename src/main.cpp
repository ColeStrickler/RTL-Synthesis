
#include "stdio.h"
#include "node.hpp"
#include "vm.hpp"
#include "printer.hpp"


#include <iostream>
#include <chrono>
#include <fstream>
#include <string>

void WriteStringToFile(const std::string& filename, const std::string& content) {
    std::ofstream out(filename);
    if (!out) {
        throw std::runtime_error("Failed to open file: " + filename);
    }
    out << content;
}





int main()
{
    auto final_plus = new PlusNode();

    auto in1 = new InputNode();
    auto in2 = new InputNode();
    auto in3 = new InputNode();
    auto final1 = new TimesNode();
    auto plus = new PlusNode();

    in1->parent = plus;
    in2->parent = plus;
    in3->parent = final1;

    plus->leftChild = in1;
    plus->rightChild = in2;
    final1->leftChild = in3;
    final1->rightChild = plus;
    plus->parent = final1;
    final1->parent = final_plus;




    auto in4 = new InputNode();
    auto in5 = new InputNode();
    auto in6 = new InputNode();
    auto final2 = new TimesNode();
    auto plus2 = new PlusNode();

    in4->parent = plus2;
    in5->parent = plus2;
    in6->parent = final2;

    plus2->leftChild = in4;
    plus2->rightChild = in5;
    final2->leftChild = in6;
    final2->rightChild = plus2;
    plus2->parent = final2;
    final2->parent = final_plus;




    final_plus->leftChild = final2;
    final_plus->rightChild = final1;

    




    
    auto out = new OutputNode();
    out->Child = final_plus;
        final_plus->parent = out;
    //vm.Compile({in1, in2, in3, in4, in5, in6});
    //vm.SetInputs({2,1,1, 1, 1, 1});
    //vm.PrintProgram();
    //printf("Result %d\n", vm.ExecuteProgram());



   std::string module = RTLModulePrint::PrintModule("TestModule", out);
    WriteStringToFile("./test.scala", module);



    auto start = std::chrono::high_resolution_clock::now();
    Verifier verify({{1},{2}, {3} },  {92, 16, 64}, {in1, in2, in3, in4, in5, in6});
    verify.SetMaxInputFanout(6);
    if (verify.VerifyVM())
        printf("OKAY\n");
    else
        printf("NOT OKAY\n");    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time: " << duration.count() << " us\n";

   //if (verify.Verify(0))
   //{
   //    printf("Input Permutations checked %d\n", verify.input_perm_check);
   //    printf("OKAY\n");
   //} else
   //{
   //    printf("NOT OKAY\n");
   //}
   //auto end = std::chrono::high_resolution_clock::now();
   //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
   //std::cout << "Time: " << duration.count() << " us\n";






}