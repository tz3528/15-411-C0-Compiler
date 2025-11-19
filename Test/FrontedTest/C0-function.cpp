// 综合测试用例，用于测试C0语言词法分析器的各种边界情况和潜在问题

// 1. 基本数据类型和变量声明
int main() {
    int a = 42;
    bool flag = true;
    bool false_flag = false;
    char c = 'x';
    string s = "hello world";

    // 2. 各种运算符测试
    int b = a + 10;
    int c = b - 5;
    int d = c * 2;
    int e = d / 3;
    int f = e % 7;

    // 复合赋值运算符
    a += 10;
    b -= 5;
    c *= 2;
    d /= 3;
    e %= 7;

    // 比较运算符
    bool eq = (a == b);
    bool ne = (a != b);
    bool lt = (a < b);
    bool gt = (a > b);
    bool le = (a <= b);
    bool ge = (a >= b);

    // 逻辑运算符
    bool and_result = (true && false);
    bool or_result = (true || false);
    bool not_result = !true;

    // 位运算符
    int bit_and = a & b;
    int bit_or = a | b;
    int bit_xor = a ^ b;

    // 3. 控制结构关键字
    if (flag) {
        int x = 1;
    } else {
        int y = 2;
    }

    while (a > 0) {
        a = a - 1;
    }

    for (int i = 0; i < 10; i += 1) {
        int temp = i;
    }

    // 4. 函数相关
    return 0;
}

// 5. 函数声明和定义
int add(int x, int y) {
    return x + y;
}

void test_function() {
    int result = add(3, 4);
    assert(result == 7);
}

// 6. 结构体和类型定义
struct Point {
    int x;
    int y;
};

typedef int my_int;

// 7. 内存分配关键字
void memory_test() {
    int* ptr = alloc(int);
    int* arr = alloc_array(int, 10);
}

// 8. 特殊值和常量
void constant_test() {
    bool t = true;
    bool f = false;
    int null_val = NULL;
    continue;
    break;
}

// 9. 标识符和关键字边界测试
void identifier_test() {
    // 合法标识符
    int _private = 1;
    int camelCase = 2;
    int CamelCase = 3;
    int _ = 4;
    int abc123 = 5;

    // 关键字不能作为标识符
    // int int = 5;  // 应该被识别为关键字，而非标识符
}

// 10. 字符串和转义字符测试
void string_test() {
    string empty = "";
    string normal = "hello";
    string with_spaces = "hello world";
    string with_numbers = "12345";
    // 注意：当前实现可能不支持转义字符
}

// 11. 数字字面量测试
void number_test() {
    int zero = 0;
    int positive = 12345;
    // 注意：当前实现似乎不支持负数和十六进制数
}

// 12. 注释测试
void comment_test() {
    int a = 1; // 单行注释
    int b = 2; /* 单行多行注释 */
    /*
     * 多行注释
     * 包含多行
     */
    int c = 3;
    /*
     * 嵌套注释测试 /* 看起来像嵌套 */
     */
    int d = 4;
}

// 13. 边界情况和错误恢复测试
void edge_cases() {
    int;;;;;;;;;;;;;; // 多个分号
    int;;;;;;;;;;a;;;; = 5;

    // 连续的运算符
    int x = 5;
    int y = 10;
    bool b = x==y;
    bool c = x!=y;
    bool d = x<=y;
    bool e = x>=y;
    bool f = x&&y;
    bool g = x||y;

    // 特殊字符测试
    int@invalid;  // @ 不是合法字符
    int#invalid;  // # 不是合法字符
    int$invalid;  // $ 不是合法字符

    // 非法运算符组合
    int z = x =+ y;
    int w = x =- y;
    int v = x =* y;
    int u = x =/ y;
}

// 测试各种边界情况和特殊输入，专门针对词法分析器的潜在缺陷

// 1. 多行注释的正确处理
/* 这是一个简单的多行注释 */
int test_multiline_comment() {
    /*
     * 多行注释测试
     * 包含多个星号和斜杠
     * // 嵌套的单行注释符号
     * /* 嵌套的多行注释符号 * /
     */
    return 0;
}

// 2. 单行注释的边界情况
int test_single_line_comment() {
    // 注释后直接换行
    int a = 1;
    // 注释后没有换行}
    int b = 2;
    ///////////////////////// 很长的注释行
    int c = 3;
    //// 带空格的注释
    int d = 4;
}

// 3. 字符串字面量的复杂情况
void string_edge_cases() {
    string empty = "";
    string with_quote = "He said \"Hello\"";
    string multiline = "第一行
第二行";
    string special_chars = "!@#$%^&*()_+-=[]{}|;':\",./<>?";
    string unicode_chars = "你好世界";  // UTF-8字符
    string escape_sequences = "\t\n\r";  // 转义字符
}

// 4. 标识符和关键字的区分
void identifier_keyword_distinction() {
    // 正常的关键字使用
    int integer = 42;
    bool boolean = true;
    void void_var;

    // 类似关键字的标识符（应该被识别为标识符）
    int int_variable = 1;      // int后跟其他字符
    bool bool_test = false;    // bool后跟其他字符
    void void_function();      // void后跟其他字符

    // 下划线开头的标识符
    int _underscore = 0;
    int _int = 1;              // 下划线+关键字
}

// 5. 数字字面量的各种形式
void number_literal_tests() {
    // 十进制数
    int zero = 0;
    int small = 7;
    int large = 123456789;

    // 前导零的数字
    int leading_zero = 0123;   // 可能被解释为八进制?

    // 负数（需要通过减法表达式产生）
    int negative = -42;

    // 连续数字字面量
    int a = 123 456;  // 应该被识别为两个独立的token
}

// 6. 运算符的边界情况和组合
void operator_edge_cases() {
    int a = 5;
    int b = 10;

    // 相等和赋值运算符
    bool eq1 = a==b;    // 没有空格
    bool eq2 = a == b;  // 有空格
    int assign1 = a=b;  // 赋值
    int assign2 = a= b; // 赋值带空格

    // 比较运算符
    bool lt1 = a<b;
    bool gt1 = a>b;
    bool le1 = a<=b;
    bool ge1 = a>=b;
    bool ne1 = a!=b;

    // 逻辑运算符
    bool and1 = true&&false;
    bool or1 = true||false;
    bool not1 = !false;

    // 复合赋值运算符
    a+=1;
    b-=2;
    a*=3;
    b/=4;
    a%=5;

    // 无效但相似的运算符组合
    a=+1;  // 应该是赋值和正号，不是复合赋值
    b=-2;  // 应该是赋值和负号
    a=*3;  // 应该是赋值和解引用（如果支持）
    b=/4;  // 应该是赋值和除号
}

// 7. 分隔符的密集使用
void delimiter_density_test() {
    int func(((((((int a))))))) {{{{{{ // 极端嵌套的参数和语句块
        return (((((a+1)))));
    }}}}}


    int arr[5] = {1,2,3,4,5};  // 当前词法分析器可能不支持数组
    int x = (1+(2*(3/(4%5)))); // 深度嵌套的表达式
}

// 8. 空白字符处理
void whitespace_test() {
	int tab_indent = 1;         // 使用tab缩进
	 int extra_spaces = 2;      // 行首多余空格
int trailing_spaces = 3;       // 尾随空格
int form_feed = 4;             // 换页符测试
int vertical_tab = 5;          // 垂直制表符测试
}

// 9. 错误恢复测试 - 检查词法分析器如何处理非法字符
void error_recovery_test() {
    int valid = 1;
    @invalid_identifier = 2;   // 非法字符
    int another_valid = 3;
    #preprocessor_directive;   // 预处理指令字符
    int final_valid = 4;
    €currency_symbol;          // Unicode货币符号
}

// 10. 文件结尾测试
void eof_test() {
    int normal_end = 1;
}  // 文件在此处结束，无换行符

// 11. 特殊输入序列
void special_sequences() {
    // 空程序
}

// 只包含空白字符的文件

// 只包含注释的文件
/* 整个文件都是注释 */
// 另一个注释行

// 12. 最大长度测试
void max_length_test() {
    // 非常长的标识符名称
    int this_is_a_very_long_identifier_name_that_exceeds_normal_length_expectations_but_should_still_be_recognized_as_a_single_identifier_token = 0;

    // 非常大的数字
    int big_number = 2147483647;  // 32位最大整数

    // 非常长的字符串
    string long_string = "这是一个非常长的字符串字面量，用来测试词法分析器对长字符串的处理能力，包括缓冲区管理和内存使用等方面";
}