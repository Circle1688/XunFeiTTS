/**
 * Author: Circle_
 * BiliBili: https://b23.tv/EzPeUhU
*/

#pragma once

DECLARE_LOG_CATEGORY_EXTERN(LogWebSocket, Log, All)

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "WebSockets/Public/IWebSocket.h"
#include "WebSocket.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnConnectError, const FString&);
DECLARE_MULTICAST_DELEGATE(FOnConnected);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMessage, const FString&);
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnClose, int32, const FString&, bool);
/**
 * 
 */
UCLASS()
class XUNFEITTS_API UWebSocket : public UObject
{
	GENERATED_BODY()
	
public:
	static UWebSocket* ConstructWebsocket();
	
	bool Connect(FString Url);

	bool Send(FString Data);

	bool IsConnected();

	void Close();
	
	FOnConnectError& OnConnectError();
	FOnConnected& OnConnected();
	FOnMessage& OnMessage();
	FOnClose& OnClose();

	void OnWebsocketConnected();
	void OnWebsocketConnectError(const FString& ErrorMessage);
	void OnWebsocketClose(int32 StatusCode, const FString& Reason, bool bWasClean);
	void OnWebsocketMessage(const FString& Message);
	
private:
	FOnConnectError OnConnectErrorEvent;
	FOnConnected OnConnectedEvent;
	FOnMessage OnMessageEvent;
	FOnClose OnCloseEvent;
	
private:
	TSharedPtr<IWebSocket> WebSocket;
};
