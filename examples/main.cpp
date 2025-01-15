#include <Arduino.h>
#include "config_util.h"

void init_config_util(CONFIG_UTIL::ConfigUtil* pConfigUtil)
{
	if (!pConfigUtil->GetConfig<int>("inited"))
	{
		pConfigUtil->SetConfig("inited", "1");
	}
	
}

void setup() {
  // put your setup code here, to run once:
  CONFIG_UTIL::ConfigUtil config_util;
  config_util.Init("test", init_config_util);
}

void loop() {
  // put your main code here, to run repeatedly:
  printf("Hello, world!\n");
  delay(1000);
}
