//
//  Probability.hpp
//  概率论进化版
//
//  Created by 侯庆峰 on 16/7/6.
//  Copyright © 2016年 letv. All rights reserved.
//

#ifndef Probability_hpp
#define Probability_hpp

#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <map>
#include <math.h>

#include "sqlite3.h"

using namespace std;

struct ProbabilityArray{
    float   victory;
    int     victoryNum;
    float   failure;
    int     failureNum;
    int     operation;
    float   draw;
    int     drawNum;
    float   maxOdds;
    float   minOdds;
    float   willianVictoryProbality;
    float   willianFailureProbality;
    float   houVictoryProbality;
    float   houFailureProbality;
    int     location;
    
    static float averange;
};

struct Arrange{
    int *array;
    int number;
};

struct AllArrangeArray{
    Arrange *arrange;
};

struct floatArray{
    float *fArray;
    int number;
    bool print;
    int location[15];
};


class Probability{
public:
    
private:
    int arrayMAX;
    vector<floatArray> outcomeArray;
    vector<AllArrangeArray>arrangeArray;
    vector<ProbabilityArray> arrayVector;
};

#endif /* Probability_hpp */
