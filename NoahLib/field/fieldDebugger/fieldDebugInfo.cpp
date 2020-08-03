#include "noahLib.h"
#include "fieldDebugInfo.h"
#include "tinyxml2.h"
#include "fieldNames.h"

const char fieldDebugFileName[] = "debuginfo/field.xml";

sFieldDebugInfo gFieldDebugInfo;

void fieldDebugInfo_load()
{
    tinyxml2::XMLDocument fieldDebugInfo;
    if (fieldDebugInfo.LoadFile(fieldDebugFileName) != tinyxml2::XML_SUCCESS)
    {
        // build debug field debug info

        gFieldDebugInfo.mFieldList.clear();
        gFieldDebugInfo.mFieldList.resize(730);

        for (int i=0; i< gFieldDebugInfo.mFieldList.size(); i++)
        {
            const sFieldName* pFieldName = findFieldName(i);
            if (pFieldName)
            {
                char buffer[1024];
                sprintf(buffer, "%s", pFieldName->fieldName);

                // trim spaces at the end
                for (int k=strlen(buffer) - 1; k>=0; k--)
                {
                    if (buffer[k] == ' ')
                    {
                        buffer[k] = '\0';
                    }
                    else
                    {
                        break;
                    }
                }

                gFieldDebugInfo.mFieldList[i].mName = buffer;
            }
        }

        fieldDebugInfo_save();
    }
    else
    {
        // load fields
        tinyxml2::XMLElement* fieldListXML = fieldDebugInfo.FirstChildElement("fieldList");
        tinyxml2::XMLElement* fieldInfoXML = fieldListXML->FirstChildElement("fieldInfo");

        while(fieldInfoXML)
        {
            sFieldListEntry fieldInfo;
            const char* nameAttribute = nullptr;
            fieldInfoXML->QueryStringAttribute("Name", &nameAttribute);
            fieldInfo.mName = nameAttribute;

            // script entities
            tinyxml2::XMLElement* scriptEntityXML = fieldInfoXML->FirstChildElement("scriptEntity");
            while (scriptEntityXML)
            {
                sFieldActor scriptEntity;
                const char* nameAttribute = nullptr;
                scriptEntityXML->QueryStringAttribute("Name", &nameAttribute);
                scriptEntity.mName = nameAttribute;

                assert(fieldInfo.mScriptEntities.size() == scriptEntityXML->IntAttribute("ID"));
                fieldInfo.mScriptEntities.push_back(scriptEntity);
                scriptEntityXML = scriptEntityXML->NextSiblingElement("scriptEntity");
            }

            assert(gFieldDebugInfo.mFieldList.size() == fieldInfoXML->IntAttribute("ID"));
            gFieldDebugInfo.mFieldList.push_back(fieldInfo);
            fieldInfoXML = fieldInfoXML->NextSiblingElement("fieldInfo");
        }
    }
}

void fieldDebugInfo_save()
{
    tinyxml2::XMLDocument fieldDebugInfo;

    // write global

    // write fields
    tinyxml2::XMLElement* fieldListXML = fieldDebugInfo.NewElement("fieldList");
    fieldDebugInfo.InsertEndChild(fieldListXML);

    for (int i=0; i<gFieldDebugInfo.mFieldList.size(); i++)
    {
        tinyxml2::XMLElement* fieldInfoXML = fieldListXML->InsertNewChildElement("fieldInfo");
        const sFieldListEntry& fieldInfo = gFieldDebugInfo.mFieldList[i];

        fieldInfoXML->SetAttribute("ID", i);
        fieldInfoXML->SetAttribute("Name", fieldInfo.mName.c_str());

        // script entities
        for (int j=0; j<fieldInfo.mScriptEntities.size(); j++)
        {
            tinyxml2::XMLElement* scriptEntityXML = fieldInfoXML->InsertNewChildElement("scriptEntity");
            const sFieldActor& scriptEntity = fieldInfo.mScriptEntities[j];

            scriptEntityXML->SetAttribute("ID", j);
            scriptEntityXML->SetAttribute("Name", scriptEntity.mName.c_str());
        }
    }

    fieldDebugInfo.SaveFile(fieldDebugFileName);

}
