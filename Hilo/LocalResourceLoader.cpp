#include "pch.h"
#include "LocalResourceLoader.h"

using namespace Hilo;
using namespace Platform;
using namespace Windows::ApplicationModel::Resources;

// <snippet1901>
String^ LocalResourceLoader::GetString(String^ value)
{
    auto loader = ref new ResourceLoader();
    return loader->GetString(value);
}
// </snippet1901>