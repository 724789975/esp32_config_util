#include "config_util.h"

namespace CONFIG_UTIL
{
	ConfigUtil::ConfigUtil()
		: m_dwIndex(0)
	{ }

	ConfigUtil &ConfigUtil::Init(const char* namespace_, ConfigUtilInitFunc pf)
	{
		this->m_oPreferences.begin(namespace_, false);
		if (pf)
		{
			pf(this);
		}
		return *this;
	}

	bool ConfigUtil::SetConfig(const char *key, const char *value)
	{
		return this->m_oPreferences.putString(key, value);
	}


	/**
	 * @brief
	 * @param line
	 * @param result  1 正常分割，-1 错误分割
	 * @return config_status
	 */
	config_status split_str(const char *line, char result[][32])
	{
		if (line[0] != '$')
			return CONFIG_PARSE_ERROR;
		uint16_t index = 0;
		uint16_t count = 0;
		uint16_t temp_index = 0;
		for (index = 1; line[index] != '\0'; index++)
		{
			if (line[index] == '=')
			{
				result[count++][temp_index++] = '\0';
				temp_index = 0;
				continue;
			}
			result[count][temp_index++] = line[index];
		}
		result[count][temp_index++] = '\0';

		if (count != 1)
		{
			return CONFIG_PARSE_ERROR;
		}
		return CONFIG_PARSE_OK;
	}

	config_status ConfigUtil::LoopConfigUart(int c, char result[][32])
	{
		if ('\n' == c)
		{
			m_szLine[m_dwIndex] = '\0';
			this->m_dwIndex = 0;
			return split_str(m_szLine, result);
		}
		else if (0 < c && 127 > c)
		{
			m_szLine[m_dwIndex++] = c;
		}
		return config_status::CONFIG_PARSE_NODATA;
	}
}; // namespace CONFIG_UTIL
