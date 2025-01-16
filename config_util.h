#ifndef __CONFIG_UTIL_H__
#define __CONFIG_UTIL_H__

#include <Preferences.h>

namespace CONFIG_UTIL
{
	typedef enum
	{
		CONFIG_PARSE_ERROR = -2,
		CONFIG_PARSE_NODATA = -1,
		CONFIG_PARSE_OK = 0,
	} config_status;

	class ConfigUtil;

	typedef void (*ConfigUtilInitFunc)(ConfigUtil* pConfigUtil);

	class ConfigUtil
	{
	public:
		ConfigUtil(/* args */);
		~ ConfigUtil(){}

		ConfigUtil& Init(const char* namespace_, ConfigUtilInitFunc = nullptr);

		bool SetConfig(const char* key, const char* value);

		template<typename T>
		T GetConfig(const char* key);

		config_status LoopConfigUart(int c, char result[][32]);
	private:
		/* data */
		Preferences m_oPreferences;
		char m_szLine[512];
		int m_dwIndex;
	};
	template <typename T>
	inline T ConfigUtil::GetConfig(const char *key)
	{
		return T();
	}

	template<>
	inline int ConfigUtil::GetConfig<int>(const char *key)
	{
		return this->m_oPreferences.getString(key, "0").toInt();
	}

	template<>
	inline float ConfigUtil::GetConfig<float>(const char *key)
	{
		return this->m_oPreferences.getString(key, "0.0").toFloat();
	}

	template<>
	inline bool ConfigUtil::GetConfig<bool>(const char *key)
	{
		return this->m_oPreferences.getBool(key);
	}

	template<>
	inline const char* ConfigUtil::GetConfig<const char*>(const char *key)
	{
		return this->m_oPreferences.getString(key).c_str();
	}

	template<>
	inline String ConfigUtil::GetConfig<String>(const char *key)
	{
		return this->m_oPreferences.getString(key);
	}


}; // namespace CONFIG_UTIL

#endif // __CONFIG_UTIL_H__	