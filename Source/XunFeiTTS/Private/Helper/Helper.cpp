/**
 * 作者: Circle_
 * 网址: www.peihongzhe.com
 * B站: https://b23.tv/EzPeUhU
 * 微信: sylbb520520
*/


#include "Helper/Helper.h"
#include "Helper/HashingSha256Library.h"

TArray<uint8> UHelper::HMAC_256(const TArray<uint8>& ByteArray, const TArray<uint8>& Key, int32 Index, int32 Length)
{
	if (Index < 0)
	{
		Length += Index;
		Index = 0;
	}
	if (Length > ByteArray.Num() - Index)
	{
		Length = ByteArray.Num() - Index;
	}

	int32 BlockSize = 64;

	TArray<uint8> KeyBytes = Key;
	if (KeyBytes.Num() > BlockSize)
	{
		KeyBytes = Sha256(KeyBytes);// key = hash(key)
	}
	if (KeyBytes.Num() != BlockSize)
	{
		KeyBytes.SetNumZeroed(BlockSize);// key = key + 0x00 (till KeyBytes has BlockSize length)
	}

	TArray<uint8> ArrayO = KeyBytes;
	TArray<uint8> ArrayI = KeyBytes;
	for (int32 i = 0; i < KeyBytes.Num(); i++)
	{
		ArrayO[i] ^= 0x5c;// o_key_pad = 0x5c ^ key
		ArrayI[i] ^= 0x36;// i_key_pad = 0x36 ^ key
	}

	if (Length > 0)// (i_key_pad + message)
		{
		ArrayI.Append(ByteArray.GetData() + Index, Length);
		}
	ArrayI = Sha256(ArrayI);// hash(i_key_pad + message)
	ArrayO.Append(ArrayI.GetData(), ArrayI.Num());// (o_key_pad + hash(i_key_pad + message))
	return Sha256(ArrayO);// hash(o_key_pad + hash(i_key_pad + message))
}

TArray<uint8> UHelper::Sha256(const TArray<uint8>& ByteArray, int32 Index, int32 Length)
{
	return UHashingSha256Library::Hash(ByteArray, Index, Length);
}

TArray<uint8> UHelper::StringToBytesUtf8(const FString& String)
{
	if (String.Len() <= 0)
	{
		return TArray<uint8>();
	}

	TArray<uint8> ByteArray;
	FTCHARToUTF8 Src = FTCHARToUTF8(String.GetCharArray().GetData());
	ByteArray.Append((uint8*)Src.Get(), Src.Length());
	return ByteArray;
}

FString UHelper::BytesToBase64(const TArray<uint8>& ByteArray, int32 Index, int32 Length)
{
	if (ByteArray.Num() <= 0)
	{
		return TEXT("");
	}

	if (Index < 0)
	{
		Length += Index;
		Index = 0;
	}
	if (Length > ByteArray.Num() - Index)
	{
		Length = ByteArray.Num() - Index;
	}
	if (Length <= 0)
	{
		return TEXT("");
	}

	if ((Index == 0) && (Length == ByteArray.Num()))
	{
		return FBase64::Encode(ByteArray);
	}
	return FBase64::Encode(BytesSubArray(ByteArray, Index, Length));
}

TArray<uint8> UHelper::BytesSubArray(const TArray<uint8>& ByteArray, int32 Index, int32 Length)
{
	if (ByteArray.Num() <= 0)
	{
		return TArray<uint8>();
	}

	if (Index < 0)
	{
		Length += Index;
		Index = 0;
	}
	if (Length > ByteArray.Num() - Index)
	{
		Length = ByteArray.Num() - Index;
	}
	if (Length <= 0)
	{
		return TArray<uint8>();
	}

	if ((Index == 0) && (Length == ByteArray.Num()))
	{
		return ByteArray;
	}
	TArray<uint8> ReturnArray = TArray<uint8>();
	ReturnArray.Append(ByteArray.GetData() + Index, Length);
	return ReturnArray;
}

TArray<uint8> UHelper::Base64ToBytes(const FString& Base64)
{
	TArray<uint8> ByteArray;
	bool Success = FBase64::Decode(Base64, ByteArray);
	if (!Success)
	{
		return TArray<uint8>();
	}
	return ByteArray;
}
