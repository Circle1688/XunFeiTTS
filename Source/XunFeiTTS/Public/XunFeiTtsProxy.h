/**
 * 作者: Circle_
 * B站: https://b23.tv/EzPeUhU
*/

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogXunFeiTTS, Log, All)

#include "CoreMinimal.h"
#include "WebSocket.h"
#include "Sound/StreamingSoundWave.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "XunFeiTtsProxy.generated.h"

USTRUCT(BlueprintType)
struct FXunFeiKey
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite)
	FString ApiSecret;

	UPROPERTY(BlueprintReadWrite)
	FString ApiKey;

	UPROPERTY(BlueprintReadWrite)
	FString AppId;
};

/**
 * @param StreamingSoundWave 继承自USoundWave类的流式音波类，支持流式播放。基础使用方法：在On Progress后面连接一个Do Once节点，将On Finish连接至Do Once节点的Reset上，并在Do Once节点的Completed后连接Play Sound 2D（使用的声音为该StreamingSoundWave）播放流式声音。
 * @param RawDataStreamBase64 流式的Base64编码的音频字符串，内容为本次传输的部分
 * @param RawDataBytesStream 流式的二进制音频数据，内容为本次传输的部分
 * @param ErrorMessage 发生错误时的错误信息
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnTtsEvent, const USoundWave*, StreamingSoundWave, const FString&, RawDataStreamBase64, const TArray<uint8>&, RawDataBytesStream, const FString&, ErrorMessage);

/**
 * 
 */
UCLASS()
class XUNFEITTS_API UXunFeiTtsProxy : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	
public:

	UXunFeiTtsProxy();

	/*已连接上讯飞，开始合成，此时还未收到任何数据*/
	UPROPERTY(BlueprintAssignable)
	FOnTtsEvent OnStart;

	/*合成进行中，此时可以收到数据*/
	UPROPERTY(BlueprintAssignable)
	FOnTtsEvent OnProgress;

	/*完成合成*/
	UPROPERTY(BlueprintAssignable)
	FOnTtsEvent OnFinish;

	/*连接发生错误*/
	UPROPERTY(BlueprintAssignable)
	FOnTtsEvent OnError;

	/*连接关闭*/
	UPROPERTY(BlueprintAssignable)
	FOnTtsEvent OnClose;

	virtual void Activate() override;

	/**
	 * 讯飞的在线语音合成服务
	 * @param Text 语音合成的文本
	 * @param Vcn 讯飞账号的发音人
	 * @param Key 讯飞账号的通行证信息，包含Api Key, Api Secret, App Id
	 */
	UFUNCTION(BlueprintCallable, Category = "XunFei|Text to speech", meta = (BlueprintInternalUseOnly = "true", DisplayName = "XunFei Text to Speech"))
	static UXunFeiTtsProxy* TextToSpeech(const FString Text, const FString Vcn, const FXunFeiKey& Key);

private:
	FString Text;
	FString Vcn;
	FXunFeiKey Key;

	FString RawData;
protected:

	UPROPERTY()
	UWebSocket* WebSocket;

	UPROPERTY()
	UStreamingSoundWave* StreamingSoundWave;
};
