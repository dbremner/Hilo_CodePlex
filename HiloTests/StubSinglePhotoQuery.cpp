//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "StubSinglePhotoQuery.h"
#include "StubPhoto.h"

using namespace concurrency;
using namespace Hilo;
using namespace HiloTests;
using namespace Platform;

task<IPhoto^> StubSinglePhotoQuery::GetPhotoAsync(String^ photoPath, cancellation_token token)
{
    m_called = true;
    return create_task([photoPath] 
    {
        auto photo = ref new StubPhoto();
        photo->Path = photoPath;
        return static_cast<IPhoto^>(photo);
    }, token);
}

bool StubSinglePhotoQuery::GetHasBeenCalled()
{
    return m_called;
}