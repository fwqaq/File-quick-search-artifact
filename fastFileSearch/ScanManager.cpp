#include "ScanManager.h"
#include "DataManger.h"
void ScanManager::Scan(const std::string& path){
	//ͨ������path·����ȡ��Ŀ¼��������ݺ����ݿ��е�����
	std::set<std::string>localdata;
	std::vector<std::string>localdirs;
	std::vector<std::string>localfiles;
	//�õ����ص��ļ���Ŀ¼
	DirectoryList(path, localfiles, localdirs);
	localdata.insert(localdirs.begin(), localdirs.end());
	localdata.insert(localfiles.begin(), localfiles.end());

	//�õ����ݿ��е�Ŀ¼���ļ�
	std::set<std::string> dbset;
	DataManager::GetInstannce()->GetDocs(path, dbset);
	auto localit = localdata.begin();
	auto dbit = dbset.begin();

	while (localit != localdata.end() && dbit != dbset.end()){
		//1. ���ݿ�ͱ��ض��еĻ���ֱ������
		if (*localit == *dbit){
			localit++;
			dbit++;
		}
		//2. ���ݿ��е��Ǳ����о���������
		else if (*localit < *dbit){
			DataManager::GetInstannce()->InsertDocs(path, *localit);
			localit++;
		}
		//���ݿ����е��Ǳ���û�о���ɾ��
		else if (*localit > *dbit){
			DataManager::GetInstannce()->DeleteDoc(path, *dbit);
			dbit++;
		}
	}
	//���û�бȽ����ֱ�ӽ�����ӻ���ɾ��
	while (localit != localdata.end()){
		//��������
		DataManager::GetInstannce()->InsertDocs(path, *localit);
		localit++;
	}
	while (dbit != dbset.end()){
		DataManager::GetInstannce()->DeleteDoc(path, *dbit);
		dbit++;
	}
	//������Ŀ¼�������ݵĵ���
	for (const auto& dirs : localdirs){
		std::string subpath = path;
		subpath += "\\";
		subpath += dirs;
		Scan(subpath);
	}
}


void ScanManager::ScanNotRecursion(const std::string& path){
	//ͨ������path·����ȡ��Ŀ¼��������ݺ����ݿ��е�����
	std::set<std::string>localdata;
	std::vector<std::string>localdirs;
	std::vector<std::string>localfiles;
	//�õ����ص��ļ���Ŀ¼
	DirectoryList(path, localfiles, localdirs);
	localdata.insert(localdirs.begin(), localdirs.end());
	localdata.insert(localfiles.begin(), localfiles.end());

	//�õ����ݿ��е�Ŀ¼���ļ�
	std::set<std::string> dbset;
	DataManager::GetInstannce()->GetDocs(path, dbset);
	auto localit = localdata.begin();
	auto dbit = dbset.begin();

	while (localit != localdata.end() && dbit != dbset.end()){
		//1. ���ݿ�ͱ��ض��еĻ���ֱ������
		if (*localit == *dbit){
			localit++;
			dbit++;
		}
		//2. ���ݿ��е��Ǳ����о���������
		else if (*localit < *dbit){
			DataManager::GetInstannce()->InsertDocs(path, *localit);
			localit++;
		}
		//���ݿ����е��Ǳ���û�о���ɾ��
		else if (*localit > *dbit){
			DataManager::GetInstannce()->DeleteDoc(path, *dbit);
			dbit++;
		}
	}
	//���û�бȽ����ֱ�ӽ�����ӻ���ɾ��
	while (localit != localdata.end()){
		//��������
		DataManager::GetInstannce()->InsertDocs(path, *localit);
		localit++;
	}
	while (dbit != dbset.end()){
		DataManager::GetInstannce()->DeleteDoc(path, *dbit);
		dbit++;
	}
}
