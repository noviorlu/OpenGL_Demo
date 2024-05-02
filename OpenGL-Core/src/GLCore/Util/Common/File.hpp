#pragma once

#include "GLCore/Core/Log.h"
#include <string>
#include <fstream>

namespace GLCore::Utils {
	class File {
	public:
		static std::string ReadFileAsString(const std::string& filepath)
		{
			std::string result;
			std::ifstream in(filepath, std::ios::in | std::ios::binary);
			if (in)
			{
				in.seekg(0, std::ios::end);
				result.resize((size_t)in.tellg());
				in.seekg(0, std::ios::beg);
				in.read(&result[0], result.size());
				in.close();
			}
			else
			{
				LOG_ERROR("Could not open file '{0}'", filepath);
			}

			return result;
		}
	};
}