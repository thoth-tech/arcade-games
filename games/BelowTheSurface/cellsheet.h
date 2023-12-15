#include "splashkit.h"
#pragma once

class CellSheet
{
    public:
        bitmap cells;
        string type;
        int number_of_cells;
        int offset;
        
        CellSheet(bitmap cells, string type, int offset)
        {
            this->cells = cells;
            this->type = type;
            this->number_of_cells = bitmap_cell_count(cells);
            this->offset = offset;
        };

        ~CellSheet(){};

};