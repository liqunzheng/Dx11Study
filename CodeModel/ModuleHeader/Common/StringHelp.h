/*************************************************
// 作者: 李群正  2014/12/13
// 描述: 关于字符串操作的公共函数
//*************************************************/
#pragma once
#include "CommonExport.h"
#include <vector>
#include <string>

//字符串分割函数
LEE_COMMON_API std::vector<std::string> split(std::string str, std::string pattern);

//字符串转int 
LEE_COMMON_API int str2int(const std::string& str);

//字符串转float
LEE_COMMON_API float str2float(const std::string& str);


