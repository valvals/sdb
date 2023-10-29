#include "UselessDataCutter.h"
#include <QPair>

namespace  {
void shrinkVectors(QPair<double,double>interval,
                   QVector<double>&waves,
                   QVector<double>&values){

    if(waves.size()!=values.size())return;
    int startIndex = 0;
    auto waves_begin = waves.begin();
    auto values_begin = values.begin();

    for(int i=0;i<waves.size();++i){
        if(interval.first<=waves[i]){
            startIndex = i;
            break;
        }
    }
    waves.erase(waves.begin(),waves_begin+startIndex);
    values.erase(values.begin(),values_begin+startIndex);

    int endIndex = waves.size()-1;
    for(int i=waves.size()-1;i>0;--i){
        if(interval.second>=waves[i]){
            endIndex = i;
            break;
        }

    }
    waves.erase(waves.begin()+endIndex,waves.end());
    values.erase(values.begin()+endIndex,values.end());
};
} // namespace

namespace udc{

void cutUselessData(EQUIPMENT_TYPE equipment,
                    QVector<double> &waves,
                    QVector<double> &values)
{
    switch(equipment){
    case(FSR_02):
    {
        shrinkVectors({400,900},waves,values);
        break;
    }
    case(PSR_700):break;
    case(PSR_1300):break;
    default:break;
    }
}

}
