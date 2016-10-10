//
//  main.cpp
//  概率论_初论
//
//  Created by 侯庆峰 on 16/5/8.
//  Copyright © 2016年 letv. All rights reserved.
//

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <math.h>
#include<sqlite3.h>
using namespace std;

struct ProbabilityArray{
    float victory;
    int victoryNum;
    float failure;
    int failureNum;
    int operation;
    float draw;
    int drawNum;
    float maxOdds;
    float minOdds;
    float willianVictoryProbality;//威廉概率
    float willianFailureProbality;//威廉概率
    float houVictoryProbality;//hou概率
    float houFailureProbality;//hou概率
    static float averange;
    int location;
};

float ProbabilityArray::averange = 0.0;

struct Arrange{
    int *array;
    int number;
};

struct AllArrangeArray{
    Arrange *arrange;
};

struct test{
    int t;
    char *c;
};

struct floatArray{
    float *fArray;
    int number;
    bool print;
    int location[15];
};

/**
 *  全排列
 */
void AllArray(vector<AllArrangeArray> &arrangeArray,
              vector<ProbabilityArray> &arrayVector,
              vector<floatArray> &outcomeArray, int max);

void adjustArrange(vector<ProbabilityArray> &arrayVector,
                   vector<floatArray> &outcomeArray, int max);

int main()
{
    vector<ProbabilityArray> arrayVector;
    vector<AllArrangeArray>arrangeArray;
    vector<floatArray> outcomeArray;
    int arrayMAX = 0;
    
    cout << "please input data number: " << endl;
    cin >> arrayMAX;
    
    map<float, float>chanceMap;
    
    cout << "please input data(end with '-1'): " << endl;
    int location = 0;
    while (1) {
        float temp1, temp2;
        int temp3; //方向1 为- ，0为+
        float temp4, temp5, temp6;
        cin >> temp1;
        if (temp1 < 0)
            break;
        cin  >> temp2;
        if(temp2 < 0)
            break;
        cin  >> temp3;
        if(temp3 < 0)
            break;
        cin  >> temp4;
        if(temp4 < 0)
            break;
        cin  >> temp5;
        if(temp5 < 0)
            break;
        cin  >> temp6;
        if(temp6 < 0)
            break;
        
        ProbabilityArray *arr = new ProbabilityArray();
        arr->location = ++location;
        ProbabilityArray::averange += (temp1 + temp2);
        arr->operation = temp3;
        if (temp3) {
            float sum = temp4 + temp5 + temp6;
            arr->willianVictoryProbality = (1 - temp4/sum) * 100.0;
            arr->willianFailureProbality = 100.0 - arr->willianVictoryProbality;
        } else {
            float sum = temp4 + temp5 + temp6;
            arr->willianVictoryProbality = (1 - (temp4 + temp5)/sum) * 100.0;
            arr->willianFailureProbality = 100.0 - arr->willianVictoryProbality;
        }
        
        
        cout << temp1 << "  " << temp2 << endl;
        
        if (arrayVector.empty()) {
            arr->victoryNum = 1;
            arr->failureNum = 1;
            arr->failure = temp2;
            arr->victory = temp1;
        } else
            for (vector<ProbabilityArray>::iterator ite = arrayVector.begin(); ite != arrayVector.end(); ++ite) {
                bool isV = false, isF = false;
                if (ite->failure == temp1) {
                    ++ite->failureNum;
                    arr->victory = temp1 + 0.1 * pow(0.1, (ite->failureNum));
                    isV = true;
                }
                
                if (ite->victory == temp1) {
                    ++ite->victoryNum;
                    arr->victory = temp1 + 0.1 * pow(0.1, (ite->victoryNum));
                    isV = true;
                }
                
                if (ite->failure == temp2) {
                    ++ite->failureNum;
                    arr->failure = temp2 + 0.1 * pow(0.1, (ite->failureNum));
                    isF = true;
                }
                
                if (ite->victory == temp2) {
                    ++ite->victoryNum;
                    arr->failure = temp2 + 0.1 * pow(0.1, (ite->victoryNum));
                    isF = true;
                }
                
                {
                    if (!isV) {
                        arr->victory = temp1;
                    }
                    if (!isF) {
                        arr->failure = temp2;
                    }
                    
                    arr->victoryNum = 1;
                    arr->failureNum = 1;
                    
                    if (isV || isF) {
                        break;
                    }
                }
            }
        arrayVector.push_back(*arr);
    }
    cout.setf(ios::fixed);
    
    for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
         ite != arrayVector.end(); ++ite) {
        cout << ite->victory << ",  " << ite->failure << endl;
    }
    
    // AllArray(arrangeArray, arrayVector, outcomeArray, arrayMAX);
    adjustArrange(arrayVector, outcomeArray, arrayMAX);
    // cout << "概率组合：" << arrangeArray.size() << endl;
    
    /**
     *  大概率 ==============>>开始
     */
    float profit = 0.0;
    float zonggong = 0.0;
    float sum = 2.0;
    float money = 2.0;
    
    map<float, float>chanceMapTemp;
    for (vector<ProbabilityArray>::iterator mapite = arrayVector.begin();
         mapite != arrayVector.end(); ++mapite){
            chanceMap.insert(make_pair(mapite->victory, 0.0));
            chanceMap.insert(make_pair(mapite->failure, 0.0));
            chanceMapTemp.insert(make_pair(mapite->victory, 0.0));
            chanceMapTemp.insert(make_pair(mapite->failure, 0.0));
    }
    
    for (int daI = 2; daI != arrayMAX; ++daI) {
        sum = 1.0;
        money = 1.0;
        zonggong = 0.0;
        
        for (vector<ProbabilityArray>::iterator mapite = arrayVector.begin();
             mapite != arrayVector.end(); ++mapite){
            chanceMapTemp[mapite->victory] = 0.0;
            chanceMapTemp[mapite->failure] = 0.0;
        }
        
        for (vector<floatArray>::iterator ite = outcomeArray.begin() + 2;
             ite < outcomeArray.end(); ++ite) {
                sum = 1.0;
                money = 1.0;
                if (ite->number == daI) {
                    for (int i = 0; i < ite->number; ++i) {
                        sum *= ite->fArray[i];
                        money *= 2;
                    }
                    if (sum - money > 0) {
                        zonggong++;
                        for (int i = 0; i < ite->number; ++i) {
                            ++chanceMapTemp[ite->fArray[i]];
                        }
                    }
                }
            }
        
        for (map<float, float>::iterator mapite = chanceMap.begin(); mapite != chanceMap.end(); ++mapite) {
            mapite->second += chanceMapTemp[mapite->first]/zonggong * 100;
        }
    }
    
    cout << "-------------大概率origin odds ------------- " << endl;// << profit << endl;
    for (map<float, float>::iterator mapite = chanceMap.begin(); mapite != chanceMap.end(); ++mapite) {
        cout << "odds: "<< mapite->first << ", probablity: "
        << "数量：" << mapite->second << ", "<< (mapite->second/arrayMAX) << "%" << endl;
        mapite->second = 100.0 - (mapite->second/arrayMAX);
    }
    /**
     *  大概率 ==============>>结束
     */
    
    /**
     *  小概率 ==============>>开始
     */
    ProbabilityArray::averange = ProbabilityArray::averange / (arrayMAX * 2);
    float averangeTemp = powf(ProbabilityArray::averange, arrayMAX) - pow(2, arrayMAX);
    
    profit = 0.0;
    zonggong = 0.0;
    map<float, float>chanceMap1;
    
    for (vector<ProbabilityArray>::iterator mapite = arrayVector.begin();
         mapite != arrayVector.end(); ++mapite){
        map<float, float>::iterator ite = chanceMap1.find(mapite->victory);
        if (ite == chanceMap1.end()) {
            chanceMap1.insert(make_pair(mapite->victory, 0.0));
        }
        ite = chanceMap1.find(mapite->failure);
        if (ite == chanceMap1.end()) {
            chanceMap1.insert(make_pair(mapite->failure, 0.0));
        }
    }
    
    for (vector<floatArray>::iterator ite = outcomeArray.begin() + 2;
         ite < outcomeArray.end(); ++ite) {
        float sum = 2.0;
        float money = 2.0;
        float averanger = 0.0;
        sum = 0.0;
        
        for (int i = 0; i < ite->number; ++i) {
            for (auto ite1 = arrayVector.begin(); ite1 != arrayVector.end(); ++ite1) {
                if (ite1->location == ite->location[i]) {
                    sum  += (ite1->victory + ite1->failure);
                    break;
                }
            }
        }
        
        averanger = sum / (ite->number * 2);
        averangeTemp = powf(averanger, ite->number) - powf(2.0, ite->number);
        
        sum = 1.0;
        money = 1.0;
        for (int i = 0; i < ite->number; ++i) {
            sum *= ite->fArray[i];
            money *= 2;
        }
        if (sum - money < averangeTemp) {
            zonggong++;
            for (int i = 0; i < ite->number; ++i) {
                map<float, float>::iterator mapite = chanceMap1.find(ite->fArray[i]);
                if (mapite == chanceMap1.end()) {
                    chanceMap1.insert(make_pair(ite->fArray[i], 1.0));
                } else {
                    ++mapite->second;
                }
            }
        }
        profit += sum - money;
    }
    
    cout << "-------------小概率origin odds ------------- " << endl;// << profit << endl;
    auto chanceIteT = chanceMap.begin();
    map<float, float>queMap;
    for (map<float, float>::iterator mapite = chanceMap1.begin(); mapite != chanceMap1.end(); ++mapite, ++chanceIteT) {
        
        cout << "odds: "<< mapite->first << "上炔值："<< chanceIteT->second << "%, "<< "下炔值: "
        << ((float)mapite->second/zonggong) * 100 << "%" << "，波动范围：" << fabs(chanceIteT->second - ((float)mapite->second/zonggong) * 100) << endl;
        queMap.insert(make_pair(mapite->first, fabs(chanceIteT->second - ((float)mapite->second/zonggong) * 100)));
        mapite->second = ((float)mapite->second/zonggong) * 100 ;
        chanceIteT->second = (mapite->second + chanceIteT->second) / 2;
    }
    /**
     *  小概率 ==============>>结束
     */
    
    /**
     *  威廉概率 ==============>>开始
     */
    void WilliamOddsRevise(vector<ProbabilityArray> &arrayVector, map<float, float> &chanceMap);
    WilliamOddsRevise(arrayVector, chanceMap);
    
    float compensateModel = 0.0;
    float chance[arrayMAX];
    int chanceI = 0;
    cout << endl << "-------------profit------------- " << endl;// << profit << endl;
    for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
         ite != arrayVector.end(); ++ite){
        //cout << "odds: "<< mapite->first << ", probablity: "
        //     << (float)mapite->second/zonggong * 100 << "%" << endl;
        //  mapite->second = 100.0 - (float)mapite->second/zonggong * 100.0;
        cout << "odds: "<< ite->victory << ", probablity: "
        << chanceMap[ite->victory] << "%" << endl;
        cout << "odds: "<< ite->failure << ", probablity: "
        << chanceMap[ite->failure] << "%" << endl;
        
        if (chanceMap[ite->victory] > chanceMap[ite->failure]) {
            compensateModel += ite->victory;
            chance[chanceI++] = ite->victory;
        } else {
            compensateModel += ite->failure;
            chance[chanceI++] = ite->failure;
        }
        
        float z = 100 - chanceMap[ite->victory] - chanceMap[ite->failure], x;
        if(ite->operation == 1)
        {
            x = z + chanceMap[ite->failure] + chanceMap[ite->victory] - 100;
            z = x + z;
        } else {
            x = z + chanceMap[ite->failure] + chanceMap[ite->victory] - 100;
            z = x + z;
        }
        cout << "odds: --error--" << ", probablity: " << z << "%" << endl;
        cout << "波动范围：" << queMap[ite->victory] + queMap[ite->failure] << endl << endl;
    }
    /**
     *  威廉概率 ==============>>结束
     */
    
    /**
     *  补偿概率 ==============>>开始
     */
    compensateModel = compensateModel * 1.78 - 4 * arrayMAX;
    float compensateData = (-0.8316) * arrayMAX;
    map<float, float>compensateMap;
    
    for (map<float, float>::iterator mapIte = chanceMap.begin()
         ; mapIte != chanceMap.end(); ++mapIte) {
        compensateMap.insert(make_pair(mapIte->first, 0));
    }
    
    cout << "补偿上限值：" << (-0.8316) * arrayMAX << endl;
    cout << "补偿模型值：" << compensateModel << endl;
    vector<ProbabilityArray>::iterator chanceIte = arrayVector.begin();
    for (chanceI = 0; chanceI < arrayMAX; ++chanceI, ++chanceIte) {
        compensateModel = 0.0;
        if (chance[chanceI] == chanceIte->victory) {
            compensateModel += chanceIte->failure;
        } else {
            compensateModel += chanceIte->victory;
        }
        
        for (int i = 0; i < arrayMAX; ++i) {
            if (i != chanceI) {
                compensateModel += chance[i];
            }
        }
        cout << "补偿" << chanceI + 1 << "model: "  << (compensateModel * 1.78 - 4 * arrayMAX) << endl;
    }
    
    zonggong = 0;
    for (vector<floatArray>::iterator ite = outcomeArray.begin(); ite < outcomeArray.end(); ++ite) {
        //if (ite->number == arrayMAX)
        {
            compensateData = ite->number * (-0.8316);
            compensateModel = 0.0;
            for (int i = 0; i < ite->number; ++i) {
                compensateModel += ite->fArray[i];
            }
            
            float temp = compensateModel;
            compensateModel = temp * 1.77 - 4 * ite->number;
            double compensateModel1 = temp * 1.80 - 4 * ite->number;
    //compensateModel < compensateData && compensateData < compensateModel1
            if (compensateModel < -0.23 * ite->number && compensateModel > compensateData) {
                ++zonggong;
                //cout << "-----------------model--------------------" << endl;
                for (int i = 0; i < ite->number; ++i) {
                    ++compensateMap[ite->fArray[i]];
                    //cout << ite->fArray[i] << " ";
                }
                //cout << compensateModel << endl;
            }
        }
    }
    
    cout << endl;
    int i = 1;
    for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
         ite != arrayVector.end(); ++ite, ++i) {
        cout << "模型" << i << endl;
        float originV = chanceMap[ite->victory], originF = chanceMap[ite->failure];
        cout << ite->victory << "，元概率：" << chanceMap[ite->victory];
        chanceMap[ite->victory] = compensateMap[ite->victory] / zonggong * chanceMap[ite->victory];
        cout << "；修正概率：" << chanceMap[ite->victory] << endl;
        cout << ite->failure << "，元概率：" << chanceMap[ite->failure];
        chanceMap[ite->failure] = compensateMap[ite->failure] / zonggong * chanceMap[ite->failure];
        cout << "；修正概率：" << chanceMap[ite->failure] << endl;
        
        if (originV < originF) {
            float temp1 = originV / originF;
            float temp2 = chanceMap[ite->victory] / chanceMap[ite->failure];
            float temp3 =  chanceMap[ite->victory] / originV;
            float temp4 = chanceMap[ite->failure] / originF;
            cout << "元概率比值：" << temp1 << ", 修正概率比值：" << temp2 << endl;
            cout << "元概率比值：" << temp3 << ", 修正概率比值：" << temp4 << endl;
//            cout << temp3 << "：" << temp1 * temp3 << ", 修正概率比值：" << temp3 * temp2 << endl;
//            cout << temp4 << "：" << temp1 * temp4 << ", 修正概率比值：" << temp4 * temp2 << endl;
            if (temp1 < temp2) {
                cout << "异常！！！！！！！！" << endl;
            }
        } else {
            float temp2 = originF / originV;
            float temp1 = chanceMap[ite->failure] / chanceMap[ite->victory];
            float temp3 = chanceMap[ite->victory] / originV;
            float temp4 = chanceMap[ite->failure] / originF;
            cout << "元概率比值：" << temp2 << ", 修正概率比值：" << temp1 << endl;
            cout << "元概率比值：" << temp3 << ", 修正概率比值：" << temp4 << endl;
//            cout << "元概率比值：" << temp3 << ", 修正概率比值：" << temp4 << endl;
//            cout << temp3 << "：" << temp2 * temp3 << ", 修正概率比值：" << temp3 * temp1 << endl;
//            cout << temp4 << "：" << temp2 * temp4 << ", 修正概率比值：" << temp4 * temp1 << endl;
            if (temp2 < temp1) {
                cout << "异常！！！！！！！！" << endl;
            }
        }
        cout << endl;
    }
    /**
     *  补偿概率 ==============>>结束
     */
    
    void InsertSql(float odds, int isVictory, int array[], bool isexec);
    int array[2] = {1, 1};
    int outCome[arrayMAX];
    
    cout << endl << "---------------自动学习概率--------------- " << endl;
    int j = 0;
    for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
         ite != arrayVector.end(); ++ite){
        
        array[0] = 1;
        array[1] = 1;
        
        InsertSql(ite->victory, 0, array, false);
        cout << "odds: "<< ite->victory << ", history probablity: "
        << ((float)array[0]/array[1]) * 100 << "%" << "(" << array[0] << "/" << array[1] << ")"
        << ",元概率：" << chanceMap[ite->victory] << "%" << ", 数据库概率:  "
        << ((float)array[0]/array[1]) * chanceMap[ite->victory] << "%" <<endl;
        
        InsertSql(ite->failure, 0, array, false);
        cout << "odds: "<< ite->failure << ", history probablity: "
        << ((float)array[0]/array[1]) * 100 << "%" << "(" << array[0] << "/" << array[1] << ")"
        << ",元概率：" << chanceMap[ite->failure] << "%" << ", 数据库概率:  "
        << ((float)array[0]/array[1]) * chanceMap[ite->failure] << "%" << endl << endl;
    }
    
    cout << endl << "---------------repel choose--------------- " << endl;
    vector<ProbabilityArray> repelChooseVector;
    vector<floatArray> repelChoooseOutcomeArray;
    cout << "please input data(end with '-1'): " << endl;
    while (1) {
        float temp1, temp2, temp3;
        cin >> temp1;
        if (temp1 < 0)
            break;
        cin  >> temp2;
        if(temp2 < 0)
            break;
        cin >> temp3;
        if(temp3 < 0)
            break;
        
        ProbabilityArray *arr = new ProbabilityArray();
        arr->maxOdds = temp1;
        arr->minOdds = temp1;
        //int minmaxSum = 3;
        //while (minmaxSum)
        
        if (arr->maxOdds < temp2) {
            arr->maxOdds = temp2;
        }
        if (arr->maxOdds < temp3) {
            arr->maxOdds = temp3;
        }
        
        if (arr->minOdds > temp2) {
            arr->minOdds = temp2;
        }
       
        if (arr->minOdds > temp3) {
            arr->minOdds = temp3;
        }
        
        if (repelChooseVector.empty()) {
            arr->victoryNum = 1;
            arr->failureNum = 1;
            arr->drawNum = 1;
            arr->failure = temp3;
            arr->victory = temp1;
            arr->draw = temp2;
        } else
            for (vector<ProbabilityArray>::iterator ite = repelChooseVector.begin(); ite != repelChooseVector.end(); ++ite) {
                
                
                bool isV = false, isF = false, isD = false;
                if (ite->failure == temp1) {
                    ++ite->failureNum;
                    arr->victory = temp1 + 0.1 * pow(0.1, (ite->failureNum));
                    isV = true;
                }
                
                if (ite->victory == temp1) {
                    ++ite->victoryNum;
                    arr->victory = temp1 + 0.1 * pow(0.1, (ite->victoryNum));
                    isV = true;
                }
                
                if (ite->draw == temp1) {
                    ++ite->drawNum;
                    arr->victory = temp1 + 0.1 * pow(0.1, (ite->drawNum));
                    isV = true;
                }
                
                
                if (ite->failure == temp2) {
                    ++ite->failureNum;
                    arr->draw = temp2 + 0.1 * pow(0.1, (ite->failureNum));
                    isD = true;
                }
                
                if (ite->victory == temp2) {
                    ++ite->victoryNum;
                    arr->draw = temp2 + 0.1 * pow(0.1, (ite->victoryNum));
                    isD = true;
                }
                
                if (ite->draw == temp2) {
                    ++ite->drawNum;
                    arr->draw = temp2 + 0.1 * pow(0.1, (ite->drawNum));
                    isD = true;
                }
                
                if (ite->draw == temp3) {
                    ++ite->drawNum;
                    arr->failure = temp3 + 0.1 * pow(0.1, (ite->drawNum));
                    isF = true;
                }
                
                if (ite->victory == temp3) {
                    ++ite->victoryNum;
                    arr->failure = temp3 + 0.1 * pow(0.1, (ite->victoryNum));
                    isF = true;
                }
                
                if (ite->failure == temp3) {
                    ++ite->failureNum;
                    arr->failure = temp3 + 0.1 * pow(0.1, (ite->failureNum));
                    isF= true;
                }
                
                {
                    if (!isV) {
                        arr->victory = temp1;
                    }
                    if (!isF) {
                        arr->failure = temp3;
                    }
                    
                    if (!isD) {
                        arr->draw = temp2;
                    }
                    
                    arr->victoryNum = 1;
                    arr->failureNum = 1;
                    arr->drawNum = 1;
                    if (isV || isF || isD) {
                        break;
                    }
                }
            }
        repelChooseVector.push_back(*arr);
    }
    
    float repelAve = 0.0;
    float minAve = 0.0;
    float maxAve = 0.0;
    for (vector<ProbabilityArray>::iterator ite = repelChooseVector.begin();
         ite < repelChooseVector.end(); ++ite) {
        cout << ite->victory << " " << ite->draw << " " << ite->failure << endl;
        repelAve = repelAve + ite->victory + ite->draw + ite->failure;
        float temp1 = minAve;
        float temp2 = maxAve;
        minAve = ite->victory;
        maxAve = ite->victory;
        if (minAve > ite->draw) {
            minAve = ite->draw;
            if (minAve > ite->failure) {
                minAve = ite->failure;
            }
        } else {
            if (minAve > ite->failure) {
                minAve = ite->failure;
            }
        }
        
        if (maxAve < ite->draw) {
            maxAve = ite->draw;
            if (maxAve < ite->failure) {
                maxAve = ite->failure;
            }
        } else {
            if (maxAve < ite->failure) {
                maxAve = ite->failure;
            }
        }
        minAve += temp1;
        maxAve += temp2;
    }
    repelAve = repelAve / (3 * arrayMAX);
    minAve = minAve / arrayMAX;
    maxAve = maxAve /arrayMAX;
    cout << "-------------------------------" << endl;
    float zheshitemp = repelAve;
    float zhetemp = (minAve + maxAve) / 2;
    cout << "最小平均：" << minAve << ", 平均值：" << repelAve << ", 最大平均值：" << maxAve << ", 最大+最小："
    << (minAve + maxAve) / 2  << endl;
    
    if (zheshitemp < zhetemp) {
        cout << ( zheshitemp + 2.25) / 2 << endl;
        zheshitemp = ( zheshitemp + 2.25) / 2;
    } else {
        cout << ( zhetemp + 2.25) / 2 << endl;
        zheshitemp = ( zhetemp + 2.25) / 2;
    }
    
//    if (maxAve / 2 > minAve) {
//        zheshitemp =  (zheshitemp + minAve) / 2;
//        cout << ( zheshitemp + 2.25) / 2 << endl;
//    }
    
    
    
    void tripleAdjustArrange(vector<ProbabilityArray> &arrayVector,
                             vector<floatArray> &outcomeArray, int max);
    tripleAdjustArrange(repelChooseVector, repelChoooseOutcomeArray, arrayMAX);
    
    map<float, float>repelChanceMap;
    zonggong = 0.0;
    profit = 0.0;
    
    cout << "##################################################" << endl;
    cout << repelChoooseOutcomeArray.size() << endl;
    float maxProfit = 0.0;
    float maxArray[arrayMAX];
    float minProfit = 10.0;
    float minArray[arrayMAX];
    float midProfit = 0.0;
    float midArray[arrayMAX];
    float shanxian = (zheshitemp + 0.5) * 1.78 - 4;
    float xiaxian = (zheshitemp - 0.5) * 1.78 - 4;
    cout << shanxian << "," << xiaxian << endl;
    for (vector<floatArray>::iterator ite = repelChoooseOutcomeArray.begin() + 3;
         ite < repelChoooseOutcomeArray.end(); ++ite) {
        
        if (ite->number != arrayMAX) {
            continue;
        } else{
            profit = 0.0;
            for (int i = 0; i < ite->number; ++i) {
                profit += ite->fArray[i] * 1.78 - 4;
            }
            
            float temp = (float)arrayMAX;
        
            if (xiaxian * arrayMAX < profit && profit < shanxian * arrayMAX)
            {
                ++zonggong;
                for (int i = 0; i < ite->number; ++i) {
                   // cout << ite->fArray[i] << " ";
                    map<float, float>::iterator mapite = repelChanceMap.find(ite->fArray[i]);
                    if (mapite == repelChanceMap.end()) {
                        repelChanceMap.insert(make_pair(ite->fArray[i], 1.0));
                    } else {
                        ++mapite->second;
                    }
                }
                if (maxProfit < profit) {
                    for (int i = 0; i < ite->number; ++i) {
                        maxArray[i] = ite->fArray[i];
                    }
                    maxProfit = profit;
                }
                
                if (minProfit > profit) {
                    for (int i = 0; i < ite->number; ++i) {
                        minArray[i] = ite->fArray[i];
                    }
                    minProfit = profit;
                }
                
                if (profit < arrayMAX * 0.89 && midProfit < profit) {
                    for (int i = 0; i < ite->number; ++i) {
                        midArray[i] = ite->fArray[i];
                    }
                    midProfit = profit;
                }
               // cout << profit << endl;
                profit = 0.0;
            }
        }
    }
    
    cout << endl << "-------------max mid min profit------------- " << endl;
    for (int i = 0; i < arrayMAX; ++i) {
        cout << maxArray[i] << " ";
    }
    cout << maxProfit << endl;
    
    for (int i = 0; i < arrayMAX; ++i) {
        cout << midArray[i] << " ";
    }
    cout << midProfit << endl;
    
    for (int i = 0; i < arrayMAX; ++i) {
        cout << minArray[i] << " ";
    }
    cout << minProfit << endl;
    cout << endl << "-------------max mid min profit------------- " << endl;
    
    for (map<float, float>::iterator mapite = repelChanceMap.begin(); mapite != repelChanceMap.end(); ++mapite) {
        mapite->second = 100.0 - (float)mapite->second/zonggong * 100.0;
    }
    
    cout << endl << "-------------repel profit------------- " << endl;// << profit << endl;
  
    
    vector<ProbabilityArray>::iterator repelIte = repelChooseVector.begin();
    for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
         ite != arrayVector.end(); ++ite, ++repelIte) {
        
        //if (ite->operation)
        {
            cout << ite->victory << "，元概率：" << chanceMap[ite->victory];
            chanceMap[ite->victory] =  (repelChanceMap[repelIte->victory] + repelChanceMap[repelIte->draw])/ 2 / zonggong * chanceMap[ite->victory];
            cout << "；修正概率：" << chanceMap[ite->victory] << endl;
        }
        //else
        {
            cout << ite->failure << "，元概率：" << chanceMap[ite->failure];
            chanceMap[ite->failure] = (repelChanceMap[repelIte->failure] + repelChanceMap[repelIte->draw])/ 2 / zonggong * chanceMap[ite->failure];
            cout << "；修正概率：" << chanceMap[ite->failure] << endl;
        }
        
        if (chanceMap[ite->victory] > chanceMap[ite->failure]) {
            cout << "正，选择的概率为：" << repelIte->victory << ", " << repelIte->draw << endl;
        } else {
            cout << "负，选择的概率为：" << repelIte->draw << ", " << repelIte->failure << endl;
        }
        
        cout << endl;
    }

    
    vector<ProbabilityArray>::iterator ite1 = arrayVector.begin();
    int iteI = 0;
    for (vector<ProbabilityArray>::iterator ite = repelChooseVector.begin();
         ite != repelChooseVector.end(); ++ite, ++ite1, ++iteI){
        if (chanceMap[ite1->victory] < chanceMap[ite1->failure]) {
            if ((maxArray[iteI] - ite->maxOdds)  > -0.01 && (maxArray[iteI] - ite->maxOdds) < 0.01) {
                if (maxArray[iteI] == ite->draw) {
                    cout << "选择的概率：" << "nan% " << " " << ite->failure << endl;
                } else {
                    if (maxArray[iteI == ite->failure]) {
                        cout << "选择的概率：" << ite->draw << " " << "nan%" << endl;
                    } else {
                        cout << "选择的概率：" << ite->draw << " " << ite->failure << endl;
                    }
                }
            } else {
                cout << "选择的概率：" << ite->draw << " " << ite->failure << endl;
            }
            
        } else {
            if ( (maxArray[iteI] - ite->maxOdds)  > -0.01 && (maxArray[iteI] - ite->maxOdds) < 0.01 ){
                if (maxArray[iteI] == ite->victory) {
                    cout << "选择的概率：" << "nan%" << " " << ite->draw << endl;
                } else {
                    if (maxArray[iteI == ite->draw]) {
                        cout << "选择的概率："<< ite->victory << " "<< "nan%" << endl;
                    } else {
                        cout << "选择的概率：" << ite->victory << " " << ite->draw << endl;
                    }
                }
            } else {
                cout << "选择的概率：" << ite->victory << " " << ite->draw << endl;
            }
        }
        
//        if (repelChanceMap.find(ite->victory) != repelChanceMap.end()) {
//            cout << "odds: "<< ite->victory << ", probablity: "
//            << repelChanceMap[ite->victory] << "%" << endl;
//        }
//        
//        if (repelChanceMap.find(ite->draw) != repelChanceMap.end()) {
//            cout << "odds: "<< ite->draw << ", probablity: "
//            << repelChanceMap[ite->draw] << "%" << endl;
//        }
//        
//        if (repelChanceMap.find(ite->failure) != repelChanceMap.end()) {
//            cout << "odds: "<< ite->failure << ", probablity: "
//            << repelChanceMap[ite->failure] << "%" << endl;
//        }
        
        cout << endl;
    }
    
    int isUed;
    cout << "是否进行ued修正？" << endl;
    cout << "----------------------------------------"<< endl;
    cout << "    _________________________________" << endl;
    cout << "    |                               |" << endl;
    cout << "    |         0:  no                |" << endl;
    cout << "    |         1:  yes               |" << endl;
    cout << "    |         2:  continue          |" << endl;
    cout << "    _________________________________" << endl;
    cout << "----------------------------------------"<< endl;
    cin >> isUed;
    if (isUed == 1){
        vector<ProbabilityArray> uedArrayVector;
        vector<AllArrangeArray> uedArrangeArray;
        vector<floatArray> uedOutcomeArray;
        map<float, float>uedChanceMap;
        
        cout << "please input data(end with '-1'): " << endl;
        while (1) {
            float temp1;
            cin >> temp1;
            if(temp1 < 0)
                break;
            
            ProbabilityArray *arr = new ProbabilityArray();
            
            if (uedArrayVector.empty()) {
                arr->victoryNum = 1;
                arr->victory = temp1;
                arr->failure = -1;
            } else{
                bool isV = false;
                for (vector<ProbabilityArray>::iterator ite = uedArrayVector.begin(); ite != uedArrayVector.end(); ++ite) {
                    if (ite->victory == temp1) {
                        ++ite->victoryNum;
                        arr->victory = temp1 + 0.1 * pow(0.1, (ite->victoryNum));
                        isV = true;
                        break;
                    }
                }
                
                if (!isV) {
                    arr->victory = temp1;
                }
                arr->failure = -1;
                arr->victoryNum = 1;
            }
            
            uedArrayVector.push_back(*arr);
        }
        
        adjustArrange(uedArrayVector, uedOutcomeArray, arrayMAX);
        profit = 0.0;
        zonggong = 0.0;
        float uedSum = 0.0;
        int uedPositiveArray[arrayMAX];
        int uedNegativeArray[arrayMAX];
        
        for (auto i = 0; i < arrayMAX; ++i) {
            uedPositiveArray[i] = 0;
            uedNegativeArray[i] = 0;
        }
        
        float temp = ceilf(arrayMAX / 3.33);
        float temp1 = arrayMAX * 0.24;
        float temp2 = temp1 - temp;
        temp1 = arrayMAX * 0.24 - 1;
        
        cout << temp2 << ", " << temp << endl;
        
        for (vector<floatArray>::iterator ite = uedOutcomeArray.begin() + 2;
             ite < uedOutcomeArray.end(); ++ite) {
            
            if (ite->number != arrayMAX) {
                continue;
            }
            uedSum = 0.0;
            for (int i = 0; i < ite->number; ++i) {
                uedSum += ite->fArray[i];
            }
            
            if (temp2 < uedSum && uedSum < temp1) {
                zonggong++;
                for (int i = 0; i < ite->number; ++i) {
                    if (ite->fArray[i] > 0) {
                        ++uedPositiveArray[i];
                    } else {
                        ++uedNegativeArray[i];
                    }
                }
            }
        }
        
        int uedI = 0;
        for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
             ite != arrayVector.end(); ++ite, ++repelIte, ++uedI) {
            
            
            //  cout << ite->victory << "，元概率：" << chanceMap[ite->victory] << "%" << endl;
            //  cout << ite->failure << "，元概率：" << chanceMap[ite->failure] << "%" << endl;
            
            if (chanceMap[ite->victory] > chanceMap[ite->failure]) {
                chanceMap[ite->victory] = chanceMap[ite->victory] * uedPositiveArray[uedI] / zonggong;
                chanceMap[ite->failure] = chanceMap[ite->failure] * uedNegativeArray[uedI] / zonggong;
            } else {
                chanceMap[ite->failure] = chanceMap[ite->failure] * uedPositiveArray[uedI] / zonggong;
                chanceMap[ite->victory] = chanceMap[ite->victory] * uedNegativeArray[uedI] / zonggong;
            }
            
            //  cout << ite->victory << "，修正概率：" << chanceMap[ite->victory] << "%" << endl;
            //  cout << ite->failure << "，修正概率：" << chanceMap[ite->failure] << "%" << endl;
            //  cout << endl;
        }
        
        
        repelIte = repelChooseVector.begin();
        for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
             ite != arrayVector.end(); ++ite, ++repelIte) {
            cout << ite->victory << "，概率：" << chanceMap[ite->victory] << "%" << endl;
            cout << ite->failure << "，概率：" << chanceMap[ite->failure] << "%" << endl;
            
            if (chanceMap[ite->victory] > chanceMap[ite->failure]) {
                cout << "正，选择的概率为：" << repelIte->victory << ", " << repelIte->draw << endl;
            } else {
                cout << "负，选择的概率为：" << repelIte->draw << ", " << repelIte->failure << endl;
            }
            
            cout << endl;
        }
    } else {
        if (!isUed) {
            return 0;
        }
    }
    
    cout << "please choose next operation " << endl;
    cout << "----------------------------------------"<< endl;
    cout << "    _________________________________" << endl;
    cout << "    |                               |" << endl;
    cout << "    |         0: end                |" << endl;
    cout << "    |         1: write database     |" << endl;
    cout << "    _________________________________" << endl;
    cout << "----------------------------------------"<< endl;
    int isCinOutcome;
    cin >> isCinOutcome;
    
    
    if (isCinOutcome == 0) {
        return 0;
    }
    
    cout << "please input outcome: ";
    for (int i = 0; i < arrayMAX; ++i) {
        cin >> outCome[i];
    }
    
    cout << endl << "-------------自动学习修正概率------------- " << endl;
    j = 0;
    for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
         ite != arrayVector.end(); ++ite, ++j){
        
        int temp11 = 0, temp21 = 0;
        switch (outCome[j]) {
            case 0:
                temp11 = 0;
                temp21 = 1;
                break;
            case 1:
                temp11 = 1;
                temp21 = 0;
                break;
            case 2:
                temp11 = 1;
                temp21 = 1;
                break;
            default:
                break;
        }
        
        InsertSql(ite->victory, temp11, array, true);
        cout << "odds: "<< ite->victory << ", history probablity: "
        << ((float)array[0]/array[1]) * 100 << "%" << ", adjusted probablity: "
        << ((float)array[0]/array[1]) * chanceMap[ite->victory] * 100 << "%" << endl;
       
        InsertSql(ite->failure, temp21, array, true);
        cout << "odds: "<< ite->failure << ", history probablity: "
        << ((float)array[0]/array[1]) * 100 << "%" << ", adjusted probablity: "
        << ((float)array[0]/array[1]) * chanceMap[ite->failure] * 100 << "%" << endl;
        
        cout << endl;
    }
    return 0;
}

bool ExecSql(sqlite3 *database, string sql)
{
    char *errorMsg;
    if (sqlite3_exec(database, sql.c_str(), NULL, NULL, &errorMsg) != SQLITE_OK) {
        sqlite3_close(database);
        cout << "数据库操作数据失败!" << endl;
        return false;
    }
    return true;
}

void InsertSql(float odds, int isVictory, int array[], bool isexec)
{
    sqlite3 *database;
    int openDataBasesResult = sqlite3_open("/Users/houqingfeng/Documents/Git/ProbabilityTheory/概率论/MemoryPalace.db", &database);
    if (openDataBasesResult != SQLITE_OK) {
        sqlite3_close(database);//关闭数据库
        cout << "打开数据库失败！" << endl;
    }
    
    char *errorMsg;
    const char *createSQL = "CREATE TABLE IF NOT EXISTS DataMemoryCenter (ID INTEGER PRIMARY KEY AUTOINCREMENT, ODDS Victory Sum)";
    sqlite3_exec(database, createSQL, NULL, NULL, &errorMsg);
    
    //执行查询
    char ch[100] = {0};
    sprintf(ch, "select * from DataMemoryCenter where odds=%f", odds);
    string query(ch);
    int vNum = 1;
    int sumNum = 1;
    sqlite3_stmt *statement;
    bool isUpdate = false;
    if (sqlite3_prepare_v2(database, query.c_str(), -1, &statement, NULL) == SQLITE_OK) {
        //依次读取数据库表格FIELDS中每行的内容，并显示在对应的TextField
        while (sqlite3_step(statement) == SQLITE_ROW) {
            isUpdate = true;
            char *victory = (char *)sqlite3_column_text(statement, 2);
            char *sum = (char *)sqlite3_column_text(statement, 3);
            
            if (!isexec) {
                array[0] = atoi(victory);
                array[1] = atoi(sum);
                sqlite3_finalize(statement);
                sqlite3_close(database);//关闭数据库
                return;
            }
            
            vNum = atoi(victory) + 1;
            if(isVictory)
            {
                
                char ch[100] = {0};
                sprintf(ch, "UPDATE DataMemoryCenter SET Victory = %d WHERE ODDS=%f", vNum, odds);
                string updateSqlV(ch);
                if(!ExecSql(database, updateSqlV))
                {
                    sqlite3_finalize(statement);
                    return;
                }
            }
            sumNum = atoi(sum) + 1;
            char ch[100] = {0};
            sprintf(ch, "UPDATE DataMemoryCenter SET Sum = %d WHERE ODDS=%f", sumNum, odds);
            string updateSqlS(ch);
            if(!ExecSql(database, updateSqlS))
            {
                sqlite3_finalize(statement);
                return;
            }
            array[0] = vNum;
            array[1] = sumNum;
        }
        sqlite3_finalize(statement);
    }
    
    if (!isUpdate)
    {
        array[0] = 1;
        array[1] = 1;
        if(isexec)
        {
            char ch[100] = {0};
            sprintf(ch, "INSERT INTO 'DataMemoryCenter' ('ODDS', 'Victory', 'Sum') VALUES ('%f', '%d', '%d')", odds, isVictory, sumNum);
            string sql2(ch);
            if(ExecSql(database, sql2))
                return;
        }
    }
    
    sqlite3_close(database);//关闭数据库
}

/**
 *  willian概率修正
 */
void WilliamOddsRevise(vector<ProbabilityArray> &arrayVector, map<float, float> &chanceMap)
{
    // cout << "--------------willian------------------" << endl;
    for (vector<ProbabilityArray>::iterator ite = arrayVector.begin();
         ite != arrayVector.end(); ++ite){
        if (chanceMap.find(ite->victory) == chanceMap.end()) {
            //cout << "willian: " << ite->victory << ", probablity: "<< ite->willianVictoryProbality << endl;
            chanceMap[ite->victory] = ite->willianVictoryProbality;
        } else {
            //cout << "willian: " << ite->victory << ", probablity: "<< ite->willianVictoryProbality << endl;
            chanceMap[ite->victory] = chanceMap[ite->victory] * ite->willianVictoryProbality / 100.0;
        }
        if (chanceMap.find(ite->failure) == chanceMap.end()) {
            //cout << "willian: " << ite->failure << ", probablity: "<< ite->willianFailureProbality << endl;
            chanceMap[ite->failure] = ite->willianFailureProbality;
        } else {
            //cout << "willian: " << ite->failure << ", probablity: "<< ite->willianFailureProbality << endl;
            chanceMap[ite->failure] = chanceMap[ite->failure] * ite->willianFailureProbality / 100.0;
        }
    }
}

/**
 *  全排列
 */
void AllArray(vector<AllArrangeArray> &arrangeArray,
              vector<ProbabilityArray> &arrayVector,
              vector<floatArray> &outcomeArray, int max)
{
    int arrangeID[max];
    for (int i = 1; i <= max; ++i) {
        arrangeID[i-1] = i;
    }
    
    AllArrangeArray *arrTemp = new AllArrangeArray();
    arrTemp->arrange = new Arrange();
    arrTemp->arrange->array = new int();
    arrTemp->arrange->number = 1;
    *arrTemp->arrange->array = max;
    arrangeArray.push_back(*arrTemp);
    
    floatArray *temp1 = new floatArray();
    floatArray *temp2 = new floatArray();
    temp1->fArray = new float();
    *temp1->fArray = arrayVector[ max-1 ].victory;
    temp1->number = 1;
    temp2->fArray = new float();
    *temp2->fArray = arrayVector[ max-1 ].failure;
    temp2->number = 1;
    outcomeArray.push_back(*temp1);
    outcomeArray.push_back(*temp2);
    
    while (1) {
        int num = *arrangeArray[arrangeArray.size() - 1].arrange->array - 1;
        if (num == 0) {
            break;
        }
        int  outcomeLength = (int)outcomeArray.size();
        for (int i = (int)arrangeArray.size() - 1, j = 0; i >= 0; --i, ++j) {
            if (j == 0) {
                for (int k = num + 1; k <= max; ++k) {
                    AllArrangeArray *arrTemp = new AllArrangeArray();
                    arrTemp->arrange = new Arrange();
                    arrTemp->arrange->array = new int[2];
                    arrTemp->arrange->number = 2;
                    arrTemp->arrange->array[0] = num;
                    arrTemp->arrange->array[1] = k;
                    arrangeArray.push_back(*arrTemp);
                }
                
                for (int k1 = num + 1; k1 < max; ++k1) {
                    if (k1 != num) {
                        floatArray *temp1 = new floatArray();
                        temp1->fArray = new float[2];
                        temp1->fArray[0] = arrayVector[num].victory;
                        temp1->fArray[1] = arrayVector[k1].victory;
                        temp1->number = 2;
                        outcomeArray.push_back(*temp1);
                        
                        floatArray *temp2 = new floatArray();
                        temp2->fArray = new float[2];
                        temp2->fArray[0] = arrayVector[num].victory;
                        temp2->fArray[1] = arrayVector[k1].failure;
                        temp2->number = 2;
                        outcomeArray.push_back(*temp2);
                        
                        floatArray *temp3 = new floatArray();
                        temp3->fArray = new float[2];
                        temp3->fArray[0] = arrayVector[num].failure;
                        temp3->fArray[1] = arrayVector[k1].failure;
                        temp3->number = 2;
                        outcomeArray.push_back(*temp3);
                        
                        floatArray *temp4 = new floatArray();
                        temp4->fArray = new float[2];
                        temp4->fArray[0] = arrayVector[num].failure;
                        temp4->fArray[1] = arrayVector[k1].victory;
                        temp4->number = 2;
                        outcomeArray.push_back(*temp4);
                    }
                }
                
            } else {
                AllArrangeArray *arrTemp = new AllArrangeArray();
                arrTemp->arrange = new Arrange();
                arrTemp->arrange->array = new int[arrangeArray[j].arrange->number + 1];
                arrTemp->arrange->number = arrangeArray[j].arrange->number + 1;
                arrTemp->arrange->array[0] = num;
                for (int z = 0; z < arrangeArray[j].arrange->number; ++z) {
                    arrTemp->arrange->array[z + 1] = arrangeArray[j].arrange->array[z];
                }
                arrangeArray.push_back(*arrTemp);
                
                for (int i = 2; i < outcomeLength; ++i) {
                    int j = 0;
                    while (j++ < 2) {
                        if (j == 1) {
                            floatArray *temp = new floatArray();
                            temp->fArray = new float[outcomeArray[i].number + 1];
                            temp->fArray[0] = arrayVector[num].victory;
                            temp->number = outcomeArray[i].number + 1;
                            for (int k = 0; k < outcomeArray[i].number; ++k) {
                                temp->fArray[k + 1] = outcomeArray[i].fArray[k];
                            }
                            outcomeArray.push_back(*temp);
                        } else {
                            floatArray *temp = new floatArray();
                            temp->fArray = new float[outcomeArray[i].number + 1];
                            temp->fArray[0] = arrayVector[num].failure;
                            temp->number = outcomeArray[i].number + 1;
                            for (int k = 0; k < outcomeArray[i].number; ++k) {
                                temp->fArray[k + 1] = outcomeArray[i].fArray[k];
                            }
                            outcomeArray.push_back(*temp);
                        }
                    }
                }
            }
        }
    }
}

/**
 *  变形的全排列
 */
void adjustArrange(vector<ProbabilityArray> &arrayVector,
                   vector<floatArray> &outcomeArray, int max)
{
    floatArray *temp1 = new floatArray();
    floatArray *temp2 = new floatArray();
    temp1->fArray = new float();
    *temp1->fArray = arrayVector[ max-1 ].victory;
    temp1->number = 1;
    temp1->location[0] = arrayVector[ max-1 ].location;
    temp2->fArray = new float();
    *temp2->fArray = arrayVector[ max-1 ].failure;
    temp2->location[0] = arrayVector[ max-1 ].location;
    temp2->number = 1;
    outcomeArray.push_back(*temp1);
    outcomeArray.push_back(*temp2);
    int num = max - 2;
    
    while (1) {
        if (num < 0) {
            break;
        }
        int  outcomeLength = (int)outcomeArray.size();
        {
            for (int k1 = num + 1; k1 < max; ++k1) {
                floatArray *temp1 = new floatArray();
                temp1->fArray = new float[2];
                temp1->fArray[0] = arrayVector[num].victory;
                temp1->fArray[1] = arrayVector[k1].victory;
                temp1->location[0] = arrayVector[num].location;
                temp1->location[1] = arrayVector[k1].location;
                temp1->number = 2;
                temp1->print = true;
                outcomeArray.push_back(*temp1);
                
                floatArray *temp2 = new floatArray();
                temp2->fArray = new float[2];
                temp2->fArray[0] = arrayVector[num].victory;
                temp2->fArray[1] = arrayVector[k1].failure;
                temp2->location[0] = arrayVector[num].location;
                temp2->location[1] = arrayVector[k1].location;
                temp2->number = 2;
                temp2->print = false;
                outcomeArray.push_back(*temp2);
                
                floatArray *temp3 = new floatArray();
                temp3->fArray = new float[2];
                temp3->fArray[0] = arrayVector[num].failure;
                temp3->fArray[1] = arrayVector[k1].failure;
                temp3->location[0] = arrayVector[num].location;
                temp3->location[1] = arrayVector[k1].location;
                temp3->number = 2;
                temp3->print = false;
                outcomeArray.push_back(*temp3);
                
                floatArray *temp4 = new floatArray();
                temp4->fArray = new float[2];
                temp4->fArray[0] = arrayVector[num].failure;
                temp4->fArray[1] = arrayVector[k1].victory;
                temp4->location[0] = arrayVector[num].location;
                temp4->location[1] = arrayVector[k1].location;
                temp4->number = 2;
                temp4->print = false;
                outcomeArray.push_back(*temp4);
            }
            
            for (int i = 2; i < outcomeLength; ++i)
            {
                int j = 2;
                while (j > 0) {
                    if (j == 1) {
                        floatArray *temp = new floatArray();
                        temp->fArray = new float[outcomeArray[i].number + 1];
                        temp->fArray[0] = arrayVector[num].victory;
                        temp->location[0] = arrayVector[num].location;
                        temp->number = outcomeArray[i].number + 1;
                        temp->print = false;
                        for (int k = 0; k < outcomeArray[i].number; ++k) {
                            temp->fArray[k + 1] = outcomeArray[i].fArray[k];
                            temp->location[k + 1] = outcomeArray[i].location[k];
                        }
                        outcomeArray.push_back(*temp);
                    } else {
                        floatArray *temp = new floatArray();
                        temp->fArray = new float[outcomeArray[i].number + 1];
                        temp->fArray[0] = arrayVector[num].failure;
                        temp->location[0] = arrayVector[num].location;
                        temp->number = outcomeArray[i].number + 1;
                        temp->print = false;
                        for (int k = 0; k < outcomeArray[i].number; ++k) {
                            temp->fArray[k + 1] = outcomeArray[i].fArray[k];
                            temp->location[k + 1] = outcomeArray[i].location[k];
                        }
                        outcomeArray.push_back(*temp);
                    }
                    --j;
                }
            }
        }
        --num;
    }
}

/**
 *  三次选择全排列
 */
void tripleAdjustArrange(vector<ProbabilityArray> &arrayVector,
                   vector<floatArray> &outcomeArray, int max)
{
    floatArray *temp1 = new floatArray();
    floatArray *temp2 = new floatArray();
    floatArray *temp3 = new floatArray();
    temp1->fArray = new float();
    *temp1->fArray = arrayVector[ max-1 ].victory;
    temp1->number = 1;
    temp2->fArray = new float();
    *temp2->fArray = arrayVector[ max-1 ].failure;
    temp2->number = 1;
    temp3->fArray = new float();
    *temp3->fArray = arrayVector[ max-1 ].draw;
    temp3->number = 1;
    outcomeArray.push_back(*temp1);
    outcomeArray.push_back(*temp2);
    outcomeArray.push_back(*temp3);
    int num = max - 2;
    
    while (1) {
        if (num < 0) {
            break;
        }
        int  outcomeLength = (int)outcomeArray.size();
        {
            for (int k1 = num + 1; k1 < max; ++k1) {
                floatArray *temp = new floatArray();
                temp->fArray = new float[2];
                temp->fArray[0] = arrayVector[num].victory;
                temp->fArray[1] = arrayVector[k1].victory;
                temp->number = 2;
                temp->print = true;
                outcomeArray.push_back(*temp);
                
                floatArray *temp2 = new floatArray();
                temp2->fArray = new float[2];
                temp2->fArray[0] = arrayVector[num].victory;
                temp2->fArray[1] = arrayVector[k1].failure;
                temp2->number = 2;
                temp2->print = false;
                outcomeArray.push_back(*temp2);
                
                floatArray *temp3 = new floatArray();
                temp3->fArray = new float[2];
                temp3->fArray[0] = arrayVector[num].failure;
                temp3->fArray[1] = arrayVector[k1].failure;
                temp3->number = 2;
                temp3->print = false;
                outcomeArray.push_back(*temp3);
                
                floatArray *temp4 = new floatArray();
                temp4->fArray = new float[2];
                temp4->fArray[0] = arrayVector[num].failure;
                temp4->fArray[1] = arrayVector[k1].victory;
                temp4->number = 2;
                temp4->print = false;
                outcomeArray.push_back(*temp4);
                
                floatArray *temp5 = new floatArray();
                temp5->fArray = new float[2];
                temp5->fArray[0] = arrayVector[num].victory;
                temp5->fArray[1] = arrayVector[k1].draw;
                temp5->number = 2;
                temp5->print = false;
                outcomeArray.push_back(*temp5);
                
                floatArray *temp6 = new floatArray();
                temp6->fArray = new float[2];
                temp6->fArray[0] = arrayVector[num].failure;
                temp6->fArray[1] = arrayVector[k1].draw;
                temp6->number = 2;
                temp6->print = false;
                outcomeArray.push_back(*temp6);
                
                floatArray *temp7 = new floatArray();
                temp7->fArray = new float[2];
                temp7->fArray[0] = arrayVector[num].draw;
                temp7->fArray[1] = arrayVector[k1].draw;
                temp7->number = 2;
                temp7->print = false;
                outcomeArray.push_back(*temp7);
                
                floatArray *temp8 = new floatArray();
                temp8->fArray = new float[2];
                temp8->fArray[0] = arrayVector[num].draw;
                temp8->fArray[1] = arrayVector[k1].victory;
                temp8->number = 2;
                temp8->print = false;
                outcomeArray.push_back(*temp8);
                
                floatArray *temp9 = new floatArray();
                temp9->fArray = new float[2];
                temp9->fArray[0] = arrayVector[num].draw;
                temp9->fArray[1] = arrayVector[k1].failure;
                temp9->number = 2;
                temp4->print = false;
                outcomeArray.push_back(*temp9);
            }
            
            for (int i = 3; i < outcomeLength; ++i)
            {
                int j = 3;
                while (j > 0) {
                    if (j == 1) {
                        floatArray *temp = new floatArray();
                        temp->fArray = new float[outcomeArray[i].number + 1];
                        temp->fArray[0] = arrayVector[num].victory;
                        temp->number = outcomeArray[i].number + 1;
                        temp->print = false;
                        for (int k = 0; k < outcomeArray[i].number; ++k) {
                            temp->fArray[k + 1] = outcomeArray[i].fArray[k];
                        }
                        outcomeArray.push_back(*temp);
                    } else {
                        if (j == 2) {
                            floatArray *temp = new floatArray();
                            temp->fArray = new float[outcomeArray[i].number + 1];
                            temp->fArray[0] = arrayVector[num].failure;
                            temp->number = outcomeArray[i].number + 1;
                            temp->print = false;
                            for (int k = 0; k < outcomeArray[i].number; ++k) {
                                temp->fArray[k + 1] = outcomeArray[i].fArray[k];
                            }
                            outcomeArray.push_back(*temp);
                        } else {
                            floatArray *temp = new floatArray();
                            temp->fArray = new float[outcomeArray[i].number + 1];
                            temp->fArray[0] = arrayVector[num].draw;
                            temp->number = outcomeArray[i].number + 1;
                            temp->print = false;
                            for (int k = 0; k < outcomeArray[i].number; ++k) {
                                temp->fArray[k + 1] = outcomeArray[i].fArray[k];
                            }
                            outcomeArray.push_back(*temp);
                        }
                    }
                    --j;
                }
            }
        }
        --num;
    }
}


//
///**
// *  1.48 2.24 1.44 2.34 1.67 1.91 -1  2016.5.4/13:00
// *  odds: 1.670000, probablity: 20.000000%
// *  odds: 1.910000, probablity: 60.000004%
// *  odds: 2.240000, probablity: 80.000000%
// *  odds: 2.340000, probablity: 80.000000%
// */
//
//
///**
// *  3.48 1.20 1.82 1.75 1.51 2.18 1.58 2.05 1.65 1.94 1.26 3.05 -1
// *
// * odds: 1.200000, probablity: 0.431034%
// * odds: 1.260000, probablity: 7.758620%
// * odds: 1.510000, probablity: 23.706896%
// * odds: 1.580000, probablity: 25.431034%
// * odds: 1.650000, probablity: 26.293104%
// * odds: 1.750000, probablity: 30.603447%
// * odds: 1.820000, probablity: 32.758621%
// * odds: 1.940000, probablity: 36.637932%
// * odds: 2.050000, probablity: 39.224140%
// * odds: 2.180000, probablity: 41.810345%
// * odds: 3.050000, probablity: 60.775864%
// * odds: 3.480000, probablity: 72.413788%
// * 1.43 2.37 1.68 1.90 1.66 1.93 2.51 1.38 -1
//
// -130.426971
// odds: 1.660000, probablity: 22.222223%
// odds: 1.680000, probablity: 27.777779%
// odds: 1.900000, probablity: 38.888889%
// odds: 1.930000, probablity: 44.444447%
// odds: 2.370000, probablity: 66.666672%
// odds: 2.510000, probablity: 83.333328%
//
// 1.95 1.64 2.1001 1.55 1.49 2.22 1.51 2.18 2.29 1.46 1.75 1.82 1.8201 1.7501 1.27 3.00 2.1401 1.53 1.5502 2.1002 2.14 1.5301 2.37 1.43 2.26 1.47 1.71 1.86 1.4901 2.2201 2.1003 1.5501 2.24 1.48  -1
//
// */
/**
 *  1.48 2.24 1.44 2.34 1.67 1.91 -1  2016.5.4/13:00
 *  odds: 1.670000, probablity: 20.000000%
 *  odds: 1.910000, probablity: 60.000004%
 *  odds: 2.240000, probablity: 80.000000%
 *  odds: 2.340000, probablity: 80.000000%
 */

/**
 *  1.43 2.37 1.71 1.86 1.72 1.85 2.85 1.30 1.47 2.27 1.85 1.72 -1
 2.14 1.53 1.88 1.70 2.49 1.39 1.45 2.31 1.56 2.08 1.98 1.62 1.37 2.55 1.87 1.70 1.49 2.22 1.98 1.62 2.16 1.52 2.05 1.58 -1
 
 
 2.01 1.77 4.70 2.67 1.55 1.68 2.05 1.74 2.84 1.55 1.60 1.80 1.10 1.34 2.10 1.90 2.58 1.83 1.30 2.10 -1
 1.77 1.80 1.68 1.90 2.20 1.50 1.84 1.73 1.22 3.32 1.94 1.65 1.67 1.91 1.73 1.84 -1
 
 1.79 1.78 1 2.00 3.10 3.75
 1.60 2.01 0 2.90 3.30 2.25
 2.52 1.38 0 5.50 3.60 1.67
 1.53 2.14 1 1.67 3.60 4.80
 1.67 1.91 0 3.30 3.10 2.15
 -1
 
 2.22 1.49 1 2.40 3.10 3.10
 1.45 2.32 0 2.90 3.40 2.37
 1.37 2.55 1 1.44 4.33 7.50
 1.53 2.14 0 3.10 3.20 2.37
 2.62 1.35 1 2.87 3.00 2.60
 1.91 1.67 1 2.15 3.50 3.25
 2.25 1.48 1 2.50 3.10 3.00
 2.43 1.41 1 2.60 3.00 2.90
 2.48 1.39 1 2.70 3.10 2.70
 1.30 2.85 1 1.40 4.50 8.00
 -1
2.22 3.10 3.25 2.32 1.37 4.15 3.20 2.14 2.95 2.48 1.91 3.40 2.25 3.10 2.43 2.95 2.48 3.15 1.30 4.55 -1
 2.40 3.10 3.40 2.37 1.44 4.33 3.20 2.37 2.15 3.50 2.50 3.10 2.60 3.00 2.70 3.10 1.40 4.50 -1
 
 
 1.62 1.98 1 1.80 3.60 4.50
 1.55 2.10 0 3.30 3.10 2.30
 2.27 1.47 1 2.75 3.40 2.50
 1.47 2.27 0 2.90 3.00 2.60
 1.77 1.80 0 4.20 3.75 1.80
 1.65 1.94 1 1.80 3.50 4.50
 1.98 1.62 1 2.20 3.30 3.30
 1.54 2.12 1 1.80 3.50 4.50
 1.77 1.80 0 4.00 3.50 1.91
 1.44 2.34 1 1.67 3.80 5.00
 -1
 1.62 3.80 3.15 2.10 3.40 2.70 3.20 2.57 3.70 1.80 1.65 3.55 1.98 3.40 1.54 3.70 3.50 1.80 1.44 4.05 -1
 
 1.41 2.42 1 1.57 3.80 6.00
 2.00 1.61 1 2.20 3.60 3.00
 1.86 1.71 1 2.05 3.30 3.60
 2.16 1.52 1 2.40 3.10 3.10
 2.75 1.32 1 3.20 3.75 2.10
 1.48 2.25 1 1.61 4.20 5.00
 1.45 2.32 0 2.80 3.50 2.40
 2.01 1.60 1 2.20 3.30 3.30
 1.82 1.75 1 4.00 3.30 1.95
 3.85 1.16 1 13.00 5.50 1.25
 2.31 1.45 1 2.30 3.00 3.10
 1.87 1.70 1 2.05 3.40 3.25
 -1
 
 odds: 1.410000, probablity: 72.571327%
 odds: 2.420000, probablity: 7.040911%
 
 odds: 2.000000, probablity: 46.705406%
 odds: 1.610000, probablity: 18.861130%
 
 odds: 1.860000, probablity: 51.330807%
 odds: 1.710000, probablity: 16.406040%
 
 odds: 2.160000, probablity: 41.642719%
 odds: 1.520000, probablity: 22.077181%
 
 odds: 2.750000, probablity: 27.894569%
 odds: 1.320000, probablity: 31.593369%
 
 odds: 1.480000, probablity: 68.889374%
 odds: 2.250000, probablity: 8.246087%
 
 odds: 1.450000, probablity: 22.710005%
 odds: 2.320000, probablity: 38.746666%
 
 odds: 2.010000, probablity: 46.463203%
 odds: 1.600000, probablity: 18.943554%
 
 odds: 1.820000, probablity: 38.558178%
 odds: 1.750000, probablity: 30.381943%
 
 odds: 3.850000, probablity: 6.387885%
 odds: 1.160000, probablity: 65.083908%
 
 odds: 2.310000, probablity: 39.013065%
 odds: 1.451000, probablity: 22.511650%
 
 odds: 1.870000, probablity: 50.630215%
 odds: 1.700000, probablity: 16.956985%
 
 
 
 odds: 2.220000, probablity: 40.069183%
 odds: 1.490000, probablity: 22.588858%
 odds: --error--,probablity: 37.341957%
 
 odds: 1.450000, probablity: 22.678425%
 odds: 2.320000, probablity: 38.410107%
 odds: --error--,probablity: 38.911472%
 
 odds: 1.370000, probablity: 77.627876%
 odds: 2.550000, probablity: 5.088477%
 odds: --error--,probablity: 17.283646%
 
 odds: 1.530000, probablity: 21.652643%
 odds: 2.140000, probablity: 42.089588%
 odds: --error--,probablity: 36.257771%
 
 odds: 2.620000, probablity: 29.827044%
 odds: 1.350000, probablity: 29.870579%
 odds: --error--,probablity: 40.302380%
 
 odds: 1.910000, probablity: 49.216888%
 odds: 1.670000, probablity: 17.715235%
 odds: --error--,probablity: 33.067879%
 
 odds: 2.250000, probablity: 38.872395%
 odds: 1.480000, probablity: 23.698730%
 odds: --error--,probablity: 37.428875%
 
 odds: 2.430000, probablity: 34.745110%
 odds: 1.410000, probablity: 25.994583%
 odds: --error--,probablity: 39.260307%
 
 odds: 2.480000, probablity: 33.215282%
 odds: 1.390000, probablity: 27.283997%
 odds: --error--,probablity: 39.500717%
 
 odds: 1.300000, probablity: 82.094666%
 odds: 2.850000, probablity: 3.978384%
 odds: --error--,probablity: 13.926950%
 
 
 1.41 2.42 1 1.57 3.80 6.00
 2.00 1.61 1 2.20 3.60 3.00
 1.86 1.71 1 2.05 3.30 3.60
 2.16 1.52 1 2.40 3.10 3.10
 2.75 1.32 1 3.20 3.75 2.10
 1.48 2.25 1 1.61 4.20 5.00
 1.45 2.32 0 2.80 3.50 2.40
 2.01 1.60 1 2.20 3.30 3.30
 1.82 1.75 1 4.00 3.30 1.95
4.10 1.14 1 13.00 5.50 1.25
 2.31 1.45 1 2.30 3.00 3.10
 1.96 1.64 1 2.05 3.40 3.25
 -1
 

 1.43 2.37 1 1.65 3.75 4.60
 1.64 1.95 1 1.80 3.40 4.00
 2.35 1.44 1 2.40 3.40 2.62
 1.75 1.82 1 1.91 3.20 4.00
 2.15 1.52 1 2.30 3.00 3.10
 2.05 1.58 1 2.20 3.00 3.30
 -1
 1
 1 1 0 1 1 1
 
 1.48 2.25 0 2.95 3.10 2.25
 2.37 1.43 1 2.35 3.35 2.55
 1.83 1.74 1 1.88 3.20 3.65
 1.75 1.82 0 3.20 3.15 2.05
 -1
 */

//2 2 0 1 1 2 1 1 1 1 0 2 1


//1 0 0 2 1 2 2 0 1 2 2 1 2 1

//1 1 0 0

/*
 69.741096%
 odds: 1.400000, probablity: 66.343040%
 odds: 1.420000, probablity: 65.048546%
 odds: 1.560000, probablity: 57.928802%
 odds: 1.610000, probablity: 55.825245%
 odds: 1.611000, probablity: 55.825245%
 odds: 1.640000, probablity: 54.530746%
 odds: 1.660000, probablity: 53.883492%
 odds: 1.680000, probablity: 53.398060%
 odds: 1.740000, probablity: 51.294498%
 odds: 1.830000, probablity: 48.705502%
 odds: 1.900000, probablity: 46.601944%
 odds: 1.930000, probablity: 46.116505%
 odds: 1.950000, probablity: 45.469254%
 odds: 2.000000, probablity: 44.174759%
 odds: 2.001000, probablity: 44.174759%
 odds: 2.080000, probablity: 42.071198%
 odds: 2.400000, probablity: 34.951458%
 odds: 2.460000, probablity: 33.656960%
 odds: 2.580000, probablity: 30.258900%
 
 
 odds: 1.360000, probablity: 0.316331%
 odds: 1.400000, probablity: 1.265322%
 odds: 1.420000, probablity: 1.897983%
 odds: 1.560000, probablity: 11.229734%
 odds: 1.610000, probablity: 14.729142%
 odds: 1.611000, probablity: 14.748913%
 odds: 1.640000, probablity: 17.240017%
 odds: 1.660000, probablity: 18.248320%
 odds: 1.680000, probablity: 19.829971%
 odds: 1.740000, probablity: 25.148279%
 odds: 1.830000, probablity: 30.367735%
 odds: 1.900000, probablity: 36.041912%
 odds: 1.930000, probablity: 38.117832%
 odds: 1.950000, probablity: 39.620403%
 odds: 2.000000, probablity: 42.684856%
 odds: 2.001000, probablity: 42.684856%
 odds: 2.080000, probablity: 47.587978%
 odds: 2.400000, probablity: 67.793594%
 odds: 2.460000, probablity: 70.778969%
 odds: 2.580000, probablity: 76.571770%
*/