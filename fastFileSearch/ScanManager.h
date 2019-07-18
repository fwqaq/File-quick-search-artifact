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
	//封装一个SqliteManager对象，用于我们数据的插入和删除
	ScanManager(){
	}//构造单例模式
	//需要的就是讲扫描出来的数据和数据库中的数据进行对别，新增的数据插入到数据库，删除的数据在数据库中删除。
};
//遍历给出目录下的文件和目录的集合
static void DirectoryList(const std::string& path, std::vector<std::string>&subfiles, \
	std::vector<std::string>&subdirs){
	_finddata_t file;//定义一个文件结构体
	//此时的路径是需要改变的，需要遍历该目录下面的，传递进来的只是到底此目录文件
	std::string _path = path + "\\*.*";//
	
	intptr_t handle = _findfirst(_path.c_str(), &file);
	if (handle == -1){
		ERROE_LOG("_findfirst:%s", _path.c_str());
	}
	do {
		// _A_SUBDIR（文件夹）就是目录，否则就是文件,name就是file的名字属性，是一个数组，长度是256。
		if ((file.attrib & _A_SUBDIR)&&!(file.attrib&_A_HIDDEN)){
			//目录的时候需要判断是不是.或者..，如果包含了这两个文件在查询的时候就会递归死循环
			if ((strcmp(file.name, ".") != 0) && (strcmp(file.name, "..") != 0)){
				subdirs.push_back(file.name);
			}
		}
		else{
			//此时就是文件了
			subfiles.push_back(file.name);
		}

	} while (_findnext(handle, &file) == 0);

	_findclose(handle);
}