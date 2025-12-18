#include "napi/native_api.h"
// 引入OpenHarmony Hilog C语言版头文件（正确的头文件）
#include "hilog/log.h"
#include <vector>
#include "Algorithm/UGSort.h"

// 定义日志域（domain）：使用自定义的32位整数，符合OpenHarmony规范
#define LOG_DOMAIN 0x8000 // 可自定义，范围0x0~0x7FFFFFFF
// 定义日志标签（tag）：长度不超过32个字符，描述模块功能
#define LOG_TAG "NAPI_Calc_Module"

// 辅助函数：将napi_value的数组转换为C++的vector<int>
static bool NapiArrayToVector(napi_env env, napi_value array, std::vector<double>& out_vec) {
    // 1. 获取数组长度
    uint32_t arr_len = 0;
    napi_status status = napi_get_array_length(env, array, &arr_len);
    if (status != napi_ok) {
        return false;
    }

    // 2. 遍历数组，将每个元素转为int并存入vector
    out_vec.clear();
    for (uint32_t i = 0; i < arr_len; i++) {
        napi_value element;
        status = napi_get_element(env, array, i, &element);
        if (status != napi_ok) {
            return false;
        }

        double num = 0;
        status = napi_get_value_double(env, element, &num);
        if (status != napi_ok) {
            return false;
        }

        out_vec.push_back(num);
    }

    return true;
}

// 辅助函数：将C++的vector<int>转换为napi_value的数组
static napi_value VectorToNapiArray(napi_env env, const std::vector<double>& vec) {
    // 1. 创建新数组
    napi_value array;
    napi_status status = napi_create_array(env, &array);
    if (status != napi_ok) {
        return nullptr;
    }

    // 2. 将vector中的元素存入数组
    for (uint32_t i = 0; i < vec.size(); i++) {
        napi_value element;
        status = napi_create_double(env, vec[i], &element);
        if (status != napi_ok) {
            return nullptr;
        }

        status = napi_set_element(env, array, i, element);
        if (status != napi_ok) {
            return nullptr;
        }
    }

    return array;
}

// 辅助函数：打印std::vector<int>的内容
void printVector(const std::vector<double>& vec) {
    if (vec.empty()) {
        OH_LOG_INFO(LOG_APP, "[INFO] 空数组!!!");
        return;
    }
    std::string oss;
    for (int num : vec) {
        oss+= " " +std::to_string(num);
    }
    OH_LOG_INFO(LOG_APP,"[INFO] 当前输出数组: %{public}s", oss.c_str());
}

// 核心排序函数：接收数组和排序类型（asc:递增，desc:递减），返回排序后的数组
static napi_value QuickSort(napi_env env, napi_callback_info info) {
    // 1. 获取传入的参数（参数1：数字数组，参数2：排序类型字符串"asc"/"desc"）
    size_t argc = 2;
    napi_value args[2];
    napi_status status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
    if (status != napi_ok || argc < 2) {
        napi_throw_error(env, nullptr, "参数错误：需要传入数组和排序类型（asc/desc）");
        return nullptr;
    }

    // 2. 处理第一个参数：数字数组
    std::vector<double> num_vec;
    if (!NapiArrayToVector(env, args[0], num_vec)) {
        napi_throw_error(env, nullptr, "第一个参数必须是数字数组");
        return nullptr;
    }

    // 3. 处理第二个参数：排序类型（asc/desc）
    char sort_type[10] = {0};
    size_t str_len = 0;
    status = napi_get_value_string_utf8(env, args[1], sort_type, sizeof(sort_type), &str_len);
    if (status != napi_ok) {
        napi_throw_error(env, nullptr, "第二个参数必须是字符串（asc/desc）");
        return nullptr;
    }
    
    if (strcmp(sort_type, "asc") == 0) {
        // 递增排序：使用std::sort默认的升序
        OH_LOG_INFO(LOG_APP, "[Init] 正在使用SuAlgorithm Qsort算法->递增");
        UGC::UGSort<double>::QSort(num_vec.data(), 0, num_vec.size() - 1,TRUE);
        printVector(num_vec);
    } else if (strcmp(sort_type, "desc") == 0) {
        // 递减排序：使用std::greater<int>()
        OH_LOG_INFO(LOG_APP, "[Init] 正在使用SuAlgorithm Qsort算法->递减");
        UGC::UGSort<double>::QSort(num_vec.data(), 0, num_vec.size() - 1,FALSE);
        printVector(num_vec);
    } else {
        napi_throw_error(env, nullptr, "排序类型只能是asc（递增）或desc（递减）");
        return nullptr;
    }

    // 5. 将排序后的vector转为napi数组并返回
    return VectorToNapiArray(env, num_vec);
}


// 模块初始化：将所有函数暴露给JS
EXTERN_C_START
static napi_value Init(napi_env env, napi_value exports)
{
    napi_property_descriptor desc[] = {
        { "QuickSort", nullptr, QuickSort, nullptr, nullptr, nullptr, napi_default, nullptr }
    };
    // 将所有函数挂载到exports对象上
    napi_define_properties(env, exports, sizeof(desc) / sizeof(desc[0]), desc);

    OH_LOG_INFO(LOG_APP, "[Init] 模块初始化完成");

    return exports;
}
EXTERN_C_END

// 模块元信息定义
static napi_module libugc = {
    .nm_version = 1,
    .nm_flags = 0,
    .nm_filename = nullptr,
    .nm_register_func = Init,
    .nm_modname = "ugc", // 模块名改为calc
    .nm_priv = ((void*)0),
    .reserved = { 0 },
};

// 程序启动时自动注册模块
extern "C" __attribute__((constructor)) void Registerlibugc(void)
{
    OH_LOG_INFO(LOG_APP, "[Register] 开始注册ugc模块");
    napi_module_register(&libugc);
}