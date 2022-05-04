#pragma once

#include <string>
#include <vector>

struct sVariableDebugInfo
{
    int mOffset;
    int mType; // placeHolder
    std::string mName;
    std::string mComment;

    // todo: bitfields
};

struct sFunctionDebugInfo
{
    int mIdOrOffset;
    std::string mName;
    std::string mComment;
};

struct sFieldEntityDebugInfo
{
    int mId;
    std::string mName;

    std::vector<sFunctionDebugInfo> mFunctions;
};

struct sBytecodeComments
{
    int mOffset;
    std::string mComment;
};

struct sFieldActor
{
    std::string mName;
};

struct sFieldListEntry
{
    std::string mName;
    std::string mComment;

    std::vector<sVariableDebugInfo> mFieldVars;
    std::vector<sFunctionDebugInfo> mFieldFunctions;
    std::vector<sBytecodeComments> mFieldBytecodeComments;
    std::vector<sFieldActor> mActors;
    std::vector<sFieldActor> mScriptEntities;
};

struct sFieldDebugInfo
{
    std::vector<sVariableDebugInfo> mGlobalVars;
    std::vector<sFieldListEntry> mFieldList;
};

extern sFieldDebugInfo gFieldDebugInfo;

void fieldDebugInfo_load();
void fieldDebugInfo_save();

