#pragma once
#include "Interactor.h";

int main(int argc, char* argv[]) {
	Interactor* interactor;
	if (argc == 1) {
		interactor = new Interactor();
	}
	else if(argc == 2){
		interactor = new Interactor(argv[1], argv[1]);
	}
	else if (argc == 3) {
		interactor = new Interactor(argv[1], argv[2]);
	}
	else {
		std::cout << "��������! ��ȷ������ʽΪ[�����ļ���] [����ļ���(��ѡ)]";
		return 0;
	}

	interactor->startup();

	delete interactor;
	return 0;
}