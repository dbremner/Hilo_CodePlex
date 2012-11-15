// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#pragma once

// Overrides 
namespace Microsoft{ namespace VisualStudio {namespace CppUnitTestFramework
{
    template<> static std::wstring ToString<concurrency::task_status>(const concurrency::task_status& status) { RETURN_WIDE_STRING(status); }
    template<> static std::wstring ToString<long long>(const long long& value) { RETURN_WIDE_STRING(value); }
    template<> static std::wstring ToString<Windows::UI::Xaml::Thickness>(const Windows::UI::Xaml::Thickness& value) { std::wstringstream _s; _s << "(" << value.Top << "," << value.Right<< "," << value.Bottom << "," << value.Left << ")"; return _s.str(); }

}}}