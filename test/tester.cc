// ---------------------------------------------------------------------------
// IMLAB
// ---------------------------------------------------------------------------

#include <gtest/gtest.h>
#include "tbb/task_scheduler_init.h"

int main(int argc, char *argv[]) {
    // add a tbb::task_scheduler_init, because otherwise the sanitizer will complain about leaks inside TBB.
    tbb::task_scheduler_init init;

    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
