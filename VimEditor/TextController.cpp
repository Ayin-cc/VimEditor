#include "TextController.h"

TextController::TextController(const char inputFile[], const char outputFile[]) {
    // ���ļ�
    inputStream.open(inputFile);
    outputStream.open(outputFile);
    if (!inputStream.is_open()) {
        std::cout << "�ļ�" << inputFile << "��ʧ�ܣ�";
        exit(0);
    }

    Buffer* line = new Buffer;
    head = new Buffer;
    line = head;
    head->data[0] = '\0';
    head->prev = nullptr;
    head->next = nullptr;

    charCount = 0;
    lineCount = 1;
    int k = 0;
    char c;
    // ��ȡ�ļ�
    while ((c = inputStream.get()) != EOF) {
        if (c == '\r' || c == '\n') {
            if (c == '\r') {
                char nextChar = inputStream.get();
                if (nextChar == '\n') {
                    // Windows��β
                    c = '\n'; 
                }
                else {
                    // ��Windows��β�������ַ�
                    inputStream.unget();
                }
            }
            line->data[k] = '\0';
            // ��һ��
            lineCount++;
            k = 0;        
            Buffer* newLine = new Buffer;
            line->next = newLine;
            newLine->prev = line;
            newLine->next = nullptr;
            line = newLine;
            continue;
        }
        charCount++;
        line->data[k++] = c;
    }
    line->data[k] = '\0';

    // ������β
    tail = new Buffer();
    line->prev->next = tail;
    strcpy(tail->data, line->data);
    tail->next = nullptr;
    tail->prev = line->prev;
    delete line;
}

TextController::~TextController() {
    delete head;
    delete tail;
}

void TextController::append(char data[]) {
}

void TextController::insert(int line, char data[]) {
}

void TextController::jump(int line) {
}

void TextController::remove(int line) {
}

void TextController::print() {
    Buffer* line = head;
    while (line->next != nullptr) {
        std::cout << line->data << std::endl;
        line = line->next;
    }
    std::cout << tail->data;
}
