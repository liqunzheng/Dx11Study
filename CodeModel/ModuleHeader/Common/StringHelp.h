/*************************************************
// ����: ��Ⱥ��  2014/12/13
// ����: �����ַ��������Ĺ�������
//*************************************************/
#pragma once
#include "CommonExport.h"
#include <vector>
#include <string>

//�ַ����ָ��
LEE_COMMON_API std::vector<std::string> split(std::string str, std::string pattern);

//�ַ���תint 
LEE_COMMON_API int str2int(const std::string& str);

//�ַ���תfloat
LEE_COMMON_API float str2float(const std::string& str);


