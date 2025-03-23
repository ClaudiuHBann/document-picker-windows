
/*
 * This file is auto-generated from a NativeModule spec file in js.
 *
 * This is a C++ Spec class that should be used with MakeTurboModuleProvider to register native modules
 * in a way that also verifies at compile time that the native module matches the interface required
 * by the TurboModule JS spec.
 */
#pragma once
// clang-format off


#include <NativeModules.h>
#include <tuple>

namespace PickerCodegen {

struct DocumentPickerWindowsSpec : winrt::Microsoft::ReactNative::TurboModuleSpec {
  static constexpr auto methods = std::tuple{
      Method<void(::React::JSValue, Promise<::React::JSValueArray>) noexcept>{0, L"pick"},
      Method<void(::React::JSValue, Promise<::React::JSValue>) noexcept>{1, L"saveDocument"},
      Method<void(::React::JSValue, Promise<std::vector<::React::JSValue>>) noexcept>{2, L"writeDocuments"},
      Method<void(::React::JSValue, Promise<::React::JSValue>) noexcept>{3, L"pickDirectory"},
      Method<void(::React::JSValue, Promise<::React::JSValueArray>) noexcept>{4, L"keepLocalCopy"},
      SyncMethod<::React::JSValue(std::string, std::string) noexcept>{5, L"isKnownType"},
      Method<void(std::vector<std::string>, Promise<void>) noexcept>{6, L"releaseSecureAccess"},
      Method<void(std::vector<std::string>, Promise<::React::JSValue>) noexcept>{7, L"releaseLongTermAccess"},
  };

  template <class TModule>
  static constexpr void ValidateModule() noexcept {
    constexpr auto methodCheckResults = CheckMethods<TModule, DocumentPickerWindowsSpec>();

    REACT_SHOW_METHOD_SPEC_ERRORS(
          0,
          "pick",
          "    REACT_METHOD(pick) void pick(::React::JSValue && options, ::React::ReactPromise<::React::JSValueArray> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(pick) static void pick(::React::JSValue && options, ::React::ReactPromise<::React::JSValueArray> &&result) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          1,
          "saveDocument",
          "    REACT_METHOD(saveDocument) void saveDocument(::React::JSValue && options, ::React::ReactPromise<::React::JSValue> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(saveDocument) static void saveDocument(::React::JSValue && options, ::React::ReactPromise<::React::JSValue> &&result) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          2,
          "writeDocuments",
          "    REACT_METHOD(writeDocuments) void writeDocuments(::React::JSValue && options, ::React::ReactPromise<std::vector<::React::JSValue>> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(writeDocuments) static void writeDocuments(::React::JSValue && options, ::React::ReactPromise<std::vector<::React::JSValue>> &&result) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          3,
          "pickDirectory",
          "    REACT_METHOD(pickDirectory) void pickDirectory(::React::JSValue && options, ::React::ReactPromise<::React::JSValue> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(pickDirectory) static void pickDirectory(::React::JSValue && options, ::React::ReactPromise<::React::JSValue> &&result) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          4,
          "keepLocalCopy",
          "    REACT_METHOD(keepLocalCopy) void keepLocalCopy(::React::JSValue && options, ::React::ReactPromise<::React::JSValueArray> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(keepLocalCopy) static void keepLocalCopy(::React::JSValue && options, ::React::ReactPromise<::React::JSValueArray> &&result) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          5,
          "isKnownType",
          "    REACT_SYNC_METHOD(isKnownType) ::React::JSValue isKnownType(std::string kind, std::string value) noexcept { /* implementation */ }\n"
          "    REACT_SYNC_METHOD(isKnownType) static ::React::JSValue isKnownType(std::string kind, std::string value) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          6,
          "releaseSecureAccess",
          "    REACT_METHOD(releaseSecureAccess) void releaseSecureAccess(std::vector<std::string> const & uris, ::React::ReactPromise<void> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(releaseSecureAccess) static void releaseSecureAccess(std::vector<std::string> const & uris, ::React::ReactPromise<void> &&result) noexcept { /* implementation */ }\n");
    REACT_SHOW_METHOD_SPEC_ERRORS(
          7,
          "releaseLongTermAccess",
          "    REACT_METHOD(releaseLongTermAccess) void releaseLongTermAccess(std::vector<std::string> const & uris, ::React::ReactPromise<::React::JSValue> &&result) noexcept { /* implementation */ }\n"
          "    REACT_METHOD(releaseLongTermAccess) static void releaseLongTermAccess(std::vector<std::string> const & uris, ::React::ReactPromise<::React::JSValue> &&result) noexcept { /* implementation */ }\n");
  }
};

} // namespace PickerCodegen
