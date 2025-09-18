#include <iostream>
#include <vector>
#include <string>
#include <cctype>
using namespace std;

int main() {
    const float q[26] = {7.88,1.56,2.68,3.89,12.68,2.56,1.87,5.73,7.07,0.10,0.60,3.94,2.44,7.06,7.76,1.86,0.09,5.94,6.34,9.78,2.80,1.02,2.14,0.16,2.02,0.06};
    string t;
    int s;
    int ss;

    cout << "密文是大写字母请输入1，是小写请输入0：";
    cin >> ss;
    cin.ignore(); // 忽略掉上一行输入的回车
    s = (ss == 1) ? 'A' : 'a';

    cout << "请输入密文：";
    getline(cin, t); // 读取整行密文，包括空格

    // 只保留字母，去掉空格和标点
    string text;
    for (char c : t) {
        if (isalpha(c)) {
            text += c;
        }
    }

    int long_of_text = text.size();
    int m_min, m_max, m;
    cout << "请输入密钥长度范围（最小值）：";
    cin >> m_min;
    cout << "请输入密钥长度范围（最大值）：";
    cin >> m_max;

    // 分组和重合指数分析
    for (int fenzu = m_min; fenzu <= m_max; fenzu++) {
        int number_of_zu = (long_of_text + fenzu - 1) / fenzu; // 向上取整
        int shen_yu = long_of_text % fenzu;
        if (shen_yu == 0) shen_yu = fenzu;

        vector<vector<int>> mi_wen(number_of_zu, vector<int>(fenzu, 0));
        int fenchai = 0;
        for (int row = 0; row < number_of_zu; row++) {
            for (int col = 0; col < fenzu; col++) {
                if (fenchai < long_of_text) {
                    mi_wen[row][col] = text[fenchai] - s;
                }
                fenchai++;
            }
        }

        float CI_guji = 0.0;
        for (int col = 0; col < fenzu; col++) {
            int chang_du_L = number_of_zu;
            if (col >= shen_yu) chang_du_L--;
            vector<int> f(26, 0);
            for (int row = 0; row < chang_du_L; row++) {
                f[mi_wen[row][col]]++;
            }
            for (int i = 0; i < 26; i++) {
                CI_guji += 1.0 * f[i] * (f[i] - 1) / chang_du_L / (chang_du_L - 1);
            }
        }
        CI_guji /= fenzu;
        cout << "m=" << fenzu << "：" << CI_guji << endl;
    }

    cout << "请输入最接近0.065的m：";
    cin >> m;

    // 密钥和明文恢复
    int number_of_zu = (long_of_text + m - 1) / m;
    int shen_yu = long_of_text % m;
    if (shen_yu == 0) shen_yu = m;

    vector<vector<int>> mi_wen(number_of_zu, vector<int>(m, 0));
    vector<vector<int>> ming_wen(number_of_zu, vector<int>(m, 0));

    int fenchai = 0;
    for (int row = 0; row < number_of_zu; row++) {
        for (int col = 0; col < m; col++) {
            if (fenchai < long_of_text) {
                mi_wen[row][col] = text[fenchai] - s;
            }
            fenchai++;
        }
    }

    cout << "密钥为：";
    for (int col = 0; col < m; col++) {
        int chang_du_L = number_of_zu;
        if (col >= shen_yu) chang_du_L--;
        vector<int> f(26, 0);
        vector<float> p(26, 0.0);

        for (int row = 0; row < chang_du_L; row++) f[mi_wen[row][col]]++;
        for (int i = 0; i < 26; i++) p[i] = 1.0 * f[i] / chang_du_L;

        double corr_max = 0.0;
        int max_number = 0;
        for (int k = 0; k < 26; k++) {
            double corr = 0.0;
            for (int i = 0; i < 26; i++) corr += p[(k + i) % 26] * (q[i] / 100);
            if (corr > corr_max) {
                corr_max = corr;
                max_number = k;
            }
        }

        for (int i = 0; i < chang_du_L; i++)
            ming_wen[i][col] = (mi_wen[i][col] + 26 - max_number) % 26;

        cout << char(max_number + s);
    }

    string plaintext = "";
    for (int row = 0; row < number_of_zu; row++) {
        for (int col = 0; col < m; col++) {
            if (row * m + col < long_of_text)
                plaintext += char(ming_wen[row][col] + s);
        }
    }

    cout << endl << "明文为：" << plaintext << endl;
    return 0;
}
