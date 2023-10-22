#pragma once

#include "TextController.h"
#include "Help.h"

class Interactor {
private:
	TextController* controller;

public:
	Interactor();
	Interactor(const char inputFile[], const char outputFile[] = nullptr);
	~Interactor();

	void startup();
	void paramCommand(const char* command);
	void view();
	void begin();
	void end();
	void prev();
	void next();
	void jump(int line);
	void insert(int line, const char* data);
	void del(int line = -1);
	void replace(const char* data);
	void strReplace(const char* str, const char* data, bool a = false);
	void find(const char* str, bool regex = false);
	void info();
	void reload();
	void save();
	void saveQuit();
	void quit();
};

