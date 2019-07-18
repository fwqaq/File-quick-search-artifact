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
	char file_name[MAX_PATH] = {'\0'}; //�����ļ���;
	char file_rename[MAX_PATH] = {'\0'}; //�����ļ��������������;
	char notify[1024] = { '\0' };
	int count = 0; //�ļ�����������ͬʱ������ɾ������ļ������Խ��и��Ѻõ���ʾ;
	TCHAR *dir = _T("D:");
	std::string s = "D:";
	//HANDLE����һ�����
	HANDLE dirHandle = CreateFile(dir,
		GENERIC_READ | GENERIC_WRITE | FILE_LIST_DIRECTORY,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (dirHandle == INVALID_HANDLE_VALUE){ //�������ض����Ŀ���ļ�ϵͳ��֧�ָò���������ʧ�ܣ�ͬʱ����GetLastError()����ERROR_INVALID_FUNCTION
		cout << "error" + GetLastError() << endl;
	}

	//FILE_NOTIFY_INFORMATION��һ���ṹ�壬����������,������ǿתΪ�ṹ��ָ��
	memset(notify, 0, strlen(notify));
	FILE_NOTIFY_INFORMATION *pnotify = (FILE_NOTIFY_INFORMATION*)notify;

//	cout << "Start Monitor..." << endl;
	
	while (true){
		if (ReadDirectoryChangesW(dirHandle, &notify, 1024, true,
			FILE_NOTIFY_CHANGE_FILE_NAME | FILE_NOTIFY_CHANGE_DIR_NAME | FILE_NOTIFY_CHANGE_SIZE,
			&cbBytes, NULL, NULL))
		{			
			pnotify = (FILE_NOTIFY_INFORMATION*)notify;
			//ת���ļ���Ϊ���ֽ��ַ���;
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
				//�����������Щ�¼��Ļ���Ҫ����ɨ�赱ǰ��Ŀ¼�ļ�
				//����Ҫ��ȡ��ǰ��Ŀ¼����
				std::string dirpath = s + "\\" + file_name;
				get_dir_path(dirpath);
			//	cout << dirpath << endl;
				//������һ���Ŀ¼
				ScanManager::CreateIntance()->ScanNotRecursion(dirpath);
				//cout << "ɨ��ģ��ĵ�ַmonitor" << ScanManager::CreateIntance() << endl;
				//cout << "DirPath:" << dirpath << endl;
			}
		}
	}
	CloseHandle(dirHandle);
}