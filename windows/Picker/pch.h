#pragma once

#define _SILENCE_ALL_CXX23_DEPRECATION_WARNINGS 1

// windows

#include <SDKDDKVer.h>

#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#define WINRT_LEAN_AND_MEAN 1

#include <Windows.h>
#undef GetCurrentTime
#include <ShObjIdl_core.h>

// winrt
#include <winrt/Microsoft.ReactNative.h>
#include <winrt/Windows.Storage.Pickers.h>

// rnw
#include <ReactPromise.h>

// std
#include <filesystem>
#include <format>
#include <string>
#include <string_view>

using namespace std::string_literals;
using namespace std::string_view_literals;
