#include "DataManger.h"

void DataManager::Search(const std::string& key, std::vector<std::pair<std::string, std::string>>&doc_paths){
	//使用关键字得到数据，使用一个pair对组来存储数据，第一个是name，第二个是path
	char ch[256];
	std::string pinyin;
	ChineseConvertPy(key,pinyin);
	std::string InitPy;
	InitPy = GetFirstLetter(key.c_str());
	memset(ch, 0, sizeof(ch));
	if (key.size() == 1){
		sprintf(ch, "select doc_name,doc_path from Doc_Tab where \
													doc_name_init like '%%%s%%';", InitPy.c_str());
	}
	else{
		sprintf(ch, "select doc_name,doc_path from Doc_Tab where doc_name_py  like '%%%s%%' or \
													doc_name_init like '%%%s%%';", pinyin.c_str(), InitPy.c_str());
	}
	//使用AutoGetTable来得到数据
	std::string sql(ch);
	int row;
	int col;
	char ** ppRet;
	//得到数据
	AutoGetTable agt(&_sqma, sql, row, col, ppRet);
	for (int i = 1; i <= row; i++){
		//i*2+0和i*2+1 这两个位置就是我们需要的位置
		std::pair<std::string, std::string> p;
		p.first = ppRet[i * 2];
		p.second = ppRet[i * 2 + 1];
		doc_paths.push_back(p);
	}
}

void DataManager::GetDocs(std::string path, std::set<std::string>& dbset){
	//这里是得到所有的文件名字和目录名字
	char ch[1024];
	memset(ch, 0, sizeof(ch));
	sprintf(ch, "select doc_name from Doc_Tab where doc_path='%s';", path.c_str());
	std::string sql(ch);
	int row;
	int col;
	char** ppRet;
	AutoGetTable s(&_sqma, sql, row, col, ppRet);
	//得到结果ppRet
	for (int i = 1; i <= row; i++){
		dbset.insert(ppRet[i]);
	}
}

void DataManager::InsertDocs(const std::string path, const std::string doc){
	//将路径和doc插入到我们的数据库中
	char ch[4096];
	memset(ch, 0, sizeof(ch));
	std::string piny;
	int ret = ChineseConvertPy(doc, piny);
	if (ret != 0){
		ERROE_LOG("ChineseConvertPY error!!!");
	}
	std::string name_init;
	name_init = GetFirstLetter(doc.c_str());
	const std::string s;
	sprintf(ch, "insert into Doc_Tab (doc_name,doc_path,doc_name_py,doc_name_init) values \
								 ('%s','%s','%s','%s');", doc.c_str(), path.c_str(), piny.c_str(), name_init.c_str());
	std::string sql(ch);
	_sqma.ExecuteSql(sql);
}

void DataManager::DeleteDoc(const std::string& path,const std::string doc){
	char ch[256];
	memset(ch, 0, sizeof(ch));
	//先删除这个文件，有可能是目录，如果是目录就要目录下面所有的东西
	sprintf(ch, "delete from Doc_Tab where doc_name='%s' and doc_path='%s';", doc.c_str(), path.c_str());
	std::string sql(ch);
	_sqma.ExecuteSql(sql);
	char ch1[256];
	memset(ch1, 0, sizeof(ch1));
	sprintf(ch1, "delete from Doc_Tab where doc_path like '%s%%';", (path + "\\" + doc).c_str());
	std::string sql1(ch1);
	_sqma.ExecuteSql(sql1);
}

void DataManager::Init(){
	_sqma.Open("doc.db");
	std::string Create_sql = "CREATE TABLE if not exists Doc_Tab( \
		ID INTEGER PRIMARY KEY AUTOINCREMENT,\
		doc_name	  TEXT    NOT NULL, \
		doc_path	  INT     NOT NULL, \
		doc_name_py       text, \
		doc_name_init    text );";
	_sqma.ExecuteSql(Create_sql);
}