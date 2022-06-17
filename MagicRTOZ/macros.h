#pragma once

#define JOIN_MACROS(x, y) _JOIN_MACROS_AGAIN(x, y)
#define _JOIN_MACROS_AGAIN(x, y) x ## y