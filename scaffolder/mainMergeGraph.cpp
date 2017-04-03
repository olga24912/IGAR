#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

const int MAX_STR_LEN = 100;

int main(int argc, char ** argv) {
    std::vector<std::string> lib;
    std::vector<std::string> vec;
    std::vector<std::string> edge;

    std::string fileName = std::string(argv[1]);

    FILE* file = fopen((fileName.c_str()), "r");
    char str[MAX_STR_LEN];
    fgets(str, MAX_STR_LEN, file);
    int x;
    sscanf(str, "%d", &x);
    for (int i = 0; i < x; ++i) {
        fgets(str, MAX_STR_LEN, file);
        lib.push_back(std::string(str));
    }
    fgets(str, MAX_STR_LEN, file);
    sscanf(str, "%d", &x);
    for (int i = 0; i < x; ++i) {
        fgets(str, MAX_STR_LEN, file);
        vec.push_back(std::string(str));
    }
    fgets(str, MAX_STR_LEN, file);
    sscanf(str, "%d", &x);
    for (int i = 0; i < x; ++i) {
        fgets(str, MAX_STR_LEN, file);
        edge.push_back(std::string(str));
    }
    fclose(file);
    for (int j = 2; j < argc - 1; ++j) {

        int oldLibCnt = (int)lib.size();
        fileName = std::string(argv[j]);
        file = fopen(fileName.c_str(), "r");
        fgets(str, MAX_STR_LEN, file);
        sscanf(str, "%d", &x);
        for (int i = 0; i < x; ++i) {
            fgets(str, MAX_STR_LEN, file);
            char c;
            int id;
            char lst[MAX_STR_LEN], rst[MAX_STR_LEN];
            sscanf(str, "%c %d %s %s", &c, &id, lst, rst);
            sprintf(str, "%c %d %s %s\n", c, oldLibCnt + id, lst, rst);
            lib.push_back(std::string(str));
        }
        fgets(str, MAX_STR_LEN, file);
        sscanf(str, "%d", &x);
        for (int i = 0; i < x; ++i) {
            fgets(str, MAX_STR_LEN, file);
        }
        fgets(str, MAX_STR_LEN, file);
        sscanf(str, "%d", &x);
        for (int i = 0; i < x; ++i) {
            fgets(str, MAX_STR_LEN, file);
            char c;
            int id, gf, gt, gel, gw;
            sscanf(str, "%c %d %d %d %d %d", &c, &id, &gf, &gt, &gel, &gw);
            sprintf(str, "%c %d %d %d %d %d\n", c, id, gf, gt, oldLibCnt + gel, gw);
            edge.push_back(std::string(str));
        }
        fclose(file);
    }

    file = fopen(argv[argc - 1], "w");
    fprintf(file, "%d\n", (int)lib.size());
    for (int i = 0; i < (int)lib.size(); ++i) {
        fputs(lib[i].c_str(), file);
    }
    fprintf(file, "%d\n", (int)vec.size());
    for (int i = 0; i < (int)vec.size(); ++i) {
        std::cerr << i << std::endl;
        fputs(vec[i].c_str(), file);
    }
    fprintf(file, "%d\n", (int)edge.size());
    for (int i = 0; i < (int)edge.size(); ++i) {
        fputs(edge[i].c_str(), file);
    }

    fclose(file);
    return 0;
}