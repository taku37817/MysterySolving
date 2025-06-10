

# 目次

<h2><a href="https://github.com/taku37817/MysterySolving/tree/RestorationBranch?tab=readme-ov-file#1%E6%A6%82%E8%A6%81">1.概要</a></h2>
<h2><a href=https://github.com/taku37817/MysterySolving/blob/RestorationBranch/README.md#2%EF%B8%8F-%E9%96%8B%E7%99%BA%E7%92%B0%E5%A2%83>2.開発環境</a></h2>
<h2><a href=https://github.com/taku37817/MysterySolving/tree/RestorationBranch?tab=readme-ov-file#6-%E5%90%84%E9%A0%85%E7%9B%AE%E3%81%AE%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%E6%89%8B%E9%A0%86>6.各インストール手順</a></h2>

---

# 🎮 タイトル・ジャンル
- ## タイトル：MysterySolving  
- ## ジャンル：謎解きゲーム  

# 1.📖 概要

このプロジェクト『MysterySolving』は、プレイヤーが制限時間内に複数の謎を解き、銅像をゴールまで運ぶことを目的とした、謎解きアクションゲームです。

本作は Unreal Engine 5 を使用して制作されており、謎解きの条件やマップ構成にこだわりを持って設計されています。

ゲーム内には、以下のような多彩なギミックが登場します：

- 火を灯す／消すことで進行ルートが変化する  
- 特定の色のオブジェクトを正しい場所に配置してドアを開ける  
- 制限時間内に複数のステップを踏んでゴールを目指す  

プレイヤーは 探索 と 論理思考 を駆使して、各ステージの攻略を目指します。

このプロジェクトは、Unreal Engine 5とC++の2024年12月18日～2025年1月24日までの学習成果を形にすることを目的として開発された作品です。

---

---

# 2.🖥️ 開発環境

<h2><a href="https://github.com/taku37817/MysterySolving/tree/RestorationBranch?tab=readme-ov-file#%E7%9B%AE%E6%AC%A1">目次へ</a></h2>

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

①. **Epic Games Launcher（最新）**  
   ※Epic Games Launcherの利用自体インターネット接続が必要です。

②. **Unreal Engine：5.0.3**

③. **Visual Studio Installer**

④. **Visual Studio Community 2022：17.13.1** (オプション及び個別コンポーネント追加あり)

⑤. **Visual Studio Code：1.100.3（拡張機能あり）**

<h3><a href=https://github.com/taku37817/MysterySolving/tree/RestorationBranch?tab=readme-ov-file#6-%E5%90%84%E9%A0%85%E7%9B%AE%E3%81%AE%E3%82%A4%E3%83%B3%E3%82%B9%E3%83%88%E3%83%BC%E3%83%AB%E6%89%8B%E9%A0%86>🛠上記の各インストール手順についてはこちら</a></h3>

---
---

# 3.使用アセット、著作権
<h2><a href="https://github.com/taku37817/MysterySolving/tree/RestorationBranch?tab=readme-ov-file#%E7%9B%AE%E6%AC%A1">目次へ</a></h2>

---

---

# 4.資料の保存場所
<h2><a href="https://github.com/taku37817/MysterySolving/tree/RestorationBranch?tab=readme-ov-file#%E7%9B%AE%E6%AC%A1">目次へ</a></h2>

---
---

# 5.ディレクトリ構成
<h2><a href="https://github.com/taku37817/MysterySolving/tree/RestorationBranch?tab=readme-ov-file#%E7%9B%AE%E6%AC%A1">目次へ</a></h2>

---
---

# 6.📥 各項目のインストール手順
<h2><a href="https://github.com/taku37817/MysterySolving/tree/RestorationBranch?tab=readme-ov-file#%E7%9B%AE%E6%AC%A1">目次へ</a></h2>

---

### ②. Unreal Engine 5.0.3

①のEpic Games Launcherから「Unreal Engine 5.0.3 」をインストールします。

1. **Epic Games Launcher** を起動し、「**Unreal Engine**」タブを選択  
2. 「**ライブラリ**」→「**Engineバージョン⊞**」から **5.0.3** を選択し、「**インストール**」をクリック  
3. 「**オプション**」から必要な機能を追加  
4. 「**参照**」でインストール先を変更できます（※変更しない場合はそのままでOK）  
5. オプションを追加後、「インストール開始」

📷 **インストール手順の参考画像：**  
<img src="https://github.com/user-attachments/assets/1b96a7e0-00fb-4d78-946b-31be6f3e91b2" alt="UnrealEngineバージョンインストールについて" width="700"><br>
<img src="https://github.com/user-attachments/assets/a1695a20-d14b-4984-ae02-e6c0d3b60176" alt="UnrealEngineバージョンインストール、初期のオプション変更について" width="700"><br>

📷 **オプション設定の確認画面：**  
### 6のオプションの追加項目について
<img src="https://github.com/user-attachments/assets/e9f1e05d-4ef9-4255-97b1-7f7ae1ee595f" alt="UnrealEngineインストール時の確認画面" width="700"><br>

🔸プラットフォームの追加は **不要** です。
---

## ⚠️ **7でインストール場所を変更する場合の注意：**  
## 「**Epic Games**」という名前のフォルダを自分で作成し、その中にインストールしてください。  
## （例：`D:\Epic Games\UE_5.0` など）  
## 間違えて「Epic Game」や「Epic Gemas」などにしないように注意！

---

# ③. Visual Studio Installer

[公式リリース履歴](https://learn.microsoft.com/ja-jp/visualstudio/releases/2022/release-history#release-dates-and-build-numbers)より「17.13.1（2025年2月19日）」を「ビルドツール」からダウンロードして使用してます。 

### 🧱 ④Visual Studio Community の構成

#### ✅ 追加ワークロード

1. 「C++によるゲーム開発」  
2. 「C++によるデスクトップ開発」  
3. 「.NET デスクトップ開発」  

---

#### ✅ インストール詳細の「オプション」確認（ワークロード①）

以下は「C++によるゲーム開発」ワークロード選択時に、**オプションでチェックを入れるべき項目**です：

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

> ⚠️ ワークロード②と③に関しては、このあと「個別コンポーネント」から追加するので、**インストール詳細の設定は変更しなくて大丈夫**です。


#### 🧩 個別コンポーネントから手動で追加する項目について
**▼ .NET**
- ・.NET 6.0 ランタイム（サポート対象外）
- ・.NET Croe 3.1 ランタイム(サポート対象外)
- ・.NET Framwork 4.6.2SDK
  
**▼ SDK、ライブラリ、およびフレームワーク**
- v142 ビルドツール用 C++ v14.29 (16.11) ALT（x86およびx64）  
- v143 ビルドツール用 C++ v14.31 (17.1) ALT（x86およびx64）（サポート対象外）  
- v143 ビルドツール用 C++ v14.32 (17.2) ALT（x86およびx64）（サポート対象外）  

**▼ コンパイラ、ビルドツール、およびランタイム**
- MSVC v142 - VS 2019 C++ x64/x86 ビルドツール（v14.29–16.11）  
- MSVC v143 - VS 2022 C++ x64/x86 ビルドツール（v14.31–17.1）（サポート対象外）  
- MSVC v143 - VS 2022 C++ x64/x86 ビルドツール（v14.32–17.2）（サポート対象外）  

---

# ⑤. Visual Studio Code（1.100.3）

[ダウンロードリンク](https://code.visualstudio.com/updates/v1_100)

**利用機能：**

- C/C++ Themes  
- C/C++  
- C/C++ Extension Pack  
- Unreal Engine Snippets  
- Unreal Intellisense Fixes  
- Japanese Language Pack for Visual Studio Code（日本語化用）

---
# 7.リポジトリ「ダウンロード」または「クローン」後のフォルダー移動の手順

---
