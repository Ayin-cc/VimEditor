#include "Interactor.h"

Interactor::Interactor() {
    char inputFile[25];
    char outputFile[25];

    std::cout << "请输入要编辑的文件名称: ";
    std::cin.getline(inputFile, sizeof(inputFile));

    std::cout << "请输入要输出的文件名(留空则为覆盖原文件): ";
    std::cin.getline(outputFile, sizeof(outputFile));

    if (outputFile[0] == '\0') {
        controller = new TextController(inputFile, inputFile);
    }
    else {
        controller = new TextController(inputFile, outputFile);
    }
}

Interactor::Interactor(const char inputFile[], const char outputFile[]) {
    controller = new TextController(inputFile, outputFile);
}

Interactor::~Interactor() {
}
