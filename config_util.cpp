#include "config_util.h"

namespace CONFIG_UTIL
{
	ConfigUtil &ConfigUtil::Init(const char* namespace_, ConfigUtilInitFunc pf)
	{
		this->preferences.begin(namespace_, false);
		if (pf)
		{
			pf(this);
		}
		return *this;
	}

	bool ConfigUtil::SetConfig(const char *key, const char *value)
	{
		return this->preferences.putString(key, value);
	}
}; // namespace CONFIG_UTIL
