#include "LoadUnload.h"
#include "includes.h"

#define LOAD 1
#define UNLOAD 2
#define NONE 0

#define HEAT_TEMP_HOTEND 200

const MENUITEMS loadUnloadItems = {
  // title
  LABEL_LOAD_UNLOAD,
  // icon                         label
  {{ICON_BACKGROUND,              LABEL_BACKGROUND},
   {ICON_BACKGROUND,              LABEL_BACKGROUND},
   {ICON_BACKGROUND,              LABEL_BACKGROUND},
   {ICON_BACKGROUND,              LABEL_BACKGROUND},
   {ICON_FILA_WEIGHT,             LABEL_SET_FILA_WEIGHT},
   {ICON_LOAD,                    LABEL_LOAD},
   {ICON_UNLOAD,                  LABEL_UNLOAD},
   {ICON_BACK,                    LABEL_BACK},}
};

static uint8_t curExt_index = 0;
static uint8_t lastcmd = NONE;

void extruderIdReDraw(void)
{
  char tempstr[20];

  sprintf(tempstr, "%2s: %3d/%-3d", heatDisplayID[curExt_index], heatGetCurrentTemp(curExt_index), heatGetTargetTemp(curExt_index));
  setLargeFont(true);
  GUI_DispStringInPrect(&exhibitRect, (uint8_t *)tempstr);
  setLargeFont(false);
}

// set the hotend to the minimum extrusion temperature if user selected "OK"
void loadMinTemp_OK(void)
{
  heatSetTargetTemp(curExt_index, HEAT_TEMP_HOTEND);
}

void menuLoadUnload(void)
{
  startTesting();
  KEY_VALUES key_num = KEY_IDLE;
  //while(infoCmd.count != 0) {loopProcess();}
  loopProcessToCondition(&isNotEmptyCmdQueue);  // wait for the communication to be clean

  menuDrawPage(&loadUnloadItems);
  extruderIdReDraw();

  while(infoMenu.menu[infoMenu.cur] == menuLoadUnload)
  {
    key_num = menuKeyGetValue();

    if (infoHost.wait == true && key_num != KEY_IDLE &&
        key_num != KEY_ICON_7)  // show reminder for process running if presses any button other than bacnk button
    {
      if (lastcmd == UNLOAD)
      { // unloading
        popupReminder(DIALOG_TYPE_INFO, LABEL_UNLOAD, LABEL_UNLOAD_STARTED);
      }
      else if (lastcmd == LOAD)
      { // loading
        popupReminder(DIALOG_TYPE_INFO, LABEL_LOAD, LABEL_LOAD_STARTED);
      }
      else
      { // in case user gets to Load/Unload menu while host is busy
        popupReminder(DIALOG_TYPE_INFO, LABEL_SCREEN_INFO, LABEL_BUSY);
      }
    }
    else
    {
      switch(key_num)
      {
      case KEY_ICON_4:
        storeCmd("M408\n");
        infoMenu.menu[++infoMenu.cur] = menuSelectFilaWeight;
        break;

      case KEY_ICON_6: // Unload
      case KEY_ICON_5: // Load
        if (heatGetCurrentTemp(curExt_index) < (HEAT_TEMP_HOTEND-5))
        { // low temperature warning
          char tempMsg[120];
          LABELCHAR(tempStr, LABEL_EXT_TEMPLOW);
          sprintf(tempMsg, tempStr, HEAT_TEMP_HOTEND);
          strcat(tempMsg, "\n");
          sprintf(tempStr, (char *)textSelect(LABEL_HEAT_HOTEND), HEAT_TEMP_HOTEND);
          strcat(tempMsg, tempStr);

          setDialogText(LABEL_WARNING, (uint8_t *)tempMsg, LABEL_CONFIRM, LABEL_CANCEL);
          showDialog(DIALOG_TYPE_ERROR, loadMinTemp_OK, NULL, NULL);
        }
        else if (key_num == KEY_ICON_6)
        { // unload
          mustStoreCmd("M702 T%d\n", curExt_index);
          lastcmd = UNLOAD;
        }
        else
        { // load
          mustStoreCmd("M701 T%d\n", curExt_index);
          lastcmd = LOAD;
        }
        break;

      case KEY_ICON_7:
        infoMenu.cur--;
        finishTesting();
        lastcmd = NONE;
        break;

      default:
        extruderIdReDraw();
        break;
      }
    }
    loopProcess();
  }
}
