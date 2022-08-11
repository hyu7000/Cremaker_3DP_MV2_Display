#ifndef _ADD_Z_OFFSET_CAL_H_
#define _ADD_Z_OFFSET_CAL_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "includes.h"

#define PROBING_POINT_X0 35 
#define PROBING_POINT_Y0 85
#define PROBING_POINT_X1 195
#define PROBING_POINT_Y1 245

#define PROBING_POINT_X_COL 3
#define PROBING_POINT_Y_ROW 3

#define FRIST_LAYER_MOVE_SPEED 4000
#define FRIST_LAYER_PRINTING_SPEED 1000
#define FRIST_LAYER_EXTRUDE_MM (1.5)

#define Z_CAL_PRINTING_LINE_WIDTH (15)


typedef struct
{
    uint16_t rowN;
    uint16_t colN;
    short rowindex;
    short colindex;
    short posindex;
    short printingindex;
    float add_z_value[MESH_GRID_MAX_POINTS_X][MESH_GRID_MAX_POINTS_Y];
    float max_add_z_value, min_add_z_value;
    bool isNeedRedraw;
} ADDZOFFSETCAL;

extern ADDZOFFSETCAL addZOffsetCal;

void menuAddZOffsetCal(void);

#ifdef __cplusplus
}
#endif

#endif
