#include "Interactor.h"

Interactor::Interactor() {
    char inputFile[25];
    char outputFile[25];

    std::cout << "������Ҫ�༭���ļ�����: ";
    std::cin.getline(inputFile, sizeof(inputFile));

    std::cout << "������Ҫ������ļ���(������Ϊ����ԭ�ļ�): ";
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
