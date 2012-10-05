﻿

#pragma once
//------------------------------------------------------------------------------
//     This code was generated by a tool.
//
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
//------------------------------------------------------------------------------

namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Data {
                ref class CollectionViewSource;
            }
        }
    }
}
namespace Windows {
    namespace UI {
        namespace Xaml {
            namespace Controls {
                ref class Grid;
                ref class ListView;
                ref class Button;
                ref class TextBlock;
                ref class AppBar;
            }
        }
    }
}
namespace Hilo {
    ref class VariableGridView;
}
namespace Windows {
    namespace UI {
        namespace Xaml {
            ref class VisualStateGroup;
            ref class VisualState;
        }
    }
}

namespace Hilo
{
    partial ref class MainHubView : public ::Hilo::HiloPage, 
        public ::Windows::UI::Xaml::Markup::IComponentConnector
    {
    public:
        void InitializeComponent();
        virtual void Connect(int connectionId, ::Platform::Object^ target);
    
    private:
        bool _contentLoaded;
    
        private: ::Windows::UI::Xaml::Data::CollectionViewSource^ GroupedItemsViewSource;
        private: ::Windows::UI::Xaml::Controls::Grid^ ContentRoot;
        private: ::Hilo::VariableGridView^ ItemGridView;
        private: ::Windows::UI::Xaml::Controls::ListView^ SnapItemListView;
        private: ::Windows::UI::Xaml::Controls::Button^ BackButton;
        private: ::Windows::UI::Xaml::Controls::TextBlock^ PageTitle;
        private: ::Windows::UI::Xaml::VisualStateGroup^ ApplicationViewStates;
        private: ::Windows::UI::Xaml::VisualState^ FullScreenLandscape;
        private: ::Windows::UI::Xaml::VisualState^ Filled;
        private: ::Windows::UI::Xaml::VisualState^ FullScreenPortrait;
        private: ::Windows::UI::Xaml::VisualState^ Snapped;
        private: ::Windows::UI::Xaml::Controls::AppBar^ MainHubViewBottomAppBar;
        private: ::Windows::UI::Xaml::Controls::Button^ RotateButton;
        private: ::Windows::UI::Xaml::Controls::Button^ CropButton;
        private: ::Windows::UI::Xaml::Controls::Button^ CartoonizeButton;
        private: ::Windows::UI::Xaml::Controls::Button^ RotateButtonNoLabel;
        private: ::Windows::UI::Xaml::Controls::Button^ CropButtonNoLabel;
        private: ::Windows::UI::Xaml::Controls::Button^ CartoonizeButtonNoLabel;
    };
}

