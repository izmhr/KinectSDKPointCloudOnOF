# KinectSDK Point Cloud on oF

## 開発環境

- Windows 10 x64
- Kinect V2
- [Kinect for Windows SDK 2.0](https://www.microsoft.com/en-us/download/details.aspx?id=44561)
- openFrameworks v0.10.0 VS2017

### 利用PCスペックの概要

下記スペックで 60fps 安定 (※Kinect は 30fps でデプスを出します)

- Intel Core i7-6700K CPU@4.00GHz
- GeForece GTX 1080

## ビルド手順

- $(OF_ROOT)\apps\myApps\<thisproject> となるように配置する
- Kinect for Windows SDK はデフォルトの位置 (C:\Program Files\Microsoft SDKs\Kinect\v2.0_1409) にインストールされている物とする
  - もしそうで無い場合、VSのプロパティマネージャで、各構成(Debug/Release)とプラットフォーム(x64/Win32) の KinectSDK に関する部分を書き換える。具体的には、 `C/C++ > 全般 > 追加のインクルードディレクトリ` と `リンカー > 全般 > 追加のライブラリディレクトリ` を、Kinect SDK の位置になるよう修正する。
- ビルドの構成を所望の物に変える。 (Debug/Release) (Win32/x64) の両方で動作確認済
- 実行

### プロジェクトに施しているちょっとした工夫

- Kinect SDK 関連のパスは、プロパティマネージャで外部ファイル `KinectSDKWin32.props` `KinectSDKx64.props` として作成し登録してる。(他のプロジェクトにも転用可能)
- openFrameworks のオリジナルコードにおいて size_t の代入等警告が出るので黙らせている。参照 > `C/C++ > 詳細設定 > 指定の警告を無効にする` で 4819 を登録
- 自作のコードは Unicode (65001) で保存している (Gitで日本語表示のため) これに併せて oF 側との保存形式矛盾の警告が出るが、これも同上の方法で黙らせている。 4267。

## 操作法

- ofEasyCam に従う

## 既知の問題

- カメラの初期位置が遠すぎる
