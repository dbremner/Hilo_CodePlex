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
#include "DelegateCommand.h"
#include "PageType.h"
#include "MainHubViewModel.h"

using namespace Hilo;

using namespace Platform;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml::Input;

MainHubViewModel::MainHubViewModel(IObservableVector<PhotoGroup^>^ photoGroups) : m_photoGroups(photoGroups)
{
    m_navigateToPicturesCommand = ref new DelegateCommand(ref new ExecuteDelegate(this, &MainHubViewModel::NavigateToPictures), nullptr);
}

Object^ MainHubViewModel::PhotoGroups::get()
{
    return m_photoGroups;
}

ICommand^ MainHubViewModel::NavigateToPicturesCommand::get()
{
    return m_navigateToPicturesCommand;
}

void MainHubViewModel::NavigateToPictures(Platform::Object^ parameter)
{
    ViewModelBase::GoToPage(PageType::Browse, nullptr); 
}