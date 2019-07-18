#include "ScanManager.h"
#include "DataManger.h"
void ScanManager::Scan(const std::string& path){
	//通过我们path路径来取得目录下面的数据和数据库中的数据
	std::set<std::string>localdata;
	std::vector<std::string>localdirs;
	std::vector<std::string>localfiles;
	//得到本地的文件和目录
	DirectoryList(path, localfiles, localdirs);
	localdata.insert(localdirs.begin(), localdirs.end());
	localdata.insert(localfiles.begin(), localfiles.end());

	//得到数据库中的目录和文件
	std::set<std::string> dbset;
	DataManager::GetInstannce()->GetDocs(path, dbset);
	auto localit = localdata.begin();
	auto dbit = dbset.begin();

	while (localit != localdata.end() && dbit != dbset.end()){
		//1. 数据库和本地都有的话就直接跳过
		if (*localit == *dbit){
			localit++;
			dbit++;
		}
		//2. 数据库中但是本地有就是新增的
		else if (*localit < *dbit){
			DataManager::GetInstannce()->InsertDocs(path, *localit);
			localit++;
		}
		//数据库中有但是本地没有就是删除
		else if (*localit > *dbit){
			DataManager::GetInstannce()->DeleteDoc(path, *dbit);
			dbit++;
		}
	}
	//如果没有比较完就直接进行添加或者删除
	while (localit != localdata.end()){
		//新增数据
		DataManager::GetInstannce()->InsertDocs(path, *localit);
		localit++;
	}
	while (dbit != dbset.end()){
		DataManager::GetInstannce()->DeleteDoc(path, *dbit);
		dbit++;
	}
	//遍历子目录进行数据的导入
	for (const auto& dirs : localdirs){
		std::string subpath = path;
		subpath += "\\";
		subpath += dirs;
		Scan(subpath);
	}
}


void ScanManager::ScanNotRecursion(const std::string& path){
	//通过我们path路径来取得目录下面的数据和数据库中的数据
	std::set<std::string>localdata;
	std::vector<std::string>localdirs;
	std::vector<std::string>localfiles;
	//得到本地的文件和目录
	DirectoryList(path, localfiles, localdirs);
	localdata.insert(localdirs.begin(), localdirs.end());
	localdata.insert(localfiles.begin(), localfiles.end());

	//得到数据库中的目录和文件
	std::set<std::string> dbset;
	DataManager::GetInstannce()->GetDocs(path, dbset);
	auto localit = localdata.begin();
	auto dbit = dbset.begin();

	while (localit != localdata.end() && dbit != dbset.end()){
		//1. 数据库和本地都有的话就直接跳过
		if (*localit == *dbit){
			localit++;
			dbit++;
		}
		//2. 数据库中但是本地有就是新增的
		else if (*localit < *dbit){
			DataManager::GetInstannce()->InsertDocs(path, *localit);
			localit++;
		}
		//数据库中有但是本地没有就是删除
		else if (*localit > *dbit){
			DataManager::GetInstannce()->DeleteDoc(path, *dbit);
			dbit++;
		}
	}
	//如果没有比较完就直接进行添加或者删除
	while (localit != localdata.end()){
		//新增数据
		DataManager::GetInstannce()->InsertDocs(path, *localit);
		localit++;
	}
	while (dbit != dbset.end()){
		DataManager::GetInstannce()->DeleteDoc(path, *dbit);
		dbit++;
	}
}
