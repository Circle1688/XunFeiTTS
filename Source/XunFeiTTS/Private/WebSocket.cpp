/**
 * Author: Circle_
 * Website: www.peihongzhe.com
 * BiliBili: https://b23.tv/EzPeUhU
 * Wechat: sylbb520520
*/


#include "WebSocket.h"
#include "WebSockets/Public/WebSocketsModule.h"

DEFINE_LOG_CATEGORY(LogWebSocket)

UWebSocket* UWebSocket::ConstructWebsocket()
{
	return NewObject<UWebSocket>();
}

bool UWebSocket::Connect(FString Url)
{
	FWebSocketsModule& WebSocketModule = FWebSocketsModule::Get();
	
	WebSocket = WebSocketModule.CreateWebSocket(Url);
	if (WebSocket)
	{
		WebSocket->OnConnectionError().AddUObject(this, &UWebSocket::OnWebsocketConnectError);
		WebSocket->OnConnected().AddUObject(this, &UWebSocket::OnWebsocketConnected);
		WebSocket->OnClosed().AddUObject(this, &UWebSocket::OnWebsocketClose);
		WebSocket->OnMessage().AddUObject(this, &UWebSocket::OnWebsocketMessage);
		WebSocket->Connect();
		return true;
	}
	return false;
}

bool UWebSocket::Send(FString Data)
{
	if (WebSocket)
	{
		if (WebSocket->IsConnected())
		{			
			WebSocket->Send(Data);
			return true;
		}
		else {
			UE_LOG(LogWebSocket, Warning, TEXT("Websocker is not connected!"));
		}
	}
	else {
		UE_LOG(LogWebSocket, Warning, TEXT("Send failed!WebSocket is Null!"));
	}
	return false;
}

bool UWebSocket::IsConnected()
{
	if (WebSocket)
	{
		return WebSocket->IsConnected();
	}
	UE_LOG(LogWebSocket, Warning, TEXT("Check IsConnected failed!WebSocket is Null!"));
	return false;
}

void UWebSocket::Close()
{
	if (WebSocket)
	{
		WebSocket->Close();
	}
	else {
		UE_LOG(LogWebSocket, Warning, TEXT("Close failed!WebSocket is Null!"));
	}
}

FOnConnectError& UWebSocket::OnConnectError()
{
	return OnConnectErrorEvent;
}

FOnConnected& UWebSocket::OnConnected()
{
	return OnConnectedEvent;
}

FOnMessage& UWebSocket::OnMessage()
{
	return OnMessageEvent;
}

FOnClose& UWebSocket::OnClose()
{
	return OnCloseEvent;
}

void UWebSocket::OnWebsocketConnected()
{
	if (WebSocket)
	{
		OnConnectedEvent.Broadcast();
	}
	else {
		UE_LOG(LogWebSocket, Warning, TEXT("WebSocket is Null!"));
	}
}

void UWebSocket::OnWebsocketConnectError(const FString& ErrorMessage)
{
	if (WebSocket)
	{
		OnConnectErrorEvent.Broadcast(ErrorMessage);
	}
	else {
		UE_LOG(LogWebSocket, Warning, TEXT("WebSocket is Null!"));
	}
}

void UWebSocket::OnWebsocketClose(int32 StatusCode, const FString& Reason, bool bWasClean)
{
	if (WebSocket)
	{
		OnCloseEvent.Broadcast(StatusCode, Reason, bWasClean);
	}
	else {
		UE_LOG(LogWebSocket, Warning, TEXT("WebSocket is Null!"));
	}
}

void UWebSocket::OnWebsocketMessage(const FString& Message)
{
	if (WebSocket)
	{
		OnMessageEvent.Broadcast(Message);
	}
	else {
		UE_LOG(LogWebSocket, Warning, TEXT("WebSocket is Null!"));
	}
}