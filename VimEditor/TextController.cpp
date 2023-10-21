#include "TextController.h"

TextController::TextController(const char inputFile[], const char outputFile[]) {
    // 打开文件
    inputStream.open(inputFile);
    outputStream.open(outputFile);
    if (!inputStream.is_open()) {
        std::cout << "文件" << inputFile << "打开失败！";
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
    // 读取文件
    while ((c = inputStream.get()) != EOF) {
        if (c == '\r' || c == '\n') {
            if (c == '\r') {
                char nextChar = inputStream.get();
                if (nextChar == '\n') {
                    // Windows行尾
                    c = '\n'; 
                }
                else {
                    // 非Windows行尾，回退字符
                    inputStream.unget();
                }
            }
            line->data[k] = '\0';
            // 下一行
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

    // 保存行尾
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
