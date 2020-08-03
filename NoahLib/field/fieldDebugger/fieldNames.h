#pragma once

struct sFieldName
{
    int fieldId;
    const char* fieldName;
};

const sFieldName* findFieldName(int index);
