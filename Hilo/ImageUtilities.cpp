#include "pch.h"
#include "ImageUtilities.h"

#include <robuffer.h>
#include <wrl.h>

using namespace Microsoft::WRL;
using namespace Platform;
using namespace Windows::Storage::Streams;

// <snippet810>
inline void ThrowIfFailed(HRESULT hr)
{
    if (FAILED(hr))
    {
        throw Exception::CreateException(hr);
    }
}
// </snippet810>

// <snippet809>
// Retrieves the raw pixel data from the provided IBuffer object.
byte* GetPointerToPixelData(IBuffer^ buffer)
{
    // Cast to Object^, then to its underlying IInspectable interface.
    Object^ obj = buffer;
    ComPtr<IInspectable> insp(reinterpret_cast<IInspectable*>(obj));

    // Query the IBufferByteAccess interface.
    ComPtr<IBufferByteAccess> bufferByteAccess;
    ThrowIfFailed(insp.As(&bufferByteAccess));

    // Retrieve the buffer data.
    byte* pixels = nullptr;
    ThrowIfFailed(bufferByteAccess->Buffer(&pixels));
    return pixels;
}
// </snippet809>