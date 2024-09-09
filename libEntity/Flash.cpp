#include "Flash.h"
#include "EntityResList.h"

Flash::Flash(int x, int y): 
    Item(ET_flash, x, y)
{
    pixmap.load(HINT_IMAGE);
}
