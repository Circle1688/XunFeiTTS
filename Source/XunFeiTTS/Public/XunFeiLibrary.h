/**
 * 作者: Circle_
 * 网址: www.peihongzhe.com
 * B站: https://b23.tv/EzPeUhU
 * 微信: sylbb520520
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XunFeiLibrary.generated.h"

/**
 * 
 */
UCLASS()
class XUNFEITTS_API UXunFeiLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * 组合讯飞鉴权地址
	 * @param HostUrl 请求地址: wss://tts-api.xfyun.cn/v2/tts
	 * @param ApiSecret 讯飞账号的Api Secret
	 * @param ApiKey 讯飞账号的Api Key
	 * @return 讯飞鉴权地址
	 */
	static FString Assemble_Auth_Url(const FString& HostUrl, const FString& ApiSecret, const FString& ApiKey);

	/**
	 * 组合请求讯飞的Json字符串
	 * @param AppId 讯飞账号的AppId
	 * @param Vcn 讯飞账号里申请的发音人
	 * @param Text 要语音合成的文本
	 * @return 请求讯飞的Json字符串
	 */
	static FString Assemble_Data(const FString& AppId, const FString& Vcn, const FString& Text);

	/**
	 * 解析讯飞传回的Json数据
	 * @param JsonString 讯飞传回的Json数据
	 * @param RawData 合成后的音频片段，采用base64编码
	 * @param Status 当前音频流状态，1表示合成中，2表示合成结束
	 * @param Error 错误描述信息
	 * @param ErrorCode 返回码，0表示成功，其它表示异常，详情请参考错误码 https://www.xfyun.cn/document/error-code
	 * @return 成功返回真，失败返回假
	 */
	static bool ParseVoiceData(const FString& JsonString, FString& RawData, int32& Status, FString& Error, int32& ErrorCode);
	
};
