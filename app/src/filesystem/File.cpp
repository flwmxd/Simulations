//////////////////////////////////////////////////////////////////////////////
// This file is part of the PharaohStroy MMORPG client                      //
// Copyright ?2020-2022 Prime Zeng                                          //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////

#include "File.h"
#include <sys/stat.h>
#include <exception>
#include <cstdio>
#include <cstring>
#include <memory>
#include <filesystem>
#ifdef __ANDROID__
#include <dirent.h>
#endif // __ANDROID__
namespace filesystem
{
	// 128 MB.
	const size_t CHUNK_SIZE = 134217728;

	File::~File()
	{
		if (filePtr != nullptr) {
			fclose(filePtr);
		}
		//stream.close();
	}

	File::File() : fileSize(0)
	{
	}

	File::File(const std::string& file,bool write) : file(file)
	{
		
		if(!write)
		{
			filePtr = fopen(file.c_str(), "rb");
			if(filePtr!=nullptr)
			{
				fseek(filePtr, 0, SEEK_END);
				fileSize = ftell(filePtr);
				fseek(filePtr, 0, SEEK_SET);
			}
		}
		else
		{
			filePtr = fopen(file.c_str(), "wb+");
		}
	
	}
    
    
    auto File::getBuffer() -> std::unique_ptr<int8_t[]>
    {
      
 /*       std::unique_ptr<int8_t[]> array(new int8_t[fileSize]);
        
        int count = 0;
        //FILE * fd = fopen(file.c_str(), "rb");
        fread (array.get(), sizeof (int8_t) * fileSize, 1, filePtr);
        */
        return readBytes(fileSize);
    }

	auto File::write(const std::string& file) -> void
	{
		fwrite(file.c_str(), 1, file.length() + 1, filePtr);
	}

	auto File::getHash(uint64_t seed) -> std::string
	{
		return "";
	}

	auto File::getMd5() -> std::string
	{
		return "";
	}

	auto File::exists() -> bool
	{
		return fileExists(file);
	}

	auto File::isDirectory() -> bool
	{
		struct stat fileInfo;
		auto result = stat(file.c_str(), &fileInfo);
		if (result != 0)
		{
			throw std::logic_error("file not exits");
		}
		return (fileInfo.st_mode & S_IFDIR) == S_IFDIR;
	}


	auto File::readBytes(int32_t size) -> std::unique_ptr<int8_t[]>
	{
		std::unique_ptr<int8_t[]> array(new int8_t[size]);
		memset(array.get(), 0, size);
		fread(array.get(), sizeof(int8_t) * size, 1, filePtr);
		pos += sizeof(int8_t) * size;
		return array;
	}

	auto File::removeExtension(const std::string& file) -> std::string
	{
        if(file == ""){
            return "";
        }
		std::string ret = file;
		ret = ret.erase(ret.find_last_of('.'));
		return ret;
	}
#ifndef __ANDROID__
	auto File::listFiles(const std::string & path, std::vector<std::filesystem::path>& out, const std::function<bool(const std::filesystem::path &)>& filter) -> void
	{
		std::filesystem::path str(path);
		if (!std::filesystem::exists(str))
		{
			return;
		}

		std::filesystem::directory_entry entry(str);//文件入口
		if (entry.status().type() == std::filesystem::file_type::directory)
		{
			std::filesystem::directory_iterator list(str);
			for (auto & it : list) {

				if (filter == nullptr || (filter != nullptr && filter(it.path().string())))
				{
					out.emplace_back(
						it.path()
					);
				}
			}
		}

		std::sort(out.begin(), out.end(), [](auto & left, auto & right ) {
			std::filesystem::directory_entry entry(left);//文件入口
			std::filesystem::directory_entry entry2(right);//文件入口
			
			return entry.status().type() > entry2.status().type();
		});

	}
#endif
	auto File::cache(const std::vector<uint8_t>& buffer) -> void
	{
		if(fileSize != buffer.size())
		{
			fwrite(buffer.data(),sizeof(uint8_t) * buffer.size(),1,filePtr);
		}
	}

	auto File::fileExists(const std::string& file) -> bool
	{
		struct stat fileInfo;
		return (!stat(file.c_str(), &fileInfo)) != 0;
	}

	auto File::list() -> const std::vector<std::string> {
		std::vector<std::string> files;
		list(files);
		return files;
	}

	auto File::list(std::vector<std::string> &out) -> void {
		if (file.empty()) {
			return ;
		}
#ifdef __ANDROID__
		DIR *dir = opendir(file.c_str());
		// check is dir ?
		if (nullptr == dir) {
			return ;
		}
		struct dirent *file;
		// read all the files in dir
		while ((file = readdir(dir)) != NULL) {
			// skip "." and ".."
			if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..") == 0) {
				continue;
			}
			if (file->d_type == DT_DIR) {
				//std::string filePath = file + "/" + file->d_name;
				//showAllFiles(filePath); // 递归执行
			} else {
				out.emplace_back(StringUtils::format("%s/%s",this->file.c_str(),file->d_name));
			}
		}
		closedir(dir);
#else



#endif
	}

    auto File::getFileName(const std::string& file) -> const std::string {
		auto pos = file.find_last_of('/');
		return file.substr(pos+1);
	}

	auto File::listFolders(const std::string& path) -> std::vector<std::string>
	{
		std::vector<std::string> ret;
		listFolders(path, ret);
		return ret;
	}

	auto File::listFolders(const std::string& path, std::vector<std::string>& out) -> void
	{
#ifndef __ANDROID__
		std::filesystem::path str(path);
		if (!std::filesystem::exists(str))
		{
			return;
		}

		std::filesystem::directory_entry entry(str);//文件入口
		if (entry.status().type() == std::filesystem::file_type::directory)
		{
			out.emplace_back(path);
			std::filesystem::directory_iterator list(str);
			for (auto& it : list)
			{
				if (it.status().type() == std::filesystem::file_type::directory)
				{
					listFolders(it.path().string(), out);
				}
			}
		}
#endif
	}

}; // namespace FileSystem
