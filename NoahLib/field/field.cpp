#include "noahLib.h"
#include "field.h"
#include "kernel/filesystem.h"
#include "kernel/decompress.h"
#include "kernel/graphics.h"
#include "kernel/gameState.h"
#include "walkMesh.h"
#include "fieldModel.h"
#include "kernel/isoManager.h"
#include "fieldDebugger/fieldInspector.h"
#include "fieldScriptSupport.h"
#include "opcodeTables.h"
#include "fieldGraphicObject.h"
#include "sprite/spriteSetup.h"
#include "kernel/trigo.h"

struct sFieldVramMapping
{
	s16 m0_vramX;
	s16 m2_vramY;
	s16 m4;
	s16 m6;
};
std::array<sFieldVramMapping, 32> fieldVramMapping;

std::vector<u8> rawFieldBundle;
std::vector<u8> rawFieldModels;
std::vector<u8> rawFieldScriptData;
std::vector<u8> rawFieldTriggerData;
std::vector<u8> rawFieldDialogBundle;
std::vector<u8> rawFieldWalkMesh;
std::vector<u8> rawFieldActorSetupParams;

// this is malloc/free in original code, I keep it around for debugger
std::vector<u8> rawFieldImageBundle;
std::vector<u8> rawFieldImageBundle2;


std::array<s16, 4> fieldInitVar1;

s32 totalObjects;

s32 fieldMapNumber = -1;
s32 currentFieldId0 = -1;
s32 currentFieldId1 = -1;

s32 totalActors;

std::array<s32, 3> currentParty;

std::vector<sFieldEntity> actorArray;

std::vector<sFieldActorSetupParams> fieldActorSetupParams;

s16 numWalkMesh = 0;
std::array<s32, 5> walkMeshNumTrianglePerBlock;
u8* walkMeshVar1Raw;
std::vector<u32>* walkMeshVar1;
std::array<u8*, 5> walkMeshVerticesRaw;
std::array<u8*, 5> walkMeshTriangleRaw;
std::array<std::vector<SVECTOR>*, 5> walkMeshVertices;
std::array<std::vector<sWalkMesh::sTriangleData>*, 5> walkMeshTriangle;
s32 walkMeshVar4 = 0;

s16 isFogSetup = 0;
s16 cameraTan = 0;

int playerControlledEntity = 0;

s16 entityMoveSpeedFactor = 0;

void fieldModelRelocation(std::vector<u8>::iterator& pModelData)
{
	MissingCode();
}

s32 fieldChangePrevented = -1;
s32 fieldChangePrevented2 = -1;
s32 fieldTransitionMode = 0;

void resetFieldDefault()
{
	MissingCode();

	fieldTransitionMode = 2;

	MissingCode();

	entityMoveSpeedFactor = 0x1000;

	MissingCode();

	fieldChangePrevented = -1;

	MissingCode();
}

void uploadFieldImages(std::vector<u8>::iterator& pImageData)
{
	MissingCode();
}

struct RECT
{
	s16 m0_x;
	s16 m2_y;
	s16 m4_w;
	s16 m6_h;
};

RECT* currentNpcSpriteUploadRect = nullptr;
std::vector<u8>::iterator currentNpcSpriteUploadDataPtr;

void transfertNpcSpriteSheetElement()
{
	MissingCode();
}

void uploadNpcSpriteSheet(std::vector<u8>::iterator& pImageData, int x, int y)
{
	int count = READ_LE_U32(pImageData);

	int xOffset = 0;

	for (int i = 0; i < count; i++)
	{
		int offset = READ_LE_U32(pImageData + 4 + i * 4);
		std::vector<u8>::iterator data = pImageData + offset;

		RECT rect;
		rect.m0_x = x + xOffset;
		rect.m2_y = y;
		rect.m4_w = READ_LE_U16(data);
		rect.m6_h = READ_LE_U16(data + 2);

		xOffset += 0x40;

		currentNpcSpriteUploadDataPtr = data + 4;
		currentNpcSpriteUploadRect = &rect;

		transfertNpcSpriteSheetElement();
	}

	MissingCode();
}

void setupField3d(std::vector<u8>::iterator input)
{
	MissingCode();
}


long NCLIP(sFP1616 sxy0, sFP1616 sxy1, sFP1616 sxy2)
{
	s16 SX0;
	s16 SY0;
	s16 SX1;
	s16 SY1;
	s16 SX2;
	s16 SY2;

	sxy0.get(SY0, SX0);
	sxy1.get(SY1, SX1);
	sxy2.get(SY2, SX2);

	return ((int64_t)(SX0 * SY1) + (SX1 * SY2) + (SX2 * SY0) - (SX0 * SY2) - (SX1 * SY0) - (SX2 * SY1));
}

void OuterProduct12(VECTOR* a, VECTOR* b, VECTOR* r)
{
	r->vx = a->vy * b->vz - a->vz * b->vy;
	r->vy = a->vz * b->vx - a->vx * b->vz;
	r->vz = a->vx * b->vy - a->vy * b->vx;
}

void toFloat(VECTOR* v0, std::array<float, 3>& output)
{
	output[0] = v0->vx / (float)0x10000;
	output[1] = v0->vy / (float)0x10000;
	output[2] = v0->vz / (float)0x10000;
}

void fromFloat(VECTOR* v0, std::array<float, 3>& asFloat)
{
	v0->vx = asFloat[0] * (float)0x10000;
	v0->vy = asFloat[1] * (float)0x10000;
	v0->vz = asFloat[2] * (float)0x10000;
}

int VectorNormal(VECTOR* v0, VECTOR* v1)
{
	std::array<float, 3> fb0;

	toFloat(v0, fb0);

	float sum = (fb0[0] * fb0[0]) + (fb0[1] * fb0[1]) + (fb0[2] * fb0[2]);
	float lenght = sqrtf(sum);

	fb0[0] /= lenght;
	fb0[1] /= lenght;
	fb0[2] /= lenght;

	fromFloat(v1, fb0);

	return (v0->vx * v0->vx) + (v0->vy * v0->vy) + (v0->vz * v0->vz);
}

void projectPositionOnTriangle(const SVECTOR& vec0, const SVECTOR& vec1, const SVECTOR& vec2, SVECTOR* outputPosition, VECTOR* inputPosition)
{
	VECTOR sStack72;
	VECTOR sStack56;
	VECTOR local_28;

	local_28.vx = vec1.vx - vec0.vx;
	local_28.vy = vec1.vy - vec0.vy;
	local_28.vz = vec1.vz - vec0.vz;
	VectorNormal(&local_28, &sStack72);

	local_28.vx = vec2.vx - vec0.vx;
	local_28.vy = vec2.vy - vec0.vy;
	local_28.vz = vec2.vz - vec0.vz;
	VectorNormal(&local_28, &sStack56);
	OuterProduct12(&sStack72, &sStack56, inputPosition);
	if (inputPosition->vy == 0) {
		outputPosition->vy = 0;
	}
	else {
		outputPosition->vy = vec0.vy + (short)((-(inputPosition->vz * (outputPosition->vz - vec0.vz)) - inputPosition->vx * (outputPosition->vx - vec0.vx)) / inputPosition->vy);
	}
}

s16 findTriangleInWalkMesh(int posX, int posZ, int walkmeshId, SVECTOR* param_4, VECTOR* param_5)
{
	SVECTOR resultPosition;

	resultPosition.vx = (short)posX;
	resultPosition.vy = 0;
	resultPosition.vz = (short)posZ;

	std::vector<sWalkMesh::sTriangleData>::iterator pTriangle = walkMeshTriangle[walkmeshId]->begin();
	std::vector<SVECTOR>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();

	sFP1616 refPos;
	refPos.set(posX, posZ);

	for (int i = 0; i < walkMeshNumTrianglePerBlock[walkmeshId]; i++)
	{
		sFP1616 pos0;
		sFP1616 pos1;
		sFP1616 pos2;

		pos0.set(pVertices[pTriangle->m0_verticeIndex[0]].vx, pVertices[pTriangle->m0_verticeIndex[0]].vz);
		pos1.set(pVertices[pTriangle->m0_verticeIndex[1]].vx, pVertices[pTriangle->m0_verticeIndex[1]].vz);
		pos2.set(pVertices[pTriangle->m0_verticeIndex[2]].vx, pVertices[pTriangle->m0_verticeIndex[2]].vz);

		if ((NCLIP(pos0, pos1, refPos) > -1) && (NCLIP(pos1, pos2, refPos) > -1) && (NCLIP(pos2, pos0, refPos) > -1))
		{
			SVECTOR vec0 = pVertices[pTriangle->m0_verticeIndex[0]];
			SVECTOR vec1 = pVertices[pTriangle->m0_verticeIndex[1]];
			SVECTOR vec2 = pVertices[pTriangle->m0_verticeIndex[2]];

			projectPositionOnTriangle(vec0, vec1, vec2, &resultPosition, param_5);
			param_4->vx = resultPosition.vx;
			param_4->vy = resultPosition.vy;
			param_4->vz = resultPosition.vz;

			return i;
		}

		pTriangle++;
	}

	param_4->vx = 0;
	param_4->vy = 0;
	param_4->vz = 0;

	param_5->vx = 0;
	param_5->vy = 0;
	param_5->vz = 0;
	return 0;
}

int getWalkmeshTriangleFlag(sFieldScriptEntity* param_1)
{
	if ((param_1->m4_flags >> (param_1->m10_walkmeshId + 3U & 0x1f) & 1) == 0) {
		int triangleId = param_1->m8_currentWalkMeshTriangle[param_1->m10_walkmeshId];
		int materialId = (*walkMeshTriangle[param_1->m10_walkmeshId])[triangleId].mC_indexInWalkmeshData1;
		return (*walkMeshVar1)[materialId];
	}
	return 0;
}

void initFieldScriptEntityValues(int index)
{
	sFieldScriptEntity* pFieldScriptEntity = actorArray[index].m4C_scriptEntity;

	pFieldScriptEntity->m0_flags = 0xB0;
	pFieldScriptEntity->m4_flags = 0x800;
	pFieldScriptEntity->m18[0] = 0x10;
	pFieldScriptEntity->m18[2] = 0x10;
	pFieldScriptEntity->m18[1] = 0x60;
	pFieldScriptEntity->m74 = -1;

	pFieldScriptEntity->m40.vx = 0;
	pFieldScriptEntity->m40.vy = 0;
	pFieldScriptEntity->m40.vz = 0;

	pFieldScriptEntity->m30_stepVector.vx = 0;
	pFieldScriptEntity->m30_stepVector.vy = 0;
	pFieldScriptEntity->m30_stepVector.vz = 0;

	pFieldScriptEntity->m60[2] = 0;
	pFieldScriptEntity->m60[0] = 0;
	pFieldScriptEntity->m60[1] = 0;

	pFieldScriptEntity->mD0_targetPositionOffset[0] = 0;
	pFieldScriptEntity->mD0_targetPositionOffset[1] = 0;
	pFieldScriptEntity->mD0_targetPositionOffset[2] = 0;
	pFieldScriptEntity->mE6 = 0;
	pFieldScriptEntity->mEA_currentAnimationPlaying = 0xff;

	pFieldScriptEntity->mE2 = 0;
	pFieldScriptEntity->mCC_scriptPC = 0;
	pFieldScriptEntity->m6E = 0;
	pFieldScriptEntity->m12C_flags &= 0xffffffdf;
	pFieldScriptEntity->m11E = 0x200;
	pFieldScriptEntity->m18[3] = pFieldScriptEntity->m18[0];
	pFieldScriptEntity->m12C_flags &= 0xfffffffc;
	pFieldScriptEntity->mFC[5] = 0x80;
	pFieldScriptEntity->mFC[4] = 0x80;
	pFieldScriptEntity->mFC[3] = 0x80;
	pFieldScriptEntity->mFC[2] = 0x80;
	pFieldScriptEntity->mFC[1] = 0x80;
	pFieldScriptEntity->mFC[0] = 0x80;
	pFieldScriptEntity->m128 = -1;
	pFieldScriptEntity->m12C_flags &= 0xfffcffff;
	pFieldScriptEntity->m130.m0 = 0;
	pFieldScriptEntity->m12C_flags &= 0xf003ffff;

	for (int i = 0; i < 8; i++)
	{
		pFieldScriptEntity->m8C_scriptSlots[i].m2_delay = 0;
		pFieldScriptEntity->m8C_scriptSlots[i].m0_scriptPC = -1;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m16 = 0;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m18 = 0xF;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m22 = 0;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m23_walkMode = 0;
		pFieldScriptEntity->m8C_scriptSlots[i].m4_flags.m0 = -1;
	}

	delete pFieldScriptEntity->m120_special2dAnimation;
	pFieldScriptEntity->m120_special2dAnimation = nullptr;
	pFieldScriptEntity->m120_special2dAnimationRaw.clear();
	pFieldScriptEntity->mE4_playableCharacterId = 0xFF;
	pFieldScriptEntity->m76 = 0x100;

	pFieldScriptEntity->m82[1] = 0;
	pFieldScriptEntity->m82[0] = 0;
	pFieldScriptEntity->m88[1] = 0;
	pFieldScriptEntity->m88[0] = 0;
	pFieldScriptEntity->m84 = 0;
	pFieldScriptEntity->mCF = 0;
	pFieldScriptEntity->mCE_currentScriptSlot = 0;
	pFieldScriptEntity->mE8 = 0;
	pFieldScriptEntity->m10_walkmeshId = 0;
	pFieldScriptEntity->mEC_elevation = 0;

	pFieldScriptEntity->m134.m7 = 0;

	pFieldScriptEntity->m12C_flags &= 0xffffe03f;

	pFieldScriptEntity->m134.m5 = 0;

	pFieldScriptEntity->m102_rotationCount = rand();

	pFieldScriptEntity->mF4_scale3d[0] = 0x1000;
	pFieldScriptEntity->mF4_scale3d[1] = 0x1000;
	pFieldScriptEntity->mF4_scale3d[2] = 0x1000;
	pFieldScriptEntity->m10D = 0xff;
	pFieldScriptEntity->m80_DialogAvatarFace = 0xff;
	pFieldScriptEntity->m106_currentRotation = -0x8000;
	pFieldScriptEntity->m104_rotation = -0x8000;
	pFieldScriptEntity->m108_rotation3 = -0x8000;
	pFieldScriptEntity->m124_special2dAnimationId = -1;
	pFieldScriptEntity->mE3 = 0;
	pFieldScriptEntity->m8_currentWalkMeshTriangle[3] = 0;
	pFieldScriptEntity->m8_currentWalkMeshTriangle[2] = 0;
	pFieldScriptEntity->m8_currentWalkMeshTriangle[1] = 0;
	pFieldScriptEntity->m8_currentWalkMeshTriangle[0] = 0;
	pFieldScriptEntity->m12C_flags &= 0xffffffe3;

	SVECTOR local_48[4];
	VECTOR local_88[4];

	for (int i = 0; i < numWalkMesh; i++)
	{
		pFieldScriptEntity->m8_currentWalkMeshTriangle[i] = findTriangleInWalkMesh(actorArray[index].mC_matrix.t[0], actorArray[index].mC_matrix.t[2], i, &local_48[i], &local_88[i]);
		if ((pFieldScriptEntity->m8_currentWalkMeshTriangle[i] != -1) && (walkMeshNumTrianglePerBlock[i] <= pFieldScriptEntity->m8_currentWalkMeshTriangle[i]))
		{
			// seems like this would be a bug
			assert(0);

			walkMeshNumTrianglePerBlock[i] = 0;
			local_88[i].vx = 0;
			local_88[i].vy = 0;
			local_88[i].vz = 0;
			local_48[i].vx = 0;
			local_48[i].vy = 0;
			local_48[i].vz = 0;
		}
	}

	pFieldScriptEntity->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pFieldScriptEntity);

	pFieldScriptEntity->m50.vx = local_88[pFieldScriptEntity->m10_walkmeshId].vx;
	pFieldScriptEntity->m50.vy = local_88[pFieldScriptEntity->m10_walkmeshId].vy;
	pFieldScriptEntity->m50.vz = local_88[pFieldScriptEntity->m10_walkmeshId].vz;

	if ((actorArray[index].m58_flags & 0x80) == 0) {
		actorArray[index].mC_matrix.t[1] = local_48[pFieldScriptEntity->m10_walkmeshId].vy;
	}

	(pFieldScriptEntity->m20_position).vx = (int)actorArray[index].mC_matrix.t[0] << 0x10;
	(pFieldScriptEntity->m20_position).vy = (int)actorArray[index].mC_matrix.t[1] << 0x10;
	(pFieldScriptEntity->m20_position).vz = (int)actorArray[index].mC_matrix.t[2] << 0x10;

	pFieldScriptEntity->m72_elevation = actorArray[index].mC_matrix.t[1];
}

void initFieldScriptEntity2dSprite(sFieldEntity2dSprite* pSprite)
{
	MissingCode();
}

int numInitializedFieldScriptEntities = 0;

void initFieldScriptEntity(int index)
{
	if (index < totalActors)
	{
		numInitializedFieldScriptEntities++;
		sFieldScriptEntity* pNewFieldScriptEntity = new sFieldScriptEntity;
		actorArray[index].m4C_scriptEntity = pNewFieldScriptEntity;
		memset(pNewFieldScriptEntity, 0, sizeof(sFieldScriptEntity));
		new(pNewFieldScriptEntity) sFieldScriptEntity;

		actorArray[index].m5A = 0;

		if (actorArray[index].m58_flags & 0x2000)
		{
			pNewFieldScriptEntity->m118.resize(32);
			MissingCode();
		}

		initFieldScriptEntityValues(index);
		actorArray[index].m8_2dSprite = new sFieldEntity2dSprite;
		initFieldScriptEntity2dSprite(actorArray[index].m8_2dSprite);
	}
}

void traceModelFunctionState(int state)
{
	MissingCode();
}

void initModel1(sModelBlock& pModelBlock, std::vector<s16>& outputBuffer, std::vector<s16>::iterator& outputBufferEnd)
{
	traceModelFunctionState(0x25);

	outputBuffer.resize(pModelBlock.m34_count);
	outputBufferEnd = outputBuffer.end();
}

int fieldScriptEntityAlreadyInitialized = 0;

// 0: scenarioFlag
std::array<s16, 1024> fieldVars;

void setVar(int varIndex, s16 value)
{
	fieldVars[varIndex / 2] = value;
}

void setVarsForCurrentParty()
{
	setVar(0x3E, currentParty[0]);
	setVar(0x40, currentParty[1]);
	setVar(0x42, currentParty[2]);
}

sFieldEntity* pCurrentFieldEntity;
int fieldScriptInitVar0;
int currentScriptFinished;

u16 getScriptEntryPoint(int entityId, int scriptIndex)
{
	return READ_LE_U16(rawFieldScriptData.begin() + (entityId * 0x20 + scriptIndex) * 2 + 0x84);
}

int breakCurrentScript;
int fieldExectuteMaxCycles;

int fieldDebugDisable = 0;
int renderCharsDisabled = 0;

void sprintf_screen(const char* format, ...)
{
	MissingCode();
}

void initModel3(int, int)
{
	MissingCode();
}

void execSpriteCallbacks2()
{
	MissingCode();
}

void OP_INIT_ENTITY_SCRIPT_sub0(int actorId, int clutYEntry, sFieldActorSetupParams* pSetup, int param_4, int clutXEntry, int param_6, int param_7)
{
	initModel3(8, 0);
	actorArray[actorId].m4C_scriptEntity->m127 = clutYEntry;
	actorArray[actorId].m4C_scriptEntity->m126 = param_6;
	actorArray[actorId].m4C_scriptEntity->m134.m0 = clutXEntry;
	actorArray[actorId].m4C_scriptEntity->m130.m28 = param_4;
	actorArray[actorId].m4C_scriptEntity->m134.m4 = param_7;

	if (param_4 == 0)
	{
		s16 vramX = fieldVramMapping[clutYEntry].m0_vramX;
		s16 vramY = fieldVramMapping[clutYEntry].m2_vramY;

		if (clutXEntry == 0)
		{
			if (actorArray[actorId].m5A & 1) {
				deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
			}

			actorArray[actorId].m4_pVramSpriteSheet = createFieldEntitySub4(pSetup, 0x100, ((clutYEntry + 480) * 0x10000) >> 0x10, vramX, vramY, 0x40);
		}
		else
		{
			if ((actorArray[actorId].m5A & 1U) != 0) {
				deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
			}
			actorArray[actorId].m4_pVramSpriteSheet = createFieldEntitySub4Ext(pSetup, (short)((clutXEntry * 0x10 + 0x100) * 0x10000 >> 0x10), (short)((uint)((clutYEntry + 0x1e0) * 0x10000) >> 0x10), vramX, vramY, 0x40, clutXEntry);
		}
	}
	else
	{
		int iVar4;
		short sVar5;

		if ((actorArray[actorId].m5A & 1U) != 0) {
			deleteFieldEntitySub4(actorArray[actorId].m4_pVramSpriteSheet);
		}
		if (param_4 == 1) {
			iVar4 = clutYEntry + 0xe0;
			sVar5 = 0x280;
		}
		else {
			iVar4 = clutYEntry + 0xe3;
			sVar5 = 0x2a0;
		}
		actorArray[actorId].m4_pVramSpriteSheet = createFieldEntitySub4(pSetup, 0x100, (short)((uint)(iVar4 * 0x10000) >> 0x10), sVar5, (short)clutYEntry * 0x40 + 0x100, 8);
		OP_INIT_ENTITY_SCRIPT_sub0Sub3(actorArray[actorId].m4_pVramSpriteSheet, 0x20);
	}

	actorArray[actorId].m5A |= 1;

	sFieldEntitySub4* pFieldEntitySub4 = actorArray[actorId].m4_pVramSpriteSheet;

	int temp0;
	int temp1;
	int temp2;
	OP_INIT_ENTITY_SCRIPT_sub0Sub4(pFieldEntitySub4, 0, &temp0, &temp1, &temp2);
	OP_INIT_ENTITY_SCRIPT_sub0Sub5(pFieldEntitySub4, 3);

	pFieldEntitySub4->m2C_scale = 0xC00;
	pFieldEntitySub4->m82 = 0x2000;

	if (!fieldScriptEntityAlreadyInitialized)
	{
		pFieldEntitySub4->m0_position.vx = actorArray[actorId].m4C_scriptEntity->m20_position.vx;
		pFieldEntitySub4->m0_position.vy = actorArray[actorId].m4C_scriptEntity->m20_position.vy;
		pFieldEntitySub4->m0_position.vz = actorArray[actorId].m4C_scriptEntity->m20_position.vz;
		pFieldEntitySub4->mC.vy = 0;
		pFieldEntitySub4->mC.vx = 0;
		pFieldEntitySub4->mC.vy = 0;
		pFieldEntitySub4->mC.vz = 0;
		pFieldEntitySub4->m1C = 0x10000;
		pFieldEntitySub4->m84 = actorArray[actorId].mC_matrix.t[1] & 0xFFFF;
		if (!param_4)
		{
			actorArray[actorId].m4C_scriptEntity->m18[1] = temp1 * 2;
		}
		else
		{
			actorArray[actorId].m4C_scriptEntity->m18[1] = 0x40;
		}
	}
	else
	{
		assert(0);
	}

	if (isFogSetup)
	{
		pFieldEntitySub4->m40 |= 0x40000;
	}

	OP_INIT_ENTITY_SCRIPT_sub0Sub6(pFieldEntitySub4, 0);
	OP_INIT_ENTITY_SCRIPT_sub0Sub7(pFieldEntitySub4, 0);
	initModel3(8, 0);

	pFieldEntitySub4->m7C->m14_actorId = actorId;
	OP_INIT_ENTITY_SCRIPT_sub0Sub8(pFieldEntitySub4, &fieldActorCallback);

	if (!param_7)
	{
		OP_INIT_ENTITY_SCRIPT_sub0Sub9(pFieldEntitySub4);
		execSpriteCallbacks2();
		if (pFieldEntitySub4->m7C->mC == 0xff) {
			(actorArray[actorId].m4C_scriptEntity)->mEA_currentAnimationPlaying = 0xff;
			(actorArray[actorId].m4C_scriptEntity)->m4_flags = (actorArray[actorId].m4C_scriptEntity)->m4_flags | 0x1000000;
			pFieldEntitySub4->m0_position.vx = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vx;
			pFieldEntitySub4->m0_position.vy = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vy;
			pFieldEntitySub4->m0_position.vz = ((actorArray[actorId].m4C_scriptEntity)->m20_position).vz;
		}
	}

	actorArray[actorId].mC_matrix.t[0] = actorArray[actorId].m4C_scriptEntity->m20_position.vx >> 16;
	actorArray[actorId].mC_matrix.t[1] = actorArray[actorId].m4C_scriptEntity->m20_position.vy >> 16;
	actorArray[actorId].mC_matrix.t[2] = actorArray[actorId].m4C_scriptEntity->m20_position.vz >> 16;

	actorArray[actorId].m2C_matrixBackup.t = actorArray[actorId].mC_matrix.t;

	pFieldEntitySub4->m84 = actorArray[actorId].mC_matrix.t[1] & 0xFFFF;

	pFieldEntitySub4->m0_position.vx = actorArray[actorId].m4C_scriptEntity->m20_position.vx;
	pFieldEntitySub4->m0_position.vy = actorArray[actorId].m4C_scriptEntity->m20_position.vy;
	pFieldEntitySub4->m0_position.vz = actorArray[actorId].m4C_scriptEntity->m20_position.vz;
	fieldScriptInitVar0++;
}

void OP_INIT_ENTITY_SCRIPT_sub1()
{
	actorArray[currentFieldActorId].mC_matrix.t[0] = actorArray[currentFieldActorId].m2C_matrixBackup.t[0] = pCurrentFieldScriptActor->m20_position.vx >> 16;
	actorArray[currentFieldActorId].mC_matrix.t[1] = actorArray[currentFieldActorId].m2C_matrixBackup.t[1] = pCurrentFieldScriptActor->m20_position.vy >> 16;
	actorArray[currentFieldActorId].mC_matrix.t[2] = actorArray[currentFieldActorId].m2C_matrixBackup.t[2] = pCurrentFieldScriptActor->m20_position.vz >> 16;

	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vx = pCurrentFieldScriptActor->m20_position.vx;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vy = pCurrentFieldScriptActor->m20_position.vy;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m0_position.vz = pCurrentFieldScriptActor->m20_position.vz;

	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vy = 0;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m84 = pCurrentFieldScriptActor->m20_position.vy >> 16;
	pCurrentFieldScriptActor->m72_elevation = pCurrentFieldScriptActor->m20_position.vy >> 16;
}

int asyncLoadingVar1 = 0xFF;
int asyncLoadingVar2 = 0;

int isAsyncLoadingProhibited()
{
	MissingCode();
	return 0;
}

int findFreePartySlot(int param_1, int* param_2)
{
	int iVar1;

	iVar1 = 0;
	while (true) {
		if (currentParty[iVar1] == param_1) {
			return -1;
		}
		if (currentParty[iVar1])
			break;
		iVar1 = iVar1 + 1;

		if (2 < iVar1) {
			return -1;
		}
	}
	*param_2 = iVar1;
	return 0;
}

int asyncPartyCharacterLoadingCharacterIndex = 0;
int asyncPartyCharacterLoadingIndex = 0;
int fieldExecuteVar1 = 0;
std::array<int, 3> asyncPartyCharacterLoadingTable;
std::vector<u8> asyncPartyCharacterLoadingBuffer;

int getGearForCharacter(int param_1)
{
	return pKernelGameState->m294[param_1].m78_partyData_gearNum;
}

void startPartyCharacterASyncLoading(int partyCharacter, int partySlot)
{
	size_t sVar1;
	void* pvVar2;
	int iVar3;
	int iVar4;

	asyncPartyCharacterLoadingCharacterIndex = partyCharacter;
	asyncPartyCharacterLoadingIndex = partySlot;
	setCurrentDirectory(4, 0);
	if (fieldExecuteVar1 == 0) {
		waitReadCompletion(0);
	}
	iVar3 = partyCharacter + 5;
	if ((fieldMapNumber & 0xc000) == 0) {
		sVar1 = getFileSizeAligned(iVar3);
		asyncPartyCharacterLoadingTable[asyncPartyCharacterLoadingIndex] = partyCharacter;

		asyncPartyCharacterLoadingBuffer.resize(sVar1);
	}
	else {
		iVar3 = getGearForCharacter(partyCharacter);
		iVar4 = iVar3 + 0x10;
		if (iVar3 == 0xff) {
			iVar4 = 0x10;
		}
		iVar3 = iVar4 + 5;
		sVar1 = getFileSizeAligned(iVar3);
		asyncPartyCharacterLoadingBuffer.resize(sVar1);
		asyncPartyCharacterLoadingTable[asyncPartyCharacterLoadingIndex] = iVar4;
	}
	readFile(iVar3, asyncPartyCharacterLoadingBuffer, 0, 0x80);
	if (fieldExecuteVar1 == 0) {
		waitReadCompletion(0);
	}
	asyncLoadingVar1 = 1;
	return;
}

std::array<std::vector<u8>, 3> partyCharacterBuffersRaw;
std::array<std::vector<u8>, 3> partyCharacterBuffersCompressed;
std::array<sFieldActorSetupParams, 3> partyCharacterBuffers;
s16 actorCameraTracked = 0;
s32 pcInitVar1 = 0;
std::array<int, 11> PCToActorArray;

const std::array<s8, 12> characterMappingTable = {
	0,1,2,3,4,5,6,7,8,2,6,0
};

s16 pcInitVar2 = 0;
std::array<int, 3> unkPartyTable;


u8 OPE7_param[3];


void setCurrentActor2DPosition(int posX, int posZ)
{
	VECTOR alStack136[4];
	SVECTOR auStack72[4];
	for (int i = 0; i < numWalkMesh; i++)
	{
		pCurrentFieldScriptActor->m8_currentWalkMeshTriangle[i] = findTriangleInWalkMesh(posX, posZ, i, &auStack72[i], &alStack136[i]);
	}

	MissingCode();

	actorArray[currentFieldActorId].mC_matrix.t[0] = posX;
	actorArray[currentFieldActorId].m2C_matrixBackup.t[0] = posX;

	MissingCode();

	actorArray[currentFieldActorId].mC_matrix.t[2] = posZ;
	actorArray[currentFieldActorId].m2C_matrixBackup.t[2] = posZ;

	MissingCode();

	pCurrentFieldScriptActor->m20_position.vx = posX << 16;
	pCurrentFieldScriptActor->m20_position.vz = posZ << 16;

	MissingCode();
}


const std::array<u16, 8> actorDirectionTable = {
	0x8C00,  0x8E00,  0x8000,  0x8200,
	0x8400,  0x8600,  0x8800,  0x8A00
};

const std::array<u16, 8> actorDirectionTable3 = {
	0x8C00,  0x8400,  0x8800,  0x8000,
	0x8A00,  0x8E00,  0x8600,  0x8200
};

int getCharacter(int param_1)
{
	int iVar1;

	iVar1 = currentParty[2];
	if ((((param_1 != 0xff) && (iVar1 = currentParty[1], param_1 != 0xfe)) && (iVar1 = currentParty[0], param_1 != 0xfd)) && (iVar1 = 0xff, param_1 != 0xfc)) {
		iVar1 = param_1;
	}
	return iVar1;
}

int findCharacterInParty(int param_1)
{
	int iVar1;
	int* piVar2;

	iVar1 = 0;
	if (param_1 != 0xff) {
		do {
			if (currentParty[iVar1] == 0xff) {
				return 0xffffffff;
			}
			if (currentParty[iVar1] == param_1) {
				return iVar1;
			}
			iVar1 = iVar1 + 1;
		} while (iVar1 < 3);
	}
	return 0xffffffff;
}

void updateScriptActor3dRotation(int index)
{
	MissingCode();
}

void Square0(sVec3* v0, sVec3* v1)
{
	v1->vx = powl(v0->vx, 2);
	v1->vy = powl(v0->vy, 2);
	v1->vz = powl(v0->vz, 2);
}

s32 SquareRoot0(s32 value)
{
	return sqrtl(value);
}

s32 length1d(s32 param_1)
{
	sVec3 local_28;
	sVec3 local_18;
	local_18.vx = param_1;

	Square0(&local_28, &local_18);
	return SquareRoot0(local_18.vx);
}

s32 length2d(s32 param_1, s32 param_2)
{
	sVec3 local_28;
	sVec3 local_18;
	local_18.vx = param_1;
	local_18.vy = param_2;
	local_18.vz = 0;
	Square0(&local_28, &local_18);
	return SquareRoot0(local_18.vx + local_18.vy);
}

int spriteWalkToPositionOrActor(int param_1)
{
	if ((actorArray[currentFieldActorId].m4C_scriptEntity->m4_flags & 0x2000) == 0)
	{
		if (actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 == 0)
		{
			actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 = 0x4000000 / pCurrentFieldScriptActor->m76;
		}
	}
	else
	{
		actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 = 0x8000000 / pCurrentFieldScriptActor->m76;
	}

	int local_30;
	int local_28;

	int lVar13 = 0;
	int lVar14 = 0;

	switch (pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode)
	{
	case 0:
		local_30 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
		local_28 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]);
		break;
	case 1:
		local_30 = getVar80(1, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]) + pCurrentFieldScriptActor->mD0_targetPositionOffset[0];
		local_28 = getVar40(3, pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 5]) + pCurrentFieldScriptActor->mD0_targetPositionOffset[2];
		break;
	case 2:
		if (readCharacter(1) == 0xFF)
		{
			return 0;
		}
		lVar14 = length1d(actorArray[readCharacter(1)].m4C_scriptEntity->m18[3] + pCurrentFieldScriptActor->m18[3]);
		local_30 = actorArray[readCharacter(1)].m4C_scriptEntity->m20_position.vx >> 16;
		local_28 = actorArray[readCharacter(1)].m4C_scriptEntity->m20_position.vz >> 16;
		if (pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC + 1] == playerControlledEntity)
		{
			pCurrentFieldScriptActor->m0_flags |= 0x200000;
		}
		break;
	case 3:
		local_30 = pCurrentFieldScriptActor->mD0_targetPositionOffset[0] + ((getAngleSin(getImmediateOrVariableUnsigned(1) & 0xfff) << 0xc) >> 0xc);
		local_28 = pCurrentFieldScriptActor->mD0_targetPositionOffset[2] + (getAngleCos(getImmediateOrVariableUnsigned(1) & 0xfff) * -0x1000 >> 0xc);
		break;
	default:
		assert(0);
	}

	sVec3 targetPosition;
	targetPosition[0] = local_30 - (pCurrentFieldScriptActor->m20_position.vx >> 16);
	targetPosition[2] = local_28 - (pCurrentFieldScriptActor->m20_position.vz >> 16);
	targetPosition[1] = 0;

	pCurrentFieldScriptActor->m0_flags |= 0x400000;

	// Reached destination yet?
	if ((pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 == 0) ||
		(length2d(targetPosition[0], targetPosition[2]) <= length1d(actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 >> 0xf) + 1 + lVar14))
	{
		//reached location
		if (param_1 == 0)
		{
			pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = fp_atan2(targetPosition);
		}
		else
		{
			if ((pCurrentFieldScriptActor->m0_flags & 0x8000) == 0)
			{
				pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m106_currentRotation | 0x8000;
			}
			else
			{
				pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m11C | 0x8000;
			}
		}

		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0 = 0xFFFF;
		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m23_walkMode = 0;
		pCurrentFieldScriptActor->m0_flags &= 0xfddff7ff;
		return 0;
	}
	else
	{
		pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m4_flags.m0--;
		pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation = fp_atan2(targetPosition);
		breakCurrentScript = 1;
		return -1;
	}
}

void resetFieldScriptActor()
{
	breakCurrentScript = 1;
	pCurrentFieldScriptActor->m30_stepVector.vx = 0;
	pCurrentFieldScriptActor->m30_stepVector.vy = 0;
	pCurrentFieldScriptActor->m30_stepVector.vz = 0;
	pCurrentFieldScriptActor->m40.vx = 0;
	pCurrentFieldScriptActor->m40.vy = 0;
	pCurrentFieldScriptActor->m40.vz = 0;
	pCurrentFieldScriptActor->m104_rotation = pCurrentFieldScriptActor->m106_currentRotation = pCurrentFieldScriptActor->m104_rotation | 0x8000;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vx = 0;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->mC.vz = 0;
	actorArray[currentFieldActorId].m4_pVramSpriteSheet->m18 = 0;
}


void SetGeomScreen(s32)
{
	MissingCode();
}

void SetGeomOffset(s32, s32)
{
	MissingCode();
}

s32 opA0_var0 = 0;
s32 opA0_var1 = 0;

s32 desiredCameraPosition[3] = { 0,0,0 };

s32 cameraTargetOverride[3] = { 0,0,0 };
s32 desiredCameraTarget[3] = { 0,0,0 };
s32 cameraPositionOverride[3] = { 0,0,0 };
s32 cameraInterpolationTargetStep[3] = { 0,0,0 };
s32 cameraInterpolationTargetStartPosition[3] = { 0,0,0 };
s32 cameraInterpolationPositionStep[3] = { 0,0,0 };
s32 cameraInterpolationStartPosition[3] = { 0,0,0 };
s32 cameraEye[3] = { 0,0,0 };
s32 cameraAt[3] = { 0,0,0 };
VECTOR cameraEye2;
VECTOR cameraAt2;
u16 cameraInterpolationFlags = 0;
s32 cameraInterpolationTargetNumSteps = 0;
s32 cameraInterpolationPositionNumSteps = 0;

s16 cameraRotation = 0;
void setCurrentActorRotation2(s16 param_1)
{
	int iVar1;
	sFieldScriptEntity* psVar2;
	ushort uVar3;

	psVar2 = pCurrentFieldScriptActor;
	iVar1 = fieldExecuteVar1;
	uVar3 = param_1 - cameraRotation & 0xfffU | 0x8000;
	pCurrentFieldScriptActor->m104_rotation = uVar3;
	psVar2->m106_currentRotation = uVar3;
	if (iVar1 == 0) {
		psVar2->m108_rotation3 = uVar3;
	}
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}


int getCurrentActorRotation()
{
	return (pCurrentFieldScriptActor->m106_currentRotation + 0x100 >> 9) + 2U & 7;
}

void setCurrentActorTargetRotation(s16 param_1)
{
	if (fieldExecuteVar1 == 0) {
		pCurrentFieldScriptActor->m104_rotation = param_1 | 0x8000;
		pCurrentFieldScriptActor->m106_currentRotation = param_1 | 0x8000;
		pCurrentFieldScriptActor->m108_rotation3 = param_1 | 0x8000;
	}
	pCurrentFieldScriptActor = pCurrentFieldScriptActor;
	pCurrentFieldScriptActor->m104_rotation = param_1 | 0x8000U;
	pCurrentFieldScriptActor->m106_currentRotation = param_1 | 0x8000U;
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}


int playMusicAuthorized = 0;
int musicVar1 = -1;
int musicVar2 = 0;
int currentlyPlayingMusic = 255;

void clearMusic()
{
	MissingCode();
}

void clearMusic2()
{
	MissingCode();
}

int isLoadCompleted()
{
	MissingCode();
	return 0;
}

void playMusic(int musicId, int)
{
	MissingCode();
}

s32 fieldExecuteVar2 = 0;
s32 fieldExecuteVar3 = 0;
s16 fieldRandomBattleVar = 0;


void setCurrentActorElevation(short param_1)
{
	sFieldScriptEntity* psVar1;

	psVar1 = pCurrentFieldScriptActor;
	(pCurrentFieldScriptActor->m20_position).vy = (uint)param_1 << 0x10;
	psVar1->mEC_elevation = param_1;
	psVar1->m72_elevation = param_1;
	return;
}




s16 OPX47Var = 0;
void OPX_47()
{
	OPX47Var = getImmediateOrVariableUnsigned(1);
	pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC + 3;
}

s32 load2dAnimVar = 0;
s32 loadCompleted = 0;


void setupSpecialAnimation(sFieldEntitySub4* param_1, sFieldActorSetupParams* param_2)
{
	param_1->m4C_specialAnimation = param_2;
	return;
}




void updateGearState(int param_1)

{
	ushort uVar2;
	int uVar1;

	uVar2 = (ushort)fieldMapNumber;
	if (param_1 == 1) {
		setVar(0x30, uVar2 & 0xfff);
		setVar(0x32, 0);
		uVar1 = 0x34;
	}
	else {
		if (param_1 < 2) {
			if (param_1 != 0) {
				return;
			}
			setVar(0x2a, uVar2 & 0xfff);
			setVar(0x2c, 0);
			uVar1 = 0x2e;
		}
		else {
			if (param_1 != 2) {
				return;
			}
			setVar(0x36, uVar2 & 0xfff);
			setVar(0x38, 0);
			uVar1 = 0x3a;
		}
	}
	setVar(uVar1, 0);
	return;
}

u8 OPX_50Param = 0;
u8 OPX_52Param = 0;
u16 OPX_80Params[8] = { 0,0,0,0,0,0,0,0 };
s32 OPX_81Params[3] = { 0,0,0 };
s8 OPX_82Param0[4] = { 0,0,0,0 };
s8 OPX_82Param1[4] = { 0,0,0,0 };
s8 OPX_82Param2[4] = { 0,0,0,0 };
s16 OPX_82Param3[3] = { 0,0,0 };
s16 OPX_82Param4 = 0;
s32 OPX_86Param = 0;
s32 OPX8E_param0 = 0;
s32 OPX8E_param1 = 0;

int getCurrentDiscNumber()
{
	MissingCode();
	return 0;
}



s8 OPX_E0Param = 0;

void OPX_13Sub(int)
{
	MissingCode();
}


u16 padButtonForScripts = 0;


int findDialogWindowForCurrentActor(int*)
{
	MissingCode();
	return -1;
}



int isScriptAlreadyRunning(sFieldScriptEntity* pEntity, int scriptIndex)
{
	for (int i = 0; i < 8; i++)
	{
		if (pEntity->m8C_scriptSlots[i].m3_scriptIndex == scriptIndex)
		{
			return -1;
		}
	}
	return 0;
}



void setupRGBCalcMode(int, int, int, int, int, int)
{
	MissingCode();
}

void executeFieldScript(int param)
{
	breakCurrentScript = 0;
	fieldExectuteMaxCycles = param;
	int cycles = 0;
	if (param > 0)
	{
		while (cycles <= 0x400)
		{
			//printf("PC: 0x%04X ", pCurrentFieldScriptActor->mCC_scriptPC);
			u8 opcodeId = pCurrentFieldScriptFile[pCurrentFieldScriptActor->mCC_scriptPC];

			printf("Actor0x%02X: offset 0x%04X OP_%02X\n", currentFieldActorId, pCurrentFieldScriptActor->mCC_scriptPC, opcodeId);

			if (fieldScriptOpcodes[opcodeId] == nullptr)
			{
				breakCurrentScript = 1;
				return;
			}

			//_CrtCheckMemory();
			fieldScriptOpcodes[opcodeId]();
			//_CrtCheckMemory();

			if (currentScriptFinished == 0) {
				fieldExectuteMaxCycles = 0xffff;
			}
			if (fieldExecuteVar1 != 0) {
				if (fieldExecuteVar2 == 0) {
					return;
				}
				if (fieldExecuteVar3 == 0) {
					return;
				}
				if (fieldChangePrevented == 0) {
					return;
				}
			}
			if ((breakCurrentScript == 1) && (currentScriptFinished == 1)) {
				return;
			}

			cycles++;
			if (cycles >= fieldExectuteMaxCycles)
			{
				return;
			}
		}

		if (fieldDebugDisable == 0) {
			sprintf_screen("EVENTLOOP ERROR ACT=%d\n", currentFieldActorId);
		}
	}
}

void startAllEntityScripts()
{
	if (fieldScriptEntityAlreadyInitialized == 0)
	{
		setVar(0x10, 0);
		setVarsForCurrentParty();

		for (int i = 0; i < totalActors; i++)
		{
			// that was probably some macro
			pCurrentFieldEntity = &actorArray[i];
			pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
			currentFieldActorId = i;
			pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 2); // the update script

			// Does the entry point have any code?
			if ((READ_LE_U16(pCurrentFieldScriptFile + pCurrentFieldScriptActor->mCC_scriptPC) == 0))
			{
				pCurrentFieldScriptActor->m4_flags |= 0x4000000;
			}

			// again, this time for the init script
			pCurrentFieldEntity = &actorArray[i];
			pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
			currentFieldActorId = i;
			pCurrentFieldScriptActor->mCC_scriptPC = getScriptEntryPoint(i, 0); // the init script

		}

		// execute the init script
		for (int i = 0; i < totalActors; i++)
		{
			pCurrentFieldEntity = &actorArray[i];
			pCurrentFieldScriptActor = pCurrentFieldEntity->m4C_scriptEntity;
			fieldScriptInitVar0 = 0;
			currentScriptFinished = 0;
			currentFieldActorId = i;

			executeFieldScript(0xFFFF);

			if (fieldScriptInitVar0 == 0)
			{
				OP_INIT_ENTITY_SCRIPT_sub0(i, 0, &fieldActorSetupParams[0], 0, 0, 0x80, 0);
				pCurrentFieldScriptActor->m4_flags |= 0x800;
			}
		}
	}
	else
	{
		assert(0);
	}
}

struct sShapeTransfert
{
	RECT m0_rect;
	sPS1Pointer m8_pData;
	sShapeTransfert* mC_pNext;
	// size 0x10
};

int shapeTransfertTableSize = 0;
sShapeTransfert* shapeTransfertBuffer[2] = { nullptr, nullptr };
void* shapeTransfertTemporaryBuffersLinkedLists[2] = { nullptr, nullptr };

sShapeTransfert* shapeTransfertTableCurrentEntry;
int shapeTransfertDoubleBufferIndex = 0;
sShapeTransfert* shapeTransfertTableStart;
sShapeTransfert* shapeTransfertTableEnd;
std::array<sShapeTransfert*, 2>shapeTransfertTable;

void resetSpriteTransfertList()
{
	spriteTransfertListHead = nullptr;
}

void allocateShapeTransfert(int param_1)
{
	shapeTransfertTableSize = param_1;
	shapeTransfertBuffer[0] = new sShapeTransfert[param_1];
	shapeTransfertBuffer[1] = new sShapeTransfert[param_1];
	shapeTransfertTemporaryBuffersLinkedLists[1] = nullptr;
	shapeTransfertTemporaryBuffersLinkedLists[0] = nullptr;
	shapeTransfertTable[0] = nullptr;
	resetSpriteTransfertList();
	return;
}

void addToShapeTransfertTable(sPS1Pointer pData, short x, short y, short w, short h)
{
	sShapeTransfert** ppsVar1;
	int iVar2;
	sShapeTransfert* psVar3;

	psVar3 = shapeTransfertTableCurrentEntry;
	iVar2 = shapeTransfertDoubleBufferIndex;
	if (shapeTransfertTableCurrentEntry + 1 < shapeTransfertTableEnd) {
		shapeTransfertTableCurrentEntry->m0_rect.m0_x = x;
		shapeTransfertTableCurrentEntry->m0_rect.m2_y = y;
		shapeTransfertTableCurrentEntry->m0_rect.m4_w = w;
		shapeTransfertTableCurrentEntry->m0_rect.m6_h = h;
		shapeTransfertTableCurrentEntry->m8_pData = pData;
		ppsVar1 = &shapeTransfertTableCurrentEntry->mC_pNext;
		shapeTransfertTableCurrentEntry = shapeTransfertTableCurrentEntry + 1;
		*ppsVar1 = shapeTransfertTable[iVar2];
		shapeTransfertTable[iVar2] = psVar3;
	}
	return;
}


void initFieldData()
{
	resetFieldDefault();

	for (int i = 0; i < 32; i++)
	{
		fieldVramMapping[i].m0_vramX = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 0);
		fieldVramMapping[i].m2_vramY = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 2);
		fieldVramMapping[i].m4 = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 4);
		fieldVramMapping[i].m6 = READ_LE_S16(rawFieldBundle.begin() + i * 8 + 6);
	}

	MissingCode();

	{
		{

			int rawFieldImageBundleSize = READ_LE_U32(&rawFieldBundle[0x10C]);
			rawFieldImageBundle.resize(rawFieldImageBundleSize + 0x10);
			fieldDecompress(rawFieldImageBundleSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x130]), rawFieldImageBundle);

			int numImages = READ_LE_U32(rawFieldImageBundle.begin());
			for (int i = 0; i < numImages; i++)
			{
				int offset = READ_LE_U32(rawFieldImageBundle.begin() + i * 4);
				uploadFieldImages(rawFieldImageBundle.begin() + offset);
			}
		}

		{
			int rawFieldImageBundle2Size = READ_LE_U32(&rawFieldBundle[0x11C]);
			rawFieldImageBundle2.resize(rawFieldImageBundle2Size + 0x10);
			fieldDecompress(rawFieldImageBundle2Size + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x140]), rawFieldImageBundle2);

			int numImages = READ_LE_U32(rawFieldImageBundle2.begin());
			for (int i = 0; i < numImages; i++)
			{
				int offset = READ_LE_U32(rawFieldImageBundle2.begin() + 4 + i * 4);

				if (fieldVramMapping[i].m6 == 0)
				{
					int x = fieldVramMapping[i].m0_vramX;
					int y = fieldVramMapping[i].m2_vramY;

					uploadNpcSpriteSheet(rawFieldImageBundle2.begin() + offset, x, y);
				}

			}
		}

		DrawSync(0); // needs to be done to transfert to vram before the bundles get deallocated
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x114]);
		rawFieldModels.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x138]), rawFieldModels);

		int numModels = READ_LE_U32(rawFieldModels.begin());
		gCurrentFieldModels.clear();
		gCurrentFieldModels.resize(numModels);
		for (int i = 0; i < numModels; i++)
		{
			int offset = READ_LE_U32(rawFieldModels.begin() + 4 + i * 4);
			int nextOffset;

			if (i < numModels - 1)
			{
				nextOffset = READ_LE_U32(rawFieldModels.begin() + 4 + (i + 1) * 4);
			}
			else
			{
				nextOffset = rawFieldModels.size();
			}
			fieldModelRelocation(rawFieldModels.begin() + offset);
			gCurrentFieldModels[i].init(rawFieldModels.begin() + offset, nextOffset - offset);
		}
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x120]);
		rawFieldScriptData.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x144]), rawFieldScriptData);
		totalActors = READ_LE_U32(rawFieldScriptData.begin() + 0x80);
		pCurrentFieldScriptFile = rawFieldScriptData.begin() + 0x84 + totalActors * 0x40;
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x12C]);
		rawFieldTriggerData.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x150]), rawFieldTriggerData);
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x128]);
		rawFieldDialogBundle.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x14C]), rawFieldDialogBundle);
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x110]);
		rawFieldWalkMesh.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x134]), rawFieldWalkMesh);
		walkMesh.init(rawFieldWalkMesh);
	}

	{
		std::vector<u8>::iterator walkMeshIterator = rawFieldWalkMesh.begin();
		numWalkMesh = READ_LE_U32(walkMeshIterator);
		assert(numWalkMesh < 5);
		walkMeshIterator += 4;

		for (int i = 0; i < 4; i++)
		{
			u32 block_size = READ_LE_U32(walkMeshIterator);
			assert((block_size % 0xE) == 0);
			walkMeshNumTrianglePerBlock[i] = block_size / 0xE;

			walkMeshIterator += 4;
		}

		int iVar4 = 0;
		walkMeshVar1Raw = &(rawFieldWalkMesh[0]) + READ_LE_U32(walkMeshIterator);
		walkMeshIterator += 4;

		if (numWalkMesh > 0) {
			std::array<u8*, 5>::iterator piVar11 = walkMeshVerticesRaw.begin();
			std::array<u8*, 5>::iterator ppuVar8 = walkMeshTriangleRaw.begin();
			do {
				iVar4 = iVar4 + 1;
				*ppuVar8 = &rawFieldWalkMesh[0] + READ_LE_U32(walkMeshIterator);
				*piVar11 = &rawFieldWalkMesh[0] + READ_LE_U32(walkMeshIterator + 4);
				ppuVar8++;
				piVar11++;
				walkMeshIterator = walkMeshIterator + 2 * 4;
			} while (iVar4 < numWalkMesh);
		}
		walkMeshVar4 = (walkMeshTriangleRaw[0] - walkMeshVar1Raw) / 4;

		//////////////////////////////////////////////
		// Not in disassembly

		for (int i = 0; i < numWalkMesh; i++)
		{
			walkMeshTriangle[i] = &walkMesh.m_blocks[i].m_triangles;
			walkMeshVertices[i] = &walkMesh.m_blocks[i].m_vertices;
		}
		walkMeshVar1 = &walkMesh.m_materials;
	}

	{
		int rawFieldSize = READ_LE_U32(&rawFieldBundle[0x118]);
		rawFieldActorSetupParams.resize(rawFieldSize + 0x10);
		fieldDecompress(rawFieldSize + 0x10, rawFieldBundle.begin() + READ_LE_U32(&rawFieldBundle[0x13C]), rawFieldActorSetupParams);

		u32 count = READ_LE_U32(rawFieldActorSetupParams.begin());
		fieldActorSetupParams.resize(count);
		for (int i = 0; i < count; i++)
		{
			fieldActorSetupParams[i].init(rawFieldActorSetupParams.begin() + READ_LE_U32(4 + rawFieldActorSetupParams.begin() + 4 * i));
		}
	}

	fieldInitVar1[0] = 1;
	fieldInitVar1[1] = 1;
	fieldInitVar1[2] = 1;
	fieldInitVar1[3] = 1;

	setupField3d(rawFieldBundle.begin() + 0x154);

	totalObjects = READ_LE_U16(&rawFieldBundle[0x18C]);
	actorArray.resize(totalObjects);

	for (int i = 0; i < totalObjects; i++)
	{
		memset(&actorArray[i], 0, sizeof(sFieldEntity));
	}

	std::vector<u8>::iterator fieldEntitySetup = rawFieldBundle.begin() + 0x190;
	for (int i = 0; i < totalObjects; i++)
	{
		actorArray[i].m58_flags = READ_LE_U16(fieldEntitySetup);
		actorArray[i].m50_modelRotation[0] = READ_LE_S16(fieldEntitySetup + 2);
		actorArray[i].m50_modelRotation[1] = READ_LE_S16(fieldEntitySetup + 4);
		actorArray[i].m50_modelRotation[2] = READ_LE_S16(fieldEntitySetup + 6);

		actorArray[i].m2C_matrixBackup.t[0] = actorArray[i].mC_matrix.t[0] = READ_LE_S16(fieldEntitySetup + 8);
		actorArray[i].m2C_matrixBackup.t[1] = actorArray[i].mC_matrix.t[1] = READ_LE_S16(fieldEntitySetup + 10);
		actorArray[i].m2C_matrixBackup.t[2] = actorArray[i].mC_matrix.t[2] = READ_LE_S16(fieldEntitySetup + 12);

		// Is this a 3d model?
		if (!(actorArray[i].m58_flags & 0x40))
		{
			u16 modelIndex = READ_LE_U16(fieldEntitySetup + 14);

			actorArray[i].m0 = new sFieldEntitySub0;
			sModel& pModel = gCurrentFieldModels[modelIndex];
			std::vector<sModelBlock>::iterator pModelBlock = pModel.m10_blocks.begin();

			actorArray[i].m0->m4_pModelBlock = pModelBlock;

			initModel1(*pModelBlock, actorArray[i].m0->m8, actorArray[i].m0->mC_end);

			MissingCode(); // the whole model init stuff here
		}
		else
		{
			actorArray[i].m50_modelRotation[0] = 0;
			actorArray[i].m50_modelRotation[1] = 0;
			actorArray[i].m50_modelRotation[2] = 0;

		}

		initFieldScriptEntity(i);

		fieldEntitySetup += 16;
	}

	MissingCode();

	allocateShapeTransfert(0x3c00);

	MissingCode();

	startAllEntityScripts();

	MissingCode();

	// hack!
	fieldIdForDebugger = currentFieldId0 / 2;
}

void flagAllocation(std::vector<u8>&)
{
	MissingCode();
}

void unflagAllocation(std::vector<u8>&)
{
	MissingCode();
}

int isCDBusy()
{
	MissingCode();
	return 0;
}



void loadRawFieldBundle(int fieldId)
{
	rawFieldBundle.resize(getFileSizeAligned(184 + fieldId));
	flagAllocation(rawFieldBundle);

	readFile(184 + fieldId, rawFieldBundle, 0, 0x80);
}

int loadNewField(int fieldId, int param)
{
	if ((currentFieldId1 == param) && (currentFieldId0 == fieldId))
	{
		return 0;
	}

	if (isCDBusy())
	{
		return -1;
	}

	waitReadCompletion(0);
	if (currentFieldId1 != -1)
	{
		unflagAllocation(rawFieldBundle);
		rawFieldBundle.clear();
	}
	loadRawFieldBundle(fieldId);

	currentFieldId0 = fieldId;
	currentFieldId1 = param;

	return -1;
}

void loadInitialField()
{
	waitReadCompletion(0);

	while (loadNewField((fieldMapNumber & 0xFFF) * 2, 0));
}


void* allocateBufferForVramUpload(int)
{
	MissingCode();
	return nullptr;
}

std::array<u8, 2048 * 512> gVram;
void clearModelTextureCache();

void loadImageFileToVram(int fileId, void* allocation, int, int, int, int, int, int, int, int)
{
	// this is completely different from the original code that was done async and using the cd callbacks
	MissingCode();

	// make sure we recreate all texture after loading this
	clearModelTextureCache();

	std::vector<u8> data;
	c_isoManager::getCurrentDisc()->readData(getFileStartSector(fileId), getFileSize(fileId), data);

	int offset = 0;
	while (offset < data.size() - 0x14)
	{
		std::vector<u8>::iterator texture = data.begin() + offset;
		u32 type = READ_LE_U16(texture + 0x00);
		u16 pos_x = READ_LE_U16(texture + 0x04);
		u16 pos_y = READ_LE_U16(texture + 0x06);
		u16 move_x = READ_LE_U16(texture + 0x08);
		u16 move_y = READ_LE_U16(texture + 0x0a);
		u16 width = READ_LE_U16(texture + 0x0c);
		u16 height = READ_LE_U16(texture + 0x0e);
		u16 chunks = READ_LE_U16(texture + 0x12);

		if (width > 2048 || height > 512 || width == 0 || height == 0) {
			return;
		}
		int blockSize = 0x1C + chunks * 2;
		offset += (blockSize + 2047) & ~2047;
		for (int i = 0; i < chunks; i++) {
			height = READ_LE_U16(texture + 0x1C + i * 2);
			for (int j = 0; j < height; j++) {
				memcpy(&gVram[0] + (pos_y + move_y + j) * 2048 + (pos_x + move_x) * 2, ((u8*)&data[0]) + offset + j * width * 2, width * 2);
			}
			pos_y += height;
			blockSize = width * height * 2;
			offset += (blockSize + 2047) & ~2047;
		}
	}
}

int fieldGraphicsUploaded = 0;
void loadFieldGraphics()
{
	if (!fieldGraphicsUploaded)
	{
		fieldGraphicsUploaded = 1;
		void* fieldGraphicsAllocation = allocateBufferForVramUpload(4);
		loadImageFileToVram((fieldMapNumber & 0xfff) * 2 + 0xb9, fieldGraphicsAllocation, 0, 0, 0, 0, 0, 0, 0, 0);
	}
}


void bootField()
{
	MissingCode();

	setCurrentDirectory(4, 0);
	loadInitialField();

	MissingCode();

	waitReadCompletion(0);
	setCurrentDirectory(4, 0);
	initFieldData();
	loadFieldGraphics();

	MissingCode();

	if (fieldGraphicsUploaded)
	{
		MissingCode();
		fieldGraphicsUploaded = 0;
		MissingCode();
	}

	MissingCode();
}

void allocatePartyCharacterBuffers()
{
	initModel3(8, 0);

	partyCharacterBuffersRaw[0].resize(0x14000);
	partyCharacterBuffersRaw[1].resize(0x14000);
	partyCharacterBuffersRaw[2].resize(0x14000);

	flagAllocation(partyCharacterBuffersRaw[0]);
	flagAllocation(partyCharacterBuffersRaw[1]);
	flagAllocation(partyCharacterBuffersRaw[2]);
}

void waitForReadFinished(void)

{
	int iVar1;

	do {
		iVar1 = isCDBusy();
	} while (iVar1 != 0);
	waitReadCompletion(0);
	return;
}

int characterOrGearsLoadingInProgress = 0;
int fieldRequestedGears = 0;
int typeOfPlayableCharacterLoaded = 0;

void finalizeLoadPlayableCharacters()
{
	if (characterOrGearsLoadingInProgress != 0) {
		waitForReadFinished();

		for (int i = 0; i < 3; i++)
		{
			unflagAllocation(partyCharacterBuffersRaw[i]);
			if (currentParty[i] != -1)
			{
				unflagAllocation(partyCharacterBuffersCompressed[i]);
				decompress(partyCharacterBuffersCompressed[i].begin(), partyCharacterBuffersRaw[i]);
				partyCharacterBuffersCompressed[i].clear();

				partyCharacterBuffers[i].init(partyCharacterBuffersRaw[i].begin());
			}
		}

		characterOrGearsLoadingInProgress = 0;
	}
}

void refinalizePlayableCharacters(int)
{
	MissingCode();
}

void setupFieldCurrentPartyFromKernelAsGears()
{
	MissingCode();
}

struct sLoadingBatchCommands
{
	u16 m0_fileIndex;
	std::vector<u8>* m4_loadPtr;
};

sLoadingBatchCommands playableCharacterLoadingBatchCommands[11];

void batchStartLoadingFiles(sLoadingBatchCommands* pCommands, int param_2)
{
	// TODO: this is a quick and dirty implementation, not how the original code worked
	while (pCommands->m4_loadPtr)
	{
		readFile(pCommands->m0_fileIndex, *pCommands->m4_loadPtr, 0, 0);

		pCommands++;
	}
}

void setupFieldCurrentPartyFromKernelAsCharacters()
{
	pKernelGameState = &gameState;

	for (int i = 0; i < 3; i++)
	{
		currentParty[i] = -1;
		if (gameState.m1D34_currentParty[i] != -1)
		{
			currentParty[i] = gameState.m1D34_currentParty[i];
		}
	}

	int loadSlot = 0;
	for (int i = 0; i < 3; i++)
	{
		if (currentParty[i] != -1)
		{
			playableCharacterLoadingBatchCommands[loadSlot].m0_fileIndex = currentParty[i] + 5;
			asyncPartyCharacterLoadingTable[i] = currentParty[i];
			partyCharacterBuffersCompressed[i].resize(getFileSizeAligned(currentParty[i] + 5));
			playableCharacterLoadingBatchCommands[loadSlot].m4_loadPtr = &partyCharacterBuffersCompressed[i];
			flagAllocation(partyCharacterBuffersCompressed[i]);
			loadSlot++;
		}
	}

	playableCharacterLoadingBatchCommands[loadSlot].m0_fileIndex = 0;
	playableCharacterLoadingBatchCommands[loadSlot].m4_loadPtr = nullptr;

	batchStartLoadingFiles(playableCharacterLoadingBatchCommands, 0);
	typeOfPlayableCharacterLoaded = 1;
}

void startLoadingPlayableCharacters()
{
	waitForReadFinished();
	if (characterOrGearsLoadingInProgress == 1) {
		finalizeLoadPlayableCharacters();
		if (fieldScriptEntityAlreadyInitialized != 0) {
			return;
		}
	}
	else {
		if (fieldScriptEntityAlreadyInitialized != 0) {
			refinalizePlayableCharacters(0);
			return;
		}
	}
	fieldRequestedGears = (fieldMapNumber & 0xc000) != 0;
	characterOrGearsLoadingInProgress = 0;
	if (fieldRequestedGears) {
		if (typeOfPlayableCharacterLoaded != 2) {
			setupFieldCurrentPartyFromKernelAsGears();
			characterOrGearsLoadingInProgress = 1;
		}
	}
	else {
		if (typeOfPlayableCharacterLoaded != 1) {
			setupFieldCurrentPartyFromKernelAsCharacters();
			characterOrGearsLoadingInProgress = 1;
		}
	}
}

void freeFieldData()
{
	MissingCode();

	actorArray.clear();
	rawFieldTriggerData.clear();
	rawFieldDialogBundle.clear();
	rawFieldScriptData.clear();
	rawFieldWalkMesh.clear();
	walkMesh.clear();
	rawFieldModels.clear();
	gCurrentFieldModels.clear();
	rawFieldActorSetupParams.clear();

	MissingCode();
}

void transitionFields()
{
	MissingCode();

	freeFieldData();

	MissingCode();

	switch (fieldTransitionMode)
	{
	case 2:
		MissingCode();
		startLoadingPlayableCharacters();
		finalizeLoadPlayableCharacters();
		initFieldData();
		loadFieldGraphics();
		if (fieldGraphicsUploaded)
		{
			MissingCode();
			fieldGraphicsUploaded = 0;
			MissingCode();
		}
		MissingCode();
		break;
	default:
		assert(0);
		break;
	}

	fieldTransitionMode = 2;
	MissingCode();

}

void ClearCacheAfterOverlayLoad()
{
	MissingCode();
}

char noUpdatesToPartyMemebers = 0;

int onlyUpdateDirector = 0;
void exectueEntitiesUpdateFunction()
{
	int numEntitiesToUpdate = totalActors;
	if (onlyUpdateDirector == 1)
	{
		numEntitiesToUpdate = 1;
	}

	MissingCode();

	for (int i = 0; i < numEntitiesToUpdate; i++)
	{
		sFieldEntity* pFieldEntity = &actorArray[i];
		if (((pFieldEntity->m58_flags & 0xf00) != 0) && ((pFieldEntity->m4C_scriptEntity->m4_flags & 0x100000) == 0))
		{
			MissingCode();

			pCurrentFieldScriptActor = pFieldEntity->m4C_scriptEntity;
			pCurrentFieldScriptActor->m0_flags &= 0xfeffffff;
			currentFieldActorId = i;
			pCurrentFieldEntity = pFieldEntity;

			int maxScriptIndex = 0xF;
			if (noUpdatesToPartyMemebers)
			{
				assert(0);
			}
			for (int j = 0; j < 8; j++)
			{
				if (pCurrentFieldScriptActor->m8C_scriptSlots[j].m4_flags.m18 <= maxScriptIndex)
				{
					pCurrentFieldScriptActor->mCE_currentScriptSlot = j;
					maxScriptIndex = pCurrentFieldScriptActor->m8C_scriptSlots[j].m4_flags.m18;
				}
			}
			if (maxScriptIndex == 0xF)
			{
				pCurrentFieldScriptActor->m8C_scriptSlots[0].m0_scriptPC = getScriptEntryPoint(i, 1);
				pCurrentFieldScriptActor->mCE_currentScriptSlot = 0;
				pCurrentFieldScriptActor->m8C_scriptSlots[0].m4_flags.m18 = 7;
			}
			currentScriptFinished = 1;
			pCurrentFieldScriptActor->mCC_scriptPC = pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m0_scriptPC;
			if (!(pCurrentFieldScriptActor->m0_flags & 1))
			{
				executeFieldScript(8);
			}
			pCurrentFieldScriptActor->m8C_scriptSlots[pCurrentFieldScriptActor->mCE_currentScriptSlot].m0_scriptPC = pCurrentFieldScriptActor->mCC_scriptPC;
		}
	}
}

int entityUpdateVar0 = 0;
int entityUpdateVar1 = 0;

std::array<s16, 4> deltaMoveTable0 = {
	0x4, 0x8, 0x10, 0x20
};

std::array<s16, 8> deltaMoveTable1 = {
	0xC00, 0xE00, 0x0, 0x200,
	0x400, 0x600, 0x800, 0xA00
};

s16 deltaMoveVar0 = 0;

void computeDeltaMove(sVec3* param_1, int param_2, uint param_3)
{
	int iVar1;
	int iVar2;

	iVar2 = param_2 * 0x10 * entityMoveSpeedFactor >> 0xc;
	iVar1 = getAngleSin(param_3 & 0xfff);
	param_1->vx = iVar1 * iVar2;
	iVar1 = getAngleCos(param_3 & 0xfff);
	param_1->vy = 0;
	param_1->vz = -(iVar1 * iVar2);
	return;
}

void updateEntityEventCode2(int index, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity)
{
	int triangleFlags = 0;
	if (((pFieldScriptEntity->m4_flags >> ((int)(short)pFieldScriptEntity->m10_walkmeshId + 3U & 0x1f) & 1) == 0) && (noUpdatesToPartyMemebers == '\0')) {
		triangleFlags = pFieldScriptEntity->m14_currentTriangleFlag;
	}

	sVec3 deltaMove;
	computeDeltaMove(&deltaMove, deltaMoveTable0[(triangleFlags >> 9) & 3], deltaMoveTable1[(triangleFlags >> 11) & 0x7] + deltaMoveVar0 & 0xfff);

	int iVar9 = 0;
	int iVar12 = 0;

	if ((pFieldScriptEntity->m0_flags & 0x41800) == 0)
	{
		if ((triangleFlags & 0x420000) != 0) {
			VECTOR local_60;
			local_60.vx = -((pFieldScriptEntity->m50).vx * (pFieldScriptEntity->m50).vy) >> 0xf;
			local_60.vz = -((pFieldScriptEntity->m50).vz * (pFieldScriptEntity->m50).vy) >> 0xf;
			if (local_60.vx == 0) {
				local_60.vx = 1;
			}
			local_60.vy = 1;
			if (local_60.vz == 0) {
				local_60.vz = 1;
			}
			VECTOR local_50;
			VectorNormal(&local_60, &local_50);
			if (local_50.vx == 0) {
				local_50.vx = 1;
			}
			if (local_50.vy == 0) {
				local_50.vy = 1;
			}
			if (local_50.vz == 0) {
				local_50.vz = 1;
			}
			int iVar13 = pFieldScriptEntity->mF0 >> 0x11;
			int iVar8 = 0xc;
			iVar9 = local_50.vx * iVar13 * 0x10;
			iVar12 = local_50.vz * iVar13 * 0x10;
			if ((triangleFlags & 0x400000) != 0) {
				iVar8 = 0x18;
			}
			iVar13 = iVar8 << 0x10;
			if (pFieldScriptEntity->mF0 >> 0x10 < iVar8) {
				iVar13 = pFieldScriptEntity->mF0 + pFieldEntity->m4_pVramSpriteSheet->m1C;
			}
			pFieldScriptEntity->mF0 = iVar13;
			pFieldEntity->m4_pVramSpriteSheet->mC.vy = pFieldScriptEntity->mF0 >> 1;
		}
		if ((triangleFlags & 0x400000) != 0) {
			pFieldScriptEntity->m40.vx += iVar9;
			pFieldScriptEntity->m40.vz += iVar12;
			pFieldScriptEntity->m104_rotation = pFieldScriptEntity->m104_rotation | 0x8000;
		}
		if (pFieldScriptEntity->m74 == -1)
		{
			if ((triangleFlags & 0x20000) != 0) {
				(pFieldScriptEntity->m40).vx = (pFieldScriptEntity->m40).vx + iVar9;
				(pFieldScriptEntity->m40).vz = (pFieldScriptEntity->m40).vz + iVar12;
			}
			if (triangleFlags & 0x8000)
			{
				pFieldScriptEntity->m40.vx += deltaMove.vx;
				pFieldScriptEntity->m40.vy += deltaMove.vy;
				pFieldScriptEntity->m40.vz += deltaMove.vz;
			}
		}
	}
	else
	{
		if (triangleFlags & 0x4000)
		{
			pFieldScriptEntity->m40.vx += deltaMove.vx;
			pFieldScriptEntity->m40.vy += deltaMove.vy;
			pFieldScriptEntity->m40.vz += deltaMove.vz;
		}
	}

	if (pFieldScriptEntity->m74 != -1)
	{
		assert(0);
	}

	if ((pFieldScriptEntity->m4_flags & 0x22000) == 0x22000) {
		assert(0);
	}
}

s16 playerCanRun = 1;
s32 updateEntityEventCode3Var0 = 0;
s32 updateEntityEventCode3Var1 = 0;
s32 updateEntityEventCode3Var2 = 0;
int updateEntityEventCode3Sub0(sFieldScriptEntity* param_1)
{
	int iVar1;

	iVar1 = -1;
	if (((((((param_1->m14_currentTriangleFlag & 0x420000U) == 0) && (iVar1 = -1, updateEntityEventCode3Var1 == 0)) && (iVar1 = -1, param_1->m30_stepVector.vx == 0)) &&
		((iVar1 = -1, param_1->m30_stepVector.vy == 0 && (iVar1 = -1, param_1->m30_stepVector.vz == 0)))) &&
		((iVar1 = -1, updateEntityEventCode3Var2 == 1 && ((iVar1 = -1, param_1->m74 == 0xff && (iVar1 = -1, (param_1->m0_flags & 0x401800) == 0)))))) &&
		(((param_1->m4_flags & 1) == 0 || (iVar1 = -1, param_1->m10_walkmeshId != 0)))) {
		if (((param_1->m4_flags & 2) == 0) || (param_1->m10_walkmeshId != 1)) {
			iVar1 = 0;
			if ((param_1->m4_flags & 4) != 0) {
				iVar1 = -(uint)(param_1->m10_walkmeshId == 2);
			}
		}
		else {
			iVar1 = -1;
		}
	}
	return iVar1;
}

void updateEntityEventCode3Sub1(sFieldEntitySub4* param_1, ushort rotation, sFieldEntity* param_3)
{
	int iVar1;
	sFieldScriptEntity* psVar2;
	int iVar3;
	uint uVar4;

	if ((param_3->m58_flags & 0x40) == 0) {
		uVar4 = 0x40000 / (ushort)param_3->m4C_scriptEntity->m76;
		if ((rotation & 0x8000) != 0) {
		LAB_Field__800821b0:
			(param_1->mC).vx = 0;
			(param_1->mC).vz = 0;
			goto LAB_Field__800821bc;
		}
	}
	else {
		if (((int)(short)rotation & 0x8000U) != 0) goto LAB_Field__800821b0;
		psVar2 = param_3->m4C_scriptEntity;
		uVar4 = psVar2->m4_flags;
		if ((uVar4 & 0x2000) == 0) {
			if ((uVar4 & 0x80000) == 0) {
				OP_INIT_ENTITY_SCRIPT_sub0Sub7(param_1, (int)(short)rotation);
			}
			else {
				iVar3 = ((int)(0x40000 / (ushort)psVar2->m76) >> 8) * 0x20;
				iVar1 = getAngleSin(rotation & 0xfff);
				(param_1->mC).vx = (iVar1 * iVar3 >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[0];
				iVar1 = getAngleCos(rotation & 0xfff);
				(param_1->mC).vz = (-(iVar1 * iVar3) >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[2];
				param_1->m18 = 0x4000000 / (ushort)param_3->m4C_scriptEntity->m76;
			}
			goto LAB_Field__800821bc;
		}
		if ((uVar4 & 0x20000) != 0) {
			uVar4 = psVar2->m12C_flags;
			assert(0);
			//(param_1->mC).vx = *(int*)(*(int*)(&DAT_801e8670 + (uVar4 >> 0xb & 0x1c)) + 0x128) * -0x10000;
			//(param_1->mC).vz = *(int*)(*(int*)(&DAT_801e8670 + (uVar4 >> 0xb & 0x1c)) + 0x130) * -0x10000;
			goto LAB_Field__800821bc;
		}
		uVar4 = 0x80000 / (ushort)psVar2->m76;
	}
	iVar3 = ((int)uVar4 >> 8) * 0x20;
	iVar1 = getAngleSin(rotation & 0xfff);
	(param_1->mC).vx = (iVar1 * iVar3 >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[0];
	iVar1 = getAngleCos(rotation & 0xfff);
	(param_1->mC).vz = (-(iVar1 * iVar3) >> 0xc) * (int)param_3->m4C_scriptEntity->mF4_scale3d[2];

LAB_Field__800821bc:
	(param_1->mC).vx = (param_1->mC).vx & 0xfffff000;
	(param_1->mC).vz = (param_1->mC).vz & 0xfffff000;
}

int updateEntityEventCode3Sub2(sVec3* param_1, sFieldScriptEntity* param_2)
{
	long lVar2;
	std::vector<u16>* pasVar3;

	if ((param_2->m12C_flags & 0x1000) != 0) {
		sFP1616 position;
		position.set(((param_2->m20_position).vx + param_1->vx >> 0x10), ((param_2->m20_position).vz + param_1->vz >> 0x10));

		std::vector<sVec2_s16>& pasVar3 = param_2->m114_movementBoundingZone;
		std::array<sFP1616, 4> boundingZone;
		boundingZone[0].set(pasVar3[0].vx, pasVar3[0].vx);
		boundingZone[1].set(pasVar3[1].vx, pasVar3[1].vx);
		boundingZone[2].set(pasVar3[2].vx, pasVar3[2].vx);
		boundingZone[3].set(pasVar3[3].vx, pasVar3[3].vx);

		if ((NCLIP(boundingZone[0], boundingZone[1], position) < 0) || (NCLIP(boundingZone[1], boundingZone[2], position) < 0) || (NCLIP(boundingZone[2], boundingZone[3], position) < 0)) {
			return -1;
		}
		else {
			lVar2 = NCLIP(boundingZone[2], boundingZone[0], position);
			return lVar2 >> 0x1f;
		}
	}
	return 0;
}

int updateEntityEventCode3Sub3Sub1(sVec3* param_1, VECTOR* param_2, sFieldScriptEntity* param_3, std::array<SVECTOR, 2>& param_4, SVECTOR* param_5, int param_6, uint* param_7)
{
	MissingCode();
	return 0;
}

uint updateEntityEventCode3Sub3Sub2(short param_1, std::array<SVECTOR, 2>& param_2, sVec3* param_3)

{
	short sVar1;
	short sVar2;
	long lVar3;
	uint uVar4;
	uint uVar5;
	VECTOR local_38;
	VECTOR local_28;

	lVar3 = ratan2(param_2[1].vz - param_2[0].vz, param_2[1].vx - param_2[0].vx);
	uVar4 = -lVar3 & 0xfff;
	uVar5 = (0xc00U - (int)param_1 & 0xfff) + uVar4 & 0xfff;
	if (uVar5 - 0x80 < 0xf01) {
		if (uVar5 < 0x800) {
			local_38.vx = param_2[0].vx - param_2[1].vx;
			sVar1 = param_2[0].vz;
			sVar2 = param_2[1].vz;
			uVar4 = uVar4 + 0x800 & 0xfff;
		}
		else {
			local_38.vx = param_2[1].vx - param_2[0].vx;
			sVar1 = param_2[1].vz;
			sVar2 = param_2[0].vz;
		}
		local_38.vy = 0;
		local_38.vz = (int)sVar1 - (int)sVar2;
		VectorNormal(&local_38, &local_28);
		lVar3 = length2d(param_3->vx >> 0xc, param_3->vz >> 0xc);

		param_3->vx = local_28.vx * lVar3;
		param_3->vy = 0;
		param_3->vz = local_28.vz * lVar3;
	}
	else {
		param_3->vx = 0;
		param_3->vy = 0;
		param_3->vz = 0;
	}
	return uVar4;
}

int updateEntityEventCode3Sub4Sub1(sVec3* deltaStep, VECTOR* position, sFieldScriptEntity* pFieldScriptEntity, std::array<SVECTOR, 2>& param_4, SVECTOR* param_5, int param_6)
{
	int collisionFlag;
	int uVar2;

	int triangleId = (int)pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId];
	std::vector<sWalkMesh::sTriangleData>& pWalkMeshTriangles = *walkMeshTriangle[pFieldScriptEntity->m10_walkmeshId];
	std::vector<SVECTOR>& pWalkMeshVertices = *walkMeshVertices[pFieldScriptEntity->m10_walkmeshId];

	if (triangleId == -1) {
		uVar2 = -1;
	}
	else {
		param_5->vx = (position->vx + deltaStep->vx) >> 0x10;
		param_5->vy = 0;
		param_5->vz = (position->vz + deltaStep->vz) >> 0x10;

		sFP1616 startPosition;
		sFP1616 endPosition;
		startPosition.set((position->vx >> 0x10), (position->vz >> 0x10));
		endPosition.set((position->vx + deltaStep->vx) >> 0x10, (position->vz + deltaStep->vz) >> 0x10);

		u32 mask = 0;

		if ((pFieldScriptEntity->m4_flags >> ((int)pFieldScriptEntity->m10_walkmeshId + 3U & 0x1f) & 1) == 0) {
			mask = -(uint)(noUpdatesToPartyMemebers == '\0');
		}

		int tempTriangleId;

		sFieldScriptEntity* local_58 = pFieldScriptEntity;
		int iterationCount = 0;
		for (iterationCount = 0; iterationCount < 0x20; iterationCount++)
		{
			sWalkMesh::sTriangleData* pTriangle = &pWalkMeshTriangles[triangleId];

			sFP1616 vert0;
			sFP1616 vert1;
			sFP1616 vert2;

			vert0.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[0]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[0]].vz);
			vert1.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[1]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[1]].vz);
			vert2.set(pWalkMeshVertices[pTriangle->m0_verticeIndex[2]].vx, pWalkMeshVertices[pTriangle->m0_verticeIndex[2]].vz);

			tempTriangleId = triangleId;

			collisionFlag = 0;
			if (NCLIP(vert0, vert1, endPosition) < 0) {
				collisionFlag |= 1;
			}

			if (NCLIP(vert1, vert2, endPosition) < 0) {
				collisionFlag |= 2;
			}

			if (NCLIP(vert2, vert0, endPosition) < 0) {
				collisionFlag |= 4;
			}

			if (collisionFlag < 8) {
				switch (collisionFlag)
				{
				case 0:
					mask = 0xFF;
					break;
				case 1:
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
					break;
				case 2:
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
					break;
				case 3:
					if (NCLIP(vert1, endPosition, startPosition) < 0)
					{
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
						collisionFlag = 1;
					}
					else {
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
						collisionFlag = 2;
					}
					break;
				case 4:
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
					break;
				case 5:
					if (NCLIP(vert0, endPosition, startPosition) < 0)
					{
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
						collisionFlag = 4;
					}
					else {
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
						collisionFlag = 1;
					}
					break;
				case 6:
					if (NCLIP(vert2, endPosition, startPosition) < 0)
					{
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
						collisionFlag = 2;
					}
					else {
						triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
						collisionFlag = 4;
					}
					break;
				case 7:
					triangleId = -1;
					break;
				default:
					assert(0);
				}
			}

			// todo: does this really happen to be -1?
			if (triangleId == -1)
			{
				return -1;
			}
			u32 uVar4 = ((*walkMeshVar1)[pWalkMeshTriangles[triangleId].mC_indexInWalkmeshData1]) & mask;
			if ((((local_58->m0_flags >> 9 & 3 & uVar4 >> 3) != 0) || ((local_58->m0_flags >> 8 & 7 & uVar4 >> 5) != 0)) || (((uVar4 & 0x800000) != 0 && (local_58->m10_walkmeshId == 0)))) {
				triangleId = -1;
				break;
			}

			if (triangleId == -1)
				break;
		}

		if ((triangleId == -1) || (iterationCount == 0x20)) {

			switch (collisionFlag)
			{
			case 1:
				param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
				param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
				break;
			case 2:
				param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
				param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
				break;
			case 4:
				param_4[0] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[1]];
				param_4[1] = pWalkMeshVertices[pWalkMeshTriangles[tempTriangleId].m0_verticeIndex[2]];
				break;
			default:
				return -1;
			}
			uVar2 = -1;
		}
		else {
			if (param_6 == -1) {
				uVar2 = 0;
			}
			else {
				//psVar6 = (short*)(puVar9 + iVar8 * 0xe);
				assert(0);
				//FUN_Field__8007b07c((short*)(puVar10 + *psVar6 * 8), (short*)(puVar10 + psVar6[1] * 8), (short*)(puVar10 + psVar6[2] * 8), param_5, &sStack104);
				uVar2 = 0;
			}
		}
	}
	return uVar2;
}

int updateEntityEventCode3Sub4(sVec3* position, sFieldScriptEntity* param_2, std::array<SVECTOR, 2>& param_3, s16 angle)
{
	VECTOR* psVar4;
	sVec3 local_60;
	SVECTOR auStack80;

	local_60.vx = position->vx + getAngleSin(angle & 0xfff) * 0x40;
	local_60.vz = position->vz + getAngleCos(angle & 0xfff) * -0x40;

	psVar4 = &param_2->m20_position;

	if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1) {
		local_60.vx = position->vx;
		local_60.vy = position->vy;
		local_60.vz = position->vz;
		updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
	}
	else {
		int newAngle = angle - 0x100U & 0xfff;
		local_60.vx = position->vx + getAngleSin(newAngle) * 0x40;
		local_60.vz = position->vz + getAngleCos(newAngle) * -0x40;
		if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1)
		{
			local_60.vx = position->vx;
			local_60.vy = position->vy;
			local_60.vz = position->vz;
			updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
		}
		else
		{
			int newAngle = angle + 0x100U & 0xfff;
			local_60.vx = position->vx + getAngleSin(newAngle) * 0x40;
			local_60.vz = position->vz + getAngleCos(newAngle) * -0x40;
			if (updateEntityEventCode3Sub4Sub1(&local_60, psVar4, param_2, param_3, &auStack80, -1) == -1)
			{
				local_60.vx = position->vx;
				local_60.vy = position->vy;
				local_60.vz = position->vz;
				updateEntityEventCode3Sub3Sub2(angle, param_3, &local_60);
			}
			else
			{
				local_60.vx = position->vx;
				local_60.vy = position->vy;
				local_60.vz = position->vz;
			}

		}
	}

	if (updateEntityEventCode3Sub4Sub1(&local_60, &param_2->m20_position, param_2, param_3, &auStack80, 0) == -1) {
		return -1;
	}
	else {
		if ((param_2->m0_flags & 0x40000) == 0) {
			if (((int)auStack80.vy << 0x10 < (param_2->m20_position).vy) && (updateEntityEventCode3Var1 == 0))
				return -1;
		}
		else {
			auStack80.vy = param_2->mEC_elevation;
		}

		position->vx = local_60.vx;
		position->vy = auStack80.vy * 0x10000 - param_2->m20_position.vy;
		position->vz = local_60.vz;

		param_2->m72_elevation = (param_2->m20_position.vy + position->vy) >> 0x10;
		return 0;
	}
}

int updateEntityEventCode3Sub3(sVec3* param_1, sFieldScriptEntity* param_2, std::array<SVECTOR, 2>& param_3, short angle)
{
	long lVar2;
	sVec3 local_88;
	SVECTOR local_68;
	VECTOR local_58;
	VECTOR local_48;
	uint local_38;

	local_88.vx = param_1->vx + getAngleSin(angle - 0x100U & 0xfff) * 0x40;
	local_88.vz = param_1->vz + getAngleCos(angle - 0x100U & 0xfff) * -0x40;
	if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1) {
		local_88.vx = param_1->vx;
		local_88.vy = param_1->vy;
		local_88.vz = param_1->vz;
		updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
	}
	else {
		local_88.vx = param_1->vx + getAngleSin(angle + 0x100U & 0xfff) * 0x40;
		local_88.vz = param_1->vz + getAngleCos(angle + 0x100U & 0xfff) * -0x40;
		if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1)
		{
			local_88.vx = param_1->vx;
			local_88.vy = param_1->vy;
			local_88.vz = param_1->vz;
			updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
		}
		else
		{
			local_88.vx = param_1->vx + getAngleSin(angle & 0xfff) * 0x40;
			local_88.vz = param_1->vz + getAngleCos(angle & 0xfff) * -0x40;
			if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, -1, &local_38) == -1)
			{
				local_88.vx = param_1->vx;
				local_88.vy = param_1->vy;
				local_88.vz = param_1->vz;
				updateEntityEventCode3Sub3Sub2(angle, param_3, &local_88);
			}
			else
			{
				local_88.vx = param_1->vx;
				local_88.vy = param_1->vy;
				local_88.vz = param_1->vz;
			}
		}
	}
	if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, 0, &local_38) == -1) {
		return -1;
	}
	if ((((param_2->m20_position.vy) >> 16) <= local_68.vy) && ((local_38 & 0x200000) == 0)) {
		if ((local_38 & 0x420000) == 0) {
			if (((param_2->m20_position.vy) >> 16) + 0x40 <= (int)local_68.vy)
			{
				param_1->vx = local_88.vx;
				param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
				param_1->vz = local_88.vz;
				param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
				return 0;
			}
		}
		else {
			if ((param_2->m14_currentTriangleFlag & 0x420000U) == 0)
			{
				param_1->vx = local_88.vx;
				param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
				param_1->vz = local_88.vz;
				param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
				return 0;
			}
		}
	}
	local_58.vx = -local_88.vx >> 8;
	local_58.vz = -local_88.vz >> 8;
	local_58.vy = local_68.vy * 0x10000 - (param_2->m20_position).vy >> 8;
	VectorNormal(&local_58, &local_48);
	lVar2 = length2d(local_88.vx >> 8, local_88.vz >> 8);

	local_88.vx = -(lVar2 * local_48.vx) >> 4;
	local_88.vy = lVar2 * local_48.vy >> 4;
	local_88.vz = -(lVar2 * local_48.vz) >> 4;
	if (updateEntityEventCode3Sub3Sub1(&local_88, &param_2->m20_position, param_2, param_3, &local_68, 0, &local_38) == -1) {
		return -1;
	}
	param_2->m0_flags = param_2->m0_flags | 0x4000000;

	param_1->vx = local_88.vx;
	param_1->vy = local_68.vy * 0x10000 - param_2->m20_position.vy;
	param_1->vz = local_88.vz;
	param_2->m72_elevation = (param_2->m20_position.vy + param_1->vy) >> 0x10;
	return 0;
}

s16 updateEntityEventCode4Var0 = 0;
s16 updateEntityEventCode4Var1 = 0;

void updateEntityEventCode4(sFieldEntitySub4* param_1, int param_2, sFieldEntity* param_3)
{
	sFieldScriptEntity* psVar1;

	if ((param_3->m58_flags & 0x40) != 0) {
		if ((param_2 != 3) && (updateEntityEventCode4Var0 == 0)) {
			param_3->m4C_scriptEntity->m0_flags = param_3->m4C_scriptEntity->m0_flags & 0xfffff7ff;
		}
		if (param_2 == 0xff) {
			param_2 = 0;
		}
		if (param_2 != updateEntityEventCode4Var1) {
			param_3->m4C_scriptEntity->m0_flags = param_3->m4C_scriptEntity->m0_flags & 0xfffff7ff;
		}
		psVar1 = param_3->m4C_scriptEntity;
		if ((psVar1->m4_flags & 0x2000) == 0) {
			if ((psVar1->m4_flags & 0x1000000) == 0) {
				OP_INIT_ENTITY_SCRIPT_sub0Sub6(param_1, param_2);
			}
		}
		else {
			assert(0);
		}
	}
	return;
}

void updateEntityEventCode3(int index, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity)
{
	int rotation = pFieldScriptEntity->m104_rotation;
	std::array<SVECTOR, 2> auStack56;

	if ((pFieldScriptEntity->m0_flags & 0x1000000) != 0) {
		updateEntityEventCode3Var0 = index;
		return;
	}

	int walkSpeed = 1;
	if ((((pFieldScriptEntity->m0_flags & 0x4000) != 0) && ((padButtonForScripts & 0x40) != 0)) && (playerCanRun == 1)) {
		walkSpeed = 2;
	}
	if (((pFieldScriptEntity->m0_flags & 0x1800) != 0) && (pFieldScriptEntity->mE8 != walkSpeed)) {
		if (pFieldScriptEntity->mE8 == 1) {
			walkSpeed = 1;
		}
		else {
			if (pFieldScriptEntity->mE8 == 2) {
				walkSpeed = 2;
			}
		}
	}

	updateEntityEventCode3Var0 = index;

	if (8 < pFieldScriptEntity->mE3) {
		pFieldScriptEntity->mE3--;
	}

	int moveMask = (pFieldScriptEntity->m40).vx | (pFieldScriptEntity->m40).vy | (pFieldScriptEntity->m40).vz;
	if (updateEntityEventCode3Sub0(pFieldScriptEntity) == -1)
	{
		moveMask = 1;
	}

	sVec3 stepVector;

	bool bMoved = false;

	if (((((int)(short)rotation & 0x8000U) == 0) || (moveMask != 0)) || ((pFieldScriptEntity->m0_flags & 0x40800) != 0)) {
		if (((int)(short)rotation & 0x8000U) == 0) {
			updateEntityEventCode3Sub1(pFieldEntity->m4_pVramSpriteSheet, (int)(short)rotation, pFieldEntity);
			stepVector.vx = pFieldEntity->m4_pVramSpriteSheet->mC.vx + pFieldScriptEntity->m40.vx;
			stepVector.vy = pFieldEntity->m4_pVramSpriteSheet->mC.vy + pFieldScriptEntity->m40.vy;
			stepVector.vz = pFieldEntity->m4_pVramSpriteSheet->mC.vz + pFieldScriptEntity->m40.vz;
			pFieldScriptEntity->m106_currentRotation = rotation;
		}
		else {
			stepVector.vx = (pFieldScriptEntity->m40).vx;
			stepVector.vy = (pFieldScriptEntity->m40).vy;
			stepVector.vz = (pFieldScriptEntity->m40).vz;
			rotation = pFieldScriptEntity->m106_currentRotation & 0xfff;
		}

		if (updateEntityEventCode3Sub2(&stepVector, pFieldScriptEntity) != 0)
		{
			bMoved = false;
		}
		else
		{
			if ((stepVector.vx != 0) || (stepVector.vz != 0)) {
				int lVar2 = ratan2(stepVector.vz, stepVector.vx);
				rotation = -(short)lVar2 & 0xfff;
			}

			int iVar1 = -1;
			if (pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId] != -1) {
				int backupFlags = pFieldScriptEntity->m0_flags;
				if (index == playerControlledEntity) {
					if (partyToFieldEntityArrayMapping[1] != 0xff) {
						pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags | (actorArray[partyToFieldEntityArrayMapping[1]].m4C_scriptEntity)->m0_flags & 0x600;
					}
					if (partyToFieldEntityArrayMapping[2] != 0xff) {
						pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags | (actorArray[partyToFieldEntityArrayMapping[2]].m4C_scriptEntity)->m0_flags & 0x600;
					}
				}
				if ((((pFieldScriptEntity->m0_flags & 0x41800) == 0) && (pFieldScriptEntity->m74 == -1)) && (updateEntityEventCode3Var1 == 0)) {
					iVar1 = updateEntityEventCode3Sub3(&stepVector, pFieldScriptEntity, auStack56, rotation);
				}
				else {
					iVar1 = updateEntityEventCode3Sub4(&stepVector, pFieldScriptEntity, auStack56, rotation);
				}
				pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xfffff9ff | backupFlags & 0x600;
			}
			if (iVar1 == -1)
			{
				bMoved = false;
			}
		}
	}
	else
	{
		walkSpeed = pFieldScriptEntity->mE6;
		pFieldScriptEntity->m104_rotation = pFieldScriptEntity->m104_rotation | 0x8000;

		bMoved = false;
	}

	if (!bMoved)
	{
		pFieldScriptEntity->mF0 = 0x10000;
		(pFieldScriptEntity->m40).vx = 0;
		(pFieldScriptEntity->m40).vy = 0;
		(pFieldScriptEntity->m40).vz = 0;
		stepVector.vx = 0;
		stepVector.vy = 0;
		stepVector.vz = 0;
		(pFieldEntity->m4_pVramSpriteSheet->mC).vx = 0;
		(pFieldEntity->m4_pVramSpriteSheet->mC).vz = 0;
		pFieldScriptEntity->m106_currentRotation = pFieldScriptEntity->m106_currentRotation | 0x8000;
	}

	pFieldScriptEntity->m4_flags = pFieldScriptEntity->m4_flags & 0xffffefff;

	int sVar4 = pFieldScriptEntity->mE8;// TODO: can this be a different value?

	if ((pFieldScriptEntity->m0_flags & 0x800) == 0) {
		if (((int)pFieldScriptEntity->m104_rotation & 0x8000U) != 0) {
			walkSpeed = pFieldScriptEntity->mE6;
		}
		moveMask = getWalkmeshTriangleFlag(pFieldScriptEntity);
		sVar4 = walkSpeed;
		if ((moveMask & 0x200000) != 0) {
			if (((int)pFieldScriptEntity->m104_rotation & 0x8000U) != 0) {
				sVar4 = 6;
				if (pFieldScriptEntity->mE8 != 6) goto LAB_Field__800830ac;
				pFieldScriptEntity->m4_flags = pFieldScriptEntity->m4_flags | 0x1000;
			}
			sVar4 = 6;
		}
	}
	else
	{
		assert(0);
	}

LAB_Field__800830ac:
	if (pFieldScriptEntity->mEA_currentAnimationPlaying != 0xff) {
		sVar4 = pFieldScriptEntity->mEA_currentAnimationPlaying;
	}
	if (((int)pFieldScriptEntity->mE8 != (int)sVar4) && ((pFieldScriptEntity->m0_flags & 0x2000000) == 0)) {
		pFieldScriptEntity->mE8 = sVar4;
		updateEntityEventCode4(pFieldEntity->m4_pVramSpriteSheet, (int)sVar4, pFieldEntity);
	}
	if ((pFieldScriptEntity->m14_currentTriangleFlag & 0x100U) != 0) {
		stepVector.vx = stepVector.vx >> 1;
		stepVector.vz = stepVector.vz >> 1;
	}
	pFieldScriptEntity->m30_stepVector.vx = stepVector.vx;
	pFieldScriptEntity->m30_stepVector.vy = stepVector.vy;
	pFieldScriptEntity->m30_stepVector.vz = stepVector.vz;
	pFieldScriptEntity->m40.vx = 0;
	pFieldScriptEntity->m40.vy = 0;
	pFieldScriptEntity->m40.vz = 0;
}

void EntityMoveCheck0(uint entityIndex, sFieldEntity* pEntity, sFieldScriptEntity* pScriptEntity)
{
	MissingCode();
}

struct sEntityMoveCheck1StackStruct
{
	s32 m0[4];
	s32 m10[4];
	s32 m20[4];
	s16 m30[4];
	VECTOR m38[4];
	s32 m78_position[3];
	s16 m88[4];
};

void EntityMoveCheck1Sub1Sub1(int* param_1, int* param_2, int param_3)

{
	int iVar1;

	iVar1 = *param_1;
	if (param_3 < 0) {
		*param_1 = iVar1;
	}
	else {
		*param_1 = iVar1;
		iVar1 = iVar1 + param_3;
	}
	*param_2 = iVar1;
	return;
}

s32 EntityMoveCheck1Sub1(sFieldScriptEntity* pFieldScriptEntity, int walkmeshId, s32* param_3, VECTOR* param_4, s16* param_5, s32* param_6)
{
	if (pFieldScriptEntity->m8_currentWalkMeshTriangle[walkmeshId] == -1) {
		return -1;
	}

	int triangleId = pFieldScriptEntity->m8_currentWalkMeshTriangle[walkmeshId];

	sFP1616 refPos;
	sFP1616 refPos2;

	SVECTOR projectedPosition;
	projectedPosition.vx = (pFieldScriptEntity->m20_position.vx + pFieldScriptEntity->m30_stepVector.vx) >> 16;
	projectedPosition.vy = 0;
	projectedPosition.vz = (pFieldScriptEntity->m20_position.vz + pFieldScriptEntity->m30_stepVector.vz) >> 16;

	refPos.set((pFieldScriptEntity->m20_position.vx + pFieldScriptEntity->m30_stepVector.vx) >> 16, (pFieldScriptEntity->m20_position.vz + pFieldScriptEntity->m30_stepVector.vz) >> 16);
	refPos2.set(pFieldScriptEntity->m20_position.vx >> 16, pFieldScriptEntity->m20_position.vz >> 16);

	int mask = 0;
	if ((pFieldScriptEntity->m4_flags >> (walkmeshId + 3U & 0x1f) & 1) == 0) {
		mask = -(uint)(noUpdatesToPartyMemebers == '\0');
	}

	int oldWalkmeshId = walkmeshId;

	int iterationCount = 0;
	for (iterationCount = 0; iterationCount < 0x20; iterationCount++)
	{
		sWalkMesh::sTriangleData& pTriangle = (*walkMeshTriangle[walkmeshId])[triangleId];
		std::vector<sWalkMesh::sTriangleData>::iterator pWalkMeshTriangles = walkMeshTriangle[walkmeshId]->begin();
		std::vector<SVECTOR>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();

		sFP1616 vert0;
		sFP1616 vert1;
		sFP1616 vert2;

		vert0.set(pVertices[pTriangle.m0_verticeIndex[0]].vx, pVertices[pTriangle.m0_verticeIndex[0]].vz);
		vert1.set(pVertices[pTriangle.m0_verticeIndex[1]].vx, pVertices[pTriangle.m0_verticeIndex[1]].vz);
		vert2.set(pVertices[pTriangle.m0_verticeIndex[2]].vx, pVertices[pTriangle.m0_verticeIndex[2]].vz);

		int collisionFlag = 0;
		if (NCLIP(vert0, vert1, refPos) < 0) {
			collisionFlag |= 1;
		}

		if (NCLIP(vert1, vert2, refPos) < 0) {
			collisionFlag |= 2;
		}

		if (NCLIP(vert2, vert0, refPos) < 0) {
			collisionFlag |= 4;
		}

		if (collisionFlag < 8) {
			switch (collisionFlag)
			{
			case 0:
				mask = 0xFF;
				break;
			case 1:
				triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
				break;
			case 2:
				triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
				break;
			case 3:
				if (NCLIP(vert1, refPos, refPos2) < 0)
				{
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
				}
				else {
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
				}
				break;
			case 4:
				triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
				break;
			case 5:
				if (NCLIP(vert0, refPos, refPos2) < 0)
				{
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
				}
				else {
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[0];
				}
				break;
			case 6:
				if (NCLIP(vert2, refPos, refPos2) < 0)
				{
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[1];
				}
				else {
					triangleId = pWalkMeshTriangles[triangleId].m6_connectivity[2];
				}
				break;
			case 7:
				triangleId = -1;
				break;
			default:
				assert(0);
			}
		}

		if (triangleId == -1)
		{
			return -1;
		}
	}
	if (iterationCount == 0x20)
	{
		return -1;
	}

	std::vector<SVECTOR>::iterator pVertices = walkMeshVertices[walkmeshId]->begin();
	sWalkMesh::sTriangleData& pTriangle = (*walkMeshTriangle[walkmeshId])[triangleId];

	projectPositionOnTriangle(pVertices[pTriangle.m0_verticeIndex[0]], pVertices[pTriangle.m0_verticeIndex[1]], pVertices[pTriangle.m0_verticeIndex[2]], &projectedPosition, param_4);

	*param_5 = triangleId;

	int triangleParamD = pTriangle.mD * 4;
	if (triangleParamD < 0)
	{
		triangleParamD = 0;
	}

	if (pFieldScriptEntity->m10_walkmeshId == oldWalkmeshId)
	{
		if (((*walkMeshVar1)[pTriangle.mC_indexInWalkmeshData1] & mask & 0x800000) != 0)
		{
			*param_3 = 0x7fffffff;
			*param_6 = 0x7fffffff;
			return 0;
		}

		if (((pFieldScriptEntity->m30_stepVector.vx == 0) && (pFieldScriptEntity->m30_stepVector.vy == 0)) && (pFieldScriptEntity->m30_stepVector.vz == 0)) {
			*param_3 = projectedPosition.vy;
		}
		else
		{
			*param_3 = pFieldScriptEntity->m72_elevation;
		}
	}
	else {
		if (((*walkMeshVar1)[pTriangle.mC_indexInWalkmeshData1] & mask & 0x800000) != 0)
		{
			*param_3 = 0x7fffffff;
			*param_6 = 0x7fffffff;
			return 0;
		}

		*param_3 = projectedPosition.vy;
	}

	EntityMoveCheck1Sub1Sub1(param_3, param_6, triangleId);
	return 0;
}

s16 EntityMoveCheck1Var0 = -1;
s8 EntityMoveCheck1Var1 = 0;
int EntityMoveCheck1(int entityIndex, int mask, sFieldEntity* pFieldEntity, sFieldScriptEntity* pFieldScriptEntity, uint param_5)
{
	sFieldEntitySub4* psVar17 = actorArray[entityIndex].m4_pVramSpriteSheet;
	if (entityIndex == playerControlledEntity) {
		EntityMoveCheck1Var0 = -1;
	}
	if ((pFieldScriptEntity->m0_flags & 0x1000000) != 0) {
		return -1;
	}
	if ((pFieldScriptEntity->m4_flags & 0x200000) != 0) {
		return -1;
	}
	if (((pFieldScriptEntity->m0_flags & 0x10000) != 0) ||
		((((entityIndex != playerControlledEntity || (EntityMoveCheck1Var1 != '\x01')) && ((psVar17->mC).vy == 0)) &&
			(!updateEntityEventCode3Sub0(pFieldScriptEntity) && (psVar17->m84 == (pFieldScriptEntity->m20_position.vy >> 16)))))) {
		return -1;
	}

	sEntityMoveCheck1StackStruct stackVar;

	stackVar.m78_position[0] = pFieldScriptEntity->m20_position.vx;
	stackVar.m78_position[1] = pFieldScriptEntity->m20_position.vy;
	stackVar.m78_position[2] = pFieldScriptEntity->m20_position.vz;

	int sVar2 = pFieldScriptEntity->m10_walkmeshId;

	for (int i = 0; i < 4; i++)
	{
		stackVar.m88[i] = pFieldScriptEntity->m8_currentWalkMeshTriangle[i];
	}

	for (int i = 0; i < 4; i++)
	{
		stackVar.m20[i] = i;
		stackVar.m0[i] = 0x7fffffff;
		stackVar.m10[i] = 0x7fffffff;
	}

	int iVar3 = 0;
	for (iVar3 = 0; iVar3 < numWalkMesh; iVar3++)
	{
		if (EntityMoveCheck1Sub1(pFieldScriptEntity, iVar3, &stackVar.m0[iVar3], &stackVar.m38[iVar3], &stackVar.m30[iVar3], &stackVar.m10[iVar3]));
		{
			break;
		}
	}

	if ((pFieldScriptEntity->m4_flags & 1) != 0) {
		stackVar.m0[0] = 0x7fffffff;
		stackVar.m10[0] = 0x7fffffff;
	}
	if ((pFieldScriptEntity->m4_flags & 2) != 0) {
		stackVar.m0[1] = 0x7fffffff;
		stackVar.m10[1] = 0x7fffffff;
	}
	if ((pFieldScriptEntity->m4_flags & 4) != 0) {
		stackVar.m0[2] = 0x7fffffff;
		stackVar.m10[2] = 0x7fffffff;
	}

	int mask2 = 0x7fffffff; // TODO: what is the initial value?
	int iVar13 = 0;
	int iVar4 = stackVar.m0[pFieldScriptEntity->m10_walkmeshId];
	do {
		int iVar12 = 0;
		int psVar7 = 0;
		s32* piVar11 = stackVar.m10;
		s32* ppsVar14 = stackVar.m20;
		do {
			ppsVar14 = ppsVar14++;
			piVar11 = piVar11 + 1;
			s32 iVar10 = stackVar.m0[psVar7 + 1];
			if (iVar10 < mask2) {
				s32 iVar5 = stackVar.m10[psVar7];
				stackVar.m0[psVar7 + 1] = mask2;
				stackVar.m0[psVar7] = iVar10;
				iVar10 = *piVar11;
				*piVar11 = iVar5;
				stackVar.m10[psVar7] = iVar10;
				mask2 = *(uint*)ppsVar14;
				*ppsVar14 = stackVar.m20[psVar7];
				stackVar.m20[psVar7] = mask2;
			}
			iVar12 = iVar12 + 1;
			psVar7++;
		} while (iVar12 < 2);
		iVar13 = iVar13 + 1;
	} while (iVar13 < 2);

	if (iVar3 == numWalkMesh - 1)
	{
		assert(0);
	}

	pFieldScriptEntity->mF0 = 0;

	if (updateEntityEventCode3Var1 == 0) {
		if (param_5 != 0) {
			if (actorArray[entityIndex].m4_pVramSpriteSheet->m84 < mask + 10) {
				pFieldScriptEntity->m74 = 0xff;
			}
			actorArray[entityIndex].m4_pVramSpriteSheet->m84 = (short)mask;
			(pFieldScriptEntity->m20_position).vy = mask << 0x10;
		}
	}
	else {
		if (param_5 < 2) {
			actorArray[entityIndex].m4_pVramSpriteSheet->m84 = (short)mask;
		}
	}

	auto psVar18 = actorArray[entityIndex].m4_pVramSpriteSheet;
	if ((pFieldScriptEntity->m0_flags & 0x40000) != 0) {
		(pFieldScriptEntity->m20_position).vy = (int)pFieldScriptEntity->mEC_elevation << 0x10;
		(psVar18->mC).vy = 0;
	}
	(pFieldScriptEntity->m20_position).vy = (pFieldScriptEntity->m20_position).vy + (psVar18->mC).vy;
	int uVar6 = getWalkmeshTriangleFlag(pFieldScriptEntity);
	if (pFieldScriptEntity->m10_walkmeshId != sVar2) {
		pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xfbffffff;
	}
	if ((pFieldScriptEntity->m0_flags & 0x4000000) == 0) {
		int sVar1 = (pFieldScriptEntity->m20_position).vy >> 16;
		if (psVar18->m84 <= sVar1) goto LAB_Field__80085104;
		if (psVar18->m84 != sVar1) {
			(psVar18->mC).vy = (psVar18->mC).vy + psVar18->m1C;
		}
		pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags | 0x1000;
		pFieldScriptEntity->mF0 = (psVar18->mC).vy;
	}
	else {
	LAB_Field__80085104:
		if ((uVar6 & 0x420000) == 0) {
			pFieldScriptEntity->mF0 = 0;
		}
		if (0 < (psVar18->mC).vy) {
			(psVar18->mC).vy = 0;
		}
		pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xffbfefff;
		(pFieldScriptEntity->m20_position).vy = (int)psVar18->m84 << 0x10;
	}
	pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xfbffffff;
	iVar3 = 0;
	if (0 < numWalkMesh + -1) {
		iVar4 = pFieldScriptEntity->m20_position.vy >> 16;
		int psVar8 = 0;
		do {
			if (((stackVar.m0[psVar8] < iVar4) && ((int)(iVar4 - (uint)(ushort)pFieldScriptEntity->m18[1]) < stackVar.m10[psVar8])) && (stackVar.m0[psVar8] != stackVar.m10[0])) break;
			iVar3 = iVar3 + 1;
			psVar8++;
		} while (iVar3 < numWalkMesh + -1);
	}
	if ((iVar3 == numWalkMesh + -1) &&
		((iVar3 = (*walkMeshTriangle[pFieldScriptEntity->m10_walkmeshId])[pFieldScriptEntity->m8_currentWalkMeshTriangle[pFieldScriptEntity->m10_walkmeshId]].mD * 4, -1 < iVar3 ||
			(iVar3 + psVar18->m84 <= ((pFieldScriptEntity->m20_position.vy >> 16) - (uint)(ushort)pFieldScriptEntity->m18[1]))))) {
		psVar18->m0_position.vx = (pFieldScriptEntity->m20_position).vx;
		psVar18->m0_position.vy = (pFieldScriptEntity->m20_position).vy;
		psVar18->m0_position.vz = (pFieldScriptEntity->m20_position).vz;
		actorArray[entityIndex].mC_matrix.t[0] = pFieldScriptEntity->m20_position.vx >> 16;
		actorArray[entityIndex].mC_matrix.t[1] = pFieldScriptEntity->m20_position.vy >> 16;
		actorArray[entityIndex].mC_matrix.t[2] = pFieldScriptEntity->m20_position.vz >> 16;
		iVar3 = getWalkmeshTriangleFlag(pFieldScriptEntity);
		pFieldScriptEntity->m14_currentTriangleFlag = iVar3;
	}
	else {
		iVar3 = 0;
		assert(0);
		sEntityMoveCheck1StackStruct* psVar8 = &stackVar;
		(pFieldScriptEntity->m20_position).vx = stackVar.m78_position[0];
		pFieldScriptEntity->m10_walkmeshId = sVar2;
		pFieldScriptEntity->mF0 = 0;
		(pFieldScriptEntity->m20_position).vz = stackVar.m78_position[2];
		auto psVar10 = pFieldScriptEntity;
		do {
			auto psVar17 = psVar8->m88;
			psVar8 = (sEntityMoveCheck1StackStruct*)((int)psVar8->m0 + 2);
			iVar3 = iVar3 + 1;
			psVar10->m8_currentWalkMeshTriangle[0] = *psVar17;
			psVar10 = (sFieldScriptEntity*)((int)&psVar10->m0_flags + 2);
		} while (iVar3 < 4);
		if (psVar18->m84 != *(short*)((int)&(pFieldScriptEntity->m20_position).vy + 2)) {
			(psVar18->mC).vy = (psVar18->mC).vy + psVar18->m1C;
		}
		if ((psVar18->mC).vy < 0) {
			(psVar18->mC).vy = 0;
			(pFieldScriptEntity->m20_position).vy = stackVar.m78_position[1];
		}
		psVar18->m0_position.vx = (pFieldScriptEntity->m20_position).vx;
		psVar18->m0_position.vy = (pFieldScriptEntity->m20_position).vy;
		psVar18->m0_position.vz = (pFieldScriptEntity->m20_position).vz;
		actorArray[entityIndex].mC_matrix.t[1] = pFieldScriptEntity->m20_position.vy >> 16;
	}
LAB_Field__80085490:
	MissingCode();
	//initFollowStructForPlayer(entityIndex);
	return 0;
}

void updateScriptAndMoveEntities()
{
	entityUpdateVar0 = -1;

	exectueEntitiesUpdateFunction();

	for (int i = 0; i < totalActors; i++)
	{
		actorArray[i].m4C_scriptEntity->m68_oldPosition[0] = actorArray[i].m4C_scriptEntity->m20_position.vx >> 16;
		actorArray[i].m4C_scriptEntity->m68_oldPosition[1] = actorArray[i].m4C_scriptEntity->m20_position.vy >> 16;
		actorArray[i].m4C_scriptEntity->m68_oldPosition[2] = actorArray[i].m4C_scriptEntity->m20_position.vz >> 16;
	}
	if (fieldDebugDisable == 0)
	{
		assert(0); // "EVENT_CODE"
	}

	entityUpdateVar1 = 0;

	for (int i = 0; i < totalActors; i++)
	{
		sFieldEntity* pFieldEntity = &actorArray[i];
		sFieldScriptEntity* pFieldScriptEntity = pFieldEntity->m4C_scriptEntity;

		if ((actorArray[i].m58_flags & 0xF80) == 0x200)
		{
			if ((pFieldScriptEntity->m0_flags & 0x10001) == 0)
			{
				if ((pFieldScriptEntity->m4_flags & 0x600) != 0x200)
				{
					pFieldScriptEntity->m14_currentTriangleFlag = getWalkmeshTriangleFlag(pFieldScriptEntity);
					updateEntityEventCode2(i, pFieldEntity, pFieldScriptEntity);
					updateEntityEventCode3(i, pFieldEntity, pFieldScriptEntity);
				}
			}
			else
			{
				MissingCode();
			}
		}
	}

	MissingCode();

	EntityMoveCheck0(playerControlledEntity, &actorArray[playerControlledEntity], actorArray[playerControlledEntity].m4C_scriptEntity);

	MissingCode();

	for (int i = 0; i < totalActors; i++)
	{
		if (actorArray[i].m58_flags & 0xF00)
		{
			sFieldScriptEntity* pFieldScriptEntity = actorArray[i].m4C_scriptEntity;
			if (((((pFieldScriptEntity->m4_flags & 0x600) != 0x200) && ((actorArray[i].m58_flags & 0xf80) == 0x200)) && ((pFieldScriptEntity->m0_flags & 0x10001) == 0)) &&
				((i != playerControlledEntity &&
					EntityMoveCheck1(i, 0x7fffffff, &actorArray[i], pFieldScriptEntity, 0) && (actorArray[i].m4_pVramSpriteSheet->m7C->mC == 1)))) {
				pFieldScriptEntity->m0_flags = pFieldScriptEntity->m0_flags & 0xfffff7ff;
			}
		}
	}

	MissingCode();
}

int updateAllEntitiesSub2Var0 = 0;

uint stepInterpolateDirectionSub(int currentDirection, int targetDirection, int step)
{
	uint uVar1;

	if ((currentDirection - targetDirection & 0xfff) < 0x800) {
		uVar1 = currentDirection - step;
		if (0x7ff < ((currentDirection - step) - targetDirection & 0xfff)) {
			uVar1 = targetDirection;
		}
	}
	else {
		uVar1 = currentDirection + step;
		if (((currentDirection + step) - targetDirection & 0xfff) < 0x800) {
			uVar1 = targetDirection;
		}
	}
	return uVar1 & 0xfff;
}

uint stepInterpolateDirection(int currentDirection, int targetDirection, int step)
{
	if (updateAllEntitiesSub2Var0 == 0) {
		return stepInterpolateDirectionSub(currentDirection, targetDirection, step);
	}
	else {
		return targetDirection & 0xfff;
	}
}

SVECTOR renderModelRotationAngles;
MATRIX renderModelRotationMatrix;
s16 camera2Tan;
s32 cameraDeltaTan;

std::vector<s32>* updateAllEntitiesVar0 = nullptr;

s32 updateCameraInterpolationVar0 = 0;
s32 updateCameraInterpolationVar1 = 0;

void updateCameraInterpolationSub2()
{
	if ((op99Var7 & 0x10) != 0) {
		assert(0);
	}
	if (op9DVar1 != 0) {
		op99Var5 = 1;
		op99Var6 = 1;
		op9DVar1 = op9DVar1 + -1;
	}

	s32 iVar2;
	s32 iVar1 = op99Var5 * op99Var5;
	s32 iVar3 = op99Var6 * op99Var6;
	if ((cameraEye[0] >> 0x10 != cameraEye2.vx >> 0x10) && (iVar2 = cameraEye2.vx - cameraEye[0] >> 0x10, iVar3 <= iVar2 * iVar2)) {
		cameraEye[0] = cameraEye[0] + (cameraEye2.vx - cameraEye[0]) / op99Var6;
	}
	if ((cameraEye[2] >> 0x10 != cameraEye2.vz >> 0x10) && (iVar2 = cameraEye2.vz - cameraEye[2] >> 0x10, iVar3 <= iVar2 * iVar2)) {
		cameraEye[2] = cameraEye[2] + (cameraEye2.vz - cameraEye[2]) / op99Var6;
	}
	if ((cameraEye[1] >> 0x10 != cameraEye2.vy >> 0x10) && (iVar2 = cameraEye2.vy - cameraEye[1] >> 0x10, iVar3 <= iVar2 * iVar2)) {
		cameraEye[1] = cameraEye[1] + (cameraEye2.vy - cameraEye[1]) / op99Var6;
	}
	if ((cameraAt[0] >> 0x10 != cameraAt2.vx >> 0x10) && (iVar3 = cameraAt2.vx - cameraAt[0] >> 0x10, iVar1 <= iVar3 * iVar3)) {
		cameraAt[0] = cameraAt[0] + (cameraAt2.vx - cameraAt[0]) / op99Var5;
	}
	if ((cameraAt[2] >> 0x10 != cameraAt2.vz >> 0x10) && (iVar3 = cameraAt2.vz - cameraAt[2] >> 0x10, iVar1 <= iVar3 * iVar3)) {
		cameraAt[2] = cameraAt[2] + (cameraAt2.vz - cameraAt[2]) / op99Var5;
	}
	if ((cameraAt[1] >> 0x10 != cameraAt2.vy >> 0x10) && (iVar3 = cameraAt2.vy - cameraAt[1] >> 0x10, iVar1 <= iVar3 * iVar3)) {
		cameraAt[1] = cameraAt[1] + (cameraAt2.vy - cameraAt[1]) / op99Var5;
	}

	MissingCode();
}

void updateCameraInterpolation(void)
{
	switch (cameraInterpolationMode)
	{
	case 1:
		updateCameraInterpolationVar0 = 0;
		updateCameraInterpolationVar1 = 0;
		if ((cameraInterpolationFlags & 1) != 0) {
			if (cameraInterpolationTargetNumSteps != 0) {
				cameraInterpolationTargetStartPosition[0] = cameraInterpolationTargetStartPosition[0] + cameraInterpolationTargetStep[0];
				cameraInterpolationTargetStartPosition[1] = cameraInterpolationTargetStartPosition[1] + cameraInterpolationTargetStep[1];
				cameraInterpolationTargetStartPosition[2] = cameraInterpolationTargetStartPosition[2] + cameraInterpolationTargetStep[2];
			}
			cameraInterpolationTargetNumSteps--;
			if ((cameraInterpolationTargetNumSteps & 0xffff) == 0) {
				cameraInterpolationFlags = cameraInterpolationFlags & 0xfffe;
			}
			cameraAt2.vx = cameraInterpolationTargetStartPosition[0];
			cameraAt2.vy = cameraInterpolationTargetStartPosition[1];
			cameraAt2.vz = cameraInterpolationTargetStartPosition[2];
		}
		if ((cameraInterpolationFlags & 2) != 0) {
			if (cameraInterpolationPositionNumSteps != 0) {
				cameraInterpolationStartPosition[0] = cameraInterpolationStartPosition[0] + cameraInterpolationPositionStep[0];
				cameraInterpolationStartPosition[1] = cameraInterpolationStartPosition[1] + cameraInterpolationPositionStep[1];
				cameraInterpolationStartPosition[2] = cameraInterpolationStartPosition[2] + cameraInterpolationPositionStep[2];
			}
			cameraInterpolationTargetNumSteps--;
			if ((cameraInterpolationTargetNumSteps & 0xffff) == 0) {
				cameraInterpolationFlags = cameraInterpolationFlags & 0xfffd;
			}
			cameraEye2.vx = cameraInterpolationStartPosition[0];
			cameraEye2.vy = cameraInterpolationStartPosition[1];
			cameraEye2.vz = cameraInterpolationStartPosition[2];
		}
		break;
	default:
		assert(0);
	}

	updateCameraInterpolationSub2();
	op99VarB = op99VarB & 0xfff;
}

void updateAllEntities()
{
	updateScriptAndMoveEntities();

	createRotationMatrix(&renderModelRotationAngles, &renderModelRotationMatrix);
	renderModelRotationMatrix.t[2] = 0;
	renderModelRotationMatrix.t[1] = 0;
	renderModelRotationMatrix.t[0] = 0;

	cameraTan = ratan2(cameraAt[2] - cameraEye[2], cameraAt[0] - cameraEye[0]) + -0x400;
	camera2Tan = ratan2(cameraAt2.vz - cameraEye2.vz, cameraAt2.vx - cameraEye2.vx) + -0x400;
	cameraDeltaTan = ratan2(length2d(cameraAt[0] - cameraEye[0] >> 0x10, cameraAt[2] - cameraEye[2] >> 0x10), cameraAt[1] - cameraEye[1] >> 0x10);

	std::vector<s32> aiStack72;
	aiStack72.resize(14);
	updateAllEntitiesVar0 = &aiStack72;

	updateCameraInterpolation();

	MissingCode();

	for (int i = 0; i < totalActors; i++)
	{
		sFieldEntity* pActor = &actorArray[i];
		if (((pActor->m58_flags & 0xf40) != 0) && ((pActor->m58_flags & 0x20) == 0)) {
			sFieldScriptEntity* psVar10 = pActor->m4C_scriptEntity;
			u32 uVar7 = psVar10->m4_flags;
			if (((uVar7 & 0x100000) == 0) && ((uVar7 & 0x600) != 0x200)) {
				if ((psVar10->m0_flags & 0x8000) == 0) {
					s32 iVar9;
					s32 iVar8;
					s32 sVar5;
					if (((psVar10->m14_currentTriangleFlag & 0x200000U) == 0) || (iVar9 = 0x200, (psVar10->m0_flags & 0x1800) != 0)) {
						sVar5 = OPX47Var;
						if ((uVar7 & 0x2000) == 0) {
							sVar5 = psVar10->m11E;
						}
						iVar9 = (int)sVar5;
						sVar5 = psVar10->m108_rotation3;
						iVar8 = (int)psVar10->m106_currentRotation;
					}
					else {
						iVar8 = (((uint)psVar10->m14_currentTriangleFlag >> 0xb) - 2 & 7) << 9;
						sVar5 = psVar10->m108_rotation3;
					}
					sVar5 = stepInterpolateDirection(sVar5, iVar8, iVar9);
					psVar10->m108_rotation3 = sVar5;
				}
				if (renderCharsDisabled == '\0') {
					if ((psVar10->m4_flags & 0x1000000) == 0) {
						OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub2(pActor->m4_pVramSpriteSheet, cameraTan + pActor->m4C_scriptEntity->m108_rotation3);
					}
					else {
						OP_INIT_ENTITY_SCRIPT_sub0Sub7(pActor->m4_pVramSpriteSheet, pActor->m4C_scriptEntity->m108_rotation3);
					}
				}
			}
		}
	}

	if (fieldDebugDisable == 0) {
		assert(0);
	}
}

enum class eCameraTrackingMode : s16
{
	e0_followPlayer = 0,
};
eCameraTrackingMode cameraTrackingMode = eCameraTrackingMode::e0_followPlayer;

struct sUpdateCameraAtStruct
{
	s32 m0[4];
	s32 m10[4];
	s32 m20[4];
	s32 m30[4];
	s32 m40[4];
	s32 m50[4];
};

void updateCameraAt(VECTOR* pCameraAt)
{
	sUpdateCameraAtStruct local_98;
	for (int i = 0; i < 3; i++)
	{
		local_98.m0[i] = 0xFFFF;
		local_98.m10[i] = -1;
		local_98.m40[i] = 0;
		local_98.m30[i] = 0;
		local_98.m50[i] = 0;
		local_98.m20[i] = 0;
	}

	for (int i = 0; i < totalActors; i++)
	{
		MissingCode();
	}

	MissingCode();
}

void updateCamera()
{
	switch (cameraTrackingMode)
	{
	case eCameraTrackingMode::e0_followPlayer: // follow player
		updateCameraAt(&actorArray[playerControlledEntity].m4C_scriptEntity->m20_position);
		break;
	default:
		assert(0);
	}
}

std::array<u16, 8> compassData0 = {
	0x81, 0xC0, 0x60, 0x30,
	0x18, 0xC, 0x6, 0x3,
};

std::array<std::array<u16, 16>, 8> compassData1;

std::array<u16, 16> compassData2 = {
	0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,
};

u8 DollyStop = 0;

RECT compassDataRect = { 0,0,0,0 };

VECTOR cameraUp = { 0,0,0,0 };

VECTOR* rotateVectorByMatrix(MATRIX* m, SVECTOR* inputVector, VECTOR* outputVector)
{
	outputVector->vx = m->m[0][0] * inputVector->vx + m->m[1][0] * inputVector->vy + m->m[2][0] * inputVector->vz;
	outputVector->vx = m->m[0][1] * inputVector->vx + m->m[1][1] * inputVector->vy + m->m[2][1] * inputVector->vz;
	outputVector->vx = m->m[0][2] * inputVector->vx + m->m[1][2] * inputVector->vy + m->m[2][2] * inputVector->vz;

	return outputVector;
}

void computeMatrix(MATRIX* pOutputMatrix, VECTOR* param_2, VECTOR* param_3, VECTOR* param_4)
{
	VECTOR local_60 = {
		(param_3->vx - param_2->vx) >> 16,
		(param_3->vy - param_2->vy) >> 16,
		(param_3->vz - param_2->vz) >> 16,
	};
	VECTOR local_30 = {
		(param_4->vx) >> 16,
		(param_4->vy) >> 16,
		(param_4->vz) >> 16,
	};

	VECTOR local_50;
	VectorNormal(&local_60, &local_50);
	OuterProduct12(&local_30, &local_50, &local_60);

	VECTOR local_40;
	VectorNormal(&local_60, &local_40);
	OuterProduct12(&local_50, &local_40, &local_60);

	VectorNormal(&local_60, &local_30);

	pOutputMatrix->m[0][0] = local_40.vx;
	pOutputMatrix->m[0][1] = local_40.vy;
	pOutputMatrix->m[0][2] = local_40.vz;

	pOutputMatrix->m[1][0] = local_30.vx;
	pOutputMatrix->m[1][1] = local_30.vy;
	pOutputMatrix->m[1][2] = local_30.vz;

	pOutputMatrix->m[2][0] = local_50.vx;
	pOutputMatrix->m[2][1] = local_50.vy;
	pOutputMatrix->m[2][2] = local_50.vz;

	SVECTOR local_20 = {
		(param_2->vx >> 16) * 3,
		(param_2->vy >> 16) * 3,
		(param_2->vz >> 16) * 3,
	};

	rotateVectorByMatrix(pOutputMatrix, &local_20, &local_60);

	pOutputMatrix->t[0] = -local_60.vx;
	pOutputMatrix->t[1] = -local_60.vy;
	pOutputMatrix->t[2] = -local_60.vz;
}

void setIdentityMatrix(MATRIX* param_1)
{
	SVECTOR rotation = { 0,0,0 };
	createRotationMatrix(&rotation, param_1);
	param_1->t[0] = 0;
	param_1->t[1] = 0;
	param_1->t[2] = 0;
}

s16 compassArrowTargetDirection = 0;
s16 compassArrowCurrentDirection = 0;

void setCopControlWord(int, int, s32)
{
	MissingCode();
}

void setCopControlWord(int, int, sFP1616)
{
	MissingCode();
}

void SetRotMatrix(const MATRIX* m)
{
	setCopControlWord(2, 0, sFP1616::fromValue(m->m[0][0], m->m[0][1]));
	setCopControlWord(2, 0x800, sFP1616::fromValue(m->m[0][2], m->m[1][0]));
	setCopControlWord(2, 0x1000, sFP1616::fromValue(m->m[1][1], m->m[1][2]));
	setCopControlWord(2, 0x1800, sFP1616::fromValue(m->m[2][0], m->m[2][1]));
	setCopControlWord(2, 0x2000, sFP1616::fromValue(m->m[2][2], 0));
}

void SetTransMatrix(const MATRIX* m)
{
	setCopControlWord(2, 0x2800, m->t[0]);
	setCopControlWord(2, 0x3000, m->t[1]);
	setCopControlWord(2, 0x3800, m->t[2]);
}

void resetMatrixTranslation(MATRIX* m)
{
	m->t[0] = 0;
	m->t[1] = 0;
	m->t[2] = 0;
}

void setCopReg(int, int, sFP1616)
{
	MissingCode();
}

void copFunction(int, u32)
{
	MissingCode();
}

s32 getCopReg(int, int)
{
	MissingCode();
	return 0;
}

MATRIX* MulRotationMatrix(const MATRIX* m0, MATRIX* m1)
{
	SetRotMatrix(m0);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][0], m1->m[1][0]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][0], 0));
	copFunction(2, 0x486012);
	s32 Var2 = getCopReg(2, 0x4800);
	s32 Var3 = getCopReg(2, 0x5000);
	s32 Var4 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][1], m1->m[1][1]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][1], 0));
	copFunction(2, 0x486012);
	s32 Var5 = getCopReg(2, 0x4800);
	s32 Var6 = getCopReg(2, 0x5000);
	s32 Var7 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][2], m1->m[1][2]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][2], 0));
	copFunction(2, 0x486012);

	m1->m[0][0] = Var2;
	m1->m[0][1] = Var5;
	m1->m[2][0] = Var4;
	m1->m[2][1] = Var7;

	Var2 = getCopReg(2, 0x4800);
	Var5 = getCopReg(2, 0x5000);
	m1->m[0][2] = Var2;
	m1->m[1][0] = Var3;
	m1->m[1][1] = Var6;
	m1->m[1][2] = Var5;

	m1->m[2][2] = getCopReg(2, 0xb);

	return m1;
}

MATRIX* CompMatrix(const MATRIX* m0, const MATRIX* m1, MATRIX* m2)
{
	SetRotMatrix(m0);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][0], m1->m[1][0]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][0], 0));
	copFunction(2, 0x486012);
	s32 Var2 = getCopReg(2, 0x4800);
	s32 Var3 = getCopReg(2, 0x5000);
	s32 Var5 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][1], m1->m[1][1]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][1], 0));
	copFunction(2, 0x486012);
	s32 Var7 = getCopReg(2, 0x4800);
	s32 Var8 = getCopReg(2, 0x5000);
	s32 Var9 = getCopReg(2, 0x5800);

	setCopReg(2, 0, sFP1616::fromValue(m1->m[0][2], m1->m[1][2]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->m[2][2], 0));
	copFunction(2, 0x486012);

	m2->m[0][0] = Var2;
	m2->m[0][1] = Var7;
	m2->m[2][0] = Var5;
	m2->m[2][1] = Var9;

	Var2 = getCopReg(2, 0x4800);
	Var7 = getCopReg(2, 0x5000);
	m2->m[0][2] = Var2;
	m2->m[1][0] = Var3;
	m2->m[1][1] = Var8;
	m2->m[1][2] = Var7;

	m2->m[2][2] = getCopReg(2, 0xb);

	// translation
	setCopReg(2, 0, sFP1616::fromValue(m1->t[0], m1->t[1]));
	setCopReg(2, 0x800, sFP1616::fromValue(m1->t[2], 0));
	copFunction(2, 0x486012);

	Var7 = getCopReg(2, 0xc800);
	Var3 = getCopReg(2, 0xd000);
	Var9 = getCopReg(2, 0xd800);
	s32 Var4 = m0->t[1];
	s32 Var6 = m0->t[2];
	m2->t[0] = Var7 + m0->t[0];
	m2->t[1] = Var3 + Var4;
	m2->t[2] = Var9 + Var6;

	return m2;
}

int fieldCompassVar = 0;

struct sFieldCompassVar2Sub
{
	u32 m0;
	std::array<s32, 2> m8;
	std::array<s32, 2> m10;
	std::array<s32, 2> m18;
	std::array<s32, 2> m20;
	//size 0x28
};

struct sFieldCompassVar2
{
	SVECTOR m0;
	SVECTOR m8;
	SVECTOR m10;
	SVECTOR m18;
	std::array<sFieldCompassVar2Sub,2> m20;
	// size 0x70
};

std::array<sFieldCompassVar2, 21> fieldCompassVar2;

struct sDrawCommand
{
	u32 m4;
};

void PushMatrix()
{
	MissingCode();
}

void PopMatrix()
{
	MissingCode();
}

long RotAverage4(SVECTOR* $2, SVECTOR* $3, SVECTOR* v2, SVECTOR* v3, std::array<s32, 2>* sxy0, std::array<s32, 2>* sxy1, std::array<s32, 2>* sxy2, std::array<s32, 2>* sxy3, long* p, long* flag)
{
	MissingCode();
	return(0);
}

void drawCompassArrowSegment(sDrawCommand* param_1, sFieldCompassVar2* param_2, MATRIX* param_3, int frameOddOrEven)
{
	sFieldCompassVar2Sub* psVar1;
	long lStack32;
	long lStack28;

	psVar1 = &param_2->m20[frameOddOrEven];
	PushMatrix();
	SetRotMatrix(param_3);
	SetTransMatrix(param_3);
	RotAverage4(&param_2->m0, &param_2->m8, &param_2->m10, &param_2->m18, &psVar1->m8, &psVar1->m10, &psVar1->m18, &psVar1->m20, &lStack32, &lStack28);
	psVar1->m0 = psVar1->m0 & 0xff000000 | param_1->m4 & 0xffffff;
	param_1->m4 = param_1->m4 & 0xff000000 | (uint)psVar1 & 0xffffff;
	PopMatrix();
}

u8 _drawList[0x80000];

int g_frameOddOrEven = 0;

void LoadImage(RECT* pRect, u8* data)
{
	auto vramIterator = gVram.begin() + pRect->m2_y * 2048 + pRect->m0_x;
	for (int y = 0; y < pRect->m6_h; y++)
	{
		for (int x = 0; x < pRect->m4_w; x++)
		{
			*vramIterator = *(data++);
			vramIterator++;
		}

		vramIterator += 2048 - pRect->m4_w;
	}
}

void LoadImage(RECT* pRect, sPS1Pointer data)
{
	auto vramIterator = gVram.begin() + pRect->m2_y * 2048 + pRect->m0_x;
	for (int y = 0; y < pRect->m6_h; y++)
	{
		for (int x = 0; x < pRect->m4_w; x++)
		{
			*vramIterator = READ_LE_U8(data);
			vramIterator++;
			data = data + 1;
		}

		vramIterator += 2048 - pRect->m4_w;
	}
}

void renderCompass()
{
	for (int i = 0; i < 8; i++)
	{
		if ((compassData0[i] & DollyStop) == 0)
		{
			for (int j = 0; j < 16; j++)
			{
				compassData1[i][j] = compassData2[j];
			}
		}
		else
		{
			for (int j = 0; j < 16; j++)
			{
				compassData1[i][j] = 0;
			}
		}
	}

	compassDataRect.m4_w = 0x80;
	LoadImage(&compassDataRect, (u8*)&compassData1[0][0]);
	SetGeomScreen(0x80);
	SetGeomOffset(0x10a, 0xa6);

	VECTOR local_30 = { 0,0,0 };
	VECTOR local_40 = {
		0,
		cameraEye[1] - cameraAt[1],
		length2d(cameraEye[0] - cameraAt[0] >> 0x10,cameraEye[2] - cameraAt[2] >> 0x10) * -0x10000,
	};

	MATRIX MStack200;
	computeMatrix(&MStack200, &local_40, &local_30, &cameraUp);

	MATRIX MStack232;
	setIdentityMatrix(&MStack232);
	MStack232.t[2] = 0x80;

	SetRotMatrix(&MStack232);
	SetTransMatrix(&MStack232);

	compassArrowTargetDirection = actorArray[actorCameraTracked].m4C_scriptEntity->m106_currentRotation + cameraTan + 0x400;
	compassArrowCurrentDirection = stepInterpolateDirection(compassArrowCurrentDirection, compassArrowTargetDirection, 0x40);;

	SVECTOR local_48;
	local_48.vx = 0;
	local_48.vy = compassArrowCurrentDirection;
	local_48.vz = 0;

	MATRIX MStack168;
	resetMatrixTranslation(&MStack168);
	createRotationMatrix(&local_48, &MStack168);

	MulRotationMatrix(&MStack200, &MStack168);

	MStack168.t[2] = 0x1000;

	MATRIX MStack136;
	CompMatrix(&MStack232, &MStack168, &MStack136);

	if (((OPX_52Param == '\0') && (updateAllEntitiesSub2Var0 == 0))) {
		if (fieldCompassVar == 0)
		{
			for (int i = 0x14; i < 0x15; i++)
			{
				drawCompassArrowSegment((sDrawCommand*)(_drawList + 0x80d4), &fieldCompassVar2[i], &MStack136, g_frameOddOrEven);
			}
		}
	}

	MissingCode();
}

void renderObjects()
{
	MissingCode();
}

void uploadCharacterSpriteSub1(sFieldEntitySub4* param_1, int param_2, sFieldEntitySub4_110* param_3)
{
	sPS1Pointer puVar16 = param_3->m0;
	sPS1Pointer pbVar13 = puVar16 + READ_LE_U16(puVar16 + param_2 * 2);
	int iVar23 = READ_LE_U8(pbVar13 + 3) * param_1->m2C_scale;
	int bVar2 = READ_LE_U8(pbVar13);
	sPS1Pointer local_54 = pbVar13 + 4;
	sPS1Pointer pbVar18 = pbVar13 + (bVar2 & 0x3f) * 2 + 4;
	sFieldEntitySub4_B4_sub* psVar13 = param_1->m20->m30;
	short sVar8 = (short)(iVar23 >> 0xc);
	if (iVar23 < 0) {
		sVar8 = (short)(iVar23 + 0xfff >> 0xc);
	}
	param_1->m36 = sVar8;
	iVar23 = READ_LE_U8(pbVar13 + 1) * param_1->m2C_scale;
	if (iVar23 < 0) {
		iVar23 = iVar23 + 0xfff;
	}
	param_1->m38 = (short)(iVar23 >> 0xc);
	u8 bVar3 = param_1->m3C & 0xFF;
	u16 local_50 = param_3->m4_vramLocation.vy & 0xFF;
	iVar23 = param_1->m28;
	u32 uVar21 = 4;
	u32 uVar22 = 0;

	if ((bVar2 & 0x3f) != 0) {
		do {
			psVar13->m9 = 0;
			psVar13->m8 = 0;
			psVar13->m14 = psVar13->m14 & 0xffffffdf;
			while (true) {
				u8 bVar4 = READ_LE_U8(pbVar18);
				if ((bVar4 & 0x80) == 0) break;
				sPS1Pointer pbVar19 = pbVar18 + 1;
				if ((bVar4 & 0x40) == 0) {
					if ((bVar4 & 4) != 0) {
						psVar13->m14 = psVar13->m14 | 0x20;
					}
					if ((bVar4 & 1) != 0) {
						psVar13->m8 = READ_LE_U8(pbVar19);
						pbVar19 = pbVar18 + 2;
					}
					pbVar18 = pbVar19;
					if ((bVar4 & 2) != 0) {
						bVar4 = READ_LE_U8(pbVar19);
						pbVar18 = pbVar18 + 1;
						psVar13->m9 = bVar4;
					}
				}
				else {
					uVar21 = bVar4 & 7;
					if (param_1->m20->m34 == nullptr) {
						std::array<sFieldEntitySub4_124, 8>* pasVar10 = new std::array<sFieldEntitySub4_124, 8>;
						param_1->m20->m34 = pasVar10;
						OP_INIT_ENTITY_SCRIPT_sub0Sub6Sub1Sub1(param_1);
					}
					if ((bVar4 & 0x20) != 0) {
						(*param_1->m20->m34)[uVar21].m0 = READ_LE_U8(pbVar19);
						(*param_1->m20->m34)[uVar21].m1 = READ_LE_U8(pbVar18 + 2);
						pbVar19 = pbVar18 + 3;
					}
					pbVar18 = pbVar19;
					if ((bVar4 & 0x10) == 0) {
						(*param_1->m20->m34)[uVar21].m6 = 0;
					}
					else {
						bVar4 = READ_LE_U8(pbVar18);
						pbVar18 = pbVar18 + 1;
						(*param_1->m20->m34)[uVar21].m6 = (ushort)bVar4 << 4;
					}
				}
			}
			sPS1Pointer pbVar19 = puVar16 + READ_LE_U16(local_54);
			u8 bVar4 = READ_LE_U8(pbVar19);
			u32 uVar17 = (uint)bVar4;
			local_54 = local_54 + 2;
			s8 cVar20;
			if ((bVar4 & 1) == 0) {
				psVar13->m14 = psVar13->m14 & 0xfffffff7;
				cVar20 = (char)(((ushort)(param_3->m4_vramLocation).vx & 0x3f) >> 2);
			}
			else {
				psVar13->m14 = psVar13->m14 | 8;
				cVar20 = (char)(((ushort)(param_3->m4_vramLocation).vx & 0x3f) >> 1);
			}
			u8 bVar5 = READ_LE_U8(pbVar18);
			u8 bVar7 = bVar5 >> 4;
			u8 uVar15 = bVar7 & 3;
			psVar13->m10 = iVar23;
			if (((bVar7 & 3) != 0) || (uVar15 = (uint)(bVar3 >> 5), uVar15 != 0)) {
				uVar15 = uVar15 - 1;
				*(byte*)((int)&psVar13->m10 + 3) = *(byte*)((int)&psVar13->m10 + 3) | 2;
			}
			if (((int)uVar17 >> 4 & 1U) == 0) {
				sPS1Pointer iVar12;
				if (((param_1->mA8 & 1) == 1) && (iVar12 = param_1->m7C->m18, iVar12.getPointer() != 0)) {
					sPS1Pointer puVar11 = iVar12 + ((uVar17 & 0xe) * 2);
					u16 uVar6 = READ_LE_U16(puVar11 + 2);
					u16 uVar14 = READ_LE_U16(puVar11);
					local_50 = uVar6 & 0xff;
					cVar20 = (char)((uVar14 & 0x3f) >> 2);
					psVar13->mA_tpage = (ushort)((uVar17 & 1) << 7) | (ushort)((uVar15 & 3) << 5) | (short)(uVar6 & 0x100) >> 4 | (ushort)((uVar14 & 0x3ff) >> 6) | (ushort)((uVar6 & 0x200) << 2);
				}
				else {
					u16 uVar6 = (param_3->m4_vramLocation).vy;
					psVar13->mA_tpage =
						(ushort)((uVar17 & 1) << 7) | (ushort)((uVar15 & 3) << 5) | (short)(uVar6 & 0x100) >> 4 | (ushort)((int)((int)(param_3->m4_vramLocation).vx + (uVar17 & 0xe) * 0x20 & 0x3ff) >> 6) |
						(ushort)((uVar6 & 0x200) << 2);
				}
				psVar13->mC_clut = (param_3->m8_clut).vy << 6 | (ushort)((int)((int)(param_3->m8_clut).vx + (bVar5 & 0xf) * 0x10) >> 4) & 0x3f;
			}
			else {
				assert(0);
				/*                pbVar19 = pbVar19 + 1;
								uVar17 = (uint)CONCAT11(*pbVar19, bVar4);
								uVar9 = GetTPage(uVar17 & 1, (int)(short)uVar15, (int)tpageLocationTable[uVar17 & 0xe], (int)tpageLocationTable[(uVar17 & 0xe) + 1]);
								psVar13->mA_tpage = uVar9;
								uVar9 = GetClut((int)uVar17 >> 1 & 0xf0, ((int)uVar17 >> 9 & 0xfU) + 0x1cc);
								psVar13->mC_clut = uVar9;
								*/
			}
			psVar13->m14 = psVar13->m14 & 0xfffffff8U | uVar21;
			psVar13->m4 = cVar20 + READ_LE_U8(pbVar19 + 1);
			psVar13->m5 = (char)local_50 + READ_LE_U8(pbVar19 + 2);
			psVar13->m6 = READ_LE_U8(pbVar19 + 3);
			psVar13->m7 = READ_LE_U8(pbVar19 + 4);
			psVar13->m14 = psVar13->m14 & 0xffffffefU | READ_LE_U8(pbVar18) >> 2 & 0x10;
			if ((bVar2 & 0x80) == 0) {
				psVar13->m0 = READ_LE_S8(pbVar18 + 1);
				psVar13->m2 = READ_LE_S8(pbVar18 + 2);
			}
			else {
				psVar13->m0 = (s16)READ_LE_U8(pbVar18 + 1) | (ushort)(((uint)READ_LE_S8(pbVar18 + 2) << 0x18) >> 0x10);
				pbVar19 = pbVar18 + 3;
				sPS1Pointer pbVar1 = pbVar18 + 4;
				pbVar18 = pbVar18 + 2;
				psVar13->m2 = (ushort)READ_LE_U8(pbVar19) | (ushort)(((uint)READ_LE_U8(pbVar1) << 0x18) >> 0x10);
			}
			psVar13 = psVar13 + 1;
			uVar22 = uVar22 + 1;
			pbVar18 = pbVar18 + 3;
		} while (uVar22 != (bVar2 & 0x3f));
	}
	param_1->m40 = param_1->m40 & 0xffffff03 | (uVar22 & 0x3f) << 2;
}

void uploadCharacterSprite(sFieldEntitySub4* param_1, int param_2, sFieldEntitySub4_110* param_3)
{
	param_1->m40 = param_1->m40 & 0xfff5ffff;
	sPS1Pointer local_50 = param_3->m0;
	sFieldEntitySub4_B4_sub* pFieldEntitySub4_B4_sub = param_1->m20->m30;
	if (param_2 < (int)((READ_LE_U16(local_50) & 0x1ff) + 1)) {
		if ((param_1->m3C >> 0x1e & 1) != 0) {
			param_1->m3C = param_1->m3C & 0xbfffffff;
			sPS1Pointer puVar10 = param_3->mC;
			if (READ_LE_U16(puVar10) != 0) {
				u16 local_58 = (param_3->m8_clut).vx;
				u16 local_56 = (param_3->m8_clut).vy;
				u16 local_52 = 1;
				u16 local_54 = READ_LE_U16(puVar10) << 4;

				// Transfer the clut
				sPS1Pointer clutData = puVar10 + (uint)READ_LE_U16(puVar10) * ((param_1->m3C >> 16) & 0xf0) * 2 + 4;
				int width = ((uint)READ_LE_U16(puVar10) << 0x14) >> 0x10;
				addToShapeTransfertTable(clutData, param_3->m8_clut.vx, param_3->m8_clut.vy, width, 1);
			}
		}
		if ((READ_LE_U16(local_50) & 0x8000) == 0) {
			assert(0);
		}
		else
		{
			uploadCharacterSpriteSub1(param_1, param_2, param_3);
		}
	}
}

void uploadCharacterSprites()
{
	sFieldEntitySub4* psVar1;

	psVar1 = spriteTransfertListHead;
	if (spriteTransfertListHead != (sFieldEntitySub4*)0x0) {
		do {
			if ((ushort)psVar1->m34 == 0) {
				psVar1->m40 = psVar1->m40 & 0xffffff03;
			}
			else {
				uploadCharacterSprite(psVar1, (uint)(ushort)psVar1->m34, psVar1->m24);
			}
			psVar1 = psVar1->m20->m38;
		} while (psVar1 != (sFieldEntitySub4*)0x0);
	}
	spriteTransfertListHead = (sFieldEntitySub4*)0x0;
}

int frameOddOrEven = 0;

void clearShapeTransfertTableEntry(int param_1)
{
	void* ppvVar1 = shapeTransfertTemporaryBuffersLinkedLists[param_1];
	shapeTransfertTableStart = shapeTransfertBuffer[param_1];
	shapeTransfertTableEnd = shapeTransfertTableStart + shapeTransfertTableSize;
	shapeTransfertTableCurrentEntry = shapeTransfertTableStart;
	shapeTransfertDoubleBufferIndex = param_1;
	while (ppvVar1 != nullptr) {
		assert(0);
		/*free(*ppvVar1);
		ppvVar1 = (void**)ppvVar1[1];*/
	}
	shapeTransfertTemporaryBuffersLinkedLists[param_1] = nullptr;
	return;
}

void renderChars()
{
	if (renderCharsDisabled != '\x01') {
		clearShapeTransfertTableEntry(frameOddOrEven);
		MissingCode();
		uploadCharacterSprites();
		MissingCode();
	}
}

void renderParticles()
{
	MissingCode();
}

void ClearImage(RECT* pRect, u8 r, u8 g, u8 b)
{
	MissingCode();
}

void shapeTransfert()
{
	sShapeTransfert* rect;

	rect = shapeTransfertTable[shapeTransfertDoubleBufferIndex];
	while (rect != (sShapeTransfert*)0x0) {
		if (rect->m8_pData.getPointer() == nullptr) {
			ClearImage(&rect->m0_rect, '\0', '\0', '\0');
		}
		else {
			LoadImage(&rect->m0_rect, rect->m8_pData);
		}
		rect = rect->mC_pNext;
	}
	shapeTransfertTable[shapeTransfertDoubleBufferIndex] = (sShapeTransfert*)0x0;
}


void updateAndRenderField()
{
	MissingCode();
	updateAllEntities();
	updateCamera();
	if (fieldDebugDisable == 0) {
		assert(0);
	}
	MissingCode(); //updateScreenEffectSlots(_DAT_800c426c[0x166].dr_env.code + 3, frameOddOrEven);
	if (fieldDebugDisable == 0) {
		assert(0);
	}
	renderCompass();
	MissingCode();
	renderObjects();
	renderChars();
	renderParticles();
	MissingCode();
	shapeTransfert();
	MissingCode();
}

int runningOnDTL = -1;

int compassGraphicDataLoaded = 0;
std::vector<u8> compassGraphicDataStaging;

void initCompassData()
{
	if (compassGraphicDataLoaded == 0)
	{
		compassGraphicDataStaging.resize(getFileSizeAligned(0xA7));
		flagAllocation(compassGraphicDataStaging);
		readFile(0xA7, compassGraphicDataStaging, 0, 0x80);
		waitReadCompletion(0);
	}

	unflagAllocation(compassGraphicDataStaging);
	compassGraphicDataLoaded = 0;

	MissingCode();
}

void fieldEntryPoint()
{
	fieldDebugDisable = (runningOnDTL == -1);
	ClearCacheAfterOverlayLoad();
	if (fieldDebugDisable == 0) {
		assert(0);
		//DrawSyncCallback(fieldDebuggerDrawSyncCallback);
	}

	MissingCode();

	setCurrentDirectory(4, 0); // TODO: this is not explicitly called at this level

	allocatePartyCharacterBuffers();

	MissingCode();

	fieldExecuteVar3 = -1;
	fieldExecuteVar2 = -1;
	playMusicAuthorized = -1;

	MissingCode();

	fieldMapNumber = gameState.m231A_fieldID;
	pKernelGameState = &gameState;

	MissingCode();

	initCompassData();
	fieldRequestedGears = 0;
	startLoadingPlayableCharacters();
	finalizeLoadPlayableCharacters();

	MissingCode();

	bootField();

	MissingCode();

	while (!noahFrame_end())
	{
		noahFrame_start();

		MissingCode();
		////
		updateAndRenderField();

		MissingCode();
		////
		if ((fieldChangePrevented == 0) /*&& (fieldChangePrevented2 == 0)*/)
		{
			//if ((asyncLoadingVar1 == 0xff) && (DAT_Field__800adb90 == 0))
			{
				setCurrentDirectory(4, 0); //TODO: shouldn't be necessary!
				if (loadNewField((fieldMapNumber & 0xfff) * 2, 0) == 0)
				{
					if (isCDBusy() == 0)
					{
						//if (screenEffects[0].m3C_duration == 0)
						{
							//fieldTransitionCompleted = 0;
							//saveStateToKernel();
							waitReadCompletion(0);
							transitionFields();
							//resetInputs();
							//fieldTransitionCompleted = 1;
						}
					}
				}
			}
		}
		////
		MissingCode();
	}
}
