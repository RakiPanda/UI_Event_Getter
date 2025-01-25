#include "helloworld.h"
#include <iostream>

void printHelloWorld() {
    std::cout << "Hello World!" << std::endl;
    // コマンドプロンプトがすぐに閉じないようにする
    std::cin.get();
}