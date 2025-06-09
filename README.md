# MysterySolving

## 🎮 タイトル・ジャンル
- タイトル：MysterySolving  
- ジャンル：謎解きゲーム  

---

## 📖 概要

このプロジェクト『MysterySolving』は、プレイヤーが制限時間内に複数の謎を解き、銅像をゴールまで運ぶことを目的とした、謎解きアクションゲームです。

本作は Unreal Engine 5 を使用して制作されており、謎解きの条件やマップ構成にこだわりを持って設計されています。

ゲーム内には、以下のような多彩なギミックが登場します：

- 火を灯す／消すことで進行ルートが変化する  
- 特定の色のオブジェクトを正しい場所に配置してドアを開ける  
- 制限時間内に複数のステップを踏んでゴールを目指す  

プレイヤーは 探索 と 論理思考 を駆使して、各ステージの攻略を目指します。

このプロジェクトは、Unreal Engine 5とC++の学習成果（2025年5月まで）を形にすることを目的として開発された作品です。

🛠 詳細な環境とインストール手順については [こちら](#開発環境とインストール手順) をご参照ください。

---

## 🖥️ 開発環境とインストール手順

このプロジェクトは以下の環境で構築され・動作確認しています。

### ✅ 開発環境

- **OS**  
  Windows11 Home (バージョン：24H2)  
  OSビルド：26100.4202  
  システムの種類：x64-ベース PC  
  エクスペリエンス：Windows 機能エクスペリエンス パック 1000.26100.107.0  

- **メモリ**  
  32GB DDR4  

- **CPU**  
  Intel(R) Core(TM) i5-8300H CPU @ 2.30GHz（4コア8スレッド）  

- **GPU**  
  Intel(R) UHD Graphics 630  
  NVIDIA GeForce GTX 1050  

- **ストレージ**  
  SSD 250GB  

---

### 🧰 利用ツール・エンジン一覧

1. **Epic Games Launcher（最新）**  
   ※インターネット接続が必要です。

2. **Unreal Engine：5.0.3**

3. **Visual Studio Installer**

4. **Visual Studio Community 2022：17.13.1**

5. **Visual Studio Code：1.100.3（拡張機能あり）**

---

### 📥 各項目のインストール手順

#### 2. Unreal Engine 5.0.3

Epic Games Launcherから「Unreal Engine 5.0.3 」をインストール。  
※以下にインストール手順の画像を挿入予定。

#### 3. Visual Studio Installer

[公式リリース履歴](https://learn.microsoft.com/ja-jp/visualstudio/releases/2022/release-history#release-dates-and-build-numbers)より「17.13.1（2025年2月19日）」を使用。  

**追加ワークロード：**

- C++によるゲーム開発  
- C++によるデスクトップ開発  
- .NET デスクトップ開発  

**インストール詳細オプション：**

- MSVC v143 - VS 2022 C++ x64/x86 ビルドツール（最新）  
- C++ のプロファイル ツール  
- C++ Build Insights  
- C++ AddressSanitizer  
- vcpkg パッケージ マネージャー  
- Windows 11 SDK (10.0.22621.0)  
- IntelliCode  
- Unreal Engine 用 Visual Studio Tools  
- HLSL ツール  
- Unreal Engine のインストーラー  
- 任意：Unreal Engine 用 Android Visual Studio Tools  

**個別コンポーネントで追加：**

- ・.NET 6.0 ランタイム（サポート対象外）
- ・.NET Croe 3.1 ランタイム(サポート対象外)
- ・.NET Framwork 4.6.2SDK
#項目：SDK、ライブラリ、およびフレームワーク
- ・v142 ビルドツール用 C++ v14.29(16.11)ALT (x86および x64)
- ・v143 ビルドツール用 C++ v14.31(17.1)ALT (x86 および x64)(サポート対象外)
- ・v143 ビルドツール用 C++ v14.32(17.2)ALT (x86 および x64)(サポート対象外)
#項目：コンパイラ、ビルドツール、およびランタイム
- ・MSVC v142 - VS 2019 C++ x64/x86 ビルドツール(v14.29-16.11) 
- ・MSVC v143 - VS 2022 C++ x64/x86 ビルドツール(v14.31-17.1)(サポート対象外)
- ・MSVC v143 - VS 2022 C++ x64/x86 ビルドツール(v14.32-17.2)(サポート対象外)
---

#### 5. Visual Studio Code（1.100.3）

[ダウンロードリンク](https://code.visualstudio.com/updates/v1_100)

**おすすめ拡張機能：**

- C/C++ Themes  
- C/C++  
- C/C++ Extension Pack  
- Unreal Engine Snippets  
- Unreal Intellisense Fixes  
- Japanese Language Pack for Visual Studio Code（日本語化用）

---

