/**
 * 作者: Circle_
 * B站: https://b23.tv/EzPeUhU
*/


#include "XunFeiLibrary.h"

#include "GenericPlatform/GenericPlatformHttp.h"
#include "Helper/Helper.h"

FString UXunFeiLibrary::Assemble_Auth_Url(const FString& HostUrl, const FString& ApiSecret, const FString& ApiKey)
{
	/*Prepare the string to be encrypted*/
	const FString Host = FGenericPlatformHttp::GetUrlDomain(HostUrl);
	const FString Path = FGenericPlatformHttp::GetUrlPath(HostUrl);

	const FDateTime Time = FDateTime::UtcNow();
	const FString Date = Time.ToHttpDate().Replace(TEXT("GMT"),TEXT("UTC"));

	/*Assemble the string to be encrypted*/
	const FString Source_Str = TEXT("host: ") + Host + TEXT("\n") + TEXT("date: ") + Date + TEXT("\n") + TEXT("GET ") + Path + TEXT(" HTTP/1.1");

	/*Cryptographic the string*/

	/*Computational signature*/
	TArray<uint8> Source = UHelper::StringToBytesUtf8(Source_Str);
	TArray<uint8> Key = UHelper::StringToBytesUtf8(ApiSecret);
	
	TArray<uint8> Result = UHelper::HMAC_256(Source, Key);

	const FString Signature = UHelper::BytesToBase64(Result);

	/*Computational authorization*/
	const FString Authorization_Str = FString::Printf(TEXT("api_key=\"%s\", algorithm=\"hmac-sha256\", headers=\"host date request-line\", signature=\"%s\""), *ApiKey, *Signature);
	
	const FString Authorization = FBase64::Encode(Authorization_Str);
	
	/*Url Encode*/
	const FString Authorization_Url = FGenericPlatformHttp::UrlEncode(Authorization);
	
	const FString Date_Url = FGenericPlatformHttp::UrlEncode(Date);
	const FString Date_Url_Rep = Date_Url.Replace(TEXT("%20"),TEXT("+"));
	
	const FString Host_Url = FGenericPlatformHttp::UrlEncode(Host);
	
	const FString Url = FString::Printf(TEXT("%s?authorization=%s&date=%s&host=%s"),*HostUrl, *Authorization_Url, *Date_Url_Rep, *Host_Url);

	return Url;
}

FString UXunFeiLibrary::Assemble_Data(const FString& AppId, const FString& Vcn, const FString& Text)
{
	/*Common Json*/
	TSharedPtr<FJsonObject> Common = MakeShareable(new FJsonObject);

	Common->SetStringField(TEXT("app_id"), AppId);

	/*Business Json*/
	TSharedPtr<FJsonObject> Business = MakeShareable(new FJsonObject);

	Business->SetStringField(TEXT("aue"),TEXT("raw"));
	Business->SetStringField(TEXT("vcn"),Vcn);
	Business->SetStringField(TEXT("tte"),TEXT("UTF8"));

	/*Data Json*/
	TSharedPtr<FJsonObject> Data = MakeShareable(new FJsonObject);

	Data->SetNumberField(TEXT("status"),2);

	TArray<uint8> TextBytes = UHelper::StringToBytesUtf8(Text);
	const FString Text_Base64 = UHelper::BytesToBase64(TextBytes);
	Data->SetStringField(TEXT("text"), Text_Base64);

	/*Assemble the Json Data*/
	TSharedPtr<FJsonObject> JsonData = MakeShareable(new FJsonObject);

	JsonData->SetObjectField(TEXT("common"),Common);
	JsonData->SetObjectField(TEXT("business"),Business);
	JsonData->SetObjectField(TEXT("data"),Data);

	/*Get Json String*/
	FString OutJsonData;
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&OutJsonData);
	FJsonSerializer::Serialize(JsonData.ToSharedRef(),Writer);

	return OutJsonData;
}

bool UXunFeiLibrary::ParseVoiceData(const FString& JsonString, FString& RawData, int32& Status, FString& Error, int32& ErrorCode)
{
	/*Load Json from string*/
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(JsonString);
	FJsonSerializer::Deserialize(Reader, JsonObject);

	/*Parse the Json Object*/
	int32 Code = JsonObject->GetNumberField(TEXT("code"));

	/*Failed*/
	if (Code != 0)
	{
		Error = JsonObject->GetStringField(TEXT("message"));
		ErrorCode = Code;
		return false;
	}

	/*Success*/
	TSharedPtr<FJsonObject> Data = JsonObject->GetObjectField(TEXT("data"));

	Status = Data->GetNumberField(TEXT("status"));
	RawData = Data->GetStringField(TEXT("audio"));

	return true;
}
