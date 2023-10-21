#include "TextController.h"

TextController::TextController(const char inputFile[], const char outputFile[]) {
    // ���ļ�
    this->filepath = inputFile;
    inputStream.open(inputFile);
    outputStream.open(outputFile);
    if (!inputStream.is_open()) {
        std::cout << "�ļ�" << inputFile << "��ʧ�ܣ�";
        exit(0);
    }

    // ��ʼ��
    saved = true;
    Buffer* line = new Buffer;
    head = new Buffer;
    at = head;
    line = head;
    head->data[0] = '\0';
    head->prev = nullptr;
    head->next = nullptr;

    charCount = 0;
    lineCount = 1;
    atLine = 1;
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
    while (head != nullptr) {
        Buffer* p = head;
        head = p->next;
        delete p;
    }
}

int TextController::getLineNum() {
    return this->atLine;
}

bool TextController::isSaved() {
    return this->saved;
}

char* TextController::getLine() {
    return this->at->data;
}

void TextController::insert(int line, char data[]) {
    if (line > lineCount) {
        std::cout << "�кŲ��淶!" << std::endl;
        return;
    }

    int depth = 1;
    Buffer* temp = head;
    while (temp != nullptr) {
        if (line == depth) {
            // ������
            Buffer* newLine = new Buffer;
            newLine->prev = temp;
            newLine->next = temp->next;
            strcpy(newLine->data, data);
            temp->next->prev = newLine;
            temp->next = newLine;
            this->charCount += strlen(data);
            this->lineCount++;
            this->saved = false;
            break;
        }
        depth++;
        temp = temp->next;
    }
}

void TextController::jump(int line) {
    int depth = 1;
    Buffer* temp = head;
    while (temp != nullptr) {
        if (line == depth) {
            // ��ת����
            this->at = temp;
            break;
        }
        depth++;
        temp = temp->next;
    }
}

void TextController::remove(int line) {
    int depth = 1;
    Buffer* temp = head;
    while (temp != nullptr) {
        if (line == depth) {
            // �Ƴ���
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            this->charCount -= strlen(temp->data);
            this->lineCount--;
            delete temp;
            this->saved = false;
            break;
        }
        depth++;
        temp = temp->next;
    }
}

void TextController::print() {
    Buffer* line = head;
    while (line->next != nullptr) {
        std::cout << line->data << std::endl;
        line = line->next;
    }
    std::cout << tail->data;
}

void TextController::reload() {
    inputStream.open(this->filepath);

    saved = true;
    Buffer* line = new Buffer;
    at = head;
    line = head;
    head->data[0] = '\0';
    head->prev = nullptr;
    head->next = nullptr;

    charCount = 0;
    lineCount = 1;
    atLine = 1;
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
    line->prev->next = tail;
    strcpy(tail->data, line->data);
    tail->next = nullptr;
    tail->prev = line->prev;
    delete line;
}

void TextController::save() {
    // ��Buffer�е����������output�ļ�
    Buffer* p = head;
    while (p != nullptr) {
        outputStream << p->data;
        outputStream << '\n';
        p = p->next;
    }
}

void TextController::quit() {
    this->~TextController();
}