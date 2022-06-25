#pragma once


struct sWorldTaskDefinition {
    s32(*m0_init)(s32 index);
    s32(*m4_update)(s32 index);
};

extern const std::vector<sWorldTaskDefinition> worldmapMode0_tasks;

void callAllWorlmapObjects();
void setupWorldmapTask(s32(*init)(s32 index), s32(*update)(s32 index));
