#pragma once
#include "pch.h"
#include "ISourceCodeRetrieverDataModelKeys.h"

using namespace Platform;
using namespace Windows::Foundation;

namespace SDK
{
    [Windows::Foundation::Metadata::WebHostHidden]
    public interface class ISourceCodeRetrieverClient
    {
        IAsyncOperation<Windows::Data::Json::JsonObject^>^ RetrieveAsync(String^ dataSourceName, String^ path);
        ISourceCodeRetrieverDataModelKeys^ getKeys();
    };
}