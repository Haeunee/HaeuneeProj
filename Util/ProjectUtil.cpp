#include "ProjectUtil.h"

FileUtil::FileUtil(wchar_t* _fileName, const MODE _mode)
{
	switch (_mode)
	{
	case MODE::READ: { m_FileRead.open(_fileName); } break;
	case MODE::WRITE: { m_FileWrite.open(_fileName, std::ios::app); } break;
	default: {}	break;
	};
}

FileUtil::~FileUtil()
{
	if (true == m_FileRead.is_open())
		m_FileRead.close();
	if (true == m_FileWrite.is_open())
		m_FileWrite.close();
}

bool FileUtil::ReadLine(wchar_t* _outStr, size_t _maxCnt)
{
	if (false == m_FileRead.is_open())
		return false;
	if (!m_FileRead)
		return false;

	m_FileRead.getline(_outStr, _maxCnt);
	return true;
}

bool FileUtil::WriteLine(const wchar_t* _writeStr, size_t _maxCnt)
{
	if (false == m_FileWrite.is_open())
		return false;
	if (!m_FileWrite)
		return false;

	m_FileWrite << _writeStr;
	return true;
}

void TimeUtil::SetNowDateTime()
{
	std::chrono::system_clock::time_point timePoint{ std::chrono::system_clock::now() };
	m_TimeInt = std::chrono::system_clock::to_time_t(timePoint);
	tm time{};
	localtime_s(&time, &m_TimeInt);

	m_Time = this->SetTmtoDateTimeNuw(time);
}

void TimeUtil::AddTimeSec(uint64_t _sec)
{
	m_TimeInt += _sec;
	tm time{};
	localtime_s(&time, &m_TimeInt);

	m_Time = this->SetTmtoDateTimeNuw(time);
}

uint64_t TimeUtil::SetTmtoDateTimeNuw(tm _time)
{
	uint64_t dateTime{};

	dateTime = (static_cast<uint64_t>(_time.tm_year) + 1900)* MS_FIX::YEAR;
	dateTime += (static_cast<uint64_t>(_time.tm_mon) + 1)* MS_FIX::MONTH;
	dateTime += _time.tm_mday * MS_FIX::DAY;
	dateTime += _time.tm_hour * MS_FIX::HOUR;
	dateTime += _time.tm_min * MS_FIX::MINUTE;
	dateTime += _time.tm_sec * MS_FIX::SECOND;

	return dateTime;
}

RandomUtil::RandomUtil()
{
	std::random_device randDev{};

	// 난수 생성 엔진을 초기화 함
	std::mt19937_64 makeRand(randDev());
	m_RandEngine		= makeRand;
}

uint64_t RandomUtil::GetRandNum(uint64_t _min, uint64_t _max)
{
	std::uniform_int_distribution<uint64_t> dis(_min, _max);
	return dis(m_RandEngine);
}
