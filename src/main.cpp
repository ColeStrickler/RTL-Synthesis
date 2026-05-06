
#include "stdio.h"
#include "node.hpp"
#include "vm.hpp"
#include "printer.hpp"
#include "search.hpp"


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
   // final1->parent = final_plus;


    

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
   // final2->parent = final_plus;

    auto reg2 = new RegNode();
    auto reg1 = new RegNode();
    reg1->Child = final1;
    reg2->Child = final2;


    final_plus->leftChild = reg2;
    final_plus->rightChild = reg1;
    
    final1->parent = reg1;
    final2->parent = reg2;
    reg2->parent = final_plus;
    reg1->parent = final_plus;
    
    

    auto reg3 = new RegNode();
    reg3->Child = final_plus;
    final_plus->parent = reg3;




    
    auto out = new OutputNode();
    out->Child = reg3;
        reg3->parent = out;

    //VM vm;
    //vm.Compile({in1, in2, in3, in4, in5, in6});
//
    ////((a+b)*c)
    //vm.SetInputs({2,1,3, 4, 1, 3});
    //vm.PrintProgram();
    //printf("Result %d\n", vm.ExecuteProgramDebug());

    Search searcher;
    CostModel cm{32, 1200, 0};
    auto start = std::chrono::high_resolution_clock::now();
    auto out2 = searcher.topDown({ {5}, {22}, {3}, {1}}, {5*5*5*5-5, 22*22*22*22-22, 3*3*3*3-3, 1*1*1*1-1}, cm);

    //auto out2 = searcher.topDown({{0}, {int(UINT32_MAX)}}, {int(UINT32_MAX), 0}, cm);

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Time: " << duration.count() << " us\n";
    std::string module = RTLModulePrint::PrintModule("TestModule", static_cast<OutputNode*>(out2));
    WriteStringToFile("./test.scala", module);



    // auto start = std::chrono::high_resolution_clock::now();
    // Verifier verify({{1},{2}, {3} },  {92, 16, 64}, {in1, in2, in3, in4, in5, in6});
    // verify.SetMaxInputFanout(6);
    // if (verify.VerifyVM())
    //     printf("OKAY\n");
    // else
    //     printf("NOT OKAY\n");
    // auto end = std::chrono::high_resolution_clock::now();
    // auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    // std::cout << "Time: " << duration.count() << " us\n";

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