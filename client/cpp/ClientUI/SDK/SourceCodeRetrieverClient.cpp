#include "pch.h"
#include "SourceCodeRetrieverClient.h"
#include "SourceCodeRetrieverDataModelKeys.h"

#define API_KEY_HEADER L"x-api-key"
#define DATASOURCE_NAME_QUERY_STRING L"dataSourceName"
#define PATH_QUERY_STRING L"relativePath"
#define DESERIALIZATION_EXCEPTION_MESSAGE L"The JSON returned from the server is malformed"

using namespace SDK;
using namespace Platform;

using namespace Windows::Data::Json;
using namespace Windows::Foundation;
using namespace Windows::Web::Http;

SourceCodeRetrieverClient::SourceCodeRetrieverClient(String^ endpoint, String^ apiKey) : endpoint(endpoint), apiKey(apiKey)
{
    this->keys = ref new SourceCodeRetrieverDataModelKeys();
}

SourceCodeRetrieverClient::~SourceCodeRetrieverClient()
{
}

IAsyncOperation<JsonObject^>^ SourceCodeRetrieverClient::RetrieveAsync(String^ dataSourceName, String^ path)
{
    assert(endpoint != nullptr && endpoint != L"");
    assert(apiKey != nullptr && apiKey != L"");
    assert(dataSourceName != nullptr && dataSourceName != L"");
    assert(path != nullptr && path != L"");

    const auto& asyncOp = create_async([this, &dataSourceName, &path]() -> task<JsonObject^> {
        return task<JsonObject^>([=]() {
            return this->retrieveFromBackgroundThread(dataSourceName, path);
        }, task_continuation_context::use_current());
    });
    return asyncOp;
}

const Uri^ SourceCodeRetrieverClient::generateUrlEndpoint(String^ dataSourceName, String^ path)
{
    return ref new Uri(this->endpoint + "?" + PATH_QUERY_STRING + "=" + path + "&" + DATASOURCE_NAME_QUERY_STRING + "=" + dataSourceName);
}

ISourceCodeRetrieverDataModelKeys^ SourceCodeRetrieverClient::getKeys()
{
    return this->keys;
}

void SourceCodeRetrieverClient::setHttpHeaderApiKey(Windows::Web::Http::HttpClient^* client)
{
    (*client)->DefaultRequestHeaders->Append(API_KEY_HEADER, this->apiKey);
}

JsonObject^ SourceCodeRetrieverClient::retrieveFromBackgroundThread(String^ dataSourceName, String^ path)
{
    HttpClient^ httpClient = ref new HttpClient();
    this->setHttpHeaderApiKey(&httpClient);
    const Uri^ url = generateUrlEndpoint(dataSourceName, path);
    const auto& getAsync = httpClient->GetAsync(const_cast<Uri^>(url));
    const auto& getAsyncTask = create_task(getAsync, cancellation_token::none());
    const auto& response = getAsyncTask.get();
    const auto& content = create_task(response->Content->ReadAsStringAsync()).get();

    JsonObject^ result = ref new JsonObject();
    if (JsonObject::TryParse(content, &result))
    {
        return result;
    }
    else
    {
        throw ref new Exception(-1, DESERIALIZATION_EXCEPTION_MESSAGE);
    }
}