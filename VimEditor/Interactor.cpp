#include "Interactor.h"
#include <regex>

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

    controller->print();
}

Interactor::Interactor(const char inputFile[], const char outputFile[]) {
    controller = new TextController(inputFile, outputFile);
}

Interactor::~Interactor() {
    delete controller;
}

void Interactor::paramCommand(const char* command) {
    if (strcmp(command, "h") == 0) {
        // help命令
        for (int i = 1; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "未知指令! 输入h获取帮助!" << std::endl;
                return;
            }
        }
        for (int i = 0; i < 16; i++) {
            std::cout << HELP[i] << std::endl;
        }
    }
    else if (strcmp(command, "v") == 0) {
        // view命令
        for (int i = 1; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << viewHelp << std::endl;
                return;
            }
        }
        this->view();
    }
    else if(strcmp(command, "begin") == 0) {
        // begin命令
        for (int i = 5; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << beginHelp << std::endl;
                return;
            }
        }
        this->begin();
    }
    else if (strcmp(command, "end") == 0) {
        // end命令
        for (int i = 3; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << endHelp << std::endl;
                return;
            }
        }
        this->end();
    }
    else if (strcmp(command, "p") == 0) {
        // prev命令
        for (int i = 1; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << prevHelp << std::endl;
                return;
            }
        }
        this->prev();
    }
    else if (strcmp(command, "n") == 0) {
        // next命令
        for (int i = 1; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << nextHelp << std::endl;
                return;
            }
        }
        this->next();
    }
    else if (strncmp(command, "j", 1) == 0) {
        // jump命令
        if (strlen(command) < 2 || command[1] != ' ') {
            std::cout << "命令格式错误!" << std::endl;
            std::cout << jumpHelp << std::endl;
            return;
        }

        int line;
        if (sscanf(command, "j %d", &line) == 1) {
            // 命令符合规范
            this->jump(line);
        }
        else {
            std::cout << "命令格式错误!" << std::endl;
            std::cout << jumpHelp << std::endl;
            return;
        }
    }
    else if (strncmp(command, "insert", 6) == 0) {
        // insert命令
        if (strlen(command) < 7 || command[7] != ' ') {
            std::cout << "命令格式错误!" << std::endl;
            std::cout << insertHelp << std::endl;
            return;
        }

        int line;
        char* data = nullptr;
        if (sscanf(command, "insert %d \"%[^\"]\"", &line, data) == 2) {
            // 命令符合规范
            this->insert(line, data);
        }
        else {
            std::cout << "命令格式错误!" << std::endl;
            std::cout << jumpHelp << std::endl;
            return;
        }
    }
    else if (strncmp(command, "d", 1) == 0) {
        // delete命令
        int line;
        if (sscanf(command, "d %d", &line) == 1) {
            // 命令符合规范
            this->del(line);
        }
        else {
            for (int i = 1; i < strlen(command); i++) {
                if (command[i] != ' ') {
                    std::cout << "命令格式错误!" << std::endl;
                    std::cout << deleteHelp << std::endl;
                    return;
                }
            }
            this->del();
        }
    }
    else if (strncmp(command, "r", 1) == 0) {
        // replace命令
        if (strlen(command) < 2 || command[1] != ' ') {
            std::cout << "命令格式错误!" << std::endl;
            std::cout << jumpHelp << std::endl;
            return;
        }

        // 判断是带参数-s
        char* str = nullptr;
        char* data = nullptr;
        if (sscanf(command, "r -s -a \"%[^\"]\" \"%[^\"]\"", str, data) == 2) {
            this->strReplace(str, data, true);
        }
        else if (sscanf(command, "r -s \"%[^\"]\" \"%[^\"]\"", str, data) == 2) {
            this->strReplace(str, data);
        }
        else if (sscanf(command, "r \"%[^\"]\"", data) == 1) {
            this->replace(data);
        }
        else {
            std::cout << "命令格式错误!" << std::endl;
            std::cout << replaceHelp << std::endl;
            return;
        }
    }
    else if (strncmp(command, "f", 1) == 0) {
        // find命令
        if (strlen(command) < 2 || command[1] != ' ') {
            std::cout << "命令格式错误!" << std::endl;
            std::cout << jumpHelp << std::endl;
            return;
        }

        char* str = nullptr;
        if (sscanf(command, "f -re \"%[^\"]\"", str) == 1) {
            this->find(str, true);
        }
        else {
            if (sscanf(command, "f \"%[^\"]\"", str) == 1) {
                this->find(str);
                return;
            }
            std::cout << "命令格式错误!" << std::endl;
            std::cout << findHelp << std::endl;
            return;
        }
    }
    else if (strcmp(command, "info") == 0) {
        // information命令
        for (int i = 4; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << infoHelp << std::endl;
                return;
            }
        }
        this->info();
    }
    else if (strcmp(command, "reload") == 0) {
        // reload命令
        for (int i = 6; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << reloadHelp << std::endl;
                return;
            }
        }
        this->reload();
    }
    else if (strcmp(command, "s") == 0) {
        // save命令
        for (int i = 4; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << saveHelp << std::endl;
                return;
            }
        }
        this->save();
    }
    else if (strcmp(command, "wq") == 0) {
        // save&quit命令
        for (int i = 2; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << wqHelp << std::endl;
                return;
            }
        }
        this->saveQuit();
    }
    else if (strcmp(command, "q") == 0) {
        // quit命令
        for (int i = 1; i < strlen(command); i++) {
            if (command[i] != ' ') {
                std::cout << "命令格式错误!" << std::endl;
                std::cout << qHelp << std::endl;
                return;
            }
        }
        this->quit();
    }
    else {
        std::cout << "未知指令! 输入h获取帮助!" << std::endl;
    }
}

void Interactor::startup() {
    std::cout << "## 正在编辑文件" << controller->getFilePath() << std::endl;
    std::cout << "## 通过输入h来获取更多帮助..." << std::endl;
    std::cout << std::endl;

    while (true) {
        std::cout << controller->getLineNum() << ": " << controller->getLine() << std::endl;
        std::cout << "# ";
        char input[1024];
        std::cin.getline(input, sizeof(input));

        // 匹配指令
        paramCommand(input);     
    }
}

void Interactor::view() {
    controller->print();
}

void Interactor::begin() {
    controller->goHead();
}

void Interactor::end() {
    controller->goTail();
}

void Interactor::prev() {
    controller->goPrev();
}

void Interactor::next() {
    controller->goNext();
}

void Interactor::jump(int line) {
    if (line > controller->getLineCount() || line < 1) {
        std::cout << "行号不规范!" << std::endl;
        return;
    }
    controller->jump(line);
}

void Interactor::insert(int line, const char* data) {
    if (line < 0) {
        std::cout << "行号不规范!" << std::endl;
        return;
    }
    controller->insert(line, data);
}

void Interactor::del(int line) {
    if (line != -1 && (line > controller->getLineCount() || line < 1)) {
        std::cout << "行号不规范!" << std::endl;
        return;
    }
    controller->remove(line);
}

void Interactor::replace(const char* data) {
    controller->replaceLine(data);
}

void Interactor::strReplace(const char* str, const char* data, bool a) {
    controller->replaceStr(data, str, a);
}

void Interactor::find(const char* str, bool regex) {
    controller->find(str, regex);
}

void Interactor::info() {
    controller->showInfo();
}

void Interactor::reload() {
    char c;
    bool flag = true;
    if (!controller->isSaved()) {
        std::cout << "文件未保存，是否继续(Y/N): ";
        std::cin >> c;
        if (c == 'Y') {
            flag = true;
        }
        else {
            flag = false;
        }
    }
    if (flag) {
        controller->reload();
    }
}

void Interactor::save() {
    controller->save();
}

void Interactor::saveQuit() {
    controller->save();
    controller->quit();
    exit(1);
}

void Interactor::quit() {
    char c;
    bool flag = true;
    if (!controller->isSaved()) {
        std::cout << "文件未保存，是否继续(Y/N): ";
        std::cin >> c;
        if (c == 'Y') {
            flag = true;
        }
        else {
            flag = false;
        }
    }
    if (flag) {
        controller->quit();
        exit(1);
    }
}
