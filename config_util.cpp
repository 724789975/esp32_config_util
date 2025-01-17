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
	 * @brief 处理串口数据，并调用deal_command函数处理解析结果。
	 * 只会处理command命令，配置设置直接执行，不会抛到deal_command函数中。
	 * 
	 * @param pFunc 
	 */
	void ConfigUtil::DealCommand(DealCommandFunc pFunc)
	{
		char result[10][32];
		int config_result; // 用于存储解析配置数据的结果
		// 不断读取串口数据，直到串口中没有数据可读
		while (Serial.available())
		{
			int c = Serial.read();
			// 调用config对象的loop_config_uart函数，该函数用于解析读取到的配置数据，并将解析结果存储在result数组中
			// config_result表示解析结果的状态
			config_result = this->LoopConfigUart(c, result);
			if (config_result == config_status::CONFIG_PARSE_OK)
			{
				if (strcmp(result[0], "command") == 0)
				{
					// 调用deal_command函数处理解析结果。
					pFunc(result[0], result[1]);
					return;
				}
				this->SetConfig(result[0], result[1]);
				Serial.printf("$%s=%s ok\n", result[0], result[1]);
			}
			// 调用deal_command函数处理解析结果。
			else if (config_result == config_status::CONFIG_PARSE_ERROR)
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
	config_status split_str(const char *line, char result[][32])
	{
		if (line[0] != '$')
			return config_status::CONFIG_PARSE_ERROR;
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
