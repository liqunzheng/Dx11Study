#include "stdafx.h"
#include "StringHelp.h"

std::vector<std::string> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<std::string> result;
	str += pattern;//扩展字符串以方便操作
	UINT size = str.size();

	for (UINT i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			std::string s = str.substr(i, pos - i);
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	return result;
}

int str2int(const std::string& str)
{
	return atoi(str.c_str());
}

float str2float(const std::string& str)
{
	return static_cast<float>(atof(str.c_str()));
}
