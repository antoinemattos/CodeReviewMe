#pragma once
#include "ISourceCodeRetrieverClient.h"
#include "ISourceCodeRetrieverDataModelKeys.h"
#include <ppltasks.h>


using namespace Platform;
using namespace Concurrency;
using namespace Windows::Data::Json;
using namespace Windows::Foundation;

namespace SDK
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public ref class SourceCodeRetrieverClient sealed : ISourceCodeRetrieverClient
    {
    public:
        SourceCodeRetrieverClient(String^ endpoint, String^ apiKey);
        virtual ~SourceCodeRetrieverClient();
        virtual IAsyncOperation<Windows::Data::Json::JsonObject^>^ RetrieveAsync(String^ dataSourceName, String^ path);
        virtual ISourceCodeRetrieverDataModelKeys^ getKeys();
    private:
        const Uri^ generateUrlEndpoint(String^ dataSourceName, String^ path);
        String^ endpoint;
        String^ apiKey;
        ISourceCodeRetrieverDataModelKeys^ keys;

        JsonObject^ retrieveFromBackgroundThread(String^ dataSourceName, String^ path);
        void setHttpHeaderApiKey(Windows::Web::Http::HttpClient^* client);
    };
}