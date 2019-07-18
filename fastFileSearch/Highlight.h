#pragma once

//������ʾ�ؼ���
static void ColourPrintf(const std::string str) {
	// 0-�� 1-�� 2-�� 3-ǳ�� 4-�� 5-�� 6-�� 7-�� 8-�� 9-���� 10-����    
	// 11-��ǳ��  12-���� 13-���� 14-���� 15-����     
	//��ɫ��ǰ��ɫ + ����ɫ*0x10    
	//���磺���Ǻ�ɫ������ɫ�ǰ�ɫ���� ��ɫ + ���� =  4 + 15*0x10 

	WORD color = 4 + 15 * 0x10;
	WORD colorOld;
	HANDLE handle = ::GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	GetConsoleScreenBufferInfo(handle, &csbi);
	colorOld = csbi.wAttributes;
	SetConsoleTextAttribute(handle, color);
	cout << str;
	SetConsoleTextAttribute(handle, colorOld);
}

void subString(const std::string& key, const std::string& doc_name,std::vector<std::string>& v){
	/////////////////////////////////////////////////////////////////////////////////
	//��һ�־���keyֱ�ӿ���ƥ��doc_name
	int pos = doc_name.find(key);
	if (pos != -1){
		//�����ҵ���
		v[0] = doc_name.substr(0, pos);
		v[1] = doc_name.substr(pos, key.size());
		v[2] = doc_name.substr(pos + key.size(), -1);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//�ڶ���������ĸת��,����ĸ���жԱ�
	//������Ȼ�ǽ����ǵĹؼ���ת��Ϊ����ĸ��Ȼ��doc_nameת��Ϊ����ĸ�ģ�Ȼ��ԱȲ���
	std::string key_ini = GetFirstLetter(key.c_str());
	std::string doc_ini = GetFirstLetter(doc_name.c_str());
	//�͹ؼ��ֺ�doc_name������ĸ���Ƚ�
	pos = doc_ini.find(key_ini);
	if (pos != -1){
		//�ҵ���
		//��posǰ�涼�������ؼ��֣���
		//������һ��һ��ת��Ϊȫƴ���������Ƿ�
		int num = 0;
		int pre_i = 0;
		for (int i = 0; i < doc_name.size();){
			if (num == pos){
				//�����ҵ���ǰ�����еĸ���
				v[0] = doc_name.substr(0, i);
				pre_i = i;
				break;
			}
			if (doc_name[i] < 0){
				//���֣�
				char ch[3] = { '\0' };
				ch[0] = doc_name[i];
				ch[1] = doc_name[i + 1];
				i += 2;
				num += GetFirstLetter(ch).size();
			}
			else{
				//���Ǻ���
				char ch[2] = { '\0' };
				ch[0] = doc_name[i];
				num += GetFirstLetter(ch).size();
				i++;
			}
		}
		num = 0;
		int bef_i = 0;
		for (int i = pre_i; i < doc_name.size();){
			if (num == key_ini.size()){
				bef_i = i;
				v[1] = doc_name.substr(pre_i, i - pre_i);
				v[2] = doc_name.substr(i, -1);
				return;
			}
			if (doc_name[i] < 0){
				//���Ǻ���
				char ch[3] = { '\0' };
				ch[0] = doc_name[i];
				ch[1] = doc_name[i + 1];
				i += 2;
				num += GetFirstLetter(ch).size();
			}
			else{
				char ch[2] = { '\0' };
				ch[0] = doc_name[i];
				i++;
				num += GetFirstLetter(ch).size();
			}
			if (pre_i + num >= doc_name.size()){
				//�������
				v[1] = doc_name.substr(pre_i, -1);
			}
		}

	}

	////////////////////////////////////////////////////////////////////////////
	//�����־���key��ȫƴ��ƥ�䵽doc_name
	std::string name_pinyin;
	ChineseConvertPy(doc_name, name_pinyin);
	//Ӧ�ð�keyҲרΪȫƴ���������Բ�ѯ���ּ�ƴ���ķ�ʽ��ѯ
	std::string key_piny;
	ChineseConvertPy(key, key_piny);
	pos = name_pinyin.find(key_piny);
	if (pos != -1){
		//��posǰ�涼�������ؼ��֣���
		//������һ��һ��ת��Ϊȫƴ���������Ƿ�
		int num = 0;
		int pre_i;
		for (int i = 0; i < doc_name.size(); ){
			if (num == pos){
				//�����ҵ���ǰ�����еĸ���
				v[0] = doc_name.substr(0, i);
				pre_i = i;
				break;
			}
			if (doc_name[i] <0){
				//���֣�
				char ch[3] = { '\0' };
				ch[0] = doc_name[i];
				ch[1] = doc_name[i + 1];
				i += 2;
				std::string s;
				ChineseConvertPy(ch, s);
				num += s.size();
			}
			else{
				//���Ǻ���
				char ch[2] = { '\0' };
				ch[0] = doc_name[i];
				std::string s;
				ChineseConvertPy(ch, s);
				num += s.size();
				i++;
			}
		}
		num = 0;
		int bef_i = 0;
		for (int i = pre_i; i < doc_name.size();){
			if (num == key_piny.size()){
				bef_i = i;
				v[1] = doc_name.substr(pre_i, i - pre_i);
				v[2] = doc_name.substr(i, -1);
				break;
			}
			if (doc_name[i] <0){
				//���Ǻ���
				char ch[3] = { '\0' };
				ch[0] = doc_name[i];
				ch[1] = doc_name[i + 1];
				i += 2;
				std::string s;
				ChineseConvertPy(ch, s);
				num += s.size();
			}
			else{
				char ch[2] = { '\0' };
				ch[0] = doc_name[i];
				i++;
				std::string s;
				ChineseConvertPy(ch, s);
				num += s.size();
			}
			if (pre_i+num >= doc_name.size()){
				//�������
				v[1] = doc_name.substr(pre_i, -1);
			}
		}
		return;
	}
}

void Tolowercase(std::string &str){
	for (int i = 0; i < str.size(); ){
		if (str[i] < 0){
			i += 2;
		}
		else{
			if (str[i] >= 'A'&&str[i] <= 'Z'){
				str[i] += 32;
			}
			i++;
		}
	}
}