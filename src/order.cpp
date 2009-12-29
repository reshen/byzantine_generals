#include "order.h"

#include <vector>
#include <iostream>

order::order():
    initialOrder_(OT_INVALID)
{
    communicationPath_ = std::vector<int>();
}

