#include <test.h>
#include <core/printfun.h>

int main() {
    fr_print_function(5); // exported from the fracture library
    print_func(3); // from the base library
    return 0;
}
