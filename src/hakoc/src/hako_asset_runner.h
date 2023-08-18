#ifndef _HAKO_ASSET_RUNNER_H_
#define _HAKO_ASSET_RUNNER_H_



#ifdef __cplusplus
extern "C" {
#endif

#include "hako_capi_types.h"

/***********************
 * simulation control
 ***********************/

/**
 * @brief 箱庭アセットの初期化関数
 *
 * この関数は箱庭アセットを初期化します。
 *
 * @param[in] asset_name アセット名
 * @param[in] config_path 箱庭アセットのコンフィグファイルパス
 * @return 初期化が成功した場合は true、失敗した場合は false
 */
extern bool hako_asset_runner_init(const char* asset_name, const char* config_path);

/**
 * @brief 箱庭アセットの終了関数
 *
 * この関数は箱庭アセットを終了します。
 */
extern void hako_asset_runner_fin(void);
/**
 * @brief 箱庭アセットのステップ関数
 *
 * この関数は箱庭アセットのシミュレーション時間を１ステップを進めます。
 *
 * @param[in] increment_step ステップを進める量（箱庭アセットのコンフィグで定義した delta_time_usec 単位）
 * @return ステップの進行に成功した場合は true、失敗した場合は false
 */
extern bool hako_asset_runner_step(hako_time_t increment_step);


/***********************
 * pdu io
 ***********************/
/**
 * @brief 箱庭アセットのPDU読み取り関数
 *
 * この関数は指定されたロボットの指定されたローカルチャネルからPDUを読み取ります。
 *
 * @param[in] robo_name ロボット名
 * @param[in] lchannel 論理チャネルID
 * @param[out] buffer 読み取られたデータを格納するバッファ
 * @param[in] buffer_len バッファの長さ
 * @return 読み取りが成功した場合は true、失敗した場合は false
 */
extern bool hako_asset_runner_pdu_read(const char* robo_name, HakoPduChannelIdType lchannel, char* buffer, size_t buffer_len);

/**
 * @brief 箱庭アセットのPDU書き込み関数
 *
 * この関数は指定されたロボットの指定されたローカルチャネルにPDUを書き込みます。
 *
 * @param[in] robo_name ロボット名
 * @param[in] lchannel 論理チャネルID
 * @param[in] buffer 書き込むデータが格納されたバッファ
 * @param[in] buffer_len バッファ内のデータ長さ
 * @return 書き込みが成功した場合は true、失敗した場合は false
 */
extern bool hako_asset_runner_pdu_write(const char* robo_name, HakoPduChannelIdType lchannel, const char* buffer, size_t buffer_len);


#ifdef __cplusplus
}
#endif


#endif /* _HAKO_ASSET_RUNNER_H_ */