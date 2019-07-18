#pragma once

//高亮显示关键字
static void ColourPrintf(const std::string str) {
	// 0-黑 1-蓝 2-绿 3-浅绿 4-红 5-紫 6-黄 7-白 8-灰 9-淡蓝 10-淡绿    
	// 11-淡浅绿  12-淡红 13-淡紫 14-淡黄 15-亮白     
	//颜色：前景色 + 背景色*0x10    
	//例如：字是红色，背景色是白色，即 红色 + 亮白 =  4 + 15*0x10 

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
	//第一种就是key直接可以匹配doc_name
	int pos = doc_name.find(key);
	if (pos != -1){
		//就是找到了
		v[0] = doc_name.substr(0, pos);
		v[1] = doc_name.substr(pos, key.size());
		v[2] = doc_name.substr(pos + key.size(), -1);
		return;
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//第二种是首字母转换,首字母进行对比
	//首先仍然是将我们的关键字转换为首字母，然后将doc_name转换为首字母的，然后对比查找
	std::string key_ini = GetFirstLetter(key.c_str());
	std::string doc_ini = GetFirstLetter(doc_name.c_str());
	//和关键字和doc_name的首字母做比较
	pos = doc_ini.find(key_ini);
	if (pos != -1){
		//找到了
		//在pos前面都不包含关键字，从
		//将汉字一个一个转换为全拼，看长度是否
		int num = 0;
		int pre_i = 0;
		for (int i = 0; i < doc_name.size();){
			if (num == pos){
				//就是找到了前面所有的个数
				v[0] = doc_name.substr(0, i);
				pre_i = i;
				break;
			}
			if (doc_name[i] < 0){
				//汉字，
				char ch[3] = { '\0' };
				ch[0] = doc_name[i];
				ch[1] = doc_name[i + 1];
				i += 2;
				num += GetFirstLetter(ch).size();
			}
			else{
				//不是汉字
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
				//就是汉字
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
				//到最后了
				v[1] = doc_name.substr(pre_i, -1);
			}
		}

	}

	////////////////////////////////////////////////////////////////////////////
	//第三种就是key的全拼能匹配到doc_name
	std::string name_pinyin;
	ChineseConvertPy(doc_name, name_pinyin);
	//应该把key也专为全拼，这样可以查询汉字加拼音的方式查询
	std::string key_piny;
	ChineseConvertPy(key, key_piny);
	pos = name_pinyin.find(key_piny);
	if (pos != -1){
		//在pos前面都不包含关键字，从
		//将汉字一个一个转换为全拼，看长度是否
		int num = 0;
		int pre_i;
		for (int i = 0; i < doc_name.size(); ){
			if (num == pos){
				//就是找到了前面所有的个数
				v[0] = doc_name.substr(0, i);
				pre_i = i;
				break;
			}
			if (doc_name[i] <0){
				//汉字，
				char ch[3] = { '\0' };
				ch[0] = doc_name[i];
				ch[1] = doc_name[i + 1];
				i += 2;
				std::string s;
				ChineseConvertPy(ch, s);
				num += s.size();
			}
			else{
				//不是汉字
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
				//就是汉字
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
				//到最后了
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