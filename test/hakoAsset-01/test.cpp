#include "gtest/gtest.h"
// ここにはテスト対象のヘッダーファイルをインクルードします。
#include "hako_asset.h"
#include "hako_conductor.h"

// コールバック関数の実装
static int my_on_initialize(hako_asset_context_t* context)
{
    std::cout << "INFO: my_on_initialize enter" << std::endl;
    return 0;
}

static int my_on_reset(hako_asset_context_t* context)
{
    std::cout << "INFO: my_on_reset enter" << std::endl;
    return 0;
}

static int my_on_simulation_step(hako_asset_context_t* context)
{
    std::cout << "INFO: on_simulation_step enter: " << hako_asset_simulation_time() << std::endl;
    std::cout << "INFO: sleep 1sec" << std::endl;
    usleep(1000*1000);
    std::cout << "INFO: on_simulation_step exit" << std::endl;
    return 0;
}

hako_asset_callbacks_t callbacks = {
    .on_initialize = my_on_initialize,
    .on_simulation_step = my_on_simulation_step,
    .on_reset = my_on_reset
};

hako_time_t delta_time_usec = 1000 * 1000 * 100;

TEST(HakoCApiTest, AssetRegist) {
    hako_conductor_start(delta_time_usec, delta_time_usec);
    int result = hako_asset_register("AssetDumy", "../../config.json", &callbacks,  1000, HAKO_ASSET_MODEL_CONTROLLER);
    ASSERT_EQ(result, 0);
}

TEST(HakoCApiTest, AssetStart) {
    int result = hako_asset_start();
    ASSERT_EQ(result, EINTR);   // Resetで終了するのでEINTRが返る
}

TEST(HakoCApiTest, ConductorStop) {
    hako_conductor_stop();
    // ASSERT_EQ(result, 0);
}



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}