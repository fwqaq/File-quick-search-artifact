#include "DataManger.h"

void DataManager::Search(const std::string& key, std::vector<std::pair<std::string, std::string>>&doc_paths){
	//ʹ�ùؼ��ֵõ����ݣ�ʹ��һ��pair�������洢���ݣ���һ����name���ڶ�����path
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
	//ʹ��AutoGetTable���õ�����
	std::string sql(ch);
	int row;
	int col;
	char ** ppRet;
	//�õ�����
	AutoGetTable agt(&_sqma, sql, row, col, ppRet);
	for (int i = 1; i <= row; i++){
		//i*2+0��i*2+1 ������λ�þ���������Ҫ��λ��
		std::pair<std::string, std::string> p;
		p.first = ppRet[i * 2];
		p.second = ppRet[i * 2 + 1];
		doc_paths.push_back(p);
	}
}

void DataManager::GetDocs(std::string path, std::set<std::string>& dbset){
	//�����ǵõ����е��ļ����ֺ�Ŀ¼����
	char ch[1024];
	memset(ch, 0, sizeof(ch));
	sprintf(ch, "select doc_name from Doc_Tab where doc_path='%s';", path.c_str());
	std::string sql(ch);
	int row;
	int col;
	char** ppRet;
	AutoGetTable s(&_sqma, sql, row, col, ppRet);
	//�õ����ppRet
	for (int i = 1; i <= row; i++){
		dbset.insert(ppRet[i]);
	}
}

void DataManager::InsertDocs(const std::string path, const std::string doc){
	//��·����doc���뵽���ǵ����ݿ���
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
	//��ɾ������ļ����п�����Ŀ¼�������Ŀ¼��ҪĿ¼�������еĶ���
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