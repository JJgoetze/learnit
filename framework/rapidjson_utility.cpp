#include "framework/rapidjson_utility.h"
#include "framework/logger.h"
#include "framework/logging.h"

bool RapidjsonUtility::GetDoubleValue(rapidjson::Value& json_value, const char* name, double& value)
{
	if (!json_value.IsObject())	{
		return false;
	}
	rapidjson::Value::MemberIterator Iter = json_value.FindMember(name);
	if (Iter != json_value.MemberEnd() && (Iter->value.IsInt() || Iter->value.IsDouble())){
		value = Iter->value.GetDouble();
		return true;
	}
	return false;
}

bool RapidjsonUtility::GetStringValue(rapidjson::Value& json_value, const char* name, std::string& value)
{
	 if (!json_value.IsObject()) {
		 return false;
	 }
	 rapidjson::Value::MemberIterator Iter = json_value.FindMember(name);
	 if (Iter != json_value.MemberEnd() && Iter->value.IsString()) {
		 value = Iter->value.GetString();
		 return true;
	 }
	 return false;
}

bool RapidjsonUtility::GetIntValue(rapidjson::Value& json_value, const char* name, int32_t& value)
{
	 if (!json_value.IsObject()) {
		 return false;
	 }
	 rapidjson::Value::MemberIterator Iter = json_value.FindMember(name);
	 if (Iter != json_value.MemberEnd() && (Iter->value.IsInt() || Iter->value.IsUint())){
		 value = Iter->value.GetInt();
		 return true;
	 }
	 return false;
}

bool RapidjsonUtility::GetUintValue(rapidjson::Value& json_value, const char* name, uint32_t& value)
{
	 if (!json_value.IsObject()){
		 return false;
	 }
	 rapidjson::Value::MemberIterator Iter = json_value.FindMember(name);
	 if (Iter != json_value.MemberEnd() && Iter->value.IsInt()) {
		 value = Iter->value.GetUint();
		 return true;
	 }
	 return false;
}