# Qt CSV ⇄ JSON Converter

![Build](https://github.com/acc-vcc/csv-json-converter-qt-sample/actions/workflows/build.yml/badge.svg)

Qt5 を使用して作成した、CSV と JSON の相互変換ツールです。  
Qt を用いた GUI アプリケーション開発のサンプルとして作成しました。

GUI ベースのため、コマンドラインに不慣れな環境でも扱いやすく、  
小規模な業務ツールの実装例として利用できます。

---

## 🛠 使用技術

- **Qt 5.x（Qt Widgets）**
- **C++17（Qt API を中心に構成）**
- **QFile / QTextStream によるファイル入出力**
- **qmake / CMake（どちらでもビルド可能）**
- **GitHub Actions（自動ビルド）**
- **Docker（環境構築用）**

---

## 📌 機能概要

- CSV → JSON 変換  
- JSON → CSV 変換  
- 入力ファイル形式の自動判定（CSV / JSON）  
- CSV のヘッダ行有無の自動判定  
- **改行やクォートを含む一般的な CSV 形式に対応**  
- エラー・警告ログの表示（ログレベルに応じた文字色変更）  
- シンプルで扱いやすい GUI

---

## 🧩 実装ポイント

- Qt Widgets を用いたシンプルな GUI 構成  
- 画面レイアウトはすべて `.ui` ファイルで構成し、  
  **水平・垂直レイアウトによりウィンドウサイズに応じて自動リサイズされる設計**  
- `QFile` と `QTextStream` を使用した入出力処理  
- 戻り値によるエラーハンドリング（例外は未使用）  
- CSV/JSON の自動判定ロジックを実装  
- qmake と CMake の両方でビルド可能  
- GitHub Actions による自動ビルド設定  
- Dockerfile による環境構築例を用意

---

## ⚠️ 制限事項 / 注意事項

- JSON の仕様上、オブジェクトのキー順は保証されません。  
- Qt の `QJsonObject` はキーをアルファベット順に並べ替えるため、  
  **CSV → JSON → CSV の往復で列順が変わる場合があります。**
- サンプルコードのため、列順保持などの高度な仕様は実装していません。
- 改行コードの保持は行っていません。
- CSV 判定は簡易的で、以下の条件で判定しています：  
  - 拡張子が `.csv`  
  - またはファイル先頭 20 文字以内にカンマが存在する  

---

## 📁 ディレクトリ構成
```
csv-json-converter-qt-sample/
├─ .gitignore             # Git 管理対象外ファイル設定
├─ .github
│  └─ workflows
│     └─ build.yml        # 自動ビルド用 GitHub Actions ファイル
├─ main.cpp               # エントリポイント
├─ mainwindow.cpp         # メインウィンドウの実装
├─ mainwindow.h
├─ mainwindow.ui          # UI レイアウト
├─ ConvCsvJson.pro        # Qt プロジェクトファイル
├─ CMakeLists.txt         # CMake 用設定ファイル
├─ Dockerfile             # 環境構築用 Dockerfile
└─ README.md              # プロジェクト説明
```

---

## 📂 ビルド方法

### qmake を使用する場合
1. Qt 5.x をインストール  
2. Qt Creator でプロジェクトを開く  
3. ビルド構成を選択（Debug / Release）  
4. 実行

### CMake を使用する場合
1. Qt 5.x と CMake をインストール  
2. プロジェクトルートで以下を実行

```
cmake -B build
cmake --build build
```
3. 生成された実行ファイルを起動

---

## 📷 画面イメージ

（※現在 Qt 実行環境がないため、画面イメージは後日追加予定です）

---

## 📄 License
This project is licensed under the MIT License.

---

## 📫 連絡について

ご相談やご依頼がありましたら、以下のメールアドレスまでご連絡ください。

**📧 acc.vcc.work@gmail.com**

丁寧に対応いたします。
