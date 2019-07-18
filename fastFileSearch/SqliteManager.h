#pragma once
#include "command.h"
#include <sqlite3.h>

class SqliteManager{
public:
	SqliteManager()
		:_db(nullptr){
		
	};
	~SqliteManager(){
		Close();
	}
	//��һ�����ݿ�
	void Open(const std::string& path);

	//�ر����ݿ�
	void Close();

	//�����ݿ�ִ�в���ɾ���Ȳ���
	void ExecuteSql(const std::string sql);

	//�����ݿ���в��Ҳ������õ�����һ����ά�����ָ��,��ʵ����һ��һ����ŵģ��ö�ά�����ʾ��ʵ������һά����
	void GetTable(const std::string sql, int &row, int &col, char**&ppRet);
	
	//��ֹ�����͸�ֵ
	SqliteManager(const SqliteManager&) = delete;
	SqliteManager& operator=(const SqliteManager&) = delete;
private:
	sqlite3* _db;
	std::mutex _mutex;
};



//ʹ��ARII��GetTable��װ�����ڶ��ڴ���й����Զ��ͷŷ��صĶ�ά����
class AutoGetTable{
public:
	AutoGetTable(SqliteManager* dbMag, const std::string sql, int & row, int & col, char**& ppRet)
		:_dbMag(dbMag), _ppVlaue(0)
	{
		_dbMag->GetTable(sql, row, col, ppRet);
		_ppVlaue = ppRet;
	}
	virtual ~AutoGetTable(){
		if (_ppVlaue){
			sqlite3_free_table(_ppVlaue);
		}
	}

private:
	SqliteManager* _dbMag;
	char ** _ppVlaue;
};