#pragma  once
#include <iomanip>
#include <tchar.h>
void get_dir_path(std::string& s){
	int pos = -1;
	for (int i = s.size() - 1; i >= 0; i--){
		if (s[i] == '\\'){
			pos = i;
			break;
		}
	}
	if (pos != -1){
		s = s.substr(0, pos);
	}
}

void fileWatcher()
{
	DWORD cbBytes;
	char file_name[MAX_PATH] = {'\0'}; //设置文件名;
	char file_rename[MAX_PATH] = {'\0'}; //设置文件重命名后的名字;
	char notify[1024] = { '\0' };
	int count = 0; //文件数量。可能同时拷贝、删除多个文件，可以进行更友好的提示;
	TCHAR *dir = _T("D:");
	std::string s = "D:";
	//HANDLE就是一个句柄
	HANDLE dirHandle = CreateFile(dir,
		GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (dirHandle == INVALID_HANDLE_VALUE){ //若网络重定向或目标文件系统不支持该操作，函数失败，同时调用GetLastError()返回ERROR_INVALID_FUNCTION
		cout << "error" + GetLastError() << endl;
	}

	//FILE_NOTIFY_INFORMATION是一个结构体，是柔型数组,将数组强转为结构体指针
	memset(notify, 0, strlen(notify));
	FILE_NOTIFY_INFORMATION *pnotify = (FILE_NOTIFY_INFORMATION*)notify;

//	cout << "Start Monitor..." << endl;
	
	while (true){
		if (ReadDirectoryChangesW(dirHandle, &notify, 1024, true,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE,
			&cbBytes, NULL, NULL))
		{			
			pnotify = (FILE_NOTIFY_INFORMATION*)notify;
			//转换文件名为多字节字符串;
			if (pnotify->FileName){
				memset(file_name, 0, strlen(file_name));
				int ret = WideCharToMultiByte(CP_ACP, 0, pnotify->FileName, pnotify->FileNameLength / 2, file_name, 99, NULL, NULL);
				if (ret == 0){
					GetLastError();
				}
			//	cout <<"pmotify->FileName:"<< pnotify->FileName << endl;
			
			}
			if ((pnotify->Action == FILE_ACTION_ADDED) | (pnotify->Action == FILE_ACTION_REMOVED) | (pnotify->Action == FILE_ACTION_RENAMED_OLD_NAME)){

			//	cout << "pnotify->FileName" << pnotify->FileName << endl;;
				//如果发生了这些事件的话需要重新扫描当前的目录文件
				//就需要获取当前的目录名称
				std::string dirpath = s + "\\" + file_name;
				get_dir_path(dirpath);
			//	cout << dirpath << endl;
				//返回上一层的目录
				ScanManager::CreateIntance()->ScanNotRecursion(dirpath);
				//cout << "扫描模块的地址monitor" << ScanManager::CreateIntance() << endl;
				//cout << "DirPath:" << dirpath << endl;
			}
		}
	}
	CloseHandle(dirHandle);
}