#pragma once

#include "MainPage.g.h"

namespace ClientUI
{
    public ref class MainPage sealed
    {
    public:
        MainPage();

    private:
        void SourceCodeGetAPIButton_Click(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e);
        void ShowMessageBox(const Platform::String^ message);
    };
}