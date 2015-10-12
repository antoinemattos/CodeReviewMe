#include "pch.h"

#include "EndpointsConstants.h"
#include "MainPage.xaml.h"

using namespace ClientUI;
using namespace Concurrency;
using namespace Platform;
using namespace SDK;
using namespace Windows::Data::Json;
using namespace Windows::UI::Core;
using namespace Windows::UI::Xaml;
using namespace Windows::Foundation;

MainPage::MainPage()
{
    InitializeComponent();
}

void MainPage::SourceCodeGetAPIButton_Click(Object^, RoutedEventArgs^)
{
    const auto& client = ref new SourceCodeRetrieverClient(SOURCE_CODE_GET_ENDPOINT, API_KEY);
    const auto& sourceKey = client->getKeys()->getSourceKey();
    const auto& asyncop = client->RetrieveAsync(L"github", L"/auchenberg/volkswagen/master/LICENSE");

    const task<JsonObject^> retrieveTask(asyncop, cancellation_token::none());
    retrieveTask.then([this, sourceKey](const task<JsonObject^> t) {
        const auto& sourceCode = t.get()->GetNamedString(sourceKey);
        this->ShowMessageBox(sourceCode);
    }, task_continuation_context::use_current());
}

void MainPage::ShowMessageBox(const String^ message)
{
    Dispatcher->RunAsync(CoreDispatcherPriority::Normal, ref new DispatchedHandler([message]()
    {
        const auto& msgDlg = ref new Windows::UI::Popups::MessageDialog(const_cast<String^>(message));
        msgDlg->ShowAsync();
    }));
}