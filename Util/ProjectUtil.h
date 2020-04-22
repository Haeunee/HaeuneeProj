#pragma once
#include <Strsafe.h>  
#include <stdarg.h>
#include <fstream>
#include <chrono>
#include <array>
#include <random>

#include "stdafx.h"

template<class T, size_t _maxSize>
class StringUtil
{
public:
	StringUtil(const T* _str) { this->Format(_str); }

	const T* Format(const T* _str, ...)
	{
		T* data{ m_StrArr.data() };
		va_list args{};

		va_start(args, _str);
		StringCchVPrintf(data, _maxSize, _str, args);
		va_end(args);

		return data;
	}

	const T* Append(const T* _str)
	{
		size_t len{ strlen(m_StrArr.data()) };
		char* data{ &m_StrArr[len] };
		va_list args{};

		va_start(args, _str);
		StringCchVPrintf(data, _maxSize - len, _str, args);
		va_end(args);

		return data;
	}

	const T* Get() { return m_StrArr.data(); }

private:
	size_t _maxSize;
	std::array<char, 10> m_StrArr{};
	uint64_t m_nowIdx{};
};

template<class T>
class ObjectPool
{
public:
	ObjectPool(size_t _size)
	{
		for (size_t idx{}; idx < _size; ++idx)
		{
			T* tmp = new T;
			m_ObjList.push_back(tmp);
		}
		m_MaxSize = _size;
	}
	~ObjectPool()
	{
		for (auto& data : m_ObjList)
		{
			delete(data);
		}
		m_ObjList.clear();
		m_MaxSize = 0;
	}

	T* PopObject()
	{
		if (m_ObjList.empty())
		{
			AddObject();
		}
		T* tmp{ m_ObjList.front() };
		m_ObjList.pop_front();

		return tmp;
	}

	void ReturnObject(T* _data)
	{
		m_ObjList.push_back(_data);
	}

	void AddObject()
	{
		for (size_t idx{}; idx < m_MaxSize; ++idx)
		{
			T* tmp = new T;
			m_ObjList.push_back(tmp);
		}
		m_MaxSize += m_MaxSize;
	}

private:
	std::list<T*> m_ObjList{};
	size_t m_MaxSize{};
};

class FileUtil
{
	enum class MODE : uint8_t
	{
		READ,
		WRITE,
	};
public:
	FileUtil(wchar_t* _fileName, const MODE _mode);
	~FileUtil();

	bool ReadLine(wchar_t* _outStr, size_t _maxCnt);
	bool WriteLine(const wchar_t* _writeStr, size_t _maxCnt);

private:
	std::wifstream m_FileRead{};
	std::wofstream m_FileWrite{};
};

class TimeUtil
{
	typedef enum : uint64_t
	{
		YEAR = 10000000000LL,
		MONTH = 100000000LL,
		DAY = 1000000LL,
		HOUR = 10000LL,
		MINUTE = 100LL,
		SECOND	= 1LL,
	} MS_FIX;

public:
	TimeUtil() {};
	~TimeUtil() {};
	void SetNowDateTime();
	void AddTimeSec(uint64_t _sec);
	uint64_t GetDateTimeNum() { return m_Time; }

private:
	uint64_t m_Time{};
	__time64_t m_TimeInt{};

	uint64_t SetTmtoDateTimeNuw(tm _time);
};

class RandomUtil
{
public:
	RandomUtil();
	~RandomUtil() {};

	uint64_t GetRandNum(uint64_t _min, uint64_t _max);
	
private:
	std::mt19937_64 m_RandEngine{};
};

template<class T>
class SingletonUtil
{
protected:
	static T* singleton{};
	static T* GetInstance()
	{
		if (nullptr == singleton)
		{
			singleton = new T();
		}
		return singleton;
	}
	void RealeaseInstance()
	{
		if (nullptr == singleton)
			return;

		delete(singleton);
	}
};