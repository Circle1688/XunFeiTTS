/**
 * 作者: Circle_
 * 网址: www.peihongzhe.com
 * B站: https://b23.tv/EzPeUhU
 * 微信: sylbb520520
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Helper.generated.h"

/**
 * 
 */
UCLASS()
class XUNFEITTS_API UHelper : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	/**
	* 生成HMAC 256
	*/
	static TArray<uint8> HMAC_256(const TArray<uint8>& ByteArray, const TArray<uint8>& Key, int32 Index = 0, int32 Length = 0x7FFFFFFF);

	/**
	* 将字符串转换为字节数组(使用UTF-8编码)
	*/
	static TArray<uint8> StringToBytesUtf8(const FString& String);

	/**
	* 将字节数组转换为Baseó4字符串
	*/
	static FString BytesToBase64(const TArray<uint8>& ByteArray, int32 Index = 0, int32 Length = 0x7FFFFFFF);

	/**
	* 尝试将Base64字符串转换为字节数组。失败时将返回一个空数组
	*/
	static TArray<uint8> Base64ToBytes(const FString& Base64);

private:
	/**
	* 生成SHA-256哈希，总是返回32字节
	*/
	static TArray<uint8> Sha256(const TArray<uint8>& ByteArray, int32 Index = 0, int32 Length = 0x7FFFFFFF);

	/**
	* 返回给定索引和长度的给定字节的值
	*/
	static TArray<uint8> BytesSubArray(const TArray<uint8>& ByteArray, int32 Index, int32 Length = 0x7FFFFFFF);
};
