// MIT License
// 
// Copyright (c) 2021 masaka
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once
#include <cmath> // std::isnan

#if __cplusplus >= 202002L
    #include <numbers> // std::numbers::pi_v
#endif

namespace liblatlng
{

    #if __cplusplus >= 202002L
        // C++20以降なら定数πにnumbersヘッダを使用
        namespace Constants
        {
            template <typename T>
            constexpr T pi() { return std::numbers::pi_v<T>; }
        }
    #else
        // C++20より前なら定数πを手動で指定
        // (_USE_MATH_DEFINESによるマクロ汚染を避けるためM_PIは不使用)
        namespace Constants
        {
            template <typename T>
            constexpr T pi() { return 3.14159265358979323846; }
        }
    #endif

    namespace Degree
    {
        // 角度からラジアンへ
        template <typename T>
        constexpr T toRadian(T deg)
        {
            return deg * Constants::pi<T>() / T{180.0};
        }

        // ラジアンから角度へ
        template <typename T>
        constexpr T fromRadian(T rad)
        {
            return rad * T{180.0} / Constants::pi<T>();
        }

        // 角度を -180 ～ 180 の範囲に丸める
        template <typename T>
        T normalizeRelative(T deg)
        {
            // NaNは足しても引いてもNaNのままなので無限ループに陥る
            if (std::isnan(deg))
            {
                return deg;
            }

            // 値の指数が大きいと360を足しても引いても変わらなくて無限ループに陥る
            if (deg > 1e9 || deg < -1e9)
            {
                return 0.0;
            }

            while (deg >= 180.0)
            {
                deg -= 360.0;
            }

            while (deg < -180.0)
            {
                deg += 360.0;
            }

            return deg;
        }

        // 角度を 0 ～ 360 の範囲に丸める
        template <typename T>
        T normalizeAbsolute(T deg)
        {
            // NaNは足しても引いてもNaNのままなので無限ループに陥る
            if (std::isnan(deg))
            {
                return deg;
            }

            // 値の指数が大きいと360を足しても引いても変わらなくて無限ループに陥る
            if (deg > 1e9 || deg < -1e9)
            {
                return T{0.0};
            }

            while (deg >= 360.0)
            {
                deg -= 360.0;
            }

            while (deg < 0.0)
            {
                deg += 360.0;
            }

            return deg;
        }
    }

    // 緯度/経度(クラステンプレート)
    template <typename T>
    struct BasicLatLng
    {
        // 緯度(latitude), 単位:度
        T lat;

        // 経度(longitude), 単位:度
        T lng;

        constexpr BasicLatLng(T lat, T lng)
            : lat(lat)
            , lng(lng)
        {
        }

        // 指定地点からの距離を返す(メートル単位)
        T distanceFrom(const BasicLatLng<T> & other) const;

        // 指定地点から矢印を引いた場合の方位を返す(単位:度, 0～360の範囲)
        T azimuthFrom(const BasicLatLng<T> & other) const;
    };

    template <typename T>
    T BasicLatLng<T>::distanceFrom(const BasicLatLng<T> & other) const
    {
        // 地球の長半径
        constexpr T kEarthRadius = 6378137.0;

        // 度からラジアンへ変換
        const T latRadian = Degree::toRadian(lat); // 緯度
        const T lngRadian = Degree::toRadian(lng); // 経度
        const T otherLatRadian = Degree::toRadian(other.lat);
        const T otherLngRadian = Degree::toRadian(other.lng);

        // 経度の差分
        const T lngRadianDiff = otherLngRadian - lngRadian;

        return kEarthRadius * std::acos(
            std::sin(latRadian) * std::sin(otherLatRadian) +
            std::cos(latRadian) * std::cos(otherLatRadian) * std::cos(lngRadianDiff)
        );
    }

    template <typename T>
    T BasicLatLng<T>::azimuthFrom(const BasicLatLng<T> & other) const
    {
        // 度からラジアンへ変換
        const T latRadian = Degree::toRadian(lat); // 緯度
        const T lngRadian = Degree::toRadian(lng); // 経度
        const T otherLatRadian = Degree::toRadian(other.lat);
        const T otherLngRadian = Degree::toRadian(other.lng);

        // 経度の差分
        const T lngRadianDiff = otherLngRadian - lngRadian;

        const T y = std::sin(lngRadianDiff);
        const T x = std::cos(latRadian) * std::tan(otherLatRadian) - std::sin(latRadian) * std::cos(lngRadianDiff);
        return Degree::normalizeAbsolute(Degree::fromRadian(std::atan2(y, x)) + 180.0);
    }

    // 緯度/経度(double型)
    using LatLng = BasicLatLng<double>;

    // 緯度/経度(float型)
    using LatLngF = BasicLatLng<float>;

}
