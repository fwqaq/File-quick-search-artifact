#pragma  once
#include "ScanManager.h"
#include "monitor.h"
#include <stdio.h>
#include "Highlight.h"
void FirstScanManager(){
	ScanManager::CreateIntance()->startScan("D:");
//	cout << "扫描模块的地址" <<ScanManager::CreateIntance() << endl;
}
void Run(){
	while (1){
		std::vector<std::pair<std::string, std::string>> doc_path;
		cout << "========================================开始查询===========================================" << endl;
		std::string sh;
		cout << "请输入要查询的关键字：";
		std::cin >> sh;
		DataManager::GetInstannce()->Search(sh, doc_path);
		cout << "========================================查询结果===========================================" << endl;
		printf("%-50s %-50s\n", "名称", "路径");
		for (int i = 0; i < doc_path.size(); i++){
			//传递进去我们输入的关键字，，，，
			//传递的关键字sh   --sh可能是汉字，汉字拼音全拼，汉字首字母的拼写
			//传递出来三部分，一个是
			std::vector<std::string> v;
			v.resize(3);
			//将文件的名称转换为小写
			Tolowercase(doc_path[i].first);
			Tolowercase(sh);
			subString(sh, doc_path[i].first+" ",v);
			if (v[1] == ""){
				continue;
			}
			printf("%s", v[0].c_str());
			ColourPrintf(v[1]);
			int count = 50 - v[0].size() - v[1].size();
			printf("%-*s %-50s\n",count, v[2].c_str(), doc_path[i].second.c_str());		
		}
		cout << "=======================================本次查询结束========================================" << endl;
		cout << endl << endl << endl;
	}
}

void MonitorFile(){
	fileWatcher();
}

int main(){
	FirstScanManager();//第一次需要扫描全部的内容
	/////////////////////////////////多线程/////////////////////////////////
	std::thread scanerD(MonitorFile);//使用一个线程去持续监控文件中的变化
	scanerD.detach();
	Run();//运行扫描模块

	
	system("pause");
	return EXIT_SUCCESS;
}