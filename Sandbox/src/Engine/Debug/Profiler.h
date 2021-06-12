#pragma once
#include "xpch.h"
#include <fstream>
#include <chrono>
#include <time.h>
#include <thread>
#include <iomanip>

template<size_t LENGTH>
struct staticString
{
	char data[LENGTH];


};



template<size_t LENGTH>
constexpr auto Equals(const char(&base)[LENGTH], size_t sconst, char(&str)[LENGTH])
{
	for (int i=0; i < LENGTH; i++)
	{
		if (base[i] != str[i])
			return false;
	}
	return true;
}

//source must constain clean
template <size_t SourceSize, size_t CleanSize>
constexpr auto cleanString(const char(&source)[SourceSize], const char(&clean)[CleanSize])
{
	//character right after is always removed for some reason
	staticString<SourceSize - CleanSize> temp = {};
	
	size_t sourceIndex = 0;
	size_t cleanIndex = 0;

	size_t tempIndex = 0;
	
	//go till the first space
	int u = 0;
	for (;u < SourceSize; u++)
	{
		if (source[u] == ' ')
		{
			u++;
			break;
		}
	}
	for (int i = u; i < SourceSize; i++)
	{
		if (source[i] == '(')
		{
			break;
		}
		if (source[i] != clean[0])
		{
			temp.data[tempIndex] = source[i];
			tempIndex++;
			continue;
		}
		else
		{
			//check to see if the clean string can actualy fit in
			if (i + CleanSize <= SourceSize)
			{
				bool notMatch = false;
				for (int z = i; z < CleanSize; z++)
				{
					if (source[z] != clean[z - i])
					{
						//not a match
						temp.data[tempIndex] = source[i];
						tempIndex++;
						notMatch = true;
						break; //problem, this one should continue for the larger loop
					}
				}
				if (notMatch)
				{
					continue;
				}
				else
				{
					i += CleanSize - 1;
				}
			}
			else
			{
				temp.data[tempIndex] = source[i];
				tempIndex++;
				continue;
			}
		
			//by this time, the whole string was found
			//go just before the end of the string
		

		}
		

	}

	return temp;
}


class Profiler
{
public:

	Profiler(const char* name)
	{
		m_funcName = name;
		m_StartTime = std::chrono::steady_clock::now();

	}

	~Profiler()
	{
		auto startTime = std::chrono::duration_cast<std::chrono::microseconds>(m_StartTime.time_since_epoch());
		std::chrono::time_point<std::chrono::steady_clock> endTime = std::chrono::steady_clock::now();
		auto durationTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime.time_since_epoch()) - startTime;
		if(!stopped)
			Profiler::WriteProfile(m_funcName, startTime, durationTime, std::this_thread::get_id());
		stopped = true;
		
	}

	static void Init(const std::string& name)
	{
		time_t currentTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
			struct tm timeInfo = *localtime(&currentTime);

			
				std::stringstream ss;
				ss << "Profile {" << name << "} ";
				ss << "[" << timeInfo.tm_mon << "-" << timeInfo.tm_mday << "-" << timeInfo.tm_year - 100 << "] ";
				ss << "[" << timeInfo.tm_hour << ";" << timeInfo.tm_min << ";" << timeInfo.tm_sec << "] .json";

				

				s_fileStream.open(ss.str());
		
		
			
				s_fileStream << "[\n";
			
	}

	static void Close()
	{
		s_fileStream << " { \n } ]";
	}
	
	
	static void WriteProfile(const char* name, std::chrono::microseconds startTime, std::chrono::microseconds timeDuration, std::thread::id threadID)
	{
	/*	s_fileStream << name << "\n"
			<< startTime.count() << " " << timeDuration.count() << "\n" << threadID << "\n";
	*/
		s_fileStream << std::setprecision(3) << std::fixed;
		s_fileStream << " {\n\"cat\": \"xEngineProfiler\",\n"
			<< "\"tid\": " << threadID << ",\n"
			<< "\"ph\": \"X\",\n"
			<< "\"pid\": 0,\n"
			<< "\"ts\": " << startTime.count() << ",\n"
			<< "\"dur\": " << timeDuration.count() << ",\n"
			<< "\"name\": \"" << name << "\"\n },";
	}


private:
	//private member data
	std::chrono::time_point<std::chrono::steady_clock> m_StartTime;
	const char* m_funcName;
	bool stopped = false;


	static std::ofstream s_fileStream;
};



#ifdef PROFILE
//#define xENGINE_PROFILE_FUNCTION() Profiler::lol(cleanString(__FUNCSIG__,"__cdecl"))
#define xENGINE_PROFILE_FUNCTION() Profiler l_fun_profiler = Profiler(cleanString(__FUNCSIG__,"__cdecl").data);
#define INIT_PROFILER(name) Profiler::Init(name);
#define CLOSE_PROFILER() Profiler::Close();
#else
#define xENGINE_PROFILE_FUNCTION()
#define INIT_PROFILER(name)
#define CLOSE_PROFILER()
#endif




