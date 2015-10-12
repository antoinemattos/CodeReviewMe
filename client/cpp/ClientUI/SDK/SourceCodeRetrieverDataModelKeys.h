#pragma once
#include "ISourceCodeRetrieverDataModelKeys.h"

namespace SDK
{
    ref class SourceCodeRetrieverDataModelKeys sealed : ISourceCodeRetrieverDataModelKeys
    {
    public:
        SourceCodeRetrieverDataModelKeys();
        virtual ~SourceCodeRetrieverDataModelKeys();
        virtual Platform::String^ getSourceKey();
    };
}