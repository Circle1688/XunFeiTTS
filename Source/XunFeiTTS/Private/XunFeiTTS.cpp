/**
 * 作者: Circle_
 * 网址: www.peihongzhe.com
 * B站: https://b23.tv/EzPeUhU
 * 微信: sylbb520520
*/

#include "XunFeiTTS.h"

#define LOCTEXT_NAMESPACE "FXunFeiTTSModule"

void FXunFeiTTSModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FXunFeiTTSModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FXunFeiTTSModule, XunFeiTTS)