#include "TextController.h"
#include <regex>
#include <windows.h>

TextController::TextController(const char inputFile[], const char outputFile[]) {
    // 打开文件
    this->filepath = inputFile;
    inputStream.open(inputFile);
    this->outputpath = outputFile;
    if (!inputStream.is_open()) {
        std::cout << "文件" << inputFile << "打开失败！";
        exit(0);
    }

    // 初始化
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
    if(line->prev != nullptr) {
        tail = new Buffer();
        line->prev->next = tail;
        strcpy(tail->data, line->data);
        tail->next = nullptr;
        tail->prev = line->prev;
    }
    else {
        tail = head;
    }
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

void TextController::insert(int line, const char data[]) {
    if (line > lineCount + 1) {
        std::cout << "行号不规范!" << std::endl;
        return;
    }
    else if (line == lineCount + 1) {
        // 最后一行添加新行
        Buffer* newLine = new Buffer;
        strcpy(newLine->data, data);
        this->lineCount++;
        this->charCount += strlen(data);
        this->saved = false;
        tail->next = newLine;
        newLine->prev = tail;
        tail = newLine;            
        newLine->next = nullptr;
        return;
    }

    int depth = 1;
    Buffer* temp = head;
    while (temp != nullptr) {
        if (line == depth) {
            if (depth <= this->atLine) {
                this->atLine++;
            }

            // 插入行
            Buffer* newLine = new Buffer;
            newLine->next = temp;
            if (temp->prev != nullptr) {
                temp->prev->next = newLine;
                temp->prev = newLine;
            }
            else {
                // 插入第一行
                this->head = newLine;
                newLine->prev = nullptr;
            }
            strcpy(newLine->data, data);
            this->charCount += strlen(data);
            this->lineCount++;
            this->saved = false;
            break;
        }
        depth++;
        temp = temp->next;
    }
}

// 匹配子串
int findSubstr(const char* data, const char* str) {
    int dataLength = std::strlen(data);
    int strLength = std::strlen(str);

    for (int i = 0; i <= dataLength - strLength; i++) {
        int j;
        for (j = 0; j < strLength; j++) {
            if (data[i + j] != str[j]) {
                break;
            }
        }
        if (j == strLength) {
            return i;
        }
    }

    return -1;
}

// 正则表达式匹配子串
bool findRegexStr(const char* data, const char* pattern) {
    std::regex reg(pattern);
    if (std::regex_search(data, reg)) {
        return true;
    }
    else {
        return false;
    }
}

void TextController::replaceLine(const char* data) {
    int i;
    for (i = 0; i < strlen(data); i++) {
        at->data[i] = data[i];
    }
    at->data[i + 1] = '\0';
    this->saved = false;
}

void TextController::replaceStr(const char* data, const char* str, bool a) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    this->saved = false;
    if (a) {
        // 在当前行查找
        int k;
        if ((k = findSubstr(at->data, str)) != -1) {
            char temp[1024];
            int i;
            int j = 0;
            // 匹配到子串，输出原内容
            std::cout << atLine << ": ";
            for (int i = 0; i < strlen(at->data); i++) {
                if (i >= k && i <= k + strlen(str) - 1) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                    std::cout << at->data[i];
                    SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                    continue;
                }
                std::cout << at->data[i];
            }

            // 将该行中子串后的内容保存
            for (i = k + strlen(str); i < strlen(at->data); i++) {
                temp[j++] = at->data[i];
            }
            temp[j] = '\0';
            std::cout << "   --->>" << std::endl;

            // 输出修改后的内容          
            std::cout << "   ";
            for (i = 0; i < k; i++) {
                std::cout << at->data[i];
            }
            j = 0;
            for (i = k; i < k + strlen(data); i++) {
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << *(data + j);
                at->data[i] = *(data + j);
                j++;
            }
            SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            at->data[i] = '\0';
            for (i = 0; i < strlen(temp); i++) {
                std::cout << temp[i];
            }
            strcat(at->data, temp);
            std::cout << std::endl;
        }
        else {
            std::cout << "未找到匹配子串!" << std::endl;
        }
    }
    else {
        // 在全文查找
        int depth = 1;
        Buffer* temp = head;
        bool flag = false;        
        int k;
        while (temp != nullptr) {
            if ((k = findSubstr(temp->data, str)) != -1) {
                char templ[1024];
                int j = 0;
                flag = true;
                int i;
                // 匹配到子串，先输出原内容
                std::cout << depth << ": ";
                for (i = 0; i < strlen(temp->data); i++) {
                    if (i >= k && i <= k + strlen(str) - 1) {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_INTENSITY);
                        std::cout << temp->data[i];
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        continue;
                    }
                    std::cout << temp->data[i];
                }
                std::cout << "   --->>" << std::endl;

                // 将该行中子串后的内容保存
                for (i = k + strlen(str); i < strlen(at->data); i++) {
                    templ[j++] = at->data[i];
                }
                templ[j] = '\0';
                std::cout << "   --->>" << std::endl;

                // 输出修改后的内容
                std::cout << "   ";
                for (i = 0; i < k; i++) {
                    std::cout << temp->data[i];
                }
                j = 0;
                for (i = k; i < k + strlen(data); i++) {
                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                    std::cout << *(data + j);
                    temp->data[i] = *(data + j);
                    j++;
                }
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                temp->data[j] = '\0';
                for (i = 0; i < strlen(templ); i++) {
                    std::cout << templ[i];
                }
                strcat(temp->data, templ);
                std::cout << std::endl;
            }
            depth++;
            temp = temp->next;
        }
        if (!flag) {
            std::cout << "未找到匹配子串!" << std::endl;
        }
    }
}

void TextController::jump(int line) {
    int depth = 1;
    Buffer* temp = head;
    while (temp != nullptr) {
        if (line == depth) {
            // 跳转到行
            this->at = temp;
            this->atLine = line;
            break;
        }
        depth++;
        temp = temp->next;
    }
}

void TextController::find(const char* str, bool regex) {
    int depth = 1;
    Buffer* temp = head;
    bool flag = false;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    while (temp != nullptr) {      
        if (regex) {
            // 使用正则表达式
            if (findRegexStr(temp->data, str)) {
                flag = true;
                std::cout << depth << ": ";
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::cout << temp->data << std::endl;
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            }
        }
        else {
            // 不使用正则表达式
            int k = findSubstr(temp->data, str);
            if (k != -1) {
                flag = true;
                std::cout << depth << ": ";
                for (int i = 0; i < strlen(temp->data); i++) {
                    if (i >= k && i <= k + strlen(str) - 1) {
                        SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                        std::cout << temp->data[i];            
                        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                        continue;
                    }                 
                    std::cout << temp->data[i];
                }
                std::cout << std::endl;
            }
        }
        depth++;
        temp = temp->next;
    }
    if (!flag) {
        std::cout << "未找到匹配子串!" << std::endl;
    }
}

void TextController::remove(int line) {
    if (line == -1 || line == this->atLine) {
        if (this->at->next == nullptr) {
            if (this->at->prev != nullptr) {
                this->at->prev->next = nullptr;
                this->charCount -= strlen(this->at->data);
                this->lineCount--;
                this->saved = false;
                this->at = at->prev;
                this->tail = this->at;
                this->atLine--;
            }           
            return;
        }
        else if (this->at->prev == nullptr) {
            if (this->at->next != nullptr) {
                this->at->next->prev = nullptr;
                this->charCount -= strlen(this->at->data);
                this->lineCount--;
                this->saved = false;
                this->at = this->at->next;
                this->head = this->at;
            }          
            return;
        }
        this->charCount -= strlen(this->at->data);
        this->at->prev->next = this->at->next;
        this->at = this->at->prev;        
        this->lineCount--;
        this->saved = false;
        return;
    }
    int depth = 1;
    Buffer* temp = head;
    while (temp != nullptr) {
        if (line == depth) {
            // 移除行
            if (depth < this->atLine) {
                this->atLine--;
            }
            if (temp->next == nullptr) {
                if (temp->prev != nullptr) {
                    this->charCount -= strlen(temp->data);
                    this->lineCount--;
                    this->saved = false;
                    temp->prev->next = nullptr;
                }              
                return;
            }
            else if (temp->prev == nullptr) {
                if (temp->next != nullptr) {
                    this->charCount -= strlen(temp->data);
                    this->lineCount--;
                    this->saved = false;
                    temp->next->prev = nullptr;
                }                
                return;
            }
            temp->prev->next = temp->next;
            temp->next->prev = temp->prev;
            this->charCount -= strlen(temp->data);
            this->lineCount--;
            this->saved = false;
            return;
        }
        depth++;
        temp = temp->next;
    }
}

void TextController::print() {
    Buffer* line = head;
    int depth = 1;
    std::cout << "-------- HEAD --------" << std::endl;
    while (line->next != nullptr) {
        std::cout << depth++ << ": " << line->data << std::endl;
        line = line->next;
    }
    std::cout << depth << ": " << tail->data << std::endl;
    std::cout << "-------- END ---------" << std::endl;
}

void TextController::reload() {
    inputStream.close();
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
    if (line->prev != nullptr) {
        tail = new Buffer();
        line->prev->next = tail;
        strcpy(tail->data, line->data);
        tail->next = nullptr;
        tail->prev = line->prev;
    }
    else {
        tail = head;
    }
    delete line;

    std::cout << "文件已重新读取!" << std::endl;
}

void TextController::goPrev() {
    if (this->at->prev == nullptr) {
        std::cout << "已经在第一行了!" << std::endl;
        return;
    }
    this->at = this->at->prev;
    this->atLine--;
}

void TextController::goNext() {
    if (this->at->next == nullptr) {
        std::cout << "已经在最后一行了!" << std::endl;
        return;
    }
    this->at = this->at->next;
    this->atLine++;
}

void TextController::goTail() {
    this->at = this->tail;
    this->atLine = this->lineCount;
}

const char* TextController::getFilePath() {
    return this->filepath;
}

int TextController::getLineCount() {
    return this->lineCount;
}

void TextController::save() {
    // 将Buffer中的内容输出到output文件
    outputStream.open(this->outputpath, std::ios::out);
    Buffer* p = head;
    for (int i = 0; i < this->lineCount - 1; i++) {
        outputStream << p->data << "\r";
        p = p->next;
    }
    outputStream << p->data;
    outputStream.flush();
    this->saved = true;
}

void TextController::showInfo() {
    std::cout << "文件信息: " << this->filepath << std::endl;
    std::cout << "字符数: " << this->charCount << std::endl;
    std::cout << "行数: " << this->lineCount << std::endl;
}

void TextController::quit() {
    this->~TextController();
}

void TextController::goHead() {
    this->at = this->head;
    this->atLine = 1;
}
