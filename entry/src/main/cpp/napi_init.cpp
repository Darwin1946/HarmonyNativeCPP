#include "napi/native_api.h"
// 引入OpenHarmony Hilog C语言版头文件（正确的头文件）
#include "hilog/log.h"

// 定义日志域（domain）：使用自定义的32位整数，符合OpenHarmony规范
#define LOG_DOMAIN 0x8000 // 可自定义，范围0x0~0x7FFFFFFF
// 定义日志标签（tag）：长度不超过32个字符，描述模块功能
#define LOG_TAG "NAPI_Calc_Module"

/**
 * @brief 加法函数：两数相加
 */
static napi_value Add(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {nullptr};

    // 修正：去掉LOG_CORE，使用 LOG_DOMAIN, LOG_TAG 作为前两个参数
    OH_LOG_INFO(LOG_APP, "[Add] 开始执行加法函数");
    

    // 提取JS传递的参数
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Add] 提取参数失败，状态码：%{public}d", status);
        return nullptr;
    }

    // 检查参数个数
    if (argc < 2) {
        OH_LOG_WARN(LOG_APP, "[Add] 参数个数不足，需要2个，实际：%{public}zu", argc);
        return nullptr;
    }

    // 转换参数为double类型
    double value0, value1;
    status = napi_get_value_double(env, args[0], &value0);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Add] 第一个参数转换为数字失败");
        return nullptr;
    }
    status = napi_get_value_double(env, args[1], &value1);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Add] 第二个参数转换为数字失败");
        return nullptr;
    }

    OH_LOG_DEBUG(LOG_APP, "[Add] 参与运算的数值：%{public}lf, %{public}lf", value0, value1);

    // 计算加法
    double result = value0 + value1;
    napi_value napi_result;
    napi_create_double(env, result, &napi_result);

    OH_LOG_INFO(LOG_APP, "[Add] 加法结果：%{public}lf", result);

    return napi_result;
}

/**
 * @brief 减法函数：第一个数减第二个数
 */
static napi_value Sub(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {nullptr};

    OH_LOG_DEBUG(LOG_APP, "[Sub] 开始执行减法函数");

    // 提取参数
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Sub] 提取参数失败，状态码：%{public}d", status);
        return nullptr;
    }

    if (argc < 2) {
        OH_LOG_WARN(LOG_APP, "[Sub] 参数个数不足，需要2个，实际：%{public}zu", argc);
        return nullptr;
    }

    // 转换参数为double
    double value0, value1;
    status = napi_get_value_double(env, args[0], &value0);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Sub] 第一个参数转换为数字失败");
        return nullptr;
    }
    status = napi_get_value_double(env, args[1], &value1);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Sub] 第二个参数转换为数字失败");
        return nullptr;
    }

    OH_LOG_DEBUG(LOG_APP, "[Sub] 参与运算的数值：%{public}lf, %{public}lf", value0, value1);

    // 计算减法
    double result = value0 - value1;
    napi_value napi_result;
    napi_create_double(env, result, &napi_result);

    OH_LOG_INFO(LOG_APP, "[Sub] 减法结果：%{public}lf", result);

    return napi_result;
}

/**
 * @brief 乘法函数：两数相乘
 */
static napi_value Mul(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {nullptr};

    OH_LOG_DEBUG(LOG_APP, "[Mul] 开始执行乘法函数");

    // 提取参数
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Mul] 提取参数失败，状态码：%{public}d", status);
        return nullptr;
    }

    if (argc < 2) {
        OH_LOG_WARN(LOG_APP, "[Mul] 参数个数不足，需要2个，实际：%{public}zu", argc);
        return nullptr;
    }

    // 转换参数为double
    double value0, value1;
    status = napi_get_value_double(env, args[0], &value0);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Mul] 第一个参数转换为数字失败");
        return nullptr;
    }
    status = napi_get_value_double(env, args[1], &value1);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Mul] 第二个参数转换为数字失败");
        return nullptr;
    }

    OH_LOG_DEBUG(LOG_APP, "[Mul] 参与运算的数值：%{public}lf, %{public}lf", value0, value1);

    // 计算乘法
    double result = value0 * value1;
    napi_value napi_result;
    napi_create_double(env, result, &napi_result);

    OH_LOG_INFO(LOG_APP, "[Mul] 乘法结果：%{public}lf", result);

    return napi_result;
}

/**
 * @brief 除法函数：第一个数除以第二个数（重点处理除零错误）
 */
static napi_value Div(napi_env env, napi_callback_info info)
{
    size_t argc = 2;
    napi_value args[2] = {nullptr};

    OH_LOG_DEBUG(LOG_APP, "[Div] 开始执行除法函数");

    // 提取参数
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Div] 提取参数失败，状态码：%{public}d", status);
        return nullptr;
    }

    if (argc < 2) {
        OH_LOG_WARN(LOG_APP, "[Div] 参数个数不足，需要2个，实际：%{public}zu", argc);
        return nullptr;
    }

    // 转换参数为double
    double value0, value1;
    status = napi_get_value_double(env, args[0], &value0);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Div] 被除数转换为数字失败");
        return nullptr;
    }
    status = napi_get_value_double(env, args[1], &value1);
    if (status != napi_ok) {
        OH_LOG_ERROR(LOG_APP, "[Div] 除数转换为数字失败");
        return nullptr;
    }

    OH_LOG_DEBUG(LOG_APP, "[Div] 被除数：%{public}lf，除数：%{public}lf", value0, value1);

    // 关键：处理除零错误（除数不能为0）
    if (value1 == 0.0) {
        OH_LOG_ERROR(LOG_APP, "[Div] 错误：除数不能为0");
        return nullptr; // 返回null表示调用失败，JS层会收到undefined或null
    }

    // 计算除法
    double result = value0 / value1;
    napi_value napi_result;
    napi_create_double(env, result, &napi_result);
    
    OH_LOG_INFO(LOG_APP, "[Div] 除法结果：%{public}lf", result);

    return napi_result;
}

// 模块初始化：将所有运算函数暴露给JS
static napi_value NAPI_Global_QuickSort(napi_env env, napi_callback_info info) {
    // TODO: implements the code;
}
EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports) {
    // 定义属性描述符数组：新增sub、mul、div三个函数
    napi_property_descriptor desc[] = {
        {"add", nullptr, Add, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"sub", nullptr, Sub, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"mul", nullptr, Mul, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"div", nullptr, Div, nullptr, nullptr, nullptr, napi_default, nullptr},
        {"QuickSort", nullptr, NAPI_Global_QuickSort, nullptr, nullptr, nullptr, napi_default, nullptr}};

    // 将所有函数挂载到exports对象上
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);

    OH_LOG_INFO(LOG_APP, "[Init] 加减乘除模块初始化完成");

    return exports;
}
EXTERN_C_END

// 模块元信息定义
static napi_module libcalc = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "calc", // 模块名改为calc
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

// 程序启动时自动注册模块
extern "C" __attribute__((constructor)) void Registerlibcalc(void)
{
    OH_LOG_INFO(LOG_APP, "[Register] 开始注册calc模块");
    napi_module_register(&libcalc);
}