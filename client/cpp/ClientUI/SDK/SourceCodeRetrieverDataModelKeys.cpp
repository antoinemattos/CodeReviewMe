#include "pch.h"
#include "SourceCodeRetrieverDataModelKeys.h"

#define SOURCE_KEY "source"

using namespace SDK;

SourceCodeRetrieverDataModelKeys::SourceCodeRetrieverDataModelKeys()
{
}

SourceCodeRetrieverDataModelKeys::~SourceCodeRetrieverDataModelKeys()
{
}

Platform::String^ SourceCodeRetrieverDataModelKeys::getSourceKey()
{
    return SOURCE_KEY;
}