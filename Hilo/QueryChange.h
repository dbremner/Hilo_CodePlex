//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once

namespace Hilo
{
    ref class QueryChange sealed
    {
    internal:
        QueryChange(Windows::Storage::Search::IStorageQueryResultBase^ query, std::function<void()> callback) : m_query(query), m_callback(callback)
        {
            m_watchToken = m_query->ContentsChanged += ref new Windows::Foundation::TypedEventHandler<Windows::Storage::Search::IStorageQueryResultBase^, Object^>(this, &QueryChange::OnFileQueryContentsChanged);
        }

        virtual ~QueryChange()
        {
            m_query->ContentsChanged -= m_watchToken;
        }

    private:
        Windows::Storage::Search::IStorageQueryResultBase^ m_query;
        std::function<void()> m_callback;
        Windows::Foundation::EventRegistrationToken m_watchToken;

        void OnFileQueryContentsChanged(Windows::Storage::Search::IStorageQueryResultBase^ sender, Platform::Object^ e)
        {
            Windows::UI::Core::CoreWindow^ wnd = Windows::ApplicationModel::Core::CoreApplication::MainView->CoreWindow;
            Windows::UI::Core::CoreDispatcher^ dispatcher = wnd->Dispatcher;
            dispatcher->RunAsync(Windows::UI::Core::CoreDispatcherPriority::Normal, ref new Windows::UI::Core::DispatchedHandler([this] () 
            {
                m_callback();
            }));
        }
    };
}