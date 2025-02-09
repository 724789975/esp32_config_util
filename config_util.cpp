#include "config_util.h"

namespace CONFIG_UTIL
{
	ConfigUtil::ConfigUtil()
		: m_dwIndex(0)
	{ }

	ConfigUtil &ConfigUtil::Init(const char* szNamespace, ConfigUtilInitFunc pf)
	{
		this->m_oPreferences.begin(szNamespace, false);
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

	void ConfigUtil::DealCommand(DealCommandFunc pFunc)
	{
		char pszResult[10][32];
		int wConfigResult; // 用于存储解析配置数据的结果
		// 不断读取串口数据，直到串口中没有数据可读
		while (Serial.available())
		{
			int c = Serial.read();
			// 调用config对象的loop_config_uart函数，该函数用于解析读取到的配置数据，并将解析结果存储在result数组中
			// config_result表示解析结果的状态
			wConfigResult = this->LoopConfigUart(c, pszResult);
			if (wConfigResult == config_status::CONFIG_PARSE_OK)
			{
				if (strcmp(pszResult[0], "command") == 0)
			{
					// 调用deal_command函数处理解析结果。
					pFunc(pszResult[0], pszResult[1]);
					return;
				}
				this->SetConfig(pszResult[0], pszResult[1]);
				Serial.printf("$%s=%s ok\n", pszResult[0], pszResult[1]);
			}
			// 调用deal_command函数处理解析结果。
			else if (wConfigResult == config_status::CONFIG_PARSE_ERROR)
			{
				Serial.print("$result=error parse\n");
			}
		}
	}

	/**
	 * @brief
	 * @param line
	 * @param result  1 正常分割，-1 错误分割
	 * @return config_status
	 */
	config_status split_str(const char *szLine, char pszResult[][32])
	{
		if (szLine[0] != '$')
			return config_status::CONFIG_PARSE_ERROR;
		uint16_t wIndex = 0;
		uint16_t wCount = 0;
		uint16_t wTempIndex = 0;
		for (wIndex = 1; szLine[wIndex] != '\0'; wIndex++)
		{
			if (szLine[wIndex] == '=')
			{
				pszResult[wCount++][wTempIndex++] = '\0';
				wTempIndex = 0;
				continue;
			}
			pszResult[wCount][wTempIndex++] = szLine[wIndex];
		}
		pszResult[wCount][wTempIndex++] = '\0';

		if (wCount != 1)
		{
			return config_status::CONFIG_PARSE_ERROR;
		}
		return config_status::CONFIG_PARSE_OK;
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
