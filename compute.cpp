// 逻辑完备集的计算
// 电路中的对称门与非对称门相关的内容会不会有什么启发？
// https://en.wikipedia.org/wiki/Functional_completeness
// https://blog.csdn.net/u013795675/article/details/44480789

#include <bits/stdc++.h>

std::vector<std::string> basic_input2{
    "0101",
    "0011",
    "0000",
    "1111"};

std::vector<std::string> basic_input3{
    "01010101",
    "00110011",
    "00001111"
    "00000000",
    "11111111"};

std::unordered_map<std::string, std::string> gate_map2{
    {"0000", "const0"},
    {"0001", "and2"},
    {"0010", "us2_1"},
    {"0011", "us2_2"},
    {"0100", "us2_3"},
    {"0101", "us2_4"},
    {"0110", "xor2"},
    {"0111", "or2"},
    {"1000", "nor2"},
    {"1001", "xnor2"},
    {"1010", "us2_5"},
    {"1011", "us2_6"},
    {"1100", "us2_7"},
    {"1101", "us2_8"},
    {"1110", "nand2"},
    {"1111", "const1"},
};

std::unordered_set<std::string> basic_symmetry_gates{
    {"0001"},
    {"0110"},
    {"0111"},
    {"1000"},
    {"1001"},
    {"1110"}};

std::string uint_2_string(uint32_t u, uint8_t length)
{
    std::string res;
    for (int i = length - 1; i >= 0; --i)
    {
        res += u & (1 << i) ? '1' : '0';
    }
    return res;
}

/**
 * @brief apply the inverter for the given gate
 */
std::string apply_inverter(std::string entry)
{
    std::string res;
    uint i = 0;

    for (i = 0u; i < entry.size(); ++i)
    {
        if (entry[i] == '0')
            res += '1';
        else
            res += '0';
    }
    return res;
}

/**
 * @brief compute the truth table for the given gate
 * @param entry the Boolean function (truth table) of the given gate
 * @param a the input truth table
 * @param b the input truth table
 * @return the truth table of the given gate
 */
std::string compute_new_tt_2(std::string entry, const std::string &a, const std::string &b)
{
    std::string res;
    uint i = 0;

    for (i = 0u; i < 4; ++i)
    {
        if (a[i] == '0' && b[i] == '0')
            res += entry[0];
        if (a[i] == '0' && b[i] == '1')
            res += entry[1];
        if (a[i] == '1' && b[i] == '0')
            res += entry[2];
        if (a[i] == '1' && b[i] == '1')
            res += entry[3];
    }
    return res;
}

std::string compute_new_tt_3(std::string entry, const std::string &a, const std::string &b, const std::string &c)
{
    std::string res;
    uint i = 0;

    for (i = 0u; i < 8; ++i)
    {
        if (a[i] == '0' && b[i] == '0' && c[i] == '0')
            res += entry[0];
        if (a[i] == '0' && b[i] == '0' && c[i] == '1')
            res += entry[1];
        if (a[i] == '0' && b[i] == '1' && c[i] == '0')
            res += entry[2];
        if (a[i] == '0' && b[i] == '1' && c[i] == '1')
            res += entry[3];
        if (a[i] == '1' && b[i] == '0' && c[i] == '0')
            res += entry[4];
        if (a[i] == '1' && b[i] == '0' && c[i] == '1')
            res += entry[5];
        if (a[i] == '1' && b[i] == '1' && c[i] == '0')
            res += entry[6];
        if (a[i] == '1' && b[i] == '1' && c[i] == '1')
            res += entry[7];
    }
    return res;
}

void explore_2_1()
{
    uint i = 0u, j = 0u, k = 0u;
    bool added = true;
    std::vector<std::string> fcs_s;
    std::vector<std::string> fcs_us;
    for (i = 0u; i < 16u; ++i)
    {
        std::string entry = uint_2_string(i, 4);
        std::vector<std::string> tmp_basic_input2 = basic_input2;
        std::set<std::string> tmp_set(tmp_basic_input2.begin(), tmp_basic_input2.end());

        added = true;
        while (added)
        {
            added = false;
            for (j = 0; j < tmp_basic_input2.size(); ++j)
            {
                for (k = 0; k < tmp_basic_input2.size(); ++k)
                {
                    std::string newTT = compute_new_tt_2(entry, tmp_basic_input2[j], tmp_basic_input2[k]);
                    if (tmp_set.find(newTT) == tmp_set.end())
                    {
                        tmp_basic_input2.emplace_back(newTT);
                        tmp_set.insert(newTT);
                        added = true;
                    }
                }
            }
        }
        if (tmp_basic_input2.size() == 16u)
        {
            if (basic_symmetry_gates.find(entry) != basic_symmetry_gates.end())
                fcs_s.emplace_back(entry);
            else
                fcs_us.emplace_back(entry);
        }
    }
    std::cout << "symmetry gates:" << std::endl;
    for (auto fc : fcs_s)
    {
        std::cout << "\t" << gate_map2[fc] << std::endl;
    }
    std::cout << "unsymmetry gates:" << std::endl;
    for (auto fc : fcs_us)
    {
        std::cout << "\t" << gate_map2[fc] << std::endl;
    }
}

void explore_2_2()
{
    uint i = 0u, j = 0u, k = 0u, l = 0u;
    bool added = true;
    std::set<std::tuple<std::string, std::string>> fcs;

    for (i = 0u; i < 16u; ++i)
    {
        for (l = i + 1; l < 16u; ++l)
        {
            std::string entry1 = uint_2_string(i, 4);
            std::string entry2 = uint_2_string(l, 4);

            std::vector<std::string> tmp_basic_input2 = basic_input2;
            std::set<std::string> tmp_set(tmp_basic_input2.begin(), tmp_basic_input2.end());

            added = true;
            while (added)
            {
                added = false;
                for (j = 0; j < tmp_basic_input2.size(); ++j)
                {
                    for (k = 0; k < tmp_basic_input2.size(); ++k)
                    {
                        std::string newTT1 = compute_new_tt_2(entry1, tmp_basic_input2[j], tmp_basic_input2[k]);
                        std::string newTT2 = compute_new_tt_2(entry2, tmp_basic_input2[j], tmp_basic_input2[k]);
                        if (tmp_set.find(newTT1) == tmp_set.end())
                        {
                            tmp_basic_input2.emplace_back(newTT1);
                            tmp_set.insert(newTT1);
                            added = true;
                        }
                        if (tmp_set.find(newTT2) == tmp_set.end())
                        {
                            tmp_basic_input2.emplace_back(newTT2);
                            tmp_set.insert(newTT2);
                            added = true;
                        }
                    }
                }
            }
            if (tmp_basic_input2.size() == 16u)
            {
                std::vector<std::string> entries({entry1, entry2});
                std::sort(entries.begin(), entries.end());
                fcs.insert(std::make_tuple(entries[0], entries[1]));
            }
        }
    }

    for (auto fc : fcs)
    {
        std::cout << "\t" << gate_map2[std::get<0>(fc)] << ", " << gate_map2[std::get<1>(fc)] << std::endl;
    }
}

void explore_2_3()
{
    uint i = 0u, l = 0u, m = 0u, j = 0u, k = 0u, u = 0u;
    bool added = true;

    std::set<std::tuple<std::string, std::string, std::string>> fcs;

    for (i = 0u; i < 16u; ++i)
    {
        for (l = i + 1; l < 16u; ++l)
        {
            for (m = l + 1; m < 16u; ++m)
            {
                std::string entry1 = uint_2_string(i, 4);
                std::string entry2 = uint_2_string(l, 4);
                std::string entry3 = uint_2_string(m, 4);

                std::vector<std::string> tmp_basic_input2 = basic_input2;
                std::set<std::string> tmp_set(tmp_basic_input2.begin(), tmp_basic_input2.end());

                added = true;
                while (added)
                {
                    added = false;
                    for (j = 0; j < tmp_basic_input2.size(); ++j)
                    {
                        for (k = 0; k < tmp_basic_input2.size(); ++k)
                        {

                            std::string newTT1 = compute_new_tt_2(entry1, tmp_basic_input2[j], tmp_basic_input2[k]);
                            std::string newTT2 = compute_new_tt_2(entry2, tmp_basic_input2[j], tmp_basic_input2[k]);
                            std::string newTT3 = compute_new_tt_2(entry3, tmp_basic_input2[j], tmp_basic_input2[k]);

                            if (tmp_set.find(newTT1) == tmp_set.end())
                            {
                                tmp_basic_input2.emplace_back(newTT1);
                                tmp_set.insert(newTT1);
                                added = true;
                            }
                            if (tmp_set.find(newTT2) == tmp_set.end())
                            {
                                tmp_basic_input2.emplace_back(newTT2);
                                tmp_set.insert(newTT2);
                                added = true;
                            }
                            if (tmp_set.find(newTT3) == tmp_set.end())
                            {
                                tmp_basic_input2.emplace_back(newTT3);
                                tmp_set.insert(newTT3);
                                added = true;
                            }
                        }
                    }
                }
                if (tmp_basic_input2.size() == 16u)
                {
                    std::vector<std::string> entries({entry1, entry2, entry3});
                    std::sort(entries.begin(), entries.end());
                    fcs.insert(std::make_tuple(entries[0], entries[1], entries[2]));
                }
            }
        }
    }

    for (auto fc : fcs)
    {
        std::cout << "\t" << gate_map2[std::get<0>(fc)] << ", " << gate_map2[std::get<1>(fc)] << ", " << gate_map2[std::get<2>(fc)] << std::endl;
    }
}

void explore_3_1()
{
    uint i = 0u, j = 0u, k = 0u, l = 0u;
    bool added = true;
    std::vector<std::string> fcs;
    for (i = 0u; i < 256u; ++i)
    {
        std::string entry = uint_2_string(i, 8);
        std::vector<std::string> tmp_basic_input3 = basic_input3;
        std::set<std::string> tmp_set(tmp_basic_input3.begin(), tmp_basic_input3.end());
        std::cout << "try entry: " << entry << std::endl;
        added = true;
        while (added)
        {
            added = false;
            for (j = 0; j < tmp_basic_input3.size(); ++j)
            {
                for (k = 0; k < tmp_basic_input3.size(); ++k)
                {
                    for (l = 0; l < tmp_basic_input3.size(); ++l)
                    {
                        std::string newTT = compute_new_tt_3(entry, tmp_basic_input3[j], tmp_basic_input3[k], tmp_basic_input3[l]);
                        if (tmp_set.find(newTT) == tmp_set.end())
                        {
                            tmp_basic_input3.emplace_back(newTT);
                            tmp_set.insert(newTT);
                            added = true;
                        }
                    }
                }
            }
        }
        if (tmp_basic_input3.size() == 16u)
        {
            fcs.emplace_back(entry);
        }
    }
    for (auto fc : fcs)
    {
        std::cout << fc << std::endl;
    }
}

int main(int argc, char **argv)
{
    std::cout << "Funational Completeness exploration for 2-inputs gate:" << std::endl;

    std::cout << "One gate:" << std::endl;
    explore_2_1();
    std::cout << "Two gates:" << std::endl;
    explore_2_2();
    std::cout << "Three gates:" << std::endl;
    explore_2_3();

    // std::cout << "Funational Completeness exploration for 3-inputs gate:" << std::endl;
    // explore_3_1();
    return 1;
}