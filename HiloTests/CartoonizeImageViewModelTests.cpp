#include "pch.h"
#include "CppUnitTest.h"
#include "..\Hilo\CartoonizeImageViewModel.h"
#include "StubExceptionPolicy.h"
#include "StubRepository.h"
#include "..\Hilo\PageType.h"

using namespace concurrency;
using namespace Hilo;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace Platform;
using namespace Platform::Collections;

namespace HiloTests
{
    TEST_CLASS(CartoonizeImageViewModelTests)
    {
    public:
        TEST_METHOD_INITIALIZE(Initialize)
        {
            m_exceptionPolicy = std::make_shared<StubExceptionPolicy>();
            m_repository= std::make_shared<StubRepository>(m_exceptionPolicy);
        }

        TEST_METHOD(CartoonizeImageViewModelShouldSetupCartoonizeCommandWhenConstructed)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->CartoonizeCommand);
        }

        TEST_METHOD(CartoonizeImageViewModelShouldSetupResumeCartoonizeCommandWhenConstructed)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->ResumeCartoonizeCommand);
        }

        TEST_METHOD(CartoonizeImageViewModelShouldSetupSaveCommandWhenConstructed)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->SaveCommand);
        }

        TEST_METHOD(CartoonizeImageViewModelShouldSetupCancelCommandWhenConstructed)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsNotNull((*vm)->CancelCommand);
        }

        TEST_METHOD(CartoonizeImageViewModelShouldSetIsAppBarStickyWhenConstructed)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]() 
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::IsTrue((*vm)->IsAppBarSticky);
        }

        TEST_METHOD(CartoonizeImageViewModelShouldSetNeighborWindowWhenConstructed)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::AreEqual((*vm)->NeighborWindow, 3.0);
        }

        TEST_METHOD(CartoonizeImageViewModelShouldSetPhasesWhenConstructed)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            TestHelper::RunUISynced([this, vm]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
            });
            Assert::AreEqual((*vm)->Phases, 3.0);
        }

        TEST_METHOD(CartoonizeImageViewModelCanSetNeighborWindowProperty)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            auto value = 5.0;
            TestHelper::RunUISynced([this, vm, value]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->NeighborWindow = value;
            });
            Assert::AreEqual((*vm)->NeighborWindow, value);
        }

        TEST_METHOD(CartoonizeImageViewModelCanSetPhasesProperty)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            auto value = 5.0;
            TestHelper::RunUISynced([this, vm, value]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->Phases = value;
            });
            Assert::AreEqual((*vm)->Phases, value);
        }

        TEST_METHOD(CartoonizeImageViewModelShouldSaveAndLoadNeighborWindowValue)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            auto newVm = std::make_shared<CartoonizeImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, vm, newVm]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->NeighborWindow = 5.0;
                auto state = ref new Map<String^, Object^>();
                (*vm)->SaveState(state);

                (*newVm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
                (*newVm)->LoadState(state);
            });
            Assert::AreEqual((*vm)->NeighborWindow, (*newVm)->NeighborWindow);
        }

        TEST_METHOD(CartoonizeImageviewModelShouldSaveAndLoadPhasesValue)
        {
            auto vm = std::make_shared<CartoonizeImageViewModel^>(nullptr);
            auto newVm = std::make_shared<CartoonizeImageViewModel^>(nullptr);

            TestHelper::RunUISynced([this, vm, newVm]()
            {
                (*vm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
                (*vm)->Phases= 5.0;
                auto state = ref new Map<String^, Object^>();
                (*vm)->SaveState(state);

                (*newVm) = ref new CartoonizeImageViewModel(m_repository, m_exceptionPolicy);
                (*newVm)->LoadState(state);
            });
            Assert::AreEqual((*vm)->Phases, (*newVm)->Phases);
        }

    private:
        std::shared_ptr<StubRepository> m_repository;
        std::shared_ptr<StubExceptionPolicy> m_exceptionPolicy;

    };
}
