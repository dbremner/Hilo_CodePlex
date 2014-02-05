// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved
#include "pch.h"
#include "ImageUtilities.h"

#include <robuffer.h>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Storage::Streams;

inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw Exception::CreateException(hr);
    }
}

// Retrieves the raw pixel data from the provided IBuffer object.
// Warning, the lifetime of the returned buffer is controlled by the lifetime of the
// buffer object passed to this method, once the buffer has been released 
// pointer will be invalid and must not be used.
byte* GetPointerToPixelData(IBuffer^ buffer, unsigned int *length)
{
    if (length != nullptr)
    {
        *length = buffer->Length;
    }
    // Query the IBufferByteAccess interface.
    ComPtr<IBufferByteAccess> bufferByteAccess;
    ThrowIfFailed(reinterpret_cast<IInspectable*>(buffer)->QueryInterface(IID_PPV_ARGS(&bufferByteAccess)));

    // Retrieve the buffer data.
    byte* pixels = nullptr;
    ThrowIfFailed(bufferByteAccess->Buffer(&pixels));
    return pixels;
}
