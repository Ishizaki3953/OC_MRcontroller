#include "AngleCompensation.h"
#include <cmath>

//-----------------------------------------------------
// ANGLE_OFFSET         // 角度オフセット補正値
// CENTER_OFFSET_X      // 中心オフセットX補正値
// CENTER_OFFSET_Y      // 中心オフセットY補正値
// ELLIPTIC_CORRECTION  // 楕円補正値
//-----------------------------------------------------
double AngleCompensation_sub(double dData)
{
    double dResult = 0.0;
    double dWk;
    double dWk_X, dWk_Y;
    double dX, dY;

    // 角度オフセット補正値を加算
    dWk = dData + ANGLE_OFFSET;

    // 角度オフセット後のX,Yの値を算出
    // (精度を上げる必要がある場合は変数の型と使用関数変更を検討してください)
    dWk_X = std::cos(dWk);
    dWk_Y = std::sin(dWk);

    //-------------------------
    // 楕円の補正
    //-------------------------
    // X軸 : 楕円補正値を乗算,X軸方向の中心オフセットを加算
    dX = (dWk_X * ELLIPTIC_CORRECTION) + CENTER_OFFSET_X;
    // Y軸 : Y軸方向の中心オフセットを加算
    dY = dWk_Y + CENTER_OFFSET_X;

    // 補正後のX,Yから角度を算出
    dResult = std::atan2(dY, dX);

    return dResult;
}

unsigned int AngleCompensation(unsigned int unData)
{
    unsigned int unResult;
    double dDegree;
    double dRadian;

    // ADを角度に変換
    dDegree = unData * 360.0f / 4096;
    // Radianに変換
    dRadian = dDegree * (DEF_PI / 180.0f);
    // 補正処理
    dRadian = AngleCompensation_sub(dRadian);
    // 0～359に変換
    dRadian = (dRadian > 0 ? dRadian : (2.0f * DEF_PI) + dRadian);
    // Degreeに変換
    dDegree = dRadian * (180.0f / DEF_PI);
    // 角度をADに変換
    unResult = (unsigned int)(dDegree * 4096 / 360.0f);

    return unResult;
}
