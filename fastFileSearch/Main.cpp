#pragma  once
#include "ScanManager.h"
#include "monitor.h"
#include <stdio.h>
#include "Highlight.h"
void FirstScanManager(){
	ScanManager::CreateIntance()->startScan("D:");
//	cout << "ɨ��ģ��ĵ�ַ" <<ScanManager::CreateIntance() << endl;
}
void Run(){
	while (1){
		std::vector<std::pair<std::string, std::string>> doc_path;
		cout << "========================================��ʼ��ѯ===========================================" << endl;
		std::string sh;
		cout << "������Ҫ��ѯ�Ĺؼ��֣�";
		std::cin >> sh;
		DataManager::GetInstannce()->Search(sh, doc_path);
		cout << "========================================��ѯ���===========================================" << endl;
		printf("%-50s %-50s\n", "����", "·��");
		for (int i = 0; i < doc_path.size(); i++){
			//���ݽ�ȥ��������Ĺؼ��֣�������
			//���ݵĹؼ���sh   --sh�����Ǻ��֣�����ƴ��ȫƴ����������ĸ��ƴд
			//���ݳ��������֣�һ����
			std::vector<std::string> v;
			v.resize(3);
			//���ļ�������ת��ΪСд
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
		cout << "=======================================���β�ѯ����========================================" << endl;
		cout << endl << endl << endl;
	}
}

void MonitorFile(){
	fileWatcher();
}

int main(){
	FirstScanManager();//��һ����Ҫɨ��ȫ��������
	/////////////////////////////////���߳�/////////////////////////////////
	std::thread scanerD(MonitorFile);//ʹ��һ���߳�ȥ��������ļ��еı仯
	scanerD.detach();
	Run();//����ɨ��ģ��

	
	system("pause");
	return EXIT_SUCCESS;
}