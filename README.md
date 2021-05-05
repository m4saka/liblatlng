# liblatlng
緯度/経度を扱うためのシングルヘッダライブラリ(C++11 or later)

- 2地点間の距離/方位の計算(`LatLng`クラス)
- 角度に関するユーティリティ関数群(`Degree`名前空間)

## 使用方法
使用する際は`include`ディレクトリをインクルードパスに指定してください  
(GCCなら`-I`オプション、Visual Studioなら「プロパティ」→「C/C++」→「全般」→「追加のインクルードディレクトリ」)

シングルヘッダライブラリなので、以下のようにインクルードするだけで使用できます(ライブラリ自体のコンパイルは不要です)
```cpp
#include <liblatlng.hpp>
```

## サンプルコード

以下のコードで、東京・大阪間の距離と方位を求められます

```cpp
#include <iostream>
#include <liblatlng.hpp>

int main()
{
    // 東京の緯度/経度
    liblatlng::LatLng tokyo(139.539242, 35.686991);
    
    // 大阪の緯度/経度
    liblatlng::LatLng osaka(135.545261, 34.598366);

    // 2地点間の距離を計算
    std::cout << "Tokyo from Osaka [m]: " << tokyo.distanceFrom(osaka) << std::endl;
    std::cout << "Osaka from Tokyo [m]: " << osaka.distanceFrom(tokyo) << std::endl;

    // 2地点間の方位を計算
    std::cout << "Tokyo from Osaka [deg]: " << tokyo.azimuthFrom(osaka) << std::endl;
    std::cout << "Osaka from Tokyo [deg]: " << osaka.azimuthFrom(tokyo) << std::endl;

    return 0;
}
```

実行例:
```
Tokyo from Osaka [m]: 453495.8
Osaka from Tokyo [m]: 453495.8
Tokyo from Osaka [deg]: 169.0
Osaka from Tokyo [deg]: 348.3
```

## クラス/関数一覧(基本)
- `LatLng`: 緯度/経度のクラス(double型)
  - メンバ変数
    - `double lat`: 緯度(latitude), 単位:度
    - `double lng`: 経度(longitude), 単位:度
  - メンバ関数
    - `double distanceFrom(const latLng &)`: 指定地点からの距離を返す(メートル単位)
    - `double azimuthFrom(const latLng &)`: 指定地点から矢印を引いた場合の方位を返す(単位:度, 0～360の範囲)

- `Degree`: 角度に関するユーティリティ関数群が入った名前空間
  - 関数一覧
    - `double toRadian(double)`: 角度からラジアンへ変換
    - `double fromRadian(double)`: ラジアンから角度へ変換
    - `double normalizeRelative(double)`: 角度を -180 ～ 180 の範囲に丸める
    - `double normalizeAbsolute(double)`: 角度を 0 ～ 360 の範囲に丸める
  - それぞれ関数テンプレート(`T toRadian(T)`など)となっているので, float型などにもそのまま利用可能です

## クラス/関数一覧(応用)
- `LatLngF`: 緯度/経度のクラス(float型)
  - 内容は`LatLng`と同じ

- `BasicLatLng<T>`: 緯度/経度のクラステンプレート
  - 内容は`LatLng`と同じ
  - double型, float型以外を利用する必要がある場合に使用

## ライセンス
MITライセンス

```
MIT License

Copyright (c) 2021 masaka

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

## 参考
- 距離/方位計算の実装の参考資料
  - [２地点間の距離と方位角 - 高精度計算サイト(CASIO)](https://keisan.casio.jp/exec/system/1257670779)
  - [緯度経度から方位角を算出する式 | teratail](https://teratail.com/questions/90662)
