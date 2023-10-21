static const char* UNKNOW = "未知指令! 请输入-h获取更多帮助!";
static const char* HELP[] = {
	"\t-v\t\t无参数\t\t显示当前内容",
	"\t-begin\t\t无参数\t\t到第一行",
	"\t-end\t\t无参数\t\t到最后一行",
	"\t-p\t\t无参数\t\t到前一行",
	"\t-n\t\t无参数\t\t到下一行",
	"\t-j\t\t[行号]\t\t到指定行",
	"\t-insert\t\t[行号] [行内容]\t\t添加行到指定位置",
	"\t-d\t\t[行号](可选)\t删除当前行或指定行",
	"\t-r\t\t[替换内容]\t\t\t替换当前行\n"
	"\t\t\t-s [子串内容] [替换内容]\t在全文范围替换子串\n"
	"\t\t\t-s -a [子串内容] [替换内容]\t在当前行替换子串",
	"\t-f\t\t[子串内容]\t\t查找某个子串位置\n"
	"\t\t\t-re [子串内容]\t\t使用正则表达式查找",
	"\t-info\t\t无参数\t\t查看当前文件信息",
	"\t-reload\t\t无参数\t\t重新读入文件",
	"\t-wq\t\t无参数\t\t保存退出",
	"\t-q\t\t无参数\t\t直接退出"
};
static const char* viewHelp = HELP[0];
static const char* beginHelp = HELP[1];
static const char* endHelp = HELP[2];
static const char* prevHelp = HELP[3];
static const char* nextHelp = HELP[4];
static const char* jumpHelp = HELP[5];
static const char* insertHelp = HELP[6];
static const char* deleteHelp = HELP[7];
static const char* replaceHelp = HELP[8];
static const char* findHelp = HELP[9];
static const char* infoHelp = HELP[10];
static const char* reloadHelp = HELP[11];
static const char* wqHelp = HELP[12];
static const char* qHelp = HELP[13];