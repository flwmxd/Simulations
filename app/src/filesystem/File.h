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

#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <memory>
#include <functional>
#include <filesystem>

namespace filesystem
{
	class File
	{
	public:
		File();
		explicit File(const std::string& file,bool write = false);
		~File();
		auto exists() -> bool;
		auto getHash(uint64_t seed) -> std::string;
		auto getMd5() -> std::string;
		auto isDirectory() -> bool;
		auto getFileSize() { return fileSize; }
		auto getOffset() { return pos; }
		auto readBytes(int32_t size)->std::unique_ptr<int8_t[]>;
		auto list() -> const std::vector<std::string>;
		auto list(std::vector<std::string> & out) -> void;
		auto cache(const std::vector<uint8_t> & buffer) -> void;
        auto getBuffer() -> std::unique_ptr<int8_t[]>;
		auto write(const std::string& file) -> void;
		static auto getFileName(const std::string& file) -> const std::string;
		static auto fileExists(const std::string& file) -> bool;
		static auto removeExtension(const std::string& file) -> std::string;

#ifndef __ANDROID__
		static auto listFiles(const std::string& path, std::vector<std::filesystem::path>& out, const std::function<bool(const std::filesystem::path &)>& filter = nullptr) -> void;
#endif
	private:
		std::string file;
		//std::ifstream stream;
		FILE* filePtr = nullptr;
		int64_t pos = 0;
		size_t fileSize;
	public:
		static auto listFolders(const std::string& path) -> std::vector<std::string>;
		static auto listFolders(const std::string& path, std::vector<std::string>& out) -> void;
	};
}; // namespace FileSystem
