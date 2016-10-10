#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <math.h>
#include<sqlite3.h>
#include <stack>
using namespace std;

const int ArrayMax = 100;
/**
 结果
 */
static float originArray[ArrayMax];
static int ArrayCapacity = 0;

struct ArrangeArray{
    float *array;
    int number;
};

int main()
{
    cout.setf(ios::fixed);
    
    cout << "请输入数量：";
    cin >> ArrayCapacity;
    cout << "请输入结果：" << endl;
    int originI = 0;
    float aver = 0.0;
    while (1) {
        float temp;
        cin >> temp;
        if (temp < 0) {
            break;
        }
        originArray[originI++] = temp;
        aver += temp;
    }
    
    cout << "平均值：" << aver / ArrayCapacity << endl;
    return 0;
    vector<ArrangeArray> arrangeArray;
    
    void AllArray(vector<ArrangeArray> &arrayVector, int capacity);
    AllArray(arrangeArray, ArrayCapacity);
    
//    float sum = 0.0;
//    float number = 0.0;
    float array[ArrayCapacity];
    float arrayAve[ArrayCapacity];
    for(int i = 0.0; i < ArrayCapacity; ++i) {
        array[i] = 0.0;
        arrayAve[i] = 0.0;
    }
    for (int i = 0; i < arrangeArray.size(); ++i) {
        array[arrangeArray[i].number -1] += arrangeArray[i].number;
        for (int j = 0; j < arrangeArray[i].number; ++j) {
            arrayAve[arrangeArray[i].number - 1] += arrangeArray[i].array[j];
        }
    }
    
//    for (auto ite = arrangeArray.begin(); ite != arrangeArray.end(); ++ite) {
//        array[ite->number -1] += ite->number;
//        for (int i = 0; i < ite->number; ++i) {
//            arrayAve[ite->number - 1] += ite->array[i];
//        }
//    }
    
    for (int i = 0; i < ArrayCapacity; ++i) {
        cout << i + 1 << ": " << arrayAve[i] / array[i] << endl;
    }
    
    for (auto ite = arrangeArray.begin(); ite != arrangeArray.end(); ++ite) {
        delete []ite->array;
    }
    
    return 0;
}

/**
 *  全排列
 */
void AllArray(vector<ArrangeArray> &arrayVector, int capacity)
{
    ArrangeArray *firstArrange = new ArrangeArray();
    firstArrange->array = new float[2];
    firstArrange->array[0] = originArray[0];
    firstArrange->array[1] = originArray[1];
    firstArrange->number = 2;
    arrayVector.push_back(*firstArrange);
    
    auto ite = arrayVector.begin();
    long originSize = arrayVector.size();
    for (int i = 2; i < capacity; i++) {
        originSize = arrayVector.size();
        for (int j = 0; j < i; ++j) {
            ArrangeArray *tempArrange = new ArrangeArray();
            tempArrange->array = new float[2];
            tempArrange->array[0] = originArray[j];
            tempArrange->array[1] = originArray[i];
            tempArrange->number = 2;
            arrayVector.push_back(*tempArrange);
        }
        
        ite = arrayVector.begin();
        for (int k = 0; k < originSize; ++k) {
            ArrangeArray *tempArrange = new ArrangeArray();
            tempArrange->array = new float[arrayVector[k].number + 1];
            for (int z = 0; z < arrayVector[k].number; ++z) {
                tempArrange->array[z] = arrayVector[k].array[z];
            }
            tempArrange->array[arrayVector[k].number] = originArray[i];
            tempArrange->number = arrayVector[k].number + 1;
            arrayVector.push_back(*tempArrange);
        }
    }
}

/**
56
1.93 1.95 1.90 1.61 3.65 2.58 3.85 3.45 3.05 2.83
1.91 3.40 3.25 2.24 3.35 2.20 1.32 1.40 1.50 1.56 2.68
3.35 1.85 2.01 2.45 3.20 2.03 3.20 2.90 1.46 2.18
3.25 2.45 5.30 1.32 5.20 2.45 1.77 3.28 1.21 1.37 3.00 3.50 2.05
3.60 2.22 1.97 3.45 1.51 3.25 1.93 1.47 4.50 4.00 1.37 1.68 10.00 4.05 -1
 
 1.46 1.62 2.75 1.64 1.71 2.18 1.65 2.03 2.12 2.27 1.74 1.38 1.58 -1
 1.93 1.64 1.90 1.61 2.08 2.58 1.83 1.42 1.61 1.40 -1 
 1.67 1.63 1.71 2.24 1.78 2.20 1.32 1.40 1.50 1.56 1.39 -1
 1.67 1.91 1.97 1.63 1.71 1.86 2.24 1.48 1.79 1.78 2.20 1.50
 1.32 2.75 1.40 2.46 1.50 2.20 1.56 2.08 1.39 2.48 -1
 1.89 1.69 1.85 1.72 1.60 2.01 1.40 2.45 2.18 1.51 1.59 2.03
 1.84 1.73 1.53 2.14 1.46 2.29 2.18 1.51
  1.69 1.85  1.60  1.40   1.51  2.03
  1.73 1.53  1.46  2.18 -1
 */















