//===============================================================================
// Microsoft patterns & practices
// Hilo Guidance
//===============================================================================
// Copyright © Microsoft Corporation.  All rights reserved.
// This code released under the terms of the 
// Microsoft patterns & practices license (http://hilo.codeplex.com/license)
//===============================================================================
#pragma once
#include <ppltasks.h>
#include "IExceptionPolicy.h"

namespace Hilo 
{
    template<typename T>
    struct ObserveException
    {

        ObserveException(IExceptionPolicy^ handler)
        {
            m_handler = handler;
        }

        concurrency::task<T> operator()(concurrency::task<T> antecedent) const
        {
            T result;
            try 
            {
                result = antecedent.get();
            }
            catch(const std::exception&)
            {
                // todo: Can pass a string for RP
                auto translatedException = ref new Platform::FailureException();
                m_handler->HandleException(translatedException);
                throw;
            }
            catch(Platform::Exception^ ex)
            {
                m_handler->HandleException(ex);
                throw;
            }
            return antecedent;
        }

    private:
        IExceptionPolicy^ m_handler;
    };

    template<>
    concurrency::task<void> ObserveException<void>::operator()(concurrency::task<void> antecedent) const
        {
            try 
            {
                antecedent.get();
            }
            catch(const concurrency::task_canceled)
            {
                // don't need to capture canceled exceptions
                throw;
            }
            catch(const std::exception&)
            {
                // todo: Can pass a string for RP
                auto translatedException = ref new Platform::FailureException();
                m_handler->HandleException(translatedException);
                throw;
            }
            catch(Platform::Exception^ ex)
            {
                m_handler->HandleException(ex);
                throw;
            }

            return antecedent;
        }
}