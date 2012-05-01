//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#include "pch.h"
#include "ImageBrowserViewModel.h"
#include "PhotoReader.h"
#include "PhotoGroup.h"
#include "Photo.h"
#include "DelegateCommand.h"
#include "ImageViewData.h"

using namespace Hilo;

using namespace concurrency;
using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;
using namespace Windows::Storage::BulkAccess;
using namespace Windows::UI::Xaml::Input;

ImageBrowserViewModel::ImageBrowserViewModel() : m_inProgress(true)
{
    m_groupCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &ImageBrowserViewModel::NavigateToGroup), nullptr);
}

ICommand^ ImageBrowserViewModel::GroupCommand::get()
{
    return m_groupCommand;
}

bool ImageBrowserViewModel::InProgress::get()
{
    return m_inProgress;
}

Object^ ImageBrowserViewModel::PhotoGroups::get()
{
    if (nullptr == m_photoGroups)
    {
        m_photoGroups = ref new Vector<Object^>();
        PhotoReader reader;
        auto foldersTask = task<IVectorView<FolderInformation^>^>(reader.GetVirtualPhotoFoldersByMonth());
        
        foldersTask.then([this](IVectorView<FolderInformation^>^ folders) 
        {
            auto temp = ref new Vector<Object^>();
            std::for_each(begin(folders), end(folders), [temp](FolderInformation^ folder) 
            {
                auto photoGroup = ref new PhotoGroup(folder, false);
                temp->Append(photoGroup);
            });
            return temp;
        }).then([this](Vector<Object^>^ folders)
        {
            m_inProgress = false;
            Array<Object^>^ many = ref new Array<Object^>(folders->Size);
            folders->GetMany(0, many);
            m_photoGroups->ReplaceAll(many);
            OnPropertyChanged("InProgress");
        }, task_continuation_context::use_current());
    }
    return m_photoGroups;
}

void ImageBrowserViewModel::NavigateToGroup(Object^ parameter)
{
    auto group = dynamic_cast<PhotoGroup^>(parameter);
    assert(group != nullptr);
    auto photo = dynamic_cast<Photo^>(group->Items->GetAt(0));
    assert(photo != nullptr);
    auto imageData = ref new ImageViewData(photo, group);
    ViewModelBase::GoToPage(PageType::Image, imageData);
}