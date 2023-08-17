/**
 * 作者: Circle_
 * 网址: www.peihongzhe.com
 * B站: https://b23.tv/EzPeUhU
 * 微信: sylbb520520
*/

#include "XunFeiTtsProxy.h"
#include "XunFeiLibrary.h"
#include "Helper/Helper.h"

DEFINE_LOG_CATEGORY(LogXunFeiTTS)

UXunFeiTtsProxy::UXunFeiTtsProxy() : Super(), WebSocket(NewObject<UWebSocket>())
{
}

void UXunFeiTtsProxy::Activate()
{
	/*检查输入参数是否合法*/
	if (Text.IsEmpty())
	{
		UE_LOG(LogXunFeiTTS,Error,TEXT("Text is Null!"));
		return;
	}
	if (Vcn.IsEmpty())
	{
		UE_LOG(LogXunFeiTTS,Error,TEXT("Vcn is Null!"));
		return;
	}

	/*创建流式音波对象*/
	StreamingSoundWave = UStreamingSoundWave::CreateStreamingSoundWave();
	
	/*绑定连接成功的委托*/
	WebSocket->OnConnected().AddLambda([this]() -> void
	{
		/*初始化空的字符串*/
		this->RawData.Empty();

		/*发送请求给讯飞*/
		const FString SendMsg = UXunFeiLibrary::Assemble_Data(Key.AppId, Vcn, Text);
		
		WebSocket->Send(SendMsg);
		
		this->OnStart.Broadcast(StreamingSoundWave,TEXT(""),{},TEXT(""));
	});

	/*绑定收到回传数据的委托*/
	WebSocket->OnMessage().AddLambda([this](const FString& Message) -> void
	{
		
		FString RawDataStream;
		int32 Status;
		FString Error;
		int32 ErrorCode;

		/*解析Json*/
		bool bSuccess = UXunFeiLibrary::ParseVoiceData(Message, RawDataStream, Status, Error, ErrorCode);

		/*出现错误时的处理*/
		if (!bSuccess)
		{
			const FString ErrorMessage = FString::Printf(TEXT("ErrorMessage: %s\nErrorCode: %i"),*Error, ErrorCode);
			this->OnError.Broadcast(StreamingSoundWave,TEXT(""),{},ErrorMessage);
		}
		
		/*将新的部分的数据放到末尾*/
		RawData.Append(RawDataStream);
		
		TArray<uint8> ByteArray = UHelper::Base64ToBytes(RawDataStream);

		/*将新的原始数据放入流式音波对象的缓冲区末尾，注意讯飞语音合成的音频默认是16位，16000采样率，单声道*/
		StreamingSoundWave->AppendAudioDataFromRAW(ByteArray,ERuntimeRAWAudioFormat::Int16,16000, 1);
		
		this->OnProgress.Broadcast(StreamingSoundWave, RawDataStream, ByteArray,TEXT(""));
		
		/*Status = 2时，即为最后一条数据*/
		if (Status == 2)
		{
			TArray<uint8> BytesRawData = UHelper::Base64ToBytes(RawData);
			
			this->OnFinish.Broadcast(StreamingSoundWave, RawData,BytesRawData,TEXT(""));
		}
	});

	/*绑定连接错误的委托*/
	WebSocket->OnConnectError().AddLambda([this](const FString& ErrorMessage) -> void
	{
		this->OnError.Broadcast(StreamingSoundWave,TEXT(""),{},ErrorMessage);
	});

	/*绑定连接关闭的委托*/
	WebSocket->OnClose().AddLambda([this](int32 StatusCode, const FString& Reason, bool bWasClean) -> void
	{
		this->OnClose.Broadcast(StreamingSoundWave, TEXT(""),{},Reason);
	});

	/*开始连接*/
	const FString HostUrl = TEXT("wss://tts-api.xfyun.cn/v2/tts");
	const FString Url = UXunFeiLibrary::Assemble_Auth_Url(HostUrl, Key.ApiSecret, Key.ApiKey);
	
	WebSocket->Connect(Url);
}

UXunFeiTtsProxy* UXunFeiTtsProxy::TextToSpeech(const FString Text, const FString Vcn, const FXunFeiKey& Key)
{
	UXunFeiTtsProxy* const Proxy = NewObject<UXunFeiTtsProxy>();
	Proxy->Text = Text;
	Proxy->Vcn = Vcn;
	Proxy->Key = Key;
	return Proxy;
}
