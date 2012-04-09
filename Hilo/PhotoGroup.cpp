//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "PhotoGroup.h"
#include "Photo.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Core;

PhotoGroup::PhotoGroup(IAsyncOperation<IVectorView<FileInformation^>^>^ task) : m_task(task)
{
}

Object^ PhotoGroup::Items::get()
{
    if (m_photos == nullptr)
    {
        m_photos = ref new Vector<Object^>();
        m_task.then([this](IVectorView<FileInformation^>^ files) {
            bool first = true;
            std::for_each(begin(files), end(files), [this, &first](FileInformation^ item) {
                auto photo = ref new Photo(item);
                if (first)
                {
                    photo->ColumnSpan = 3;
                    photo->RowSpan = 2;
                    first = false;
                }                
                m_photos->Append(photo);
            });
        }, concurrency::task_continuation_context::use_current());
    }
    return m_photos;
}

String^ PhotoGroup::Title::get()
{
    return m_title;
}

void PhotoGroup::Title::set(String^ value)
{
    m_title = value;
}