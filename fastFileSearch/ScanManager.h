#pragma  once
#include "DataManger.h"
#include <io.h>
class ScanManager{
public:
	void Scan(const std::string& path);
	static ScanManager* CreateIntance(){
		static ScanManager s;
		return &s;
	}
	void startScan(const std::string& path){
		Scan(path);
	}
	void ScanNotRecursion(const std::string& path);
private:
	//��װһ��SqliteManager���������������ݵĲ����ɾ��
	ScanManager(){
	}//���쵥��ģʽ
	//��Ҫ�ľ��ǽ�ɨ����������ݺ����ݿ��е����ݽ��жԱ����������ݲ��뵽���ݿ⣬ɾ�������������ݿ���ɾ����
};
//��������Ŀ¼�µ��ļ���Ŀ¼�ļ���
static void DirectoryList(const std::string& path, std::vector<std::string>&subfiles, \
	std::vector<std::string>&subdirs){
	_finddata_t file;//����һ���ļ��ṹ��
	//��ʱ��·������Ҫ�ı�ģ���Ҫ������Ŀ¼����ģ����ݽ�����ֻ�ǵ��״�Ŀ¼�ļ�
	std::string _path = path + "\\*.*";//
	
	intptr_t handle = _findfirst(_path.c_str(), &file);
	if (handle == -1){
		ERROE_LOG("_findfirst:%s", _path.c_str());
	}
	do {
		// _A_SUBDIR���ļ��У�����Ŀ¼����������ļ�,name����file���������ԣ���һ�����飬������256��
		if ((file.attrib & _A_SUBDIR)&&!(file.attrib&_A_HIDDEN)){
			//Ŀ¼��ʱ����Ҫ�ж��ǲ���.����..������������������ļ��ڲ�ѯ��ʱ��ͻ�ݹ���ѭ��
			if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0)){
				subdirs.push_back(file.name);
			}
		}
		else{
			//��ʱ�����ļ���
			subfiles.push_back(file.name);
		}

	} while (_findnext(handle, &file) == 0);

	_findclose(handle);
}