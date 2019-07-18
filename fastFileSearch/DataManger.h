#pragma  once
#include "SqliteManager.h"
class DataManager{
public:
	static DataManager* GetInstannce(){
		static DataManager m_db;
		return &m_db;
	}
	void Init();
	//得到数据库中的内容
	void GetDocs(std::string path, std::set<std::string>& dbset);
	
	//向数据库中插入数据
	void InsertDocs(const std::string path,const std::string doc);

	//在数据库中删除数据
	void DeleteDoc(const std::string& path,const std::string doc);

	//利用关键字进行查找
	void Search(const std::string& key, std::vector<std::pair<std::string, std::string>>&doc_paths);

	~DataManager(){
	}
private:
	DataManager(){
		Init();
	};
	//维护一个SqliteManager的对象
	SqliteManager _sqma;
};