#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>

struct Buffer {
	char data[1024];
	Buffer* next;
	Buffer* prev;
};

class TextController {
private:
	const char* originText;
	const char* filepath;
	const char* outputpath;
	std::ifstream inputStream;
	std::ofstream outputStream;
	int charCount;
	int lineCount;
	int atLine;
	bool saved;
	Buffer* head;
	Buffer* tail;
	Buffer* at;

public:
	TextController(const char inputFile[], const char outputFile[]);
	~TextController();

	bool isSaved();
	int getLineNum();
	char* getLine();
	int getLineCount();
	const char* getFilePath();
	void showInfo();

	void goPrev();
	void goNext();
	void goHead();
	void goTail();
	void find(const char* str, bool regex);
	void insert(int line, const char* data);
	void replaceLine(const char* data);
	void replaceStr(const char* data, const char* str, bool a);
	void jump(int line);
	void remove(int line);
	void print();
	void reload();
	void save();
	void quit();
};