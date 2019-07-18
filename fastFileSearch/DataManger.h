#pragma  once
#include "SqliteManager.h"
class DataManager{
public:
	static DataManager* GetInstannce(){
		static DataManager m_db;
		return &m_db;
	}
	void Init();
	//�õ����ݿ��е�����
	void GetDocs(std::string path, std::set<std::string>& dbset);
	
	//�����ݿ��в�������
	void InsertDocs(const std::string path,const std::string doc);

	//�����ݿ���ɾ������
	void DeleteDoc(const std::string& path,const std::string doc);

	//���ùؼ��ֽ��в���
	void Search(const std::string& key, std::vector<std::pair<std::string, std::string>>&doc_paths);

	~DataManager(){
	}
private:
	DataManager(){
		Init();
	};
	//ά��һ��SqliteManager�Ķ���
	SqliteManager _sqma;
};